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


bool isDarkTheme();
void loadFonts();
void loadIconTheme();
void checkUpdate();
int checkInternet();
void downloadUpdate();
void onFocusChanged(QWidget *oldFocus, QWidget *newFocus);


int totem = 0;
int dontKill = 1;
QApplication *app;
std::string lVersion("1.2.8-1");
std::string version("1.2.8-1");


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    app = &a;

    loadFonts();
    loadIconTheme();
    qDebug() << QIcon::themeName();

    welcomeUI dialog;
    QObject::connect(app, &QApplication::focusChanged, onFocusChanged);

    if (checkInternet()) {
        QString msg("It seems your system cannot access wikipedia.org. Please check your internet and try again!");
        QMessageBox::critical(nullptr, "wikiLYNX", msg, QMessageBox::Ok);
        return 1;
    }

    checkUpdate();
    loadingScreen banner;
    banner.setWindowFlags(Qt::FramelessWindowHint);
    banner.show();
    app->exec();
    QThread::msleep(500);
    dialog.dontKillParse0 = &dontKill;
    dialog.initialise(&totem);
    dialog.show();

    return app->exec();
}


int checkInternet() {
#ifdef Q_OS_WIN

    return QProcess::execute("ping", QStringList() << "wikipedia.org");  // Replace with wikipedia.org if preferred

#else
    return QProcess::execute("ping", QStringList() << "-c" << "3" << "wikipedia.org"); // Replace with wikipedia.org if preferred

#endif
}


void loadFonts() {
    QFontDatabase::addApplicationFont(":/base/fonts/CourierPrime-Bold.ttf");
    QFontDatabase::addApplicationFont(":/base/fonts/CourierPrime-Regular.ttf");
    QFontDatabase::addApplicationFont(":/base/fonts/NotoSans-VariableFont_wdth,wght.ttf");
    QFontDatabase::addApplicationFont(":/base/fonts/NotoSans-Regular.ttf");
}


void loadIconTheme() {

    if (isDarkTheme()) {
        QIcon::setThemeName("materialDark");
        return;
    }
    QIcon::setThemeName("materialLight");
}


bool isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}



void checkUpdate() {

    QNetworkAccessManager manager;
    QUrl url("https://repo.pcland.co.in/QtOnline/wikiLYNX/.info/version.txt");

    QNetworkReply *reply = manager.get(QNetworkRequest(url));

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->read(7);
        lVersion = QString::fromLocal8Bit(data).toStdString();
    } else {
        qDebug() << "Error fetching latest version information:" << reply->errorString();
    }

    reply->deleteLater();

    if (strcmp(lVersion.c_str(), version.c_str()) > 0) {
        downloadUpdate();
    }
    if (strcmp(lVersion.c_str(), version.c_str()) < 0) {
        qDebug() << "Meow";
    }

}


void downloadUpdate() {

    if (QSysInfo::productType() != "windows") {
        QString msg("New Update available! Check your package manager for the latest version");
        QMessageBox::information(nullptr, "wikiLYNX", msg, QMessageBox::Ok);
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "wikiLYNX", "New Update Available. Do you want to update?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./maintenancetool.exe"));
    }
}


void onFocusChanged(QWidget *oldFocus, QWidget* newFocus) {
    if(newFocus == nullptr && !dontKill && !totem) {
        //QMessageBox::critical(nullptr, "wikiLYNX", "Game Rule Violiation! You're not allowed to switch windows during game session", QMessageBox::Ok);
        //std::ofstream out("./gData/"+instance+"/report.txt", std::ios_base::app);
        qDebug() << "Game Ended Abruptly due to Game Policy Violation";
        //out.close();
        //app->quit();
        QApplication::quit();
        QMessageBox::critical(nullptr, "wikiLYNX", "Game Rule Violation! You're not allowed to switch windows during game session", QMessageBox::Ok);
        return;
    }
}
