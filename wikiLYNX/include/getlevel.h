#ifndef GETLEVEL_H
#define GETLEVEL_H

#include <QUrl>
#include <QFile>
#include <QDialog>
#include <QStatusBar>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QNetworkAccessManager>


#include "levels.h"
#include "scoresheet.h"

namespace Ui {
class getLevel;
}

class getLevel : public QDialog
{
    Q_OBJECT

public:
    int initialise(ScoreSheet*);
    explicit getLevel(QWidget *parent = nullptr);
    ~getLevel();

protected:
    void closeEvent(QCloseEvent *event) override {
        emit this->closed();
        QDialog::closeEvent(event);
    }

private:
    int updateTable();
    ScoreSheet *gameData;
    QJsonObject levelData;
    Ui::getLevel *ui;
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

private slots:
    void launchHelp();
    void setEditStatus();
    void downloadLevel(QString);

signals:
    void closed();
    void levelsUpdated();
};

#endif // GETLEVEL_H
