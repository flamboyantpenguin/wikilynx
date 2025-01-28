#include "include/whatsnew.h"
#include "ui/ui_whatsnew.h"


WhatsNew::WhatsNew(QString ver, QWidget *parent) : QDialog(parent), ui(new Ui::WhatsNew) {
    ui->setupUi(this);

    this->ver = ver;
    QFile p(":/base/info/ReleaseNotes.txt");
    p.open(QIODevice::ReadOnly);
    auto releaseNotes = QString(p.readAll());
    p.close();
    ui->textBrowser->setText(releaseNotes);

    connect(ui->closeButton, &QPushButton::clicked, this, &WhatsNew::close);
    connect(ui->infoButton, &QPushButton::clicked, this, &WhatsNew::launchInfo);
}


WhatsNew::~WhatsNew() {
    delete ui;
}


void WhatsNew::launchInfo() {
    QDesktopServices::openUrl(QUrl::fromUserInput("https://github.com/flamboyantpenguin/wikilynx/releases/"+ver));
}
