#pragma once

#include <QObject>

#include "deck.h"
#include "hand.h"

#ifndef GAME_H
#define GAME_H

enum class GameState {PlayerTurn, DealerTurn, Finished};

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject* parent = nullptr);

    void Start();
    void HitPlayer();
    void StandPlayer();

    const Hand& GetPlayerHand() const;
    const Hand& GetDealerhand() const;
    GameState GetState() const;
    QString GetResult() const;

signals:
    void updated();
    void finished();

private:
    Deck deck;
    Hand playerHand;
    Hand dealerHand;
    GameState gameState;
    QString gameResult;

};

#endif // GAME_H
