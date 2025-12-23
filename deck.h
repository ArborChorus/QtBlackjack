#pragma once

#include "card.h"

#include <QVector>

#ifndef DECK_H
#define DECK_H

class Deck
{
public:
    Deck();

    void Shuffle();
    Card Draw();

private:
    QVector<Card> deckCards;
};

#endif // DECK_H
