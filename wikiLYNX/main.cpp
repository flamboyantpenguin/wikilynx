#include "include/welcome.h"
#include "include/loadingscreen.h"
#include "ui/ui_welcome.h"
#include "ui/ui_loadingscreen.h"

#include <fstream>
#include <QDialog>
#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>
#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QProcess>
#include <QSysInfo>


void loadFonts();
void downloadUpdate();
void onFocusChanged(QWidget *oldFocus, QWidget *newFocus);


int totem = 0;
int dontKill = 1;
QApplication *app;
std::string lVersion("1.5.0-1");
std::string version("1.5.0-1");


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    loadFonts();

    loadingScreen banner;
    banner.setWindowFlags(Qt::FramelessWindowHint);
    banner.show();
    app->exec();
    //QThread::msleep(500);

    welcomeUI dialog;

    dialog.dontKillParse0 = &dontKill;
    dialog.initialise(&totem);
    dialog.setWindowFlags(Qt::WindowStaysOnTopHint);
    dialog.show();

    QObject::connect(&a, &QApplication::focusChanged, onFocusChanged);

    return app->exec();
}


void loadFonts() {
    QFontDatabase::addApplicationFont(":/base/fonts/CourierPrime-Bold.ttf");
    QFontDatabase::addApplicationFont(":/base/fonts/CourierPrime-Regular.ttf");
    QFontDatabase::addApplicationFont(":/base/fonts/NotoSans-VariableFont_wdth,wght.ttf");
    QFontDatabase::addApplicationFont(":/base/fonts/NotoSans-Regular.ttf");
}


void onFocusChanged(QWidget *oldFocus, QWidget* newFocus) {
    if(newFocus == nullptr && !dontKill && !totem) {
        //QMessageBox::critical(nullptr, "wikiLYNX", "Game Rule Violiation! You're not allowed to switch windows during game session", QMessageBox::Ok);
        //std::ofstream out("./gData/"+instance+"/report.txt", std::ios_base::app);
        //out.close();
        qDebug() << "Game Ended Abruptly due to Game Policy Violation";
        QApplication::quit();
        QMessageBox::critical(nullptr, "wikiLYNX", "Game Rule Violation! You're not allowed to switch windows during game session", QMessageBox::Ok);
        return;
    }
}
