#include "include/about.h"
#include "ui/ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    ui->tabWidget->setTabVisible(2, false);
    connect(ui->closeButton, &QPushButton::clicked, this, &about::close);
    connect(ui->showLicense, &QPushButton::clicked, this, &about::showTerms);
    connect(ui->feedbackButton, &QPushButton::clicked, this, &about::launchFeedback);

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}

about::~about()
{
    delete ui;
}


bool about::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void about::showTerms() {
    termsDialog = new terms;
    termsDialog->show();
}


void about::launchFeedback() {
    QDesktopServices::openUrl(QUrl("https://forms.gle/SScZKbFLFBffdVay8"));
}


void about::initDevMode() {
    QString indicator = dirName+"/.showLoadingScreen";

    ui->tabWidget->setTabVisible(2, true);
    connect(ui->viewDataDir, &QPushButton::clicked, this, &about::showAppData);
    connect(ui->turnOffDev, &QPushButton::clicked, this, &about::turnOffDevMode);
    connect(ui->selfDestruct, &QPushButton::clicked, this, &about::deleteAllData);
    connect(ui->coolLoadingScreen, &QCheckBox::checkStateChanged, this, &about::toggleLoadingScreen);

    if (QFile(indicator).exists()) {
        ui->coolLoadingScreen->setChecked(true);
    }
}


void about::showAppData() {
    QDesktopServices::openUrl(QUrl::fromUserInput(QDir(dirName).absolutePath()));
}


void about::deleteAllData() {
    QString msg = "Are you sure you want to delete all app data? This will remove logs, gameData, custom levels and leaderboards!";
    if ((QMessageBox::warning(this, "wikiLYNX", msg, QMessageBox::Ok | QMessageBox::Cancel)) == QMessageBox::Ok) {
        QDir(dirName).removeRecursively();
    }
    else {
        return;
    }
    msg = "App Data Deleted! Closing wikiLYNX";
    QMessageBox::information(this, "wikiLYNX", msg, QMessageBox::Ok);
    QApplication::exit();
}


void about::toggleLoadingScreen() {

    QString indicator = dirName+"/.showLoadingScreen";

    if (ui->coolLoadingScreen->isChecked()) {
        QFile file(indicator);
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else {
        QFile(indicator).remove();
    }

}


void about::turnOffDevMode() {
    emit turnOffDev();
    QString msg = "Developer Options disabled!";
    QMessageBox::information(this, "wikiLYNX", msg, QMessageBox::Ok);
}
