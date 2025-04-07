#include "include/scoresheet.h"

ScoreSheet::ScoreSheet(QString iFName, QString lFName) {
    this->iFName = iFName;
    this->stat = lFName+"/.stats";
    this->gLogFile = lFName+"/.log";
    this->lFName = lFName+"/gData.dat";
    loadData();
}


ScoreSheet::~ScoreSheet() {
    this->reset();
}


QJsonObject ScoreSheet::readBinFile(QString fname, bool b64) {

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
    QFile file(fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadOnly);
    QJsonObject data = QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    return data;
}


void ScoreSheet::writeBinFile(QString fname, QJsonObject obj, bool b64) {
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
    QFile file(fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << obj;
    file.close();
}


void ScoreSheet::writeTxtFile(QString fname, QJsonObject obj) {
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

    QJsonObject iData = readTxtFile(iFName);

    this->iLevels = iData["data"].toObject();

    loadSettings(iData["info"].toObject(), iData["base"].toObject());
    if (!QFile(lFName).exists()) saveData();

    QJsonObject lData = readBinFile(lFName);

    loadLevels(lData["data"].toObject());
    loadSettings(lData["info"].toObject(), iData["base"].toObject());}


void ScoreSheet::reset() {
    this->levels = QJsonObject();
    this->settings = QJsonObject();
}


void ScoreSheet::loadSettings(QJsonObject cfg, QJsonObject base) {

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


QJsonObject ScoreSheet::mergeJson(QJsonObject d1, QJsonObject d2) {
    QVariantMap map = d1.toVariantMap();
    map.insert(d2.toVariantMap());
    return QJsonObject::fromVariantMap(map);
}


void ScoreSheet::addLevels(QJsonObject data) {
    this->levels = mergeJson(this->levels, data);
    this->saveData();
}


void ScoreSheet::updateLevel(QString code, QJsonObject data) {
    this->levels[code] = data;
    this->saveData();
}


void ScoreSheet::removeLevel(QString levelName) {
    this->levels.remove(levelName);
    this->saveData();
}


QJsonObject ScoreSheet::getSettings() {
    return this->settings;
}


void ScoreSheet::updateSettings(QString key, QString value) {
    settings[key] = value;
    saveData();
}


void ScoreSheet::updateSettings(QString key, bool value) {
    settings[key] = value;
    saveData();
}


void ScoreSheet::updateSettings(QString key, int value) {
    settings[key] = value;
    saveData();
}


void ScoreSheet::saveData(QString *fname, QJsonObject *cfg, QJsonObject *gameData) {

    if (fname == nullptr) fname = &(this->lFName);
    if (cfg == nullptr) cfg = &(this->settings);
    if (gameData == nullptr) gameData = &(this->levels);

    QJsonObject obj;
    obj.insert("info", *cfg);
    obj.insert("data", *gameData);
    this->writeBinFile(*fname, obj);

}


QJsonObject ScoreSheet::getLevel(QString levelName) {
    if (this->iLevels.contains(levelName))
        return this->iLevels[levelName].toObject();
    return this->levels[levelName].toObject();
}


QJsonObject ScoreSheet::getLevels(QString flag) {
    if (flag == "inbuilt")
        return this->iLevels;
    else if (flag == "custom")
        return this->levels;
    else
        return mergeJson(iLevels, levels);
}


QString ScoreSheet::getLevelPresence(QString levelName) {
    if (iLevels.contains(levelName)) return "inbuilt";
    else if (levels.contains(levelName)) return "custom";
    else return "";
}


void ScoreSheet::updateGameLog(QString instance, QJsonObject log) {
    QJsonObject logs = readBinFile(gLogFile);
    logs.insert(instance, log);
    writeBinFile(gLogFile, logs);
}


QJsonObject ScoreSheet::getGameLog(QString instance) {
    QJsonObject logs = readBinFile(gLogFile);
    if (instance == "") return logs;
    return logs.value(instance).toObject();
}


void ScoreSheet::clearLogs() {
    QFile(gLogFile).remove();
}


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


QStringList ScoreSheet::getStatLevels() {
    QJsonObject statData = readBinFile(stat, true);
    return statData.keys();
}


QString ScoreSheet::getPlayerStats(QString level, QString player) {
    QJsonObject statData = readBinFile(stat, true);
    if (statData.contains(level)) {
        QStringList players = statData[level].toObject().keys();
        for (const QString &key : std::as_const(players))
            if (statData[level].toObject().value(key) == player) return key;
    }
    return "";
}


QJsonObject ScoreSheet::getLeaderBoard(QString level) {
    return readBinFile(stat, true).value(level).toObject();
}
