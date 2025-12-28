#pragma once

#include <qlabel.h>
#include <QPropertyAnimation>
#include "soundmanager.h"

#ifndef CHIPSPRITE_H
#define CHIPSPRITE_H

class ChipSprite : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QPoint pos READ pos WRITE move)
public:
    ChipSprite(int value, QWidget* parent);
    void FlyTo(QPoint endPos);
    void Collect(QPoint targetPos);
};

#endif // CHIPSPRITE_H
