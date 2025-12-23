#include "card.h"

Card::Card(Suit suit, Rank rank) : cardSuit(suit), cardRank(rank) {}

int Card::Value() const {return static_cast<int>(cardRank);}

bool Card::IsAce() const {return cardRank == Rank :: Ace;}


