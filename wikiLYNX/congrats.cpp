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

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    if (theme == "Light") ui->appLogo->setIcon(QIcon(":/base/images/DAWN_Light.svg"));
    else ui->appLogo->setIcon(QIcon(":/base/images/DAWN_Dark.svg"));
    ui->appLogo->update();

}


congrats::~congrats()
{
    delete ui;
}


bool congrats::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void congrats::initialise(QString tTaken, QString sTime, QString eTime, QString instance, QString stat, QString gamerName, QString lName, int checkPoints, int clicks) {

    this->data["level"] = lName;
    this->data["endTime"] = eTime;
    this->data["startTime"] = sTime;
    this->data["gameStatus"] = stat;
    this->data["playerName"] = gamerName;
    this->data["instanceName"] = instance;
    this->data["clicks"] = QString::number(clicks);
    this->data["chk"] = QString::number(checkPoints);
    this->data["timeTaken"] = QString::number(tTaken.toDouble(), 'f', 4);

    this->genReport();
    this->updateStats();

    ui->endTime->setText(eTime);
    ui->startTime->setText(sTime);
    ui->lnameLabel->setText(lName);
    ui->clicksLabel->setText(QString::number(clicks));
    ui->chkCleared->setText(QString::number(checkPoints));
    ui->timeTaken->setText(QString::number(tTaken.toFloat()/(60.0))+" minutes ("+tTaken+" seconds) ");



    if (stat == "Failed") {
        ui->mainLabel->setText(QString("Mission Failed"));
        ui->message->setText(QString("Oops. Seems you couldn't complete the challenge in time. Try again!"));
    }
    else if (stat == "Aborted") {
        ui->mainLabel->setText(QString("Mission Aborted"));
        ui->message->setText(QString("Game ended abruptly."));
    }

}


void congrats::viewhistory() {

    QFile lFile("./"+dirName+"/logs/"+this->data["instanceName"].toString()+"/log.txt");
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

    std::ofstream out("./"+dirName.toStdString()+"/logs/"+this->data["instanceName"].toString().toStdString()+"/report.txt", std::ios_base::app);
    out << "Status: "+this->data["gameStatus"].toString().toStdString()+"\n";
    out << "Player Name: "+this->data["playerName"].toString().toStdString()+"\n";
    out << "Level Name: "+this->data["level"].toString().toStdString()+"\n";
    out << "Time Taken: "+this->data["timeTaken"].toString().toStdString()+"\n";
    out << "Start Time: "+this->data["startTime"].toString().toStdString()+"\n";
    out << "End Time: "+this->data["endTime"].toString().toStdString()+"\n";
    out << "Clicks: "+this->data["clicks"].toString().toStdString()+"\n";
    out.close();
}


void congrats::updateStats() {

    if (data["gameStatus"].toString() != "Passed") return;

    QFile statFile("./"+dirName+"/.stat");
    statFile.open(QIODevice::ReadOnly);
    auto iData = QJsonDocument::fromJson(statFile.readAll()).object();
    this->statData = iData[statData["level"].toString()].toObject();
    statFile.close();
    if (this->statData.keys().count() < theGrandPlayers) {
        if (!this->statData.contains(this->data["timeTaken"].toString())) this->statData[this->data["timeTaken"].toString()] = data["playerName"].toString();
    }

    const QStringList& refList = this->statData.keys();
    QString maxValue = *std::max_element(refList.begin(), refList.end());
    QString minValue = *std::min_element(refList.begin(), refList.end());
    if (data["timeTaken"].toString() > minValue) {
        this->statData.remove(maxValue);
        this->statData[data["timeTaken"].toString()] = data["playerName"].toString();
    }

    QJsonDocument document;
    iData[data["level"].toString()] = this->statData;
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
