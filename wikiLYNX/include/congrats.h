#ifndef CONGRATS_H
#define CONGRATS_H

#include <QFile>
#include <iomanip>
#include <QDialog>
#include <fstream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>

#include "baselist.h"
#include "basebrowser.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class Congrats;
}
QT_END_NAMESPACE

class Congrats : public QDialog {
    Q_OBJECT

public:
    explicit Congrats(QWidget *parent = nullptr);
    ~Congrats();

    //int chk;
    //int tChk;
    void initialise(QJsonObject, QString);

private:
    QJsonObject data;

    BaseList *baselist = nullptr;
    BaseBrowser *feedbackBrowser = nullptr;

    Ui::Congrats *ui;

protected:
    void closeEvent(QCloseEvent *event) override {
        emit closed();
        QDialog::closeEvent(event);
    }


private slots:
    void viewhistory();
    bool isDarkTheme();
    void launchFeedBack();

signals:
    void closed();

};

#endif // CONGRATS_H
