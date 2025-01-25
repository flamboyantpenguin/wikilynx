#include "include/whatsnew.h"
#include "ui/ui_whatsnew.h"

WhatsNew::WhatsNew(QWidget *parent) : QDialog(parent), ui(new Ui::WhatsNew) {
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &WhatsNew::close);
    connect(ui->infoButton, &QPushButton::clicked, this, &WhatsNew::launchInfo);
}

WhatsNew::~WhatsNew() {
    delete ui;
}

void WhatsNew::launchInfo() {
    QDesktopServices::openUrl(QUrl::fromUserInput("https://github.com/flamboyantpenguin/wikilynx/releases/v1.5.5"));
}
