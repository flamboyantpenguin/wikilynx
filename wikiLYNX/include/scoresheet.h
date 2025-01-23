#ifndef SCORESHEET_H
#define SCORESHEET_H

#include <QList>
#include <QFile>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class ScoreSheet
{
public:
    ScoreSheet(QString iFName, QString lFName);
    ~ScoreSheet();

    // Base
    QString iconThemes;
    QString ver = "1.5.6";
    QString version = "1.5.6-1";

    // Game Level
    void addLevels(QJsonObject data);
    void removeLevel(QString levelName);
    QJsonObject getLevel(QString levelName);
    QJsonObject getLevels(QString flag = "");
    void updateLevel(QString code, QJsonObject data);
    QJsonObject mergeJson(QJsonObject d1, QJsonObject d2);

    // Game Settings
    QJsonObject getSettings();
    void updateSettings(QString, int);
    void updateSettings(QString, bool);
    void updateSettings(QString, QString);

    // Save Data
    void saveData(QString *fname = nullptr, QJsonObject *cfg = nullptr, QJsonObject *gameData = nullptr);

private:
    // File Names
    QString iFName;
    QString lFName;

    // GameData Top-Level Objects
    QJsonObject levels;
    QJsonObject iLevels;
    QJsonObject settings;

    // Base
    QJsonObject base;

    // Data Management
    void reset();
    void loadData();
    void loadLevels(QJsonObject gData);
    void loadSettings(QJsonObject cfg, QJsonObject base);
};

#endif // SCORESHEET_H
