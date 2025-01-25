#include "include/congrats.h"
#include "ui/ui_congrats.h"


Congrats::Congrats(QWidget *parent) : QDialog(parent), ui(new Ui::Congrats) {
    ui->setupUi(this);
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->feedBackButton, &QPushButton::clicked, this, &Congrats::launchFeedBack);
    connect(ui->viewHistory, SIGNAL(clicked()), this, SLOT(viewhistory()));

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();
}


Congrats::~Congrats() {
    delete ui;
}


bool Congrats::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void Congrats::initialise(QJsonObject statInfo, QString stat) {

    this->data = statInfo;

    ui->lnameLabel->setText(statInfo["level"].toString());
    ui->clicksLabel->setText(statInfo["clicks"].toString());
    ui->chkCleared->setText(statInfo["chk"].toString());
    double tmp = statInfo["timeTaken"].toString().toDouble()/(60.0);
    ui->timeTaken->setText(QString::number(tmp)+" minutes ("+statInfo["timeTaken"].toString()+" seconds) ");

    if (stat == "Timeout!") {
        ui->mainLabel->setText(QString("Mission Failed"));
        ui->message->setText(QString("Oops. Seems you couldn't complete the challenge in time. Try again!"));
    }
    else if (stat == "Aborted") {
        ui->mainLabel->setText(QString("Mission Aborted"));
        ui->message->setText(QString("Game ended abruptly."));
    }
    else if (stat == "MaxClicks!") {
        ui->mainLabel->setText(QString("Mission Failed"));
        ui->message->setText(QString("Oops. Seems you couldn't complete the challenge within minimum clicks. Try again!"));
    }

}


void Congrats::viewhistory() {

    QString tmp = QString::fromUtf8(QByteArray::fromBase64(data.value("log").toString().toUtf8()));
    QStringList logs = tmp.split("\n");

    QList<QString> header("Logs");
    QList<QStringList> listData;
    QList<QStringList> actionData;
    QStringList headerButtons = {"neutralOnline"};
    for (int i = 0; i < logs.count(); i++) {
        listData.append(QStringList(logs.value(i)));
        actionData.append(QStringList("history"));
    }

    baselist = new BaseList;
    baselist->dontKillMe = nullptr;
    baselist->initList("Logs", "", &header, &headerButtons, &listData, &actionData);
    baselist->show();

}


void Congrats::launchFeedBack() {
    QDesktopServices::openUrl(QUrl("https://forms.gle/SScZKbFLFBffdVay8"));
}

