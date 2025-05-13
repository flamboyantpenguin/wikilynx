#include "include/gameboi.h"

GameBoi::GameBoi(ScoreSheet *gameData, QString levelName, QString playerName) : QObject() {

    // Parse gameData and LevelName
    this->gameData = gameData;
    this->levelName = levelName;
    this->levels = gameData->getLevel(levelName, "levels").toString().split(" ");

    // Intialize Sound Subsystem
    soundSystem = new Oscillator();

    // Set Player Name. Watch The Good, The Bad and The Ugly (1966) to understand what Blondie means
    if (playerName.isEmpty()) this->gamer = "Blondie";
    else this->gamer = playerName;

    // Instance names are basically Date+Time
    this->instance = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");

}


void GameBoi::startGame() {

    // Start Time
    timer->start(100);

    // Send initial values of Clicks and Next Checkpoint URL
    this->emitClicks();

    soundSystem->playSound("init");

    // This connects the timer to the whatever should be done when timer ticks
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCountdown()));

}


void GameBoi::emitClicks() {
    int totalClicks = this->gameData->getLevel(levelName, "clicks").toInt();

    QString clicksText = "$c $t";
    clicksText.replace("$c", QString::number(clicks));
    if (totalClicks) clicksText.replace("$t", "/ " + QString::number(totalClicks));
    else clicksText.replace("$t", "");
    emit clicked(clicksText, this->levels[chk+1]);
}


void GameBoi::click(QString url) {

    // Append Log
    this->logs.append(QDateTime::currentDateTime().toString("yyyy/MM/dd|hh:mm:ss.z")+">>\t"+url);

    // Append Clicks
    this->clicks++;

    // Check whether the user is outside wikipedia.org for wiki-only levels
    // ToDo: Replace this insanely stupid method with regex
    if (this->gameData->getLevel(levelName, "wiki?").toBool()) url = url.split("wikipedia.org/wiki/")[1];
    else if (!(url.split("://")[1].split("/")[0].endsWith("wikipedia.org"))) {
        soundSystem->playSound("error");
        emit sendMessage("RLV");
        return;
    }

    // This denotes the player reaching a checkpoint
    if (url == levels[chk+1]) {
        chk++;
        int prg = (chk/ (float) (levels.count() - 1)) *100;
        emit chkSuccess(prg);
        if (prg == 100) endGame(0);
        return;
    }

    // Send Current Checkpoint status and Next URL
    this->emitClicks();

}


void GameBoi::updateCountdown() {

    countup = countup + 0.1; // Increment countdown

    // Get max allowed time. O means no time limit
    double time = this->gameData->getLevel(levelName, "time").toDouble();

    // Send counter text to the GUI. This is can perhaps be shifted to the GUI itself. Opinions are welcome
    QString counterText = "$c $t";
    counterText.replace("$c", QString::number(countup, 'f', 2));
    if ((time != 0.00)) counterText.replace("$t", "/ " + QString::number(time, 'f', 2));
    else counterText.replace("$t", "");
    emit counter(counterText);

    // Timeout!
    if (time > 0 && countup >= time) endGame(1);

    // Max Clicks Reached!
    int allowedClicks = this->gameData->getLevel(levelName, "clicks").toInt();
    if (allowedClicks > 0 && clicks > allowedClicks) endGame(2);

}

int GameBoi::getChk() {
    return chk;
}

QStringList GameBoi::getLevels() {
    return this->levels;
}


QString GameBoi::getCurrentChk() {
    QString url = this->levels[chk];
    if (this->gameData->getLevel(levelName, "wiki?").toBool()) return wikiURL+url;
    return url;
}


void GameBoi::endGame(int code) {

    timer->stop();

    // This QJsonObject will serve as an instance parameter for logs
    QJsonObject gameStat;
    gameStat["log"] = (QString) this->logs.join("\n").toUtf8().toBase64();
    gameStat["level"] = this->levelName;
    gameStat["gameStatus"] = this->endCodes[code];
    gameStat["playerName"] = this->gamer;
    gameStat["clicks"] = QString::number(clicks);
    gameStat["chk"] = QString::number(chk);
    gameStat["timeTaken"] = QString::number(countup, 'f', 4);

    if (code == 1) soundSystem->playSound("yay");
    else soundSystem->playSound("error");

    emit end(gameStat, code);

    // Append this data to GameLog. Each GameLog is an Index, Log map
    gameData->updateGameLog(instance, gameStat);
    // If the player won, send data to PlayerStats updation
    if (code == 0) gameData->appendPlayerStats(levelName, gamer, QString::number(countup, 'f', 4));

}
