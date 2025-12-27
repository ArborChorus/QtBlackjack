#include "cardwidget.h"
#include <QPainter>

CardWidget::CardWidget(QWidget* parent) : QWidget(parent)
{
    setMinimumHeight(120);
    currentSkin = 1;
}

void CardWidget:: SetHand(const Hand* h){
    hand = h;
    update();
}

void CardWidget::SetHideFirstCard(bool hide){
    hideFirst = hide;
    update();
}

void CardWidget::paintEvent(QPaintEvent*){
    if(!hand) return;

    QPainter p(this);
    int x = 0;
    const int w = 100;
    const int h =150;
    const int overlap = 60;

    const auto& cards = hand->Cards();

    for(int i = 0; i<cards.size(); ++i){
        QPixmap pix;

        if(hideFirst && i == 0) pix = QPixmap(QString(":/assets/Cards%1/back.png").arg(currentSkin));
        else pix = cardPixmap(cards[i]);

        p.drawPixmap(x, 0, w, h, pix);

        x += overlap;
    }

}

QPixmap CardWidget::cardPixmap(const Card& card) const{
    QString suit;

    switch(card.getSuit()){
    case Suit::Spades: suit = "S"; break;
    case Suit::Hearts: suit = "H"; break;
    case Suit::Clubs: suit = "C"; break;
    case Suit::Diamonds: suit = "D"; break;
    }

    QString rank;

    switch(card.getRank()){
    case Rank::Ace : rank = "A"; break;
    case Rank::Queen : rank = "Q"; break;
    case Rank::King : rank = "K"; break;
    case Rank::Jack : rank = "J"; break;
    default : rank = QString::number(static_cast<int>(card.getRank()));
    }

    QString path = QString(":/assets/Cards%1/%2%3.png").arg(currentSkin).arg(rank).arg(suit);
    qDebug() << path;
    return QPixmap(path);
}

void CardWidget::SetSkin(int skinIndex){
    currentSkin = skinIndex;
    update();
}



















