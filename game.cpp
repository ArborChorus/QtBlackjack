#include "game.h"
#include <QDebug>

Game::Game(QObject* parent) : QObject(parent) {
    gameState = GameState::Idle;
    balance = 1000;
    currentBet = 0;
    currentBet2 = 0;
    activeHandIndex = 0;
    hand2Enabled = false;
}

void Game::Start() {
    playerHand.Clear();
    playerHand2.Clear();
    dealerHand.Clear();
    gameResult = "";
    currentBet = 0;
    currentBet2 = 0;
    activeHandIndex = 0;
    hand2Enabled = false;
    gameState = GameState::Idle;
    emit updated();
}

void Game::HitPlayer() {
    if (gameState != GameState::PlayerTurn && gameState != GameState::PlayerTurn2) return;

    Hand& currentHand = (activeHandIndex == 0) ? playerHand : playerHand2;
    currentHand.AddCard(deck.Draw());

    if (currentHand.IsBust()) {
        StandPlayer();
    } else {
        emit updated();
    }
}

void Game::StandPlayer() {
    if (gameState != GameState::PlayerTurn && gameState != GameState::PlayerTurn2) return;

    if (activeHandIndex == 0 && hand2Enabled) {
        activeHandIndex = 1;
        gameState = GameState::PlayerTurn2;
        if (playerHand2.Score() >= 21) {
            StandPlayer();
        } else {
            emit updated();
        }
        return;
    }

    gameState = GameState::DealerTurn;
    while (dealerHand.Score() < 17) {
        dealerHand.AddCard(deck.Draw());
    }

    int dScore = dealerHand.Score();
    bool dVal = !dealerHand.IsBust();

    auto evaluate = [&](const Hand& h, int bet, QString label) -> QString {
        if (h.IsBust()) return label + ": Bust (Lose)";
        int pScore = h.Score();
        if (!dVal || pScore > dScore) {
            balance += bet * 2;
            return label + ": Win";
        }
        if (pScore == dScore) {
            balance += bet;
            return label + ": Draw";
        }
        return label + ": Lose";
    };

    gameResult = evaluate(playerHand, currentBet, "Hand 1");
    if (hand2Enabled) {
        gameResult += " | " + evaluate(playerHand2, currentBet2, "Hand 2");
    }

    currentBet = 0;
    currentBet2 = 0;
    gameState = GameState::Finished;
    emit finished();
}

bool Game::AddBet(int amount, int handIndex) {
    if (gameState != GameState::Idle || balance < amount) return false;

    if (handIndex == 0) {
        currentBet += amount;
    } else {
        currentBet2 += amount;
        hand2Enabled = true;
    }
    balance -= amount;
    emit updated();
    return true;
}

void Game::ClearBet() {
    if (gameState != GameState::Idle) return;
    balance += (currentBet + currentBet2);
    currentBet = 0;
    currentBet2 = 0;
    hand2Enabled = false;
    emit updated();
}

int Game::GetBalance() { return balance; }
int Game::GetBet() { return currentBet; }
int Game::GetBet2() { return currentBet2; }

void Game::StartRound() {
    if (gameState != GameState::Idle || (currentBet <= 0 && currentBet2 <= 0)) return;

    deck = Deck();
    deck.Shuffle();
    playerHand.Clear();
    playerHand2.Clear();
    dealerHand.Clear();

    if (currentBet > 0) {
        playerHand.AddCard(deck.Draw());
        playerHand.AddCard(deck.Draw());
    }

    if (currentBet2 > 0) {
        playerHand2.AddCard(deck.Draw());
        playerHand2.AddCard(deck.Draw());
        hand2Enabled = true;
    } else {
        hand2Enabled = false;
    }

    dealerHand.AddCard(deck.Draw());
    dealerHand.AddCard(deck.Draw());

    if (currentBet > 0) {
        activeHandIndex = 0;
        gameState = GameState::PlayerTurn;
        if (playerHand.Score() >= 21) StandPlayer();
        else emit updated();
    } else {
        activeHandIndex = 1;
        gameState = GameState::PlayerTurn2;
        if (playerHand2.Score() >= 21) StandPlayer();
        else emit updated();
    }
}

const Hand& Game::GetPlayerHand() const { return playerHand; }
const Hand& Game::GetPlayerHand2() const { return playerHand2; }
const Hand& Game::GetDealerhand() const { return dealerHand; }
GameState Game::GetState() const { return gameState; }
QString Game::GetResult() const { return gameResult; }
