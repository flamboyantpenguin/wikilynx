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

namespace Ui {
class getLevel;
}

class getLevel : public QDialog
{
    Q_OBJECT

public:
    int initialise(QJsonObject*);
    explicit getLevel(QWidget *parent = nullptr);
    ~getLevel();

protected:
    void closeEvent(QCloseEvent *event) override {
        emit this->closed();
        QDialog::closeEvent(event);
    }

private:
    int updateTable();
    QJsonObject* iData, levelData;
    Ui::getLevel *ui;
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

private slots:
    void launchHelp();
    void setEditStatus();
    void deleteLevel(QString);
    void downloadLevel(QString);

signals:
    void closed();
};

#endif // GETLEVEL_H
