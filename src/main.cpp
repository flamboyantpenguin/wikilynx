#include <include/welcome.h>
#include <include/modernstarter.h>
#include <include/loadingscreen.h>

//#include <fstream>
#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include <QFontDatabase>

void loadFonts();
void onFocusChanged(QWidget *oldFocus, QWidget *newFocus);


int totem = 0;
int dontKill = 1;
QApplication *app;
QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    loadFonts();

    if (!(QFile(dirName+"/wikilynx/.disableSplash").exists() || getenv("WIKILYNX_DISABLESPLASH") != NULL)) {
        LoadingScreen banner;
        banner.setWindowFlags(Qt::FramelessWindowHint);
        banner.show();
        app->exec();
    }

    WelcomeUI dialog;
    dialog.dontKillParse0 = &dontKill;
    dialog.initialise(&totem);
    //dialog.setWindowFlags(Qt::WindowStaysOnTopHint);
    dialog.show();

    // Coming Soon...
    //ModernStarter dialog;
    //dialog.initialise(&totem, &dontKill);
    //dialog.show();

    QObject::connect(&a, &QApplication::focusChanged, onFocusChanged);

    return app->exec();
}


void loadFonts() {
    QDir d(":/base/fonts");
    for (const auto &f : d.entryList({"*.ttf", "*.otf"}, QDir::Files))
        if (QFontDatabase::addApplicationFont(":/base/fonts/" + f) == 0) qWarning() << "Error: Failed to load font: " << f;
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
