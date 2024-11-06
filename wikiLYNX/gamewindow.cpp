#include "include/gamewindow.h"
#include "ui/ui_gamewindow.h"


namespace fs = std::filesystem;


GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{

    ui->setupUi(this);

    QFile plt(":/base/pallete/bannerPrg.plt");
    plt.open(QIODevice::ReadOnly);
    auto styleSheet = plt.readAll();
    plt.close();

    ui->clock->setAlignment(Qt::AlignCenter);
    ui->counter->setAlignment(Qt::AlignCenter);
    ui->progressBar->setStyleSheet(styleSheet);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateCountdown()));
    connect(ui->field, &QWebEngineView::urlChanged, this, &GameWindow::initAction);
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(endGame()));
    connect(ui->showHistory, SIGNAL(clicked(bool)), this, SLOT(launchLogs()));
    connect(ui->viewChkButton, SIGNAL(clicked(bool)), this, SLOT(viewCheckPoints()));

}

GameWindow::~GameWindow()
{
    delete ui;
}


int GameWindow::initialise(QJsonObject *cfgData, int *dontKillMeParse, QString allowedDomain, int aRD, QString playerName, QString levelName) {

    instance = "instance"+QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    fs::create_directories(dirName.toStdString()+"/logs/"+instance.toStdString());
    aTime = QTime::currentTime().toString("hh:mm:ss.zzz");

    if (playerName.isEmpty()) this->gamer = "Blondie";
    else this->gamer = playerName;

    this->chk = 0;
    this->cfg = (*cfgData)["data"].toObject();
    this->tChk = (*cfgData)["checkpoints"].toInt();
    this->endTime = (*cfgData)["time"].toDouble();
    this->alD = aRD;
    this->cCount = this->endTime;
    this->domain = allowedDomain;
    this->dontKillMe = dontKillMeParse;
    this->level = levelName;
    countdown = this->endTime;

    QUrl url(this->cfg[QString::number(chk)].toObject()["url"].toString());
    ui->field->load(url);
    ui->progressBar->setValue(0);
    QApplication::processEvents();

    timer->start(100);
    return 0;
}


void GameWindow::updateCountdown()
{
    countdown = countdown - 0.1;
    ui->clock->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->counter->setText(QString::number(countdown, 'f', 2));

    if (countdown < 0.099) {
        this->missionFailed();
    }

}


void GameWindow::initAction() {

    auto url = ui->field->page()->url();
    std::ofstream out(dirName.toStdString()+"/logs/"+instance.toStdString()+"/log.txt", std::ios_base::app);
    out << QDateTime::currentDateTime().toString("yyyy/MM/dd|hh:mm:ss.zzz").toStdString()+">>\t";
    out << url.toString().toStdString()+"\n";
    out.close();

    if (!alD) {
        std::string u = url.toString().toStdString();
        if (u.substr(0, 24) != domain.toStdString()) {
            *dontKillMe = 1;
            QMessageBox::critical(this, "wikiLYNX", "Rule Violation! You're not allowed to visit sites other domain in this game", QMessageBox::Ok);
            *dontKillMe = 0;
            QUrl url(this->cfg[QString::number(chk)].toObject()["url"].toString());
            ui->field->load(url);
        }
    }


    auto wUrl = this->cfg[QString::number((this->chk)+1)].toObject()["url"].toString();
    if (url.toString() == wUrl) {
        this->missionAccomplished();
        ui->statusbar->showMessage("Next Checkpoint: "+this->cfg[QString::number((this->chk)+1)].toObject()["url"].toString());
    }
    else {
        ui->statusbar->showMessage("Next Checkpoint: "+wUrl);
    }
}


int GameWindow::missionAccomplished() {

    chk++;
    int prg = (chk/ (float) (tChk-1)) *100;
    ui->progressBar->setValue(prg);
    this->cCount = this->countdown;

    if (prg == 100) {
        timer->stop();
        auto cTime = QTime::currentTime().toString("hh:mm:ss.zzz");
        ui->statusbar->showMessage("You won!!!");
        *dontKillMe = 1;
        QString timeTaken = QString::number(endTime-cCount);
        //ui->field->printToPdf("./gData/logs/"+instance+"/fPage.pdf");
        congratsView.initialise(timeTaken, this->aTime, cTime, this->instance, "Passed", this->gamer, this->level, this->chk);
        //congratsView.initialise(c, QString::fromStdString(this->aTime), QString::fromStdString(cTime), instance, &(this->chk), 1);
        QMessageBox::information(this, "wikiLYNX", "You Won!!!", QMessageBox::Ok);
        congratsView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
        congratsView.show();
        close();
    }

    QApplication::processEvents();
    return 0;

}


int GameWindow::missionFailed(){

    timer->stop();
    auto cTime = QTime::currentTime().toString("hh:mm:ss.zzz");
    *dontKillMe = 1;
    QString timeTaken = QString::number(endTime-cCount);
    //ui->field->printToPdf("./gData/logs/"+instance+"/fPage.pdf");
    QMessageBox::critical(this, "wikiLYNX", "Timeout!", QMessageBox::Ok);
    congratsView.initialise(timeTaken, this->aTime, cTime, this->instance, "Failed", this->gamer, this->level, this->chk);
    congratsView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    congratsView.show();
    close();
    return 0;
}


void GameWindow::launchLogs() {

    QFile f("./"+dirName+"/logs/"+instance+"/log.txt");
    f.open(QIODevice::ReadOnly);
    auto logs = QString(f.readAll());
    f.close();
    historyView.dontKillMe = (this->dontKillMe);
    historyView.initialise(&logs);
    historyView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    historyView.show();
}


void GameWindow::viewCheckPoints() {

    checkpointView.dontKillMe = (this->dontKillMe);
    checkpointView.initialise(&cfg, &this->tChk, &this->chk);
    checkpointView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    checkpointView.show();
}


void GameWindow::endGame() {

    timer->stop();
    auto cTime = QTime::currentTime().toString("hh:mm:ss.zzz");
    *dontKillMe = 1;
    QString timeTaken = QString::number(endTime-cCount);
    //ui->field->printToPdf("./gData/logs/"+instance+"/fPage.pdf");
    congratsView.initialise(timeTaken, this->aTime, cTime, this->instance, "Aborted", this->gamer, this->level, this->chk);
    congratsView.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    congratsView.show();
    close();

}
