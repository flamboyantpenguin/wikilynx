#include "include/gamewindow.h"
#include "ui/ui_gamewindow.h"


namespace fs = std::filesystem;


GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{

    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateCountdown()));
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(endGame()));
    connect(ui->field, &QWebEngineView::urlChanged, this, &GameWindow::initAction);
    connect(ui->showHistory, SIGNAL(clicked(bool)), this, SLOT(launchLogs()));
    connect(ui->viewChkButton, SIGNAL(clicked(bool)), this, SLOT(viewCheckPoints()));

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}


GameWindow::~GameWindow()
{
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
    if (this->gameData["wiki?"].toInt()) ui->field->load(QUrl::fromUserInput(wikiURL+this->levels[0]));
    else ui->field->load(QUrl::fromUserInput(this->levels[0]));

    timer->start(100);
    this->playSound("init.wav");

    this->instance = "instance"+QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    this->aTime = QTime::currentTime().toString("hh:mm:ss.zzz");
    fs::create_directories(dirName.toStdString()+"/logs/"+instance.toStdString());

    return 0;

}


void GameWindow::updateCountdown() {

    countup = countup + 0.1;

    QString counterText = "$c $t";
    counterText.replace("$c", QString::number(countup, 'f', 2));
    if ((this->gameData["time"].toDouble() != 0.00)) counterText.replace("$t", "/ " + QString::number(this->gameData["time"].toDouble(), 'f', 2));
    else counterText.replace("$t", "");

    ui->clock->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->counter->setText(counterText);

    if (gameData["time"].toDouble() > 0 && countup >= gameData["time"].toDouble()) {
        this->missionFailed("Timeout!");
    }

    if (gameData["clicks"].toInt() > 0 && clicks > gameData["clicks"].toDouble()) {
        this->missionFailed("Max Clicks Reached!");
    }

}


void GameWindow::initAction() {

    this->clicks++;

    QString clicksText = "$c $t";
    clicksText.replace("$c", QString::number(clicks));
    if (this->gameData["clicks"].toInt()) clicksText.replace("$t", "/ " + QString::number(this->gameData["clicks"].toInt()));
    else clicksText.replace("$t", "");
    ui->clicks->setText(clicksText);

    auto url = ui->field->page()->url();
    std::ofstream out(dirName.toStdString()+"/logs/"+instance.toStdString()+"/log.txt", std::ios_base::app);
    out << QDateTime::currentDateTime().toString("yyyy/MM/dd|hh:mm:ss.zzz").toStdString()+">>\t";
    out << url.toString().toStdString()+"\n";
    out.close();

    if (!alD && !(url.toString().split("://")[1].split("/")[0].endsWith("wikipedia.org"))) {
            this->playSound("error.wav");
            *dontKillMe = 1;
            QMessageBox::critical(this, "wikiLYNX", "Rule Violation! You're not allowed to visit sites outide wikipedia.org in this level! Change this in settings", QMessageBox::Ok);
            *dontKillMe = 0;
            ui->field->load(QUrl::fromUserInput(wikiURL+levels[chk]));
            return;
    }

    QString uUrl = url.toString();
    if (this->gameData["wiki?"].toInt()) uUrl = uUrl.split("wikipedia.org/wiki/")[1];

    if (uUrl == levels[chk+1]) {
        this->missionAccomplished();
    }
    if (chk+1 < levels.count()) ui->statusbar->showMessage("Next Checkpoint: "+levels[chk+1]);

}


int GameWindow::missionAccomplished() {

    chk++;
    int prg = (chk/ (float) (this->levels.count() - 1)) *100;
    ui->progressBar->setValue(prg);

    if (prg == 100) {

        this->playSound("yay.wav");

        auto cTime = QTime::currentTime().toString("hh:mm:ss.zzz");
        ui->statusbar->showMessage("You won!!!");
        *dontKillMe = 1;
        QString timeTaken = QString::number(countup);
        //ui->field->printToPdf("./gData/logs/"+instance+"/fPage.pdf");

        congratsView.initialise(timeTaken, this->aTime, cTime, this->instance, "Passed", this->gamer, this->level, this->chk, this->clicks);
        QMessageBox::information(this, "wikiLYNX", "You Won!!!", QMessageBox::Ok);
        congratsView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
        congratsView.show();
        ui->field->deleteLater();
        emit gameEnded();
        close();
    }

    return 0;

}


int GameWindow::missionFailed(QString message){

    this->playSound("error.wav");

    timer->stop();
    auto cTime = QTime::currentTime().toString("hh:mm:ss.zzz");
    *dontKillMe = 1;
    QString timeTaken = QString::number(countup);
    //ui->field->printToPdf("./gData/logs/"+instance+"/fPage.pdf");
    QMessageBox::critical(this, "wikiLYNX", message, QMessageBox::Ok);
    congratsView.initialise(timeTaken, this->aTime, cTime, this->instance, "Failed", this->gamer, this->level, this->chk, this->clicks);
    congratsView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    congratsView.show();
    ui->field->deleteLater();
    emit gameEnded();
    close();
    return 0;
}


void GameWindow::launchLogs() {

    QFile f(dirName+"/logs/"+instance+"/log.txt");
    f.open(QIODevice::ReadOnly);
    QStringList logs = QString(f.readAll()).split("\n");
    logs.pop_back();
    f.close();

    QList<QString> header("Logs");
    QList<QList<QString>> listData;
    QList<QList<QString>> actionData;
    for (int i = 0; i < logs.count(); i++) {
        listData.append(QList<QString>(logs.value(i)));
        actionData.append(QList<QString> ("history"));
    }

    baselist = new baseList;
    baselist->dontKillMe = (this->dontKillMe);
    baselist->initList("Logs", "", &header, &listData, &actionData);
    baselist->show();

}


void GameWindow::viewCheckPoints() {

    QList<QString> header("Checkpoints");
    QList<QList<QString>> listData;
    QList<QList<QString>> actionData;
    for (int i = 0; i < levels.count(); i++) {
        listData.append(QList<QString>(levels.value(i)));
        QList<QString> tmp = (i <= chk) ? QList<QString>("check_circle") : QList<QString>("cancel");
        actionData.append(tmp);
    }

    baselist = new baseList;
    baselist->dontKillMe = (this->dontKillMe);
    baselist->initList("Checkpoints", "", &header, &listData, &actionData);
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


void GameWindow::endGame() {

    this->playSound("error.wav");

    timer->stop();
    auto cTime = QTime::currentTime().toString("hh:mm:ss.zzz");
    *dontKillMe = 1;
    QString timeTaken = QString::number(countup);
    //ui->field->printToPdf("./gData/logs/"+instance+"/fPage.pdf");
    congratsView.initialise(timeTaken, this->aTime, cTime, this->instance, "Aborted", this->gamer, this->level, this->chk, this->clicks);
    congratsView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    congratsView.show();
    ui->field->deleteLater();
    emit gameEnded();
    close();

}
