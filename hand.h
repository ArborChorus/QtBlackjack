#pragma once

#include <QVector>

#include "card.h"

#ifndef HAND_H
#define HAND_H

class Hand
{
public:
    Hand();

    void AddCard(const Card& card);
    int Score() const;
    bool IsBust() const;
    void Clear();

private:
    QVector<Card> handCards;
};

#endif // HAND_H
