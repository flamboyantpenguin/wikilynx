#include "include/scoresheet.h"

ScoreSheet::ScoreSheet(QString iFName, QString lFName) {
    this->iFName = iFName;
    this->lFName = lFName;
    loadData();
}


ScoreSheet::~ScoreSheet() {
    this->reset();
}


void ScoreSheet::loadData() {

    QFile iFile(iFName);
    if (iFile.isOpen()) iFile.close();
    iFile.open(QIODevice::ReadOnly);
    QJsonObject iData = QJsonDocument::fromJson(iFile.readAll()).object();
    iFile.close();

    this->iLevels = iData["data"].toObject();

    loadSettings(iData["info"].toObject(), iData["base"].toObject());
    if (!QFile(lFName).exists()) saveData();

    QFile lFile(lFName);
    if (lFile.isOpen()) lFile.close();
    lFile.open(QIODevice::ReadOnly);
    QJsonObject lData = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

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
    this->ver = base["version"].toString().remove(QRegularExpression("-."));

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

    QJsonDocument document;
    QJsonObject obj;
    obj.insert("info", *cfg);
    obj.insert("data", *gameData);
    document.setObject(obj);
    //QFile::remove(fname);
    QByteArray bytes = document.toJson(QJsonDocument::Indented);
    QFile file(*fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::WriteOnly);
    QTextStream iStream(&file);
    iStream << bytes;
    file.close();

}


QJsonObject ScoreSheet::getLevel(QString levelName) {
    if (this->iLevels.contains(levelName)) {
        return this->iLevels[levelName].toObject();
    }
    return this->levels[levelName].toObject();
}


QJsonObject ScoreSheet::getLevels(QString flag) {
    if (flag == "inbuilt") {
        return this->iLevels;
    }
    else if (flag == "custom") {
        return this->levels;
    }
    else {
        return mergeJson(iLevels, levels);
    }
}

