#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QFile>
#include <QTimer>
#include <QString>
#include <fstream>
#include <filesystem>
#include <QMessageBox>
#include <QJsonObject>
#include <QMainWindow>
#include <QStandardPaths>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>

#include "baselist.h"
#include "congrats.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QMainWindow {
    Q_OBJECT


public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    int *dontKillMe;
    Congrats congratsView;
    int initialise(QJsonObject*, int*, QString, int, QString, QString);

private:
    // Game Variables
    int chk = 0;
    int clicks = -1;
    float endTime = 0;
    float countup = 0;

    std::map<QString, QString> code = {
       { "Aborted!", "Game Aborted!" },
       { "Win!", "You won!" },
       { "Timeout!", "Timeout!" },
       { "MaxClicks!", "Max Clicks Reached" }
    };


    int alD = 1;
    bool domain;
    QStringList log;
    QStringList levels;
    BaseList *baselist;
    Ui::GameWindow *ui;
    QJsonObject gameData;
    QString gamer, level;
    QString instance;
    QTimer *timer = new QTimer(this);
    QString wikiURL = "https://wikipedia.org/wiki/";


private slots:
    void playSound(QString sound);
    void launchLogs();
    bool isDarkTheme();
    void viewCheckPoints();
    void updateCountdown();
    void initAction();
    int endGame(QString message = "Aborted!");

signals:
    void gameEnded(QString, QJsonObject);

};

#endif // GAMEWINDOW_H
