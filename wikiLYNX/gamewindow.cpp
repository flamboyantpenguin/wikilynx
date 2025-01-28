#include "include/gamewindow.h"
#include "ui/ui_gamewindow.h"


namespace fs = std::filesystem;


GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow) {

    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateCountdown()));
    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(endGame()));
    connect(ui->field, &QWebEngineView::urlChanged, this, &GameWindow::initAction);
    connect(ui->showHistory, SIGNAL(clicked()), this, SLOT(launchLogs()));
    connect(ui->viewChkButton, SIGNAL(clicked()), this, SLOT(viewCheckPoints()));

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}


GameWindow::~GameWindow() {
    delete ui;
}


bool GameWindow::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


int GameWindow::initialise(QJsonObject *gData, int *dontKillMeParse, QString prgHex, int aRD, QString playerName, QString levelName) {

    QFile plt(":/base/pallete/bannerPrg.plt");
    plt.open(QIODevice::ReadOnly);
    QString styleSheet = plt.readAll();
    plt.close();

    styleSheet.replace("#39ff14", prgHex.split("|")[0]);
    styleSheet.replace("#181818", prgHex.split("|")[1]);

    ui->progressBar->setStyleSheet(styleSheet);

    if (playerName.isEmpty()) this->gamer = "Blondie";
    else this->gamer = playerName;

    this->alD = aRD;
    this->level = levelName;
    this->gameData = *gData;
    this->dontKillMe = dontKillMeParse;
    this->levels = (*gData)["levels"].toString().split(" ");

    ui->progressBar->setValue(0);
    if (this->gameData["wiki?"].toBool()) ui->field->load(QUrl::fromUserInput(wikiURL+this->levels[0]));
    else ui->field->load(QUrl::fromUserInput(this->levels[0]));

    timer->start(100);
    this->playSound("init.wav");

    this->instance = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");

    return 0;

}


void GameWindow::updateCountdown() {

    countup = countup + 0.1;

    QString counterText = "$c $t";
    counterText.replace("$c", QString::number(countup, 'f', 2));
    if ((this->gameData["time"].toDouble() != 0.00)) counterText.replace("$t", "/ " + QString::number(this->gameData["time"].toDouble(), 'f', 2));
    else counterText.replace("$t", "");

    ui->counter->setText(counterText);
    ui->clock->setText(QTime::currentTime().toString("hh:mm:ss"));

    if (gameData["time"].toDouble() > 0 && countup >= gameData["time"].toDouble()) endGame("Timeout!");

    if (gameData["clicks"].toInt() > 0 && clicks > gameData["clicks"].toDouble()) endGame("MaxClicks!");

}


void GameWindow::initAction() {


    // Append Clicks
    this->clicks++;
    auto url = ui->field->page()->url();
    QString clickTime = QDateTime::currentDateTime().toString("yyyy/MM/dd|hh:mm:ss.z");

    // Update UI
    QString clicksText = "$c $t";
    clicksText.replace("$c", QString::number(clicks));
    if (this->gameData["clicks"].toInt()) clicksText.replace("$t", "/ " + QString::number(this->gameData["clicks"].toInt()));
    else clicksText.replace("$t", "");
    ui->clicks->setText(clicksText);


    // Append Log
    this->log.append(clickTime+">>\t"+url.toString());


    // Check Rule Violation
    if (!alD && !(url.toString().split("://")[1].split("/")[0].endsWith("wikipedia.org"))) {
            this->playSound("error.wav");
            *dontKillMe = 1;
            QMessageBox::critical(this, "wikiLYNX", "Rule Violation! You're not allowed to visit sites outide wikipedia.org in this level! Change this in settings", QMessageBox::Ok);
            *dontKillMe = 0;
            ui->field->load(QUrl::fromUserInput(wikiURL+levels[chk]));
            return;
    }

    // Check Game Status
    QString uUrl = url.toString();
    if (this->gameData["wiki?"].toBool()) uUrl = uUrl.split("wikipedia.org/wiki/")[1];

    if (uUrl == levels[chk+1]) {
        chk++;
        int prg = (chk/ (float) (this->levels.count() - 1)) *100;
        ui->progressBar->setValue(prg);
        if (prg == 100) endGame("Win!");
    }
    if (chk+1 < levels.count()) ui->statusbar->showMessage("Next Checkpoint: "+levels[chk+1]);

}


int GameWindow::endGame(QString message) {

    timer->stop();

    QJsonObject gameStat;

    gameStat["log"] = (QString) log.join("\n").toUtf8().toBase64();
    gameStat["level"] = level;
    gameStat["gameStatus"] = message;
    gameStat["playerName"] = gamer;
    gameStat["clicks"] = QString::number(clicks);
    gameStat["chk"] = QString::number(chk);
    gameStat["timeTaken"] = QString::number(countup, 'f', 4);

    *dontKillMe = 1;
    if (message != "You Won!") this->playSound("error.wav");
    else this->playSound("yay.wav");

    ui->statusbar->showMessage(code[message]);
    if (message != "Aborted!") QMessageBox::critical(this, "wikiLYNX", code[message], QMessageBox::Ok);

    // Launch Congrats Dialog
    congratsView.initialise(gameStat, message);
    //congratsView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    congratsView.show();
    ui->field->close();
    //ui->field->deleteLater();
    emit gameEnded(instance, gameStat);
    close();
    return 0;

}


void GameWindow::launchLogs() {

    QList<QString> header("Logs");
    QList<QStringList> listData;
    QList<QStringList> actionData;
    QStringList headerButtons = {"neutralOnline"};
    for (int i = 0; i < log.count(); i++) {
        listData.append(QList<QString>(log.value(i)));
        actionData.append(QList<QString> ("history"));
    }

    baselist = new BaseList;
    baselist->dontKillMe = (this->dontKillMe);
    baselist->initList("Logs", "", &header, &headerButtons, &listData, &actionData);
    baselist->show();

}


void GameWindow::viewCheckPoints() {

    QList<QString> header("Checkpoints");
    QList<QStringList> listData;
    QList<QStringList> actionData;
    QStringList headerButtons = {"neutralOnline"};
    for (int i = 0; i < levels.count(); i++) {
        listData.append(QList<QString>(levels.value(i)));
        QList<QString> tmp = (i <= chk) ? QList<QString>("online") : QList<QString>("offline");
        actionData.append(tmp);
    }

    baselist = new BaseList;
    baselist->dontKillMe = (this->dontKillMe);
    baselist->initList("Checkpoints", "", &header, &headerButtons, &listData, &actionData);
    baselist->show();

}


void GameWindow::playSound(QString sound) {
    QMediaPlayer *player = new QMediaPlayer;
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QAudioOutput *audioOutput = new QAudioOutput;
        player->setAudioOutput(audioOutput);
        player->setSource(QUrl("qrc:/base/audio/"+sound));
        audioOutput->setVolume(50);
    #else
        player->setMedia(QUrl("qrc:/base/audio/"+sound));
        player->setVolume(50);
    #endif
    player->play();
}
