#ifndef STATUSOVERVIEW_H
#define STATUSOVERVIEW_H

#include <QUrl>
#include <QThread>
#include <QDialog>
#include <QMessageBox>
#include <QDesktopServices>


namespace Ui {
class statusOverview;
}

class statusOverview : public QDialog
{
    Q_OBJECT

public:
    explicit statusOverview(QWidget *parent = nullptr);
    ~statusOverview();

private:

    int hehe = 0;
    QString cVersion = "1.5.5";
    std::map<int, QString> code = {
       { 0, "Offline|Offline" },
       { 1, "Online|Online" },
       { 2, "Online|Online" },
       { 3, "Online|Offline" },
       { 4, "Online|Online" },
    };

    Ui::statusOverview *ui;

public slots:
    void initialise(int);

private slots:
    void developerHehe();
    void launchVersionInfo();
    void launchMaintenanceTool();
    void launchLatestVersionInfo();
};

#endif // STATUSOVERVIEW_H
