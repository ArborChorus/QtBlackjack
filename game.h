#pragma once

#include <QObject>

#include "deck.h"
#include "hand.h"

#ifndef GAME_H
#define GAME_H

enum class GameState {PlayerTurn, PlayerTurn2, DealerTurn, Finished, Idle};

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject* parent = nullptr);

    void Start();
    void HitPlayer();
    void StandPlayer();

    void ClearBet();

    int GetBet();
    int GetBalance();
    void ResetForNewBet();
    bool CanPlay();

    const Hand& GetPlayerHand() const;


    const Hand& GetPlayerHand2() const;
    int GetBet2();

    const Hand& GetDealerhand() const;
    GameState GetState() const;
    QString GetResult() const;

public slots:
    bool AddBet(int amount, int handIndex);
    void StartRound();

signals:
    void updated();
    void finished();

private:
    Deck deck;
    Hand playerHand;
    int activeHandIndex = 0;
    Hand playerHand2;
    int currentBet2 = 0;
    bool hand2Enabled = false;
    Hand dealerHand;
    GameState gameState;
    QString gameResult;
    int balance;
    int currentBet;

};

#endif // GAME_H
