#pragma once

#include <QWidget>
#include "hand.h"

#ifndef CARDWIDGET_H
#define CARDWIDGET_H

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget* parent = nullptr);

    void SetHand(const Hand* hand);
    void SetHideFirstCard(bool ishide);

    void SetSkin(int skinIndex);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    const Hand* hand = nullptr;
    bool hideFirst = false;
    int currentSkin;

    QPixmap cardPixmap(const Card& card) const;
};

#endif // CARDWIDGET_H
