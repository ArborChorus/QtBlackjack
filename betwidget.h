#ifndef BETWIDGET_H
#define BETWIDGET_H

#include <QWidget>

class BetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BetWidget(QWidget *parent = nullptr);

signals:
    void BetClicked(int amount, QPoint startPos);
};

#endif // BETWIDGET_H
