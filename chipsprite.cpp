#include "chipsprite.h"

ChipSprite::ChipSprite(int value, QWidget* parent) :QLabel(parent)
{
    setFixedSize(50, 50);
    setScaledContents(true);

    setPixmap(QPixmap(QString(":/assets/Chips/chip%1.png").arg(value)));

    show();
    raise();
}

void ChipSprite::FlyTo(QPoint endPos) {
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(500);
    anim->setStartValue(pos());
    anim->setEndValue(endPos);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    SoundManager::instance().playChip();
}

void ChipSprite::Collect(QPoint targetPos) {
    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setDuration(400);
    anim->setEndValue(targetPos);
    connect(anim, &QPropertyAnimation::finished, this, &ChipSprite::deleteLater);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
