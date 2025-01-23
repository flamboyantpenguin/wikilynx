#include "include/welcome.h"
#include "ui/ui_welcome.h"

#include "include/loadingscreen.h"

namespace fs = std::filesystem;


welcomeUI::welcomeUI(QDialog *parent) : QDialog(parent), ui(new Ui::welcomeDialog) {

    ui->setupUi(this);
    ui->genRandTxt->hide();
    ui->genRandPrg->hide();
    connect(ui->newsButton, &QPushButton::clicked, this, &welcomeUI::showNews);
    connect(ui->initButton, &QPushButton::clicked, this, &welcomeUI::startGame);
    connect(ui->helpButton, &QPushButton::clicked, this, &welcomeUI::showRules);
    connect(ui->sLogsButton, &QPushButton::clicked, this, &welcomeUI::showLogs);
    connect(ui->aboutButton, &QPushButton::clicked, this, &welcomeUI::showAbout);
    connect(ui->cLogsButton, &QPushButton::clicked, this, &welcomeUI::clearLogs);
    connect(ui->statsButton, &QPushButton::clicked, this, &welcomeUI::showStats);
    connect(ui->refreshButton, &QPushButton::clicked, this, &welcomeUI::updateUI);
    connect(ui->editLevelButton, &QPushButton::clicked, this, &welcomeUI::addCustom);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &welcomeUI::checkStatus);
    connect(ui->genRandomLevel, &QPushButton::clicked, this, &welcomeUI::genRandomLevel);
    connect(ui->levelSelector, &QPushButton::clicked, this, &welcomeUI::launchLevelSelector);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &welcomeUI::launchStatusOverview);

    ui->initButton->setFocus();

    this->checkStatus();

}

welcomeUI::~welcomeUI() {
    delete ui;
}


int welcomeUI::initialise(int *totem) {

    fs::create_directories(dirName.toStdString());
    this->totemofUndying = totem;

    gameData = new ScoreSheet(":/cfg/gameData.json", dirName+"/gData.json");
    updateUI();

    // Set icon theme
    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->privateLogo->setIcon(QIcon(":/base/images/DAWN_" + theme + ".svg"));
    ui->appLogo->update();
    ui->privateLogo->update();

    // Set tip
    QRandomGenerator *generator = QRandomGenerator::global();
    int tip = generator->bounded(0, this->tips.count());
    ui->label0->setText(tips[tip]);

    // Check Debug mode
    if (gameData->getSettings().value("debug").toBool()) ui->version->setText(ui->version->text() + " (Debug Mode)");

    theme = gameData->getSettings()["iconTheme"].toString() + theme;
    QIcon::setThemeName(theme);
    qDebug() << "Current Icon Theme:" << QIcon::themeName();
    this->update();

    connect(ui->killToggle, &QCheckBox::checkStateChanged, this, &welcomeUI::updateSettings);
    connect(ui->allowSitesToggle, &QCheckBox::checkStateChanged, this, &welcomeUI::updateSettings);
    connect(ui->iconThemeSelect, SIGNAL(currentTextChanged(QString)), this, SLOT(updateSettings()));

    return 0;
}


void welcomeUI::checkStatus() {
    thread = new QThread();
    checkUpdateWorker* worker = new checkUpdateWorker();
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &checkUpdateWorker::process);
    connect(worker, &checkUpdateWorker::finished, thread, &QThread::quit);
    connect(worker, &checkUpdateWorker::finished, worker, &checkUpdateWorker::deleteLater);
    connect(worker, &checkUpdateWorker::status, this, &welcomeUI::setStatus);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}


bool welcomeUI::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void welcomeUI::launchLevelSelector() {

    editDialog = new levelManager;
    editDialog->initialise(&gameData, 1);
    connect(editDialog, &levelManager::listDoubleClicked, this, &welcomeUI::setLevel);
    connect(editDialog, &levelManager::listDoubleClicked, editDialog, &levelManager::close);
    editDialog->show();

}


void welcomeUI::setLevel(QString lName) {
    ui->levelSelector->setText(lName);
    QJsonObject level = gameData->getLevels()[lName].toObject();
    ui->difficulty->setText(level["difficulty"].toString());
    ui->chk->setText(QString::number(level["levels"].toString().split(" ").count()));
}


int welcomeUI::startGame() {

    QString hex = "#" +gameData->iconThemes.split(",")[ui->iconThemeSelect->currentIndex()].split("#")[!(this->isDarkTheme()) + 1];
    QString bHex = (isDarkTheme()) ? "#000000" : "#ffffff";

    // KDEPlatformTheme automatically adds accelerators to QPushButton
    // https://stackoverflow.com/questions/32688153/how-to-disable-automatic-mnemonics-in-a-qt-application-on-kde
    // https://bugs.kde.org/show_bug.cgi?id=337491
    QString passcode = ui->levelSelector->text().remove(QRegularExpression("[\\&]"));

    if (!gameData->getLevels().contains(passcode) || passcode == "Select Level") {
        QMessageBox::critical(this, "wikiLYNX", "Invalid Code!", QMessageBox::Ok);
        return 1;
    }

    game = new GameWindow;
    connect(&(game->congratsView), &congrats::closed, this, &welcomeUI::reset);
    if (!(ui->keyboardToggle->isChecked())) this->grabKeyboard();
    this->hide();

    QJsonObject gData = gameData->getLevel(passcode);

    game->initialise(&gData, dontKillParse0, hex+"|"+bHex, gameData->getSettings()["notwiki?"].toInt(), ui->playerName->text(), passcode);
    *dontKillParse0 = 0;
    QThread::msleep(500);
    game->showFullScreen();
    game->setFocus();
    return 0;

}


void welcomeUI::reset() {
    this->show();
    this->setFocus();
    this->releaseKeyboard();
    *dontKillParse0 = 1;
    this->checkStatus();
}


void welcomeUI::updateUI() {

    ui->chk->setText("checkpoints");
    ui->difficulty->setText("difficulty");
    ui->levelSelector->setText("Select Level");

    for (int i = 0; i < gameData->iconThemes.split(",").count(); ++i) {
        ui->iconThemeSelect->addItem(gameData->iconThemes.split(",")[i].split("#")[0]);
    }
    ui->iconThemeSelect->setCurrentText(gameData->getSettings()["iconTheme"].toString());

    ui->allowSitesToggle->setChecked(gameData->getSettings()["allowReference"].toBool());

    ui->killToggle->setChecked(gameData->getSettings()["totemOfUndying"].toBool());

}


void welcomeUI::updateSettings() {
    gameData->updateSettings("totemOfUndying", ui->killToggle->isChecked());
    //gameData->settings["totemOfUndying"] = ui->killToggle->isChecked();
    gameData->updateSettings("iconTheme", ui->iconThemeSelect->currentText());
    //gameData->settings["iconTheme"] = ui->iconThemeSelect->currentText();
    gameData->updateSettings("allowReference", ui->allowSitesToggle->isChecked());
    //gameData->settings["allowReference"] = ui->allowSitesToggle->isChecked();
    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    theme = gameData->getSettings()["iconTheme"].toString() + theme;
    QIcon::setThemeName(theme);
    this->update();
}


void welcomeUI::addCustom() {
    editDialog = new levelManager;
    editDialog->initialise(&gameData);
    editDialog->show();
}


void welcomeUI::genRandomLevel() {

    levelEditorDlg = new levelEditor;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->genRandTxt->show();
    ui->genRandPrg->show();
    disconnect(this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandTxt, &QProgressBar::hide);
    disconnect(this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandPrg, &QProgressBar::hide);
    disconnect(this->levelEditorDlg, &levelEditor::genRandomFinished, this, &welcomeUI::startGame);
    disconnect(this->levelEditorDlg, &levelEditor::genRandomFinished, this->levelEditorDlg, &levelEditor::close);
    levelEditorDlg->genRandomLevel(gameData, "random");
    levelEditorDlg->showMaximized();
    levelEditorDlg->hide();
    this->setFocus();

    ui->chk->setText("random");
    ui->difficulty->setText("random");
    ui->levelSelector->setText("random");

    connect(this->levelEditorDlg, &levelEditor::genRandomFinished, this, &welcomeUI::startGame);
    connect(this->levelEditorDlg, &levelEditor::genRandomFinished, &QApplication::restoreOverrideCursor);
    connect(this->levelEditorDlg, &levelEditor::genRandomFinished, this->levelEditorDlg, &levelEditor::close);
    connect(this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandTxt, &QProgressBar::hide);
    connect(this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandPrg, &QProgressBar::hide);

}


void welcomeUI::showRules() {
    helpDialog = new help;
    helpDialog->initialise();
    helpDialog->showMaximized();
}


void welcomeUI::showLogs() {
    fs::create_directories(dirName.toStdString()+"/logs");
    //QDesktopServices::openUrl(QUrl::fromLocalFile(dirName+"/logs"));
    // Finally Fixed this :)
    QDesktopServices::openUrl(QUrl::fromUserInput(QDir(dirName+"/logs").absolutePath()));
}


void welcomeUI::clearLogs() {
    QDir(dirName+"/logs").removeRecursively();
    QMessageBox::information(this, "wikiLYNX", "Logs cleared successfully!", QMessageBox::Ok);
}


void welcomeUI::setStatus(int c) {

    if (gameData->version > gameData->getSettings()["version"].toString()) {
        whatsNewDialog = new whatsNew;
        whatsNewDialog->show();
        gameData->updateSettings("version", gameData->version);
    }

    ui->status->setText(code[c].split("|")[0]);
    ui->statusIndicator->setIcon(QIcon::fromTheme(code[c].split("|")[1]));
    if (c != 0) checkWorldEvent();
    overview = new statusOverview;
    overview->initialise(c);
    connect(overview, &statusOverview::devEnabled, this, &welcomeUI::toggleDevOptions);
    return;
}


void welcomeUI::toggleDevOptions() {
    if (gameData->getSettings().value("debug").toBool()) {
        ui->version->setText(ui->version->text().remove(QRegularExpression(" \\(.*\\)")));
        gameData->updateSettings("debug", false);
    }
    else {
        ui->version->setText(ui->version->text() + " (Debug Mode)");
        gameData->updateSettings("debug", true);
    }
    gameData->saveData();
}


void welcomeUI::checkWorldEvent() {

    QString day = QDate::currentDate().toString("ddMM");

    for (auto const& [key, val] : this->worldEvents) {
        if (day == key) {
            ui->status->setText(val.split('|')[1]);
            ui->statusIndicator->setIcon(QIcon::fromTheme(val.split('|')[0]));
        }
    }
}


void welcomeUI::showStats() {
    statsDialog = new leaderboard;
    statsDialog->initialise();
    statsDialog->showMaximized();
}


void welcomeUI::launchStatusOverview() {
    if (overview != nullptr) overview->show();
}


void welcomeUI::showNews() {
    newsDialog = new news;
    newsDialog->initialise();
    newsDialog->show();
}

void welcomeUI::showAbout() {
    aboutDialog = new about;
    if (gameData->getSettings().value("debug").toBool()) {
        aboutDialog->initDevMode();
        connect(aboutDialog, &about::turnOffDev, this, &welcomeUI::toggleDevOptions);
    }
    aboutDialog->show();
}


void checkUpdateWorker::process() { // Process. Start processing data.

    QNetworkAccessManager manager;
    QUrl url("https://wikipedia.org");
    QNetworkReply *reply = manager.get(QNetworkRequest(url));

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit status(0);
        return;
    }
    reply->deleteLater();

    url = QUrl("https://repo.dawn.org.in/QtOnline/wikiLYNX/.info/version.txt");
    reply = manager.get(QNetworkRequest(url));

    QEventLoop loop2;
    QObject::connect(reply, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
    loop2.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        qDebug() << "Error fetching latest version information:" << reply->errorString();
        emit status(3);
        return;
    }
    else {
        QByteArray data = reply->read(7);
        lVersion = QString::fromLocal8Bit(data).toStdString();
    }
    reply->deleteLater();

    if (strcmp(lVersion.c_str(), version.c_str()) > 0) emit status(2);
    else if (strcmp(lVersion.c_str(), version.c_str()) < 0) emit status(4);
    else emit status(1);

    emit finished();

}
