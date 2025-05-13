#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QException>

class Oscillator {
public:
    Oscillator();
    int volume = 50;
    void playSound(QString);
    int playAudio(QString fname);

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    // Pre-defined Sounds
    std::map<QString, QString> sounds = {
        { "error", "qrc:/base/audio/error.wav" },
        { "init", "qrc:/base/audio/init.wav" },
        { "yay", "qrc:/base/audio/yay.wav" }
    };
};

#endif // OSCILLATOR_H
