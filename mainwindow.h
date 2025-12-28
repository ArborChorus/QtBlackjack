#pragma once

#include "cardsprite.h"
#include <qlabel.h>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Game* game;

    QLabel* backPreviewLabel;
    int currentSkinIndex = 1;
    const int MAX_SKINS = 3;

    void SetUpSkinSwitcher();
    void UpdateSkin();
    QLabel* skinPreviewLabel;

    QVector<CardSprite*> playerSprites;
    QVector<CardSprite*> dealerSprites;
    void clearVisualCards();
    void animateDealing();

    QPoint getLandingPos(QWidget* targetArea, int cardIndex);

    QLabel* deckVisual;
    QLabel* discardVisual;
    void updateDeckSkins();

private slots:
    void UpdateUI();
};
#endif // MAINWINDOW_H
