#include "game.h"
#include <QDebug>

Game::Game(QObject* parent) : QObject(parent) {
    gameState = GameState::Idle;
    balance = 1000;
    currentBet = 0;
}

void Game::Start(){
    playerHand.Clear();
    dealerHand.Clear();
    gameResult = "";
    gameState = GameState::Idle;
    emit updated();
}

void Game::HitPlayer(){
    if(gameState != GameState::PlayerTurn) return;
    playerHand.AddCard(deck.Draw());
    if(playerHand.IsBust()){
        gameState = GameState::Finished;
        gameResult = "Dealer wins";
        currentBet = 0;
        emit finished();
    } else if(playerHand.Score() == 21){
        StandPlayer();
    }
    else{
        emit updated();
    }
}

void Game::StandPlayer(){
    if(gameState != GameState::PlayerTurn) return;

    while (dealerHand.Score() < 17) {
        dealerHand.AddCard(deck.Draw());
    }

    int dScore = dealerHand.Score();
    int pScore = playerHand.Score();

    if (dealerHand.IsBust()) {
        gameResult = "Dealer Busts! Player wins";
        balance += currentBet * 2;
    } else if (dScore < pScore) {
        gameResult = "Player wins";
        balance += currentBet * 2;
    } else if (dScore == pScore) {
        gameResult = "Draw";
        balance += currentBet;
    } else {
        gameResult = "Dealer wins";
    }

    currentBet = 0;
    gameState = GameState::Finished;
    emit finished();
}

void Game::AddBet(int amount){
    if (balance >= amount && gameState == GameState::Idle){
        currentBet += amount;
        balance -= amount;
        emit updated();
    }
}

void Game::ClearBet(){
    balance += currentBet;
    currentBet = 0;
    emit updated();
}

int Game::GetBalance(){
    return balance;
}

int Game::GetBet(){
    return currentBet;
}

void Game::StartRound(){
    if (gameState != GameState::Idle || currentBet <= 0) return;

    deck = Deck();
    deck.Shuffle();
    playerHand.Clear();
    dealerHand.Clear();

    playerHand.AddCard(deck.Draw());
    playerHand.AddCard(deck.Draw());
    dealerHand.AddCard(deck.Draw());
    dealerHand.AddCard(deck.Draw());

    gameState = GameState::PlayerTurn;

    if(playerHand.Score() == 21) StandPlayer();
    else emit updated();
};

const Hand& Game::GetPlayerHand() const {return playerHand;}
const Hand& Game::GetDealerhand() const {return dealerHand;}
GameState Game::GetState() const {return gameState;}
QString Game::GetResult() const {return gameResult;}
