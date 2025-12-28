#pragma once
#include <QSoundEffect>
#include <QUrl>
#include <qdebug.h>

class SoundManager {
public:
    static SoundManager& instance() {
        static SoundManager inst;
        return inst;
    }

    void playCard()  { cardEffect.play(); }
    void playChip()  { chipEffect.play(); }


private:
    SoundManager() {
        setupEffect(cardEffect,  ":/assets/Sounds/card_slide.wav");
        setupEffect(chipEffect,  ":/assets/Sounds/chip_bet.wav");
    }

    void setupEffect(QSoundEffect& effect, const QString& path) {
        effect.setSource(QUrl("qrc" + path));
        if (effect.status() == QSoundEffect::Error) {
            qDebug() << "Sound Error:" << path << "is missing or invalid!";
        }
    }

    QSoundEffect cardEffect;
    QSoundEffect chipEffect;
};
