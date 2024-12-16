#ifndef CONGRATS_H
#define CONGRATS_H

#include <QFile>
#include <iomanip>
#include <QDialog>
#include <fstream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDesktopServices>

#include "viewstats.h"
#include "viewhistory.h"
#include "viewcheckpoint.h"

#define theGrandPlayers 12


QT_BEGIN_NAMESPACE
namespace Ui {
class congrats;
}
QT_END_NAMESPACE

class congrats : public QDialog
{
    Q_OBJECT

public:
    explicit congrats(QWidget *parent = nullptr);
    ~congrats();

    //int chk;
    //int tChk;
    int chk;
    void initialise(QString, QString, QString, QString, QString, QString, QString, int, int);

private:
    QJsonObject data;
    viewHistory hView;
    viewcheckpoint cView;
    viewStats statsDialog;
    Ui::congrats *ui;
    QJsonObject statData;
    void genReport();
    void updateStats();
    QString dirName = ".wikilynx";

protected:

    void closeEvent(QCloseEvent *event) override {
        emit closed();
    }


private slots:
    void showStats();
    void viewhistory();
    void launchFeedBack();

signals:
    void closed();

};

#endif // CONGRATS_H
