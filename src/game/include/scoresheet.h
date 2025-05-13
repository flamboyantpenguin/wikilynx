#ifndef SCORESHEET_H
#define SCORESHEET_H

#include <QList>
#include <QFile>
#include <QString>
#include <QJsonObject>
#include <QDataStream>
#include <QJsonDocument>
#include <QStandardPaths>


class ScoreSheet {


public:
    ScoreSheet();
    ~ScoreSheet();

    // Base
    QString iconThemes;
    QString ver = "1.6.0";
    QString version = "1.6.0-1";

    const int theGrandPlayers = 12;

    // Game Log
    QJsonObject getGameLog(QString instance = "");
    void updateGameLog(QString, QJsonObject);
    void clearLogs();

    // Player Stats
    QStringList getStatLevels();
    QJsonObject getLeaderBoard(QString);
    QString getPlayerStats(QString, QString);
    void appendPlayerStats(QString, QString, QString);

    // Game Level
    void addLevels(QJsonObject);
    void removeLevel(QString);
    QJsonValue getLevel(QString, QString);
    QJsonObject getLevel(QString);
    QJsonObject getLevels(QString flag = "");
    QString getLevelPresence(QString levelName);
    void updateLevel(QString, QJsonObject);
    QJsonObject mergeJson(QJsonObject, QJsonObject);

    // Game Settings
    QJsonValue getSetting(QString);
    void updateSettings(QString, int);
    void updateSettings(QString, bool);
    void updateSettings(QString, QString);

    // Data Managers
    static QJsonObject readTxtFile(QString);
    static void writeTxtFile(QString, QJsonObject);
    static QJsonObject readBinFile(QString, bool b64 = false);
    static void writeBinFile(QString, QJsonObject, bool b64 = false);

private:
    // File Names
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation); // Local Game Data

    QString stat = dirName+"/.stats"; // Leaderboard
    QString iFName = ":/cfg/gameData.json"; // Inbuilt Game Data
    QString lFName = dirName+"/gData.dat";
    QString sLogFile; // Application Log
    QString gLogFile = dirName+"/.log"; // Game Log

    // GameData Top-Level Objects
    QJsonObject levels;
    QJsonObject iLevels;
    QJsonObject settings;

    // Base
    QJsonObject base;

    // Data Management
    void reset();
    void loadData();
    void loadLevels(QJsonObject);
    void loadSettings(QJsonObject, QJsonObject);
    void saveData(QString *fname = nullptr, QJsonObject *cfg = nullptr, QJsonObject *gameData = nullptr);

};

#endif // SCORESHEET_H
