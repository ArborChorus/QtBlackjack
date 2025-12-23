#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    game = new Game(this);

    connect(ui->hitBtn, &QPushButton::clicked,
            game, &Game::HitPlayer);
    connect(ui->standBtn, &QPushButton::clicked,
            game, &Game::StandPlayer);
    connect(ui->newGameBtn, &QPushButton::clicked,
            game, &Game::Start);

    connect(game, &Game::updated,
            this, &MainWindow::UpdateUI);
    connect(game, &Game::finished,
            this, &MainWindow::UpdateUI);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateUI(){
    ui->playerScore->setText(QString::number(game->GetPlayerHand().Score()));
    ui->dealerScore->setText(QString::number(game->GetDealerhand().Score()));

    if(game->GetState() == GameState::Finished) ui->resultLabel->setText(game->GetResult());
    else(ui->resultLabel->setText(""));
}
