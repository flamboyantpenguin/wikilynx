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
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>


#include "congrats.h"
#include "viewhistory.h"
#include "viewcheckpoint.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QMainWindow
{
    Q_OBJECT


public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    int *dontKillMe;
    congrats congratsView;
    int initialise(QJsonObject*, int*, QString, int, QString, QString);

private:

    // Game Variables
    int chk = 0;
    int clicks = -1;


    int alD = 1;
    bool domain;
    float endTime = 0;
    float countup = 0;
    QStringList levels;
    Ui::GameWindow *ui;
    QJsonObject gameData;
    QString gamer, level;
    QString aTime, instance;
    viewHistory historyView;
    QString dirName = ".wikilynx";
    viewcheckpoint checkpointView;
    QTimer *timer = new QTimer(this);
    QString wikiURL = "https://wikipedia.org/wiki/";

private slots:
    void playSound(QString sound);
    void launchLogs();
    void viewCheckPoints();
    int missionAccomplished();
    int missionFailed(QString message);
    void updateCountdown();
    void initAction();
    void endGame();

};

#endif // GAMEWINDOW_H
