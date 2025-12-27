#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "betwidget.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    game = new Game(this);

    SetUpSkinSwitcher();

    connect(ui->hitButton, &QPushButton::clicked, game, &Game::HitPlayer);
    connect(ui->standButton, &QPushButton::clicked, game, &Game::StandPlayer);

    connect(ui->newGameButton, &QPushButton::clicked, this, [this](){
        if (game->GetState() == GameState::Idle) {
            game->StartRound();
        } else if (game->GetState() == GameState::Finished) {
            game->Start();
        }
    });

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
    ui->playerHand->SetSkin(currentSkinIndex);
    ui->dealerHand->SetSkin(currentSkinIndex);
}
