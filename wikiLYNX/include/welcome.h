#include <sys/stat.h>
#include <fstream>
#include <filesystem>

#include <QDir>
#include <QString>
#include <QThread>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonDocument>
#include <QStandardPaths>

// UI Dialogs
#include "news.h"
#include "about.h"
#include "baselist.h"
#include "congrats.h"
#include "whatsnew.h"
#include "gamewindow.h"
#include "leaderboard.h"
#include "leveleditor.h"
#include "basebrowser.h"
#include "levelmanager.h"
#include "statusoverview.h"

// GameData Manager
#include "scoresheet.h"

// Update Checker
#include "renovatio.h"

namespace Ui {
class welcomeDialog;
}


class WelcomeUI : public QDialog {
    Q_OBJECT

public:
    QThread *thread;
    explicit WelcomeUI(QDialog *parent = nullptr);
    ~WelcomeUI();

    int *dontKillParse0;
    int *totemofUndying;
    QString theme;

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
    void updateLogs(QString, QJsonObject);
    void clearLogs();
    int startGame();
    void updateUI();
    void updateSettings();
    void addCustom();
    void genRandomLevel();
    void launchLevelSelector();
    void showRules();
    void checkWorldEvent();
    void launchStatusOverview();
    void toggleDevOptions();


public slots:
    int initialise(int*);
    void setLevel(QString lname);
    void setStatus(int);


private:
    std::map<int, QString> code = {
        { 0, "Offline|offline" },
        { 1, "Online|online" },
        { 2, "Update Available|upgrade" },
        { 3, "Update Check Failed|neutralOnline" },
        { 4, "Meow|meow" },
    };

    News *newsDialog;
    GameWindow *game;
    BaseList *baselist;
    About *aboutDialog;
    BaseBrowser *helpDialog;
    WhatsNew *whatsNewDialog;

    LeaderBoard *statsDialog;
    LevelManager *editDialog;
    LevelEditor *levelEditorDlg;
    StatusOverview *overview = nullptr;

    ScoreSheet *gameData;

    Ui::welcomeDialog *ui;
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

};
