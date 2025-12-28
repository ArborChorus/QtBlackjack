#include "betwidget.h"
#include <QPushButton>
#include <QHBoxLayout>

BetWidget::BetWidget(QWidget *parent)
    : QWidget{parent}
{
    auto* layout = new QHBoxLayout(this);

    QPushButton* chip10 = new QPushButton(this);
    QPushButton* chip50 = new QPushButton(this);
    QPushButton* chip100 = new QPushButton(this);

    layout->addWidget(chip10);
    layout->addWidget(chip50);
    layout->addWidget(chip100);

    auto setupBtn = [this](QPushButton* btn, int value) {
        btn->setFixedSize(60, 60);

        btn->setStyleSheet(QString(
                               "QPushButton { "
                               "  border: none; "
                               "  background-image: url(:/assets/Chips/chip%1.png); "
                               "  background-repeat: no-repeat; "
                               "  background-position: center; "
                               "  background-color: transparent;"
                               "}"
                               "QPushButton:hover { margin-bottom: 5px; }"
                               ).arg(value));

        // Fix the connection logic
        connect(btn, &QPushButton::clicked, this, [this, btn, value]() {
            // mapTo(window(), QPoint(0,0)) finds where the button is
            // relative to the very top-left of the game window
            QPoint globalPos = btn->mapTo(this->window(), QPoint(0, 0));
            emit BetClicked(value, globalPos);
        });
    };

    setupBtn(chip10, 10);
    setupBtn(chip50, 50);
    setupBtn(chip100, 100);
}
