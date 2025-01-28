#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QFile>
#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include "scoresheet.h"

namespace Ui {
class LeaderBoard;
}

class LeaderBoard : public QDialog {
    Q_OBJECT

public:
    int initialise(ScoreSheet *);
    explicit LeaderBoard(QWidget *parent = nullptr);
    ~LeaderBoard();

private:
    bool isDarkTheme();
    ScoreSheet *gameData;
    Ui::LeaderBoard *ui;

private slots:
    void loadData(int s = 0);
};

#endif // LEADERBOARD_H
