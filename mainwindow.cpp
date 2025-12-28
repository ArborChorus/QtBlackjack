#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "betwidget.h"

#include <QDebug>
#include <qboxlayout.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new Game(this);

    ui->centralwidget->setStyleSheet("#centralwidget { background-image: url(:/assets/table_texture.jpg); }");

    SetUpSkinSwitcher();

    connect(ui->hitButton, &QPushButton::clicked, game, &Game::HitPlayer);
    connect(ui->standButton, &QPushButton::clicked, game, &Game::StandPlayer);

    connect(ui->newGameButton, &QPushButton::clicked, this, [this](){
        if (game->GetState() == GameState::Idle) {
            game->StartRound();
        } else if (game->GetState() == GameState::Finished) {
            clearVisualCards();
            game->Start();
        }
    });

    deckVisual = new QLabel(ui->cardDeck);
    deckVisual->setFixedSize(96, 134);
    deckVisual->setScaledContents(true);

    discardVisual = new QLabel(ui->usedCardDeck);
    discardVisual->setFixedSize(96, 134);
    discardVisual->setScaledContents(true);
    discardVisual->hide();

    updateDeckSkins();

    connect(game, &Game::updated, this, &MainWindow::UpdateUI);
    connect(game, &Game::finished, this, &MainWindow::UpdateUI);
    connect(ui->betZone, &BetWidget::BetClicked, game, &Game::AddBet);

    UpdateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateUI()
{
    GameState state = game->GetState();

    ui->dealerScore->setText(QString::number(game->GetDealerhand().Score()));

    if(state == GameState::Finished) {
        ui->gameResult->setText(game->GetResult());
    } else if (state == GameState::Idle && game->GetBalance() <= 0 && game->GetBet() <= 0) {
        ui->gameResult->setText("GAME OVER (Out of money)");
    } else {
        ui->gameResult->setText("");
    }

    ui->dealerHand->SetHand(&game->GetDealerhand());
    ui->playerHand->SetHand(&game->GetPlayerHand());
    ui->betLabel->setText("Bet: " + QString::number(game->GetBet()));
    ui->balanceLabel->setText("Balance: " + QString::number(game->GetBalance()));

    ui->dealerHand->SetHideFirstCard(state == GameState::PlayerTurn);

    ui->hitButton->setEnabled(state == GameState::PlayerTurn);
    ui->standButton->setEnabled(state == GameState::PlayerTurn);

    ui->betZone->setEnabled(state == GameState::Idle);

    if (state == GameState::Idle) {
        ui->newGameButton->setText("Deal");
        ui->newGameButton->setEnabled(game->GetBet() > 0);
    } else if (state == GameState::Finished) {
        ui->newGameButton->setText("Next Round");
        ui->newGameButton->setEnabled(game->GetBalance() > 0 || game->GetBet() > 0);
    } else {
        ui->newGameButton->setText("Playing...");
        ui->newGameButton->setEnabled(false);
    }

    const auto& pCards = game->GetPlayerHand().Cards();
    const auto& dCards = game->GetDealerhand().Cards();

    // 1. Spawn Player Cards (Always visible)
    for(int i = playerSprites.size(); i < pCards.size(); ++i) {
        CardSprite* s = new CardSprite(pCards[i], currentSkinIndex, ui->centralwidget, false);
        s->move(ui->cardDeck->pos());
        s->FlyTo(getLandingPos(ui->playerHand, i), i * 150);
        playerSprites.append(s);
    }

    // 2. Spawn Dealer Cards
    for(int i = dealerSprites.size(); i < dCards.size(); ++i) {
        // If it's the very first dealer card and we are still in PlayerTurn, hide it
        bool shouldHide = (i == 0 && state == GameState::PlayerTurn);

        CardSprite* s = new CardSprite(dCards[i], currentSkinIndex, ui->centralwidget, shouldHide);
        s->move(ui->cardDeck->pos());
        s->FlyTo(getLandingPos(ui->dealerHand, i), i * 150);
        dealerSprites.append(s);
    }

    // 3. REVEAL the dealer's card if the game is finished
    if (state == GameState::Finished && !dealerSprites.isEmpty()) {
        dealerSprites[0]->setHidden(false);
        // Also update dealer score now that the card is visible
        ui->dealerScore->setText(QString::number(game->GetDealerhand().Score()));
    } else if (state == GameState::PlayerTurn) {
        // While player is playing, maybe hide the real dealer score or show "?"
        ui->dealerScore->setText("?");
    }
}

void MainWindow::SetUpSkinSwitcher(){
    QHBoxLayout* layout = new QHBoxLayout(ui->coatChange);

    QPushButton* prevBtn = new QPushButton("<");
    QPushButton* nextBtn = new QPushButton(">");

    skinPreviewLabel = new QLabel("Skin 1");
    skinPreviewLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(prevBtn);
    layout->addWidget(skinPreviewLabel);
    layout->addWidget(nextBtn);

    connect(prevBtn, &QPushButton::clicked, this, [this]() {
        currentSkinIndex--;
        if (currentSkinIndex < 1) currentSkinIndex = MAX_SKINS;
        UpdateSkin();
    });

    connect(nextBtn, &QPushButton::clicked, this, [this]() {
        currentSkinIndex++;
        if (currentSkinIndex > MAX_SKINS) currentSkinIndex = 1;
        UpdateSkin();
    });
}

void MainWindow::UpdateSkin(){
    skinPreviewLabel->setText(QString("Skin %1").arg(currentSkinIndex));

    // Update existing hand widgets (static display)
    ui->playerHand->SetSkin(currentSkinIndex);
    ui->dealerHand->SetSkin(currentSkinIndex);

    // Update the corner decks
    updateDeckSkins();

    deckVisual->setStyleSheet(
        "border-right: 3px solid #1a3a18;"
        "border-bottom: 3px solid #1a3a18;"
        "border-radius: 5px;"
        );

    // Update any active sprites on screen
    for(auto s : playerSprites) s->updatePixmap(); // You might need to make updatePixmap public or add a setSkin method to CardSprite
    for(auto s : dealerSprites) s->updatePixmap();
}

QPoint MainWindow::getLandingPos(QWidget* targetArea, int cardIndex) {
    int x = targetArea->x() + (cardIndex * 32);
    int y = targetArea->y();
    return QPoint(x, y);
}

void MainWindow::clearVisualCards() {
    if(playerSprites.isEmpty() && dealerSprites.isEmpty()) return;

    QPoint discardPilePos = ui->usedCardDeck->pos();

    for (CardSprite* s : playerSprites) s->DiscardTo(discardPilePos);
    for (CardSprite* s : dealerSprites) s->DiscardTo(discardPilePos);

    playerSprites.clear();
    dealerSprites.clear();

    // After cards fly there, make the static discard pile visual appear
    // We delay this slightly so it appears after cards "arrive"
    QTimer::singleShot(400, [this]() {
        discardVisual->show();
    });
}

void MainWindow::updateDeckSkins() {
    QString backPath = QString(":/assets/Cards%1/back.png").arg(currentSkinIndex);
    QPixmap backPix(backPath);

    deckVisual->setPixmap(backPix);
    discardVisual->setPixmap(backPix);
}
