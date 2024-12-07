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

#include "news.h"
#include "help.h"
#include "about.h"
#include "congrats.h"
#include "editlevel.h"
#include "viewstats.h"
#include "gamewindow.h"


namespace Ui {
class welcomeDialog;
}


class welcomeUI : public QDialog
{
    Q_OBJECT


public:
    explicit welcomeUI(QDialog *parent = nullptr);

    int initialise(int*);

    int *dontKillParse0;
    int aRD = 0;
    int *totemofUndying;
    QJsonObject data, cfg;
    //Json::Value cfg;


private slots:
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

public slots:
    void setStatus(int);


private:
    news newsDialog;
    help helpDialog;
    GameWindow *game;
    about aboutDialog;
    editLevel editDialog;
    viewStats statsDialog;
    Ui::welcomeDialog *ui;
    QString dirName = ".wLnKMeow";

};


class checkUpdateWorker : public QObject  {
    Q_OBJECT

public:
    std::string lVersion = "1.0.0-1";
    std::string version = "1.0.0-1";

public slots:
    void process();

signals:
    void finished();
    void status(int);
};
