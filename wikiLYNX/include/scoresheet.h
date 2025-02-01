#ifndef SCORESHEET_H
#define SCORESHEET_H

#include <QList>
#include <QFile>
#include <QString>
#include <QJsonObject>
#include <QDataStream>
#include <QJsonDocument>


class ScoreSheet {


public:
    ScoreSheet(QString, QString);
    ~ScoreSheet();

    // Base
    QString iconThemes;
    QString ver = "1.5.6";
    QString version = "1.5.6-3";

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
    QJsonObject GetLevel(QString);
    QJsonObject GetLevels(QString flag = "");
    void updateLevel(QString, QJsonObject);
    QJsonObject mergeJson(QJsonObject, QJsonObject);

    // Game Settings
    QJsonObject getSettings();
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
    QString stat; // Leaderboard
    QString iFName; // Inbuilt Game Data
    QString lFName; // Local Game Data
    QString sLogFile; // Application Log
    QString gLogFile; // Game Log

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
