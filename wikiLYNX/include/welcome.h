#include <sys/stat.h>
#include <fstream>
#include <filesystem>
#include <QJsonDocument>
#include <QFile>
#include <QThread>
#include <QJsonObject>
#include <QResource>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QDesktopServices>
#include <QDir>
#include <QToolTip>

#include "news.h"
#include "help.h"
#include "about.h"
#include "congrats.h"
#include "editlevel.h"
#include "viewstats.h"
#include "gamewindow.h"
#include "statusoverview.h"


namespace Ui {
class welcomeDialog;
}


class welcomeUI : public QDialog
{
    Q_OBJECT


public:

    QThread *thread;
    explicit welcomeUI(QDialog *parent = nullptr);

    int *dontKillParse0;
    int *totemofUndying;
    QString theme;
    QJsonObject data, cfg, base;

    std::map<QString, QString> worldEvents = {
        { "2512", "christmas|Merry Christmas!" },
        { "0101", "newyear|Happy New Year!" }
    };


private slots:
    void checkStatus();
    bool isDarkTheme();
    void reset();
    void showStats();
    void showAbout();
    void showLogs();
    void showNews();
    void clearLogs();
    int startGame();
    void updateUI();
    void loadSettings();
    void updateSettings();
    void saveSettings();
    void checkCustom();
    void addCustom();
    void showLevelInfo();
    void showRules();
    void checkWorldEvent();
    void launchStatusOverview();


public slots:
    int initialise(int*);
    void setStatus(int);


private:

    std::map<int, QString> code = {
        { 0, "Offline|offline" },
        { 1, "Online|online" },
        { 2, "Update Available|update" },
        { 3, "Update Check Failed|neutralOnline" },
        { 4, "Meow|meow" },
    };

    news newsDialog;
    help helpDialog;
    GameWindow *game;
    about aboutDialog;
    editLevel editDialog;
    viewStats statsDialog;
    statusOverview overview;

    Ui::welcomeDialog *ui;
    QString dirName = ".wikilynx";

};


class checkUpdateWorker : public QObject  {
    Q_OBJECT

public:
    std::string lVersion = "1.5.0-1";
    std::string version = "1.5.0-1";

public slots:
    void process();

signals:
    void finished();
    void status(int);
};
