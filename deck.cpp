#include "deck.h"

#include <random>

Deck::Deck() {
    for (int suit = 0; suit < 4; suit++){
        for (int rank = 1; rank <= 13; rank++){
            deckCards.append(Card(static_cast<Suit>(suit), static_cast<Rank>(rank)));
        }
    }
}

void Deck::Shuffle(){
    std::shuffle(deckCards.begin(), deckCards.end(), std::mt19937{std::random_device{}()});
}

Card Deck::Draw(){
    return deckCards.takeLast();
}
