#include <sys/stat.h>
#include <fstream>
#include <filesystem>

#include <QDir>
#include <QFile>
#include <QString>
#include <QThread>
#include <QResource>
#include <QJsonObject>
#include <QMessageBox>
#include <QTextStream>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QSystemTrayIcon>
#include <QDesktopServices>

#include "news.h"
#include "help.h"
#include "about.h"
#include "congrats.h"
#include "whatsnew.h"
#include "viewstats.h"
#include "gamewindow.h"
#include "leveleditor.h"
#include "levelmanager.h"
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

private:

    std::map<QString, QString> worldEvents = {
        { "2512", "christmas|Merry Christmas!" },
        { "0101", "newyear|Happy New Year!" }
    };

    QStringList tips = {
        "Meow?",
        "May the force be with you!",
        "Welcome to wikiLYNX!",
        "Did you know that this game was developed for a college event?",
        "Contribute to this project on GitHub!",
        "Did you find all the hidden easter eggs? ;)",
        "Blondie is a reference to a certain Western Cowboy film released in 1966",
        "This game is a bundle of messages for the world",
        "Make this world a better place to live",
        "Show off your custom levels in GitHub Discussions",
        "Try random generated levels for a quick challenge!",
        "Word of Advice: Don't code when you're tired or when your mind is in chaos!"
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
    void genRandomLevel();
    void showLevelInfo(int s = 0);
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
        { 2, "Update Available|upgrade" },
        { 3, "Update Check Failed|neutralOnline" },
        { 4, "Meow|meow" },
    };

    news newsDialog;
    help helpDialog;
    GameWindow *game;
    about aboutDialog;
    whatsNew whatsNewDialog;

    viewStats statsDialog;
    levelManager editDialog;
    levelEditor levelEditorDlg;
    statusOverview overview;

    Ui::welcomeDialog *ui;
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

};


class checkUpdateWorker : public QObject  {
    Q_OBJECT

public:
    std::string version = "1.5.5-3";
    std::string lVersion = "1.5.5-3";

public slots:
    void process();

signals:
    void finished();
    void status(int);
};
