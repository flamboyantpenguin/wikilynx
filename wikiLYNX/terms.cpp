#include "include/terms.h"
#include "ui/ui_terms.h"


terms::terms(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::terms)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &terms::close);
    this->initialise();

}


terms::~terms()
{
    delete ui;
}


void terms::initialise() {

    QFile f(":/base/info/LICENSE.txt");
    f.open(QIODevice::ReadOnly);
    auto license = QString(f.readAll());
    f.close();
    ui->licenseView->setText(license);

    QFile c(":/base/info/COPYING.txt");
    c.open(QIODevice::ReadOnly);
    auto copying = QString(c.readAll());
    c.close();
    ui->copyingView->setText(copying);

    QFile w(":/base/info/COPYING.QtWebEngine.txt");
    w.open(QIODevice::ReadOnly);
    auto copyingWeb = QString(w.readAll());
    w.close();
    ui->webEngineLicenseView->setText(copyingWeb);

    QFile n(":/base/fonts/NotoSans_OFL.txt");
    n.open(QIODevice::ReadOnly);
    auto notoOFL = QString(n.readAll());
    n.close();
    ui->notoOFL->setText(notoOFL);

    QFile cP(":/base/fonts/CourierPrime_OFL.txt");
    cP.open(QIODevice::ReadOnly);
    auto courierPrimeOFL = QString(cP.readAll());
    cP.close();
    ui->courierPrimeOFL->setText(courierPrimeOFL);

    QFile mP(":/base/info/LICENSE-MaterialIcons.txt");
    mP.open(QIODevice::ReadOnly);
    auto materialLicense = QString(mP.readAll());
    mP.close();
    ui->materialLicenseView->setText(materialLicense);

}
