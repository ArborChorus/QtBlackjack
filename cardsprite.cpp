#include "cardsprite.h"

CardSprite::CardSprite(const Card& card, int skin, QWidget* parent, bool isHidden)
    : QLabel(parent), m_card(card), m_skin(skin), m_isHidden(isHidden)
{
    setFixedSize(96, 134);
    setScaledContents(true);
    updatePixmap();
    show();
    raise();
}

void CardSprite::FlyTo(QPoint endPos, int delay, bool rotate){
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(600);
    anim->setStartValue(pos());
    anim->setEndValue(endPos);
    anim->setEasingCurve(QEasingCurve::OutCubic);

    QTimer::singleShot(delay, [anim]() { anim->start(QAbstractAnimation::DeleteWhenStopped); });
}

void CardSprite::DiscardTo(QPoint discardPos) {
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(400); // Speed of discarding
    anim->setStartValue(this->pos());
    anim->setEndValue(discardPos);
    anim->setEasingCurve(QEasingCurve::InCubic);

    // This is the most important part:
    connect(anim, &QPropertyAnimation::finished, this, &CardSprite::deleteLater);

    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void CardSprite::setHidden(bool hidden) {
    if (m_isHidden != hidden) {
        m_isHidden = hidden;
        updatePixmap();
    }
}

void CardSprite::updatePixmap() {
    if (m_isHidden) {
        setPixmap(QPixmap(QString(":/assets/Cards%1/back.png").arg(m_skin)));
    } else {
        // Your existing logic to find the card rank/suit path
        QString suit;
        switch(m_card.getSuit()){
        case Suit::Spades:   suit = "S"; break;
        case Suit::Hearts:   suit = "H"; break;
        case Suit::Clubs:    suit = "C"; break;
        case Suit::Diamonds: suit = "D"; break;
        }
        QString rank;
        switch(m_card.getRank()){
        case Rank::Ace:   rank = "A"; break;
        case Rank::Jack:  rank = "J"; break;
        case Rank::Queen: rank = "Q"; break;
        case Rank::King:  rank = "K"; break;
        default:          rank = QString::number((int)m_card.getRank());
        }
        setPixmap(QPixmap(QString(":/assets/Cards%1/%2%3.png").arg(m_skin).arg(rank).arg(suit)));
    }
}


