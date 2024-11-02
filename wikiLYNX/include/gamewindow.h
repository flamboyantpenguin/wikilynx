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


    int chk = 0;
    int alD = 1;
    int tChk = 0;
    float endTime = 0;
    float cCount = 0;
    float countdown = 0;
    QString aTime, instance, domain;
    QTimer *timer = new QTimer(this);

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    congrats congratsView;

    QJsonObject cfg;
    int *dontKillMe;
    int initialise(QJsonObject*, int*, QString, int, QString, QString);

private:
    QString dirName = ".wLnKMeow";
    QString gamer, level;
    Ui::GameWindow *ui;
    viewHistory historyView;
    viewcheckpoint checkpointView;

private slots:
    void launchLogs();
    void viewCheckPoints();
    int missionAccomplished();
    int missionFailed();
    void updateCountdown();
    void initAction();
    void endGame();

};

#endif // GAMEWINDOW_H
