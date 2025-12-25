#include "hand.h"

Hand::Hand() {}

const QVector<Card>& Hand::Cards() const {
    return handCards;
}

void Hand::AddCard(const Card& card){
    handCards.append(card);
}

void Hand::Clear(){
    handCards.clear();
}

int Hand::Score() const {
    int total = 0;
    int aces = 0;
    for(const Card& card : handCards){
        total += card.Value();
        if(card.IsAce()) aces++;
    }
    while(total > 21 && aces > 0){
        total -= 10;
        aces--;
    }
    return total;
}

bool Hand::IsBust() const {
    return Score() > 21;
}
