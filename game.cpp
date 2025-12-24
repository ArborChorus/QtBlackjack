#include "game.h"
#include <QDebug>

Game::Game(QObject* parent) : QObject(parent) {}

void Game::Start(){
    deck = Deck();
    deck.Shuffle();

    playerHand.Clear();
    dealerHand.Clear();

    playerHand.AddCard(deck.Draw());
    playerHand.AddCard(deck.Draw());
    dealerHand.AddCard(deck.Draw());
    dealerHand.AddCard(deck.Draw());
    gameState = GameState::PlayerTurn;
    emit updated();
}

void Game::HitPlayer(){
    if(gameState != GameState::PlayerTurn) return;
    playerHand.AddCard(deck.Draw());
    if(playerHand.IsBust()){
        gameState = GameState::Finished;
        gameResult = "Dealer wins";
        emit finished();
    } else {
        emit updated();
    }
}

void Game::StandPlayer(){
    if(gameState != GameState::PlayerTurn) return;
    while (dealerHand.Score() < 17) dealerHand.AddCard(deck.Draw());

    if (dealerHand.IsBust() || dealerHand.Score() < playerHand.Score()) gameResult = "Player wins";
    else if(playerHand.Score() == dealerHand.Score()) gameResult = "Draw";
    else gameResult = "Dealer wins";

    gameState = GameState::Finished;
    emit finished();
}

const Hand& Game::GetPlayerHand() const {return playerHand;}
const Hand& Game::GetDealerhand() const {return dealerHand;}
GameState Game::GetState() const {return gameState;}
QString Game::GetResult() const {return gameResult;}
