#pragma once

#ifndef CARD_H
#define CARD_H

enum class Suit {Hearts, Diamonds, Clubs, Spades};

enum class Rank {
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

class Card
{
public:
    Card(Suit suit, Rank rank);

    int Value() const;
    bool IsAce() const;
    Rank getRank() const;
    Suit getSuit() const;

private:
    Suit cardSuit;
    Rank cardRank;
};

#endif // CARD_H
