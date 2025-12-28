#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "betwidget.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new Game(this);


    backgroundLabel = new QLabel(ui->centralwidget);
    backgroundLabel->setGeometry(0, 0, 800, 500);
    backgroundLabel->setPixmap(QPixmap(":/assets/table_texture.jpg"));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->lower();

    deckVisual = new QLabel(ui->cardDeck);
    deckVisual->setFixedSize(96, 134);
    deckVisual->setScaledContents(true);

    discardVisual = new QLabel(ui->usedCardDeck);
    discardVisual->setFixedSize(96, 134);
    discardVisual->setScaledContents(true);
    discardVisual->hide();

    connect(game, &Game::updated, this, &MainWindow::UpdateUI);
    connect(game, &Game::finished, this, &MainWindow::UpdateUI);
    connect(ui->hitButton, &QPushButton::clicked, game, &Game::HitPlayer);
    connect(ui->standButton, &QPushButton::clicked, game, &Game::StandPlayer);

    connect(ui->prevSkin, &QPushButton::clicked, this, [this]() {
        currentSkinIndex = (currentSkinIndex <= 1) ? MAX_SKINS : currentSkinIndex - 1;
        UpdateSkin();
    });

    connect(ui->nextSkin, &QPushButton::clicked, this, [this]() {
        currentSkinIndex = (currentSkinIndex >= MAX_SKINS) ? 1 : currentSkinIndex + 1;
        UpdateSkin();
    });

    connect(ui->newGameButton, &QPushButton::clicked, this, [this](){
        if (game->GetState() == GameState::Idle) game->StartRound();
        else if (game->GetState() == GameState::Finished) {
            clearVisualCards();
            game->Start();
        }
    });

    connect(ui->betZone, &BetWidget::BetClicked, this, [this](int amount, QPoint startPos) {
        if (game->AddBet(amount, bettingTarget)) {
            ChipSprite* chip = new ChipSprite(amount, ui->centralwidget);
            chip->move(ui->centralwidget->mapFrom(this, startPos));
            QWidget* targetHand = (bettingTarget == 0) ? ui->playerHand : ui->playerHand2;
            QVector<ChipSprite*>& targetList = (bettingTarget == 0) ? hand1Chips : hand2Chips;
            QPoint targetPos = QPoint(targetHand->x() + 20, targetHand->y() - 60 - (targetList.size() * 5));
            chip->FlyTo(targetPos);
            targetList.append(chip);
        }
    });

    connect(ui->switchHandBtn, &QPushButton::clicked, this, [this](){
        bettingTarget = (bettingTarget == 0) ? 1 : 0;
        ui->switchHandBtn->setText(bettingTarget == 0 ? "Betting on: Hand 1" : "Betting on: Hand 2");
        UpdateUI();
    });

    UpdateSkin();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::UpdateUI() {
    GameState state = game->GetState();
    bool anyPlayerTurn = (state == GameState::PlayerTurn || state == GameState::PlayerTurn2);

    if (state == GameState::Finished) {
        ui->gameResult->setText(game->GetResult());
    } else if (anyPlayerTurn) {
        ui->gameResult->setText(state == GameState::PlayerTurn ? "HAND 1 TURN" : "HAND 2 TURN");
        const auto& dCards = game->GetDealerhand().Cards();
    } else {
        ui->gameResult->setText("");
    }

    ui->balanceLabel->setText("Balance: " + QString::number(game->GetBalance()));
    ui->betLabel->setText("Bet 1: " + QString::number(game->GetBet()));
    ui->betLabel2->setText("Bet 2: " + QString::number(game->GetBet2()));

    ui->hitButton->setEnabled(anyPlayerTurn);
    ui->standButton->setEnabled(anyPlayerTurn);
    ui->betZone->setEnabled(state == GameState::Idle);
    ui->switchHandBtn->setEnabled(state == GameState::Idle);

    QString activeStyle = "border: 4px solid #FFD700; border-radius: 10px; background: rgba(255, 215, 0, 0.15);";
    QString idleStyle = "border: 2px solid rgba(255, 255, 255, 0.3); border-radius: 10px;";
    QString betFocusStyle = "border: 2px solid white; border-radius: 8px; background: rgba(255,255,255,0.1);";

    ui->playerHand->setStyleSheet(state == GameState::PlayerTurn ? activeStyle : idleStyle);
    ui->playerHand2->setStyleSheet(state == GameState::PlayerTurn2 ? activeStyle : idleStyle);

    if (state == GameState::Idle) {
        ui->betLabel->setStyleSheet(bettingTarget == 0 ? betFocusStyle : "border:none;");
        ui->betLabel2->setStyleSheet(bettingTarget == 1 ? betFocusStyle : "border:none;");
        ui->newGameButton->setText("Deal");
        // FIXED: Enable Deal if EITHER hand has money
        ui->newGameButton->setEnabled(game->GetBet() > 0 || game->GetBet2() > 0);
    } else if (state == GameState::Finished) {
        ui->newGameButton->setText("Next Round");
        ui->newGameButton->setEnabled(true);
    } else {
        ui->newGameButton->setText("Playing...");
        ui->newGameButton->setEnabled(false);
    }

    auto spawnSprites = [&](const QVector<Card>& cards, QVector<CardSprite*>& sprites, QWidget* targetArea, bool hideFirst) {
        for (int i = sprites.size(); i < cards.size(); ++i) {
            CardSprite* s = new CardSprite(cards[i], currentSkinIndex, ui->centralwidget, (i == 0 && hideFirst));
            s->move(ui->cardDeck->pos());
            s->FlyTo(getLandingPos(targetArea, i), (i < 2 ? i * 150 : 100));
            sprites.append(s);
        }
    };

    spawnSprites(game->GetPlayerHand().Cards(), playerSprites, ui->playerHand, false);
    spawnSprites(game->GetPlayerHand2().Cards(), playerSprites2, ui->playerHand2, false);
    spawnSprites(game->GetDealerhand().Cards(), dealerSprites, ui->dealerHand, anyPlayerTurn);

    if (state == GameState::Finished && !dealerSprites.isEmpty()) dealerSprites[0]->setHidden(false);
}

void MainWindow::clearVisualCards() {
    QPoint discardPos = ui->usedCardDeck->pos();
    auto discard = [&](QVector<CardSprite*>& list) {
        for (auto* s : list) if (s) s->DiscardTo(discardPos);
        list.clear();
    };
    discard(playerSprites);
    discard(playerSprites2);
    discard(dealerSprites);

    QPoint collectPos = (game->GetBalance() > 0) ? ui->betZone->pos() : ui->dealerHand->pos();
    auto collect = [&](QVector<ChipSprite*>& list) {
        for (auto* c : list) if (c) c->Collect(collectPos);
        list.clear();
    };
    collect(hand1Chips);
    collect(hand2Chips);

    bettingTarget = 0;
    ui->switchHandBtn->setText("Betting on: Hand 1");
    QTimer::singleShot(500, discardVisual, &QLabel::show);
}

QPoint MainWindow::getLandingPos(QWidget* targetArea, int cardIndex) {
    return QPoint(targetArea->x() + (cardIndex * 32), targetArea->y());
}

void MainWindow::UpdateSkin() {
    ui->playerHand->SetSkin(currentSkinIndex);
    ui->playerHand2->SetSkin(currentSkinIndex);
    ui->dealerHand->SetSkin(currentSkinIndex);
    QString backPath = QString(":/assets/Cards%1/back.png").arg(currentSkinIndex);
    deckVisual->setPixmap(QPixmap(backPath));
    discardVisual->setPixmap(QPixmap(backPath));
    for (auto* s : playerSprites) s->updatePixmap();
    for (auto* s : playerSprites2) s->updatePixmap();
    for (auto* s : dealerSprites) s->updatePixmap();
    UpdateUI();
}
