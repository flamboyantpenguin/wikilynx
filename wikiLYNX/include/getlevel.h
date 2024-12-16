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
#include <QNetworkAccessManager>


#include "levels.h"

namespace Ui {
class getLevel;
}

class getLevel : public QDialog
{
    Q_OBJECT

public:
    int initialise();
    explicit getLevel(QWidget *parent = nullptr);
    ~getLevel();

protected:
    void closeEvent(QCloseEvent *event) override {
        emit this->closed();
        QDialog::closeEvent(event);
    }

private:
    int updateTable();
    QJsonObject cfg, iData, levelData;
    Ui::getLevel *ui;
    QString dirName = ".wikilynx";

private slots:
    void setEditStatus();
    void deleteLevel(QString);
    void downloadLevel(QString);

signals:
    void closed();
};

#endif // GETLEVEL_H
