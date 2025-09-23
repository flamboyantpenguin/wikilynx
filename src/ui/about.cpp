#include "include/about.h"
#include "forms/ui_about.h"


About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About) {
    ui->setupUi(this);
    ui->tabWidget->setTabVisible(2, false);
    connect(ui->closeButton, &QPushButton::clicked, this, &About::close);
    connect(ui->showLicense, &QPushButton::clicked, this, &About::showTerms);
    connect(ui->feedbackButton, &QPushButton::clicked, this, &About::launchFeedback);

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

    QFile p(":/base/info/CREDITS.txt");
    p.open(QIODevice::ReadOnly);
    auto releaseNotes = QString(p.readAll());
    p.close();
    ui->credits->setText(releaseNotes);

}


About::~About() {
    delete ui;
    //if (termsDialog != nullptr) termsDialog->deleteLater();
    //if (feedbackBrowser != nullptr) feedbackBrowser->deleteLater();
}


bool About::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void About::showTerms() {
    termsDialog = new Terms;
    termsDialog->showMaximized();
}


void About::launchFeedback() {
    feedbackBrowser = new BaseBrowser();
    feedbackBrowser->initialise("Feedback", "https://forms.gle/SScZKbFLFBffdVay8");
    feedbackBrowser->showMaximized();
    //QDesktopServices::openUrl(QUrl("https://forms.gle/SScZKbFLFBffdVay8"));
}


void About::initDevMode() {
    QString indicator = dirName+"/.disableSplash";

    ui->tabWidget->setTabVisible(2, true);
    connect(ui->viewDataDir, &QPushButton::clicked, this, &About::showAppData);
    connect(ui->turnOffDev, &QPushButton::clicked, this, &About::turnOffDevMode);
    connect(ui->selfDestruct, &QPushButton::clicked, this, &About::deleteAllData);
    connect(ui->coolLoadingScreen, &QCheckBox::checkStateChanged, this, &About::toggleLoadingScreen);

    if (QFile(indicator).exists()) {
        ui->coolLoadingScreen->setChecked(true);
    }
}


void About::showAppData() {
    QDesktopServices::openUrl(QUrl::fromUserInput(QDir(dirName).absolutePath()));
}


void About::deleteAllData() {
    QString msg = "Are you sure you want to delete all app data? This will remove logs, gameData, custom levels and LeaderBoards!";
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


void About::toggleLoadingScreen() {

    QString indicator = dirName+"/.disableSplash";

    if (ui->coolLoadingScreen->isChecked()) {
        QFile file(indicator);
        file.open(QIODevice::WriteOnly);
        file.close();
    }
    else {
        QFile(indicator).remove();
    }

}


void About::turnOffDevMode() {
    emit turnOffDev();
    QString msg = "Developer Options disabled!";
    QMessageBox::information(this, "wikiLYNX", msg, QMessageBox::Ok);
}
