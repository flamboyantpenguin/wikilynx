#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QFile>
#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace Ui {
class leaderboard;
}

class leaderboard : public QDialog
{
    Q_OBJECT

public:
    int initialise();
    explicit leaderboard(QWidget *parent = nullptr);
    ~leaderboard();

private:
    bool isDarkTheme();
    QJsonObject data;
    Ui::leaderboard *ui;
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

private slots:
    void loadData(int s = 0);
};

#endif // LEADERBOARD_H
