#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    game = new Game(this);

    connect(ui->hitButton, &QPushButton::clicked,
            game, &Game::HitPlayer);
    connect(ui->standButton, &QPushButton::clicked,
            game, &Game::StandPlayer);

    connect(game, &Game::updated,
            this, &MainWindow::UpdateUI);
    connect(game, &Game::finished,
            this, &MainWindow::UpdateUI);

    connect(ui->newGameButton, &QPushButton::clicked,
            game, &Game::Start);

    game->Start();
    UpdateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateUI(){
    ui->playerScore->setText(QString::number(game->GetPlayerHand().Score()));
    ui->dealerScore->setText(QString::number(game->GetDealerhand().Score()));

    if(game->GetState() == GameState::Finished) ui->gameResult->setText(game->GetResult());
    else(ui->gameResult->setText(""));
}
