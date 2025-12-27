#include "betwidget.h"
#include <QPushButton>
#include <QHBoxLayout>

BetWidget::BetWidget(QWidget *parent)
    : QWidget{parent}
{
    auto* layout = new QHBoxLayout(this);

    QPushButton* chip10 = new QPushButton("10");
    QPushButton* chip50 = new QPushButton("50");
    QPushButton* chip100 = new QPushButton("100");

    layout->addWidget(chip10);
    layout->addWidget(chip50);
    layout->addWidget(chip100);

    connect(chip10, &QPushButton::clicked, this, [this]() {
        emit BetClicked(10);
    });
    connect(chip50, &QPushButton::clicked, this, [this]() {
        emit BetClicked(50);
    });
    connect(chip100, &QPushButton::clicked, this, [this]() {
        emit BetClicked(100);
    });
}
