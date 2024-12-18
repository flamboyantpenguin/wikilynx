#include "include/help.h"
#include "ui/ui_help.h"

help::help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &help::close);
}

help::~help()
{
    delete ui;
}


void help::initialise(QString url) {
    ui->webEngineView->load(QUrl::fromUserInput(url));
}
