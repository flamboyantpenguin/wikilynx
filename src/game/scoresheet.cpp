#include "include/scoresheet.h"

ScoreSheet::ScoreSheet() {
    std::filesystem::create_directories(dirName.toStdString());
    loadData();
}


ScoreSheet::~ScoreSheet() {
    this->reset();
}


QJsonObject ScoreSheet::readBinFile(QString fname, bool b64) {

    // Experimental base64 support. Assumes the text file stores JSON data as base64
    if (b64) {
        QFile file(fname);
        if (file.isOpen()) file.close();
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        QByteArray data;
        in >> data;
        file.close();
        return QJsonDocument::fromJson(QByteArray::fromBase64(data)).object();
    }
    // This one reads the JSON data from binary blob (Standard serialization)
    QFile file(fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    QJsonObject data;
    in >> data;
    file.close();
    return data;
}


QJsonObject ScoreSheet::readTxtFile(QString fname) {
    // Read JSON
    QFile file(fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadOnly);
    QJsonObject data = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    return data;
}


void ScoreSheet::writeBinFile(QString fname, QJsonObject obj, bool b64) {

    // Experimental base64 support. Encodes the data to base64 before writing to file
    if (b64) {
        QFile file(fname);
        if (file.isOpen()) file.close();
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        QJsonDocument document;
        document.setObject(obj);
        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        out << bytes.toBase64();
        file.close();
        return;
    }
    // Writes the JSON as binary
    QFile file(fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << obj;
    file.close();
}


void ScoreSheet::writeTxtFile(QString fname, QJsonObject obj) {

    // Write JSON as TXT
    QJsonDocument document;
    document.setObject(obj);
    QByteArray bytes = document.toJson(QJsonDocument::Indented);
    QFile file(fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::WriteOnly);
    QTextStream iStream(&file);
    iStream << bytes;
    file.close();
}


void ScoreSheet::loadData() {

    // Read GameData from QRC (embedded in the executable on build)
    QJsonObject iData = readTxtFile(iFName);

    // Load inbuilt levels
    this->iLevels = iData["data"].toObject();

    // Load initial settings value
    loadSettings(iData["info"].toObject(), iData["base"].toObject());
    // Write a local GameData file if it doesn't exist. Load them with intial values
    if (!QFile(lFName).exists()) saveData();

    // Read local GameData
    QJsonObject lData = readBinFile(lFName);

    // Load custom levels and user preferences
    loadLevels(lData["data"].toObject());
    loadSettings(lData["info"].toObject(), iData["base"].toObject());}


void ScoreSheet::reset() {
    this->levels = QJsonObject();
    this->settings = QJsonObject();
}


void ScoreSheet::loadSettings(QJsonObject cfg, QJsonObject base) {

    //ToDo: Handle invalid values

    //Load Settings
    this->base = base;
    this->settings = cfg;

    // Load Icon Theme Colours
    this->iconThemes = base["iconThemes"].toString();

    // Load Version
    this->version = base["version"].toString();
    static QRegularExpression shortVer("-.");
    this->ver = base["version"].toString().remove(shortVer);

}


void ScoreSheet::loadLevels(QJsonObject gData) {
    this->levels = gData;
}


// Merge two QJsonObjects
QJsonObject ScoreSheet::mergeJson(QJsonObject d1, QJsonObject d2) {
    QVariantMap map = d1.toVariantMap();
    map.insert(d2.toVariantMap());
    return QJsonObject::fromVariantMap(map);
}


// Append levels
void ScoreSheet::addLevels(QJsonObject data) {
    this->levels = mergeJson(this->levels, data);
    this->saveData();
}


// Replace current a level with new one
void ScoreSheet::updateLevel(QString code, QJsonObject data) {
    this->levels[code] = data;
    this->saveData();
}


// Delete level
void ScoreSheet::removeLevel(QString levelName) {
    this->levels.remove(levelName);
    this->saveData();
}


// Safe way to get a setting value by key
QJsonValue ScoreSheet::getSetting(QString key) {
    return this->settings.value(key);
}


// Safe way (not yet) to update a setting by key (Override: QString)
void ScoreSheet::updateSettings(QString key, QString value) {
    settings[key] = value;
    saveData();
}


// Safe way (not yet) to update a setting by key (Override: bool)
void ScoreSheet::updateSettings(QString key, bool value) {
    settings[key] = value;
    saveData();
}


// Safe way (not yet) to update a setting by key (Override: int)
void ScoreSheet::updateSettings(QString key, int value) {
    settings[key] = value;
    saveData();
}


// Save data to file
void ScoreSheet::saveData(QString *fname, QJsonObject *cfg, QJsonObject *gameData) {

    if (fname == nullptr) fname = &(this->lFName);
    if (cfg == nullptr) cfg = &(this->settings);
    if (gameData == nullptr) gameData = &(this->levels);

    QJsonObject obj;
    obj.insert("info", *cfg);
    obj.insert("data", *gameData);
    this->writeBinFile(*fname, obj);

}


// Safge way to get level data
QJsonObject ScoreSheet::getLevel(QString levelName) {
    if (this->iLevels.contains(levelName))
        return this->iLevels[levelName].toObject();
    return this->levels[levelName].toObject();
}


// Safe way to get key-values inside levels
QJsonValue ScoreSheet::getLevel(QString levelName, QString key) {
    if (this->iLevels.contains(levelName))
        return this->iLevels[levelName].toObject().value(key);
    return this->levels[levelName].toObject().value(key);
}


// Safe way to get a QJsonObject containing all level data
QJsonObject ScoreSheet::getLevels(QString flag) {
    if (flag == "inbuilt")
        return this->iLevels;
    else if (flag == "custom")
        return this->levels;
    else
        return mergeJson(iLevels, levels);
}


// Checks whether a level exists
QString ScoreSheet::getLevelPresence(QString levelName) {
    if (iLevels.contains(levelName)) return "inbuilt";
    else if (levels.contains(levelName)) return "custom";
    else return "";
}


// Write log to file
void ScoreSheet::updateGameLog(QString instance, QJsonObject log) {
    QJsonObject logs = readBinFile(gLogFile);
    logs.insert(instance, log);
    writeBinFile(gLogFile, logs);
}

\
// Read data from log based on instance name (datetime stamp)
QJsonObject ScoreSheet::getGameLog(QString instance) {
    QJsonObject logs = readBinFile(gLogFile);
    if (instance == "") return logs;
    return logs.value(instance).toObject();
}


// Delete log file (Logs are not stored in scoresheet instance memory)
void ScoreSheet::clearLogs() {
    QFile(gLogFile).remove();
}


// Update stats (ToDo: Improve this)
void ScoreSheet::appendPlayerStats(QString level, QString player, QString timeTaken) {

    QJsonObject leaderBoard = readBinFile(stat, true);
    QJsonObject levelBoard = leaderBoard[level].toObject();
    if (levelBoard.keys().count() < theGrandPlayers) {
        if (!(levelBoard.contains(timeTaken))) levelBoard.insert(timeTaken, player);
        leaderBoard[level] = levelBoard;
        writeBinFile(stat, leaderBoard, true);
        return;
    }

    const QStringList& refList = levelBoard.keys();
    QString maxValue = *std::max_element(refList.begin(), refList.end());
    QString minValue = *std::min_element(refList.begin(), refList.end());
    if (timeTaken > minValue) {
        levelBoard.remove(maxValue);
        levelBoard[timeTaken] = player;
    }

    leaderBoard[level] = levelBoard;
    writeBinFile(stat, leaderBoard, true);
}


// Get all levels registered in Stats
QStringList ScoreSheet::getStatLevels() {
    QJsonObject statData = readBinFile(stat, true);
    return statData.keys();
}


// Get the stats info of a player
QString ScoreSheet::getPlayerStats(QString level, QString player) {
    QJsonObject statData = readBinFile(stat, true);
    if (statData.contains(level)) {
        QStringList players = statData[level].toObject().keys();
        for (const QString &key : std::as_const(players))
            if (statData[level].toObject().value(key) == player) return key;
    }
    return "";
}


// Get Leaderboard of a level
QJsonObject ScoreSheet::getLeaderBoard(QString level) {
    return readBinFile(stat, true).value(level).toObject();
}
