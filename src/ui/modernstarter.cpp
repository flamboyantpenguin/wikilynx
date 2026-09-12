#include "include/modernstarter.h"
#include "forms/ui_modernstarter.h"

ModernStarter::ModernStarter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ModernStarter)
{
    ui->setupUi(this);
    //this->setAttribute();
    ui->playButton->setAttribute(Qt::WA_Hover, true);

}

ModernStarter::~ModernStarter() {
    delete ui;
}


int ModernStarter::initialise(int *totem, int* dontKillMe) {
    this->totemofUndying = totem;
    this->dontKillParse0 = dontKillMe;

    QString theme;
    this->gameData = new ScoreSheet();
    theme = gameData->getSetting("iconTheme").toString() + theme;
    QIcon::setThemeName(theme);
    qDebug() << "Current Icon Theme:" << QIcon::themeName();
    this->update();

    return 0;
}
