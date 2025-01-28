#ifndef STATUSOVERVIEW_H
#define STATUSOVERVIEW_H

#include <QUrl>
#include <QThread>
#include <QDialog>
#include <QMessageBox>
#include <QDesktopServices>

#include "whatsnew.h"


namespace Ui {
class StatusOverview;
}

class StatusOverview : public QDialog {
    Q_OBJECT

public:
    explicit StatusOverview(QWidget *parent = nullptr);
    ~StatusOverview();

private:
    int hehe = 0;
    QString cVersion = "1.5.6";
    std::map<int, QString> code = {
       { 0, "Offline|Offline" },
       { 1, "Online|Online" },
       { 2, "Online|Online" },
       { 3, "Online|Offline" },
       { 4, "Online|Online" },
    };

    WhatsNew *whatsnew = nullptr;
    Ui::StatusOverview *ui;

public slots:
    void initialise(int, QString);

private slots:
    void developerHehe();
    void launchVersionInfo();
    void launchMaintenanceTool();
    void launchLatestVersionInfo();

signals:
    void devEnabled();

};

#endif // STATUSOVERVIEW_H
