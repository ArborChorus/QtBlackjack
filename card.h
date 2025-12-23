#pragma once

#ifndef CARD_H
#define CARD_H

enum class Suit {Hearts, Diamonds, Clubs, Spades};

enum class Rank {Two = 2, Three = 3, Four = 4, Five = 5, Six = 6, Seven = 7, Eight = 8, Nine = 9, Ten = 10,
                  Jack = 10, Queen = 10, King = 10, Ace = 11};

class Card
{
public:    
    int value() const;
    bool isAce() const;

    Card(Suit s, Rank r);

private:
    Suit cardSuit;
    Rank cardRank;
};

#endif // CARD_H
