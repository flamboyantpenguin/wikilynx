#include "include/terms.h"
#include "forms/ui_terms.h"

Terms::Terms(QWidget *parent) : QDialog(parent), ui(new Ui::Terms) {

    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &Terms::close);
    this->initialise();

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();
}


Terms::~Terms() {
    delete ui;
}


bool Terms::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}

void Terms::initialise() {

    ui->privacy->setText(Util::justReadThisFile(":/base/info/PRIVACY.txt"));

    ui->licenseView->setText(Util::justReadThisFile(":/base/info/LICENSE.txt"));

    ui->copyingView->setText(Util::justReadThisFile(":/base/info/COPYING.txt"));

    ui->webEngineLicenseView->setText(Util::justReadThisFile(":/base/info/COPYING.QtWebEngine.txt"));

    ui->notoOFL->setText(Util::justReadThisFile(":/base/fonts/NotoSans_OFL.txt"));

    ui->courierPrimeOFL->setText(Util::justReadThisFile(":/base/fonts/CourierPrime_OFL.txt"));

    ui->comfortaaOFL->setText(Util::justReadThisFile(":/base/fonts/Comfortaa_OFL.txt"));

    ui->materialLicenseView->setText(Util::justReadThisFile(":/base/info/LICENSE-MaterialIcons.txt"));

}
