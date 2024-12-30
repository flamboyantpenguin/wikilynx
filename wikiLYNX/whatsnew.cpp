#include "include/whatsnew.h"
#include "ui/ui_whatsnew.h"

whatsNew::whatsNew(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::whatsNew)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &whatsNew::close);
    connect(ui->infoButton, &QPushButton::clicked, this, &whatsNew::launchInfo);
}

whatsNew::~whatsNew()
{
    delete ui;
}

void whatsNew::launchInfo() {
    QDesktopServices::openUrl(QUrl::fromUserInput("https://github.com/flamboyantpenguin/wikilynx/releases/v1.5.5"));
}
