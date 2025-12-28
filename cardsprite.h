#ifndef CARDSPRITE_H
#define CARDSPRITE_H

#include "card.h"
#include <qlabel.h>
#include <QPropertyAnimation>
#include <QTimer>

class CardSprite : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QPoint pos READ pos WRITE move);
public:
    CardSprite(const Card& card, int skin, QWidget* parent, bool isHidden = false);
    void FlyTo(QPoint endPos, int delay = 0, bool rotate = false);
    void DiscardTo(QPoint discardPos);
    void setHidden(bool hidden);
    void updatePixmap();

private:
    QString getPath(const Card& card, int skin);
    Card m_card;
    int m_skin;
    bool m_isHidden;

};

#endif // CARDSPRITE_H
