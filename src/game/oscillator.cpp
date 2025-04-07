#include "include/oscillator.h"

Oscillator::Oscillator() {

    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;

}

void Oscillator::playSound(QString sound) {
    this->playAudio(sounds.at(sound));
}


int Oscillator::playAudio(QString fname) {

    try {
        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            player->setAudioOutput(audioOutput);
            player->setSource(QUrl(fname));
            audioOutput->setVolume(volume);
        #else
            player->setMedia(QUrl(fname));
            player->setVolume(volume);
        #endif
        player->play();
    }
    catch (QException e) {
        //qDebug() << e.what();
        return 1;
    }
    return 0;

}
