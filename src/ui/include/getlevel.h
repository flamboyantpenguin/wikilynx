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
#include "include/scoresheet.h"

namespace Ui {
class GetLevel;
}

class GetLevel : public QDialog {
    Q_OBJECT

public:
    int initialise(ScoreSheet*);
    explicit GetLevel(QWidget *parent = nullptr);
    ~GetLevel();

protected:
    void closeEvent(QCloseEvent *event) override {
        emit this->closed();
        QDialog::closeEvent(event);
    }

private:
    int updateTable();
    ScoreSheet *gameData;
    QJsonObject levelData;
    Ui::GetLevel *ui;

private slots:
    void launchHelp();
    void setEditStatus();
    void downloadLevel(QString);

signals:
    void closed();
    void levelsUpdated();
};

#endif // GETLEVEL_H
