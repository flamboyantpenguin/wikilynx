#ifndef GAMEBOI_H
#define GAMEBOI_H

#include <QTimer>
#include <filesystem>
#include <QStandardPaths>

#include "scoresheet.h"
#include "oscillator.h"

class GameBoi : public QObject {
    Q_OBJECT

public:

    int getChk(); // Get current Ch
    void startGame(); // Start the game system
    QStringList getLevels();

    // Game Variables
    QStringList logs;

    QString getCurrentChk(); // Get Current Checkpoint URL

    GameBoi(ScoreSheet* gameData, QString, QString);

private:
    // Game Variables
    int chk = 0; // Checkpoint status (Numeber of checkpoints cleared)
    QString gamer; // Player Name
    int clicks = 0; // Click Counter
    QString instance; // Name of the game instance (Date+Time)
    double countup = 0; // Countup (Basically countdown but upwards)

    QTimer *timer = new QTimer();

    // GameInfo
    QString levelName;
    QStringList levels;

    // Subsystems
    ScoreSheet *gameData; // This one handles gameData
    Oscillator *soundSystem; // This one handles soundSystem

    // This is appended before levels meant be wiki only
    QString wikiURL = "https://wikipedia.org/wiki/"; // URL of Wikipedia

    QStringList endCodes = {"WIN", "TMT", "CKL", "ABR"}; // Game End Codes

    void emitClicks(); // Emits Click Status and Next Checkpoint URL


public slots:
    void click(QString);
    void endGame(int code = 3);

private slots:
    void updateCountdown();


signals:
    void counter(QString);
    void clicked(QString, QString);
    void sendMessage(QString);
    void chkSuccess(int);
    void end(QJsonObject, int);

};

#endif // GAMEBOI_H
