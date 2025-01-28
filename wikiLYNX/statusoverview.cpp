#include "include/statusoverview.h"
#include "ui/ui_statusoverview.h"


StatusOverview::StatusOverview(QWidget *parent) : QDialog(parent), ui(new Ui::StatusOverview) {
    ui->setupUi(this);
}


StatusOverview::~StatusOverview() {
    delete ui;
}


void StatusOverview::initialise(int status, QString version) {

    this->cVersion = version;

    this->hehe = 0;
    ui->updateButton->hide();
    ui->wikiStatus->setText(this->code[status].split("|")[0]);
    ui->archiveStatus->setText(this->code[status].split("|")[1]);
    ui->wiki->setIcon(QIcon::fromTheme(this->code[status].split("|")[0].toLower()));
    ui->archive->setIcon(QIcon::fromTheme(this->code[status].split("|")[1].toLower()));
    connect(ui->version, &QPushButton::clicked, this, &StatusOverview::developerHehe);
    connect(ui->infoButton, &QPushButton::clicked, this, &StatusOverview::launchVersionInfo);

    if (status == 2) {
        ui->updateButton->show();
        ui->versionHelper->setText("New Version Available!");
        #ifdef Q_OS_WIN
            connect(ui->updateButton, &QPushButton::clicked, this, &StatusOverview::launchMaintenanceTool);
        #else
            connect(ui->updateButton, &QPushButton::clicked, this, &StatusOverview::launchLatestVersionInfo);
        #endif
    }
    else if (!status || status == 3) ui->versionHelper->setText("Update Check Failed!");

}


void StatusOverview::launchMaintenanceTool() {
    QDesktopServices::openUrl(QUrl::fromLocalFile("./maintenancetool.exe"));
}


void StatusOverview::launchLatestVersionInfo() {
    QDesktopServices::openUrl(QUrl("https://github.com/flamboyantpenguin/wikilynx/releases/latest"));
}


void StatusOverview::launchVersionInfo() {
    whatsnew = new WhatsNew(this->cVersion);
    whatsnew->show();
    //QDesktopServices::openUrl(QUrl("https://github.com/flamboyantpenguin/wikilynx/releases/v"+cVersion));
}


void StatusOverview::developerHehe() {

    this->hehe++;
    QString msg = "Developer Options enabled!";

    if ((this->hehe) == 10) {
        emit devEnabled();
        QMessageBox::information(this, "wikiLYNX", msg, QMessageBox::Ok);
    }

}
