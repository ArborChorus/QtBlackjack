#include "card.h"

Card::Card(Suit suit, Rank rank) : cardSuit(suit), cardRank(rank) {}

int Card::Value() const {
    switch(cardRank) {
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:
        return 10;
    case Rank::Ace:
        return 11;
    default:
        return static_cast<int>(cardRank);
    }
}

bool Card::IsAce() const {return cardRank == Rank :: Ace;}

Rank Card::getRank() const{return cardRank;}

Suit Card::getSuit() const{return cardSuit;}
