#include "include/whatsnew.h"
#include "forms/ui_whatsnew.h"


WhatsNew::WhatsNew(QString ver, QWidget *parent) : QDialog(parent), ui(new Ui::WhatsNew) {
    ui->setupUi(this);

    this->ver = ver;
    ui->textBrowser->setText(Util::justReadThisFile(":/base/info/ReleaseNotes.txt"));

    connect(ui->closeButton, &QPushButton::clicked, this, &WhatsNew::close);
    connect(ui->infoButton, &QPushButton::clicked, this, &WhatsNew::launchInfo);
}


WhatsNew::~WhatsNew() {
    delete ui;
}


void WhatsNew::launchInfo() {
    QDesktopServices::openUrl(QUrl::fromUserInput("https://github.com/flamboyantpenguin/wikilynx/releases/"+ver));
}
