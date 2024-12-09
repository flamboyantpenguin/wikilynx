#include "include/congrats.h"
#include "ui/ui_congrats.h"


congrats::congrats(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::congrats)
{
    ui->setupUi(this);
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->feedBackButton, &QPushButton::clicked, this, &congrats::launchFeedBack);
    connect(ui->viewHistory, SIGNAL(clicked()), this, SLOT(viewhistory()));
    connect(ui->viewStatsButton, &QPushButton::clicked, this, &congrats::showStats);
}

congrats::~congrats()
{
    delete ui;
}


void congrats::initialise(QString tTaken, QString sTime, QString eTime, QString instance, QString stat, QString gamerName, QString lName, int checkPoints) {

    this->chk = checkPoints;
    this->instanceName = instance;
    this->startTime = sTime;
    this->endTime = eTime;
    this->timeTaken = QString::number(tTaken.toDouble(), 'f', 4);
    this->gameStatus = stat;
    this->playerName = gamerName;
    this->level = lName;
    this->genReport();
    this->updateStats();

    QString timeTakenM = QString::number(timeTaken.toFloat()/(60.0));

    ui->timeTaken->setText(timeTakenM+" minutes ("+tTaken+" seconds) ");
    ui->startTime->setText(startTime);
    ui->endTime->setText(endTime);
    ui->chkCleared->setText(QString::number(chk));
    ui->lnameLabel->setText(this->level);

    if (this->gameStatus == "Failed") {
        ui->mainLabel->setText(QString("Mission Failed"));
        ui->message->setText(QString("Oops. Seems you couldn't complete the challenge in time. Try again!"));
    }
    else if (this->gameStatus == "Aborted") {
        ui->mainLabel->setText(QString("Mission Aborted"));
        ui->message->setText(QString("Game ended abruptly."));
    }

}


void congrats::viewhistory() {

    QFile lFile("./"+dirName+"/logs/"+this->instanceName+"/log.txt");
    lFile.open(QIODevice::ReadOnly);
    QString logs = QString(lFile.readAll());
    lFile.close();
    hView.dontKillMe = nullptr;
    hView.initialise(&logs);
    hView.show();
}


void congrats::launchFeedBack() {
    QDesktopServices::openUrl(QUrl("https://forms.gle/SScZKbFLFBffdVay8"));
}


void congrats::genReport() {

    std::ofstream out("./"+dirName.toStdString()+"/logs/"+this->instanceName.toStdString()+"/report.txt", std::ios_base::app);
    out << "Status: "+this->gameStatus.toStdString()+"\n";
    out << "Player Name: "+this->playerName.toStdString()+"\n";
    out << "Time Taken: "+this->timeTaken.toStdString()+"\n";
    out << "Start Time: "+this->startTime.toStdString()+"\n";
    out << "End Time: "+this->endTime.toStdString()+"\n";
    out.close();
}


void congrats::updateStats() {

    if (this->gameStatus != "Passed") return;

    QFile statFile("./"+dirName+"/.stat");
    statFile.open(QIODevice::ReadOnly);
    auto iData = QJsonDocument::fromJson(statFile.readAll()).object();
    this->data = iData[this->level].toObject();
    statFile.close();
    if (this->data.keys().count() < theGrandPlayers) {
        if (!this->data.contains(this->timeTaken)) this->data[this->timeTaken] = this->playerName;
    }

    const QStringList& refList = this->data.keys();
    QString maxValue = *std::max_element(refList.begin(), refList.end());
    QString minValue = *std::min_element(refList.begin(), refList.end());
    if (this->timeTaken > minValue) {
        this->data.remove(maxValue);
        this->data[this->timeTaken] = this->playerName;
    }

    QJsonDocument document;
    iData[this->level] = this->data;
    document.setObject(iData);

    QFile::remove("./"+dirName+"/.stat");
    QFile file("./"+dirName+"/.stat");
    file.open(QIODevice::ReadWrite);
    file.write(document.toJson());
    file.flush();
    file.close();

}

void congrats::showStats() {
    statsDialog.initialise();
    statsDialog.show();
}
