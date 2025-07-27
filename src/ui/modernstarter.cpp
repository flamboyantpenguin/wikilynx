#include "include/modernstarter.h"
#include "forms/ui_modernstarter.h"

ModernStarter::ModernStarter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ModernStarter)
{
    ui->setupUi(this);
    //this->setAttribute();
    ui->playButton->setAttribute(Qt::WA_Hover, true);
    connect(ui->playButton, &QPushButt
}

ModernStarter::~ModernStarter()
{
    delete ui;
}
