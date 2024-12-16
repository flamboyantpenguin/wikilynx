#include "include/statusoverview.h"
#include "ui/ui_statusoverview.h"


statusOverview::statusOverview(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::statusOverview)
{
    ui->setupUi(this);
}


statusOverview::~statusOverview()
{
    delete ui;
}


void statusOverview::initialise(int status) {

    ui->updateButton->hide();
    ui->wikiStatus->setText(this->code[status].split("|")[0]);
    ui->archiveStatus->setText(this->code[status].split("|")[1]);
    ui->wiki->setIcon(QIcon::fromTheme(this->code[status].split("|")[0].toLower()));
    ui->archive->setIcon(QIcon::fromTheme(this->code[status].split("|")[1].toLower()));
    connect(ui->version, &QPushButton::clicked, this, &statusOverview::developerHehe);
    connect(ui->infoButton, &QPushButton::clicked, this, &statusOverview::launchVersionInfo);

    if (status == 2) {
        ui->updateButton->show();
        #ifdef Q_OS_WIN
            connect(ui->updateButton, &QPushButton::clicked, this, &statusOverview::launchMaintenanceTool);
        #else
            connect(ui->updateButton, &QPushButton::clicked, this, &statusOverview::launchLatestVersionInfo);
        #endif
    }

}


void statusOverview::launchMaintenanceTool() {
    QDesktopServices::openUrl(QUrl::fromLocalFile("./maintenancetool.exe"));
}


void statusOverview::launchLatestVersionInfo() {
    QDesktopServices::openUrl(QUrl("https://github.com/flamboyantpenguin/wikilynx/releases/latest"));
}


void statusOverview::launchVersionInfo() {
    QDesktopServices::openUrl(QUrl("https://github.com/flamboyantpenguin/wikilynx/releases/v"+cVersion));
}


void statusOverview::developerHehe() {
    this->hehe++;

    if (!(this->hehe % 3))
        QMessageBox::information(this, "wikiLYNX", "Congratulations! You're now a developer! Get started by checking the project on GitHub :)", QMessageBox::Ok);
}
