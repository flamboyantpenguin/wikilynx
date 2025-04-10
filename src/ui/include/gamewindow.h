#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QString>
#include <QMessageBox>
#include <QMainWindow>

#include "baselist.h"
#include "congrats.h"

#include <include/gameboi.h>
#include <include/scoresheet.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QMainWindow {
    Q_OBJECT


public:
    GameWindow(GameBoi *gameSystem, int *dontKillMeParse, QString prgHex,
               QWidget *parent = nullptr);
    ~GameWindow();

    int *dontKillMe;
    Congrats congratsView;

private:
    std::map<int, QString> endCodes = {
       { 0, "You won!"},
       { 1, "Timeout!"},
       { 2, "Max Clicks Reached!" },
       { 3, "Game Aborted!" }
    };


    std::map<QString, QString> messageCodes = {
        { "RLV", "Rule Violation! You're not allowed to visit sites outide wikipedia.org in this level! Change this in settings" }
    };

    BaseList *baselist;
    Ui::GameWindow *ui;

    GameBoi *gameSystem;

private slots:
    void startGame();
    void click(QUrl);
    void launchLogs();
    bool isDarkTheme();
    void updatePRG(int);
    void viewCheckPoints();
    void updateCountdown(QString);
    int endGame(QJsonObject, int);
    void showMessage(QString warning);
    void updateClicks(QString, QString);


signals:
    void gameEnded(QString, QJsonObject);

};

#endif // GAMEWINDOW_H
