#include "include/welcome.h"
#include "ui/ui_welcome.h"

#include "include/loadingscreen.h"

namespace fs = std::filesystem;


WelcomeUI::WelcomeUI(QDialog *parent) : QDialog(parent), ui(new Ui::welcomeDialog) {

    ui->setupUi(this);
    ui->genRandTxt->hide();
    ui->genRandPrg->hide();
    connect(ui->newsButton, &QPushButton::clicked, this, &WelcomeUI::showNews);
    connect(ui->initButton, &QPushButton::clicked, this, &WelcomeUI::startGame);
    connect(ui->helpButton, &QPushButton::clicked, this, &WelcomeUI::showRules);
    connect(ui->sLogsButton, &QPushButton::clicked, this, &WelcomeUI::showLogs);
    connect(ui->aboutButton, &QPushButton::clicked, this, &WelcomeUI::showAbout);
    connect(ui->cLogsButton, &QPushButton::clicked, this, &WelcomeUI::clearLogs);
    connect(ui->statsButton, &QPushButton::clicked, this, &WelcomeUI::showStats);
    connect(ui->refreshButton, &QPushButton::clicked, this, &WelcomeUI::updateUI);
    connect(ui->editLevelButton, &QPushButton::clicked, this, &WelcomeUI::addCustom);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &WelcomeUI::checkStatus);
    connect(ui->genRandomLevel, &QPushButton::clicked, this, &WelcomeUI::genRandomLevel);
    connect(ui->levelSelector, &QPushButton::clicked, this, &WelcomeUI::launchLevelSelector);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &WelcomeUI::launchStatusOverview);

    ui->initButton->setFocus();

}

WelcomeUI::~WelcomeUI() {
    delete ui;
}


int WelcomeUI::initialise(int *totem) {

    fs::create_directories(dirName.toStdString());
    this->totemofUndying = totem;

    gameData = new ScoreSheet(":/cfg/gameData.json", dirName);
    updateUI();

    // Check for Updates
    this->checkStatus();

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

    theme = gameData->getSettings().value("iconTheme").toString() + theme;
    QIcon::setThemeName(theme);
    qDebug() << "Current Icon Theme:" << QIcon::themeName();
    this->update();

    connect(ui->killToggle, &QCheckBox::checkStateChanged, this, &WelcomeUI::updateSettings);
    connect(ui->allowSitesToggle, &QCheckBox::checkStateChanged, this, &WelcomeUI::updateSettings);
    connect(ui->iconThemeSelect, SIGNAL(currentTextChanged(QString)), this, SLOT(updateSettings()));

    return 0;
}


void WelcomeUI::checkStatus() {
    thread = new QThread();
    Renovatio* worker = new Renovatio(gameData->version);
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &Renovatio::process);
    connect(worker, &Renovatio::finished, thread, &QThread::quit);
    connect(worker, &Renovatio::status, this, &WelcomeUI::setStatus);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}


bool WelcomeUI::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void WelcomeUI::launchLevelSelector() {

    editDialog = new LevelManager;
    editDialog->initialise(&gameData, 1);
    connect(editDialog, &LevelManager::listDoubleClicked, this, &WelcomeUI::setLevel);
    connect(editDialog, &LevelManager::listDoubleClicked, editDialog, &LevelManager::close);
    editDialog->show();

}


void WelcomeUI::setLevel(QString lName) {
    ui->levelSelector->setText(lName);
    QJsonObject level = gameData->GetLevels().value(lName).toObject();
    ui->difficulty->setText(level["difficulty"].toString());
    ui->chk->setText(QString::number(level["levels"].toString().split(" ").count()));
}


int WelcomeUI::startGame() {

    QString hex = "#" +gameData->iconThemes.split(",")[ui->iconThemeSelect->currentIndex()].split("#")[!(this->isDarkTheme()) + 1];
    QString bHex = (isDarkTheme()) ? "#000000" : "#ffffff";

    // KDEPlatformTheme automatically adds accelerators to QPushButton
    // https://stackoverflow.com/questions/32688153/how-to-disable-automatic-mnemonics-in-a-qt-application-on-kde
    // https://bugs.kde.org/show_bug.cgi?id=337491
    static QRegularExpression accelerator("[\\&]");
    QString passcode = ui->levelSelector->text().remove(accelerator);

    if (!gameData->GetLevels().contains(passcode) || passcode == "Select Level") {
        QMessageBox::critical(this, "wikiLYNX", "Invalid Code!", QMessageBox::Ok);
        return 1;
    }

    game = new GameWindow;
    connect(&(game->congratsView), &Congrats::closed, this, &WelcomeUI::reset);
    connect(game, &GameWindow::gameEnded, this, &WelcomeUI::updateLogs);
    if (!(ui->keyboardToggle->isChecked())) this->grabKeyboard();
    this->hide();

    QJsonObject gData = gameData->GetLevel(passcode);

    game->initialise(&gData, dontKillParse0, hex+"|"+bHex, gameData->getSettings().value("notwiki").toInt(), ui->playerName->text(), passcode);
    *dontKillParse0 = 0;
    QThread::msleep(500);
    game->showFullScreen();
    game->setFocus();
    return 0;

}


void WelcomeUI::reset() {
    this->show();
    this->setFocus();
    this->releaseKeyboard();
    *dontKillParse0 = 1;
    this->checkStatus();
}


void WelcomeUI::updateUI() {

    ui->chk->setText("checkpoints");
    ui->difficulty->setText("difficulty");
    ui->levelSelector->setText("Select Level");

    for (int i = 0; i < gameData->iconThemes.split(",").count(); ++i) {
        ui->iconThemeSelect->addItem(gameData->iconThemes.split(",")[i].split("#")[0]);
    }
    ui->iconThemeSelect->setCurrentText(gameData->getSettings().value("iconTheme").toString());

    ui->allowSitesToggle->setChecked(gameData->getSettings().value("allowReference").toBool());

    ui->killToggle->setChecked(gameData->getSettings().value("totemOfUndying").toBool());

}


void WelcomeUI::updateSettings() {
    gameData->updateSettings("totemOfUndying", ui->killToggle->isChecked());
    gameData->updateSettings("iconTheme", ui->iconThemeSelect->currentText());
    gameData->updateSettings("allowReference", ui->allowSitesToggle->isChecked());
    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    theme = gameData->getSettings().value("iconTheme").toString() + theme;
    QIcon::setThemeName(theme);
    this->update();
}


void WelcomeUI::addCustom() {
    editDialog = new LevelManager;
    editDialog->initialise(&gameData);
    editDialog->show();
}


void WelcomeUI::genRandomLevel() {

    levelEditorDlg = new LevelEditor;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->genRandTxt->show();
    ui->genRandPrg->show();
    disconnect(this->levelEditorDlg, &LevelEditor::genRandomFinished, ui->genRandTxt, &QProgressBar::hide);
    disconnect(this->levelEditorDlg, &LevelEditor::genRandomFinished, ui->genRandPrg, &QProgressBar::hide);
    disconnect(this->levelEditorDlg, &LevelEditor::genRandomFinished, this, &WelcomeUI::startGame);
    disconnect(this->levelEditorDlg, &LevelEditor::genRandomFinished, this->levelEditorDlg, &LevelEditor::close);
    levelEditorDlg->genRandomLevel(gameData, "random");
    levelEditorDlg->showMaximized();
    levelEditorDlg->hide();
    this->setFocus();

    ui->chk->setText("random");
    ui->difficulty->setText("random");
    ui->levelSelector->setText("random");

    connect(this->levelEditorDlg, &LevelEditor::genRandomFinished, this, &WelcomeUI::startGame);
    connect(this->levelEditorDlg, &LevelEditor::genRandomFinished, &QApplication::restoreOverrideCursor);
    connect(this->levelEditorDlg, &LevelEditor::genRandomFinished, this->levelEditorDlg, &LevelEditor::close);
    connect(this->levelEditorDlg, &LevelEditor::genRandomFinished, ui->genRandTxt, &QProgressBar::hide);
    connect(this->levelEditorDlg, &LevelEditor::genRandomFinished, ui->genRandPrg, &QProgressBar::hide);

}


void WelcomeUI::showRules() {
    helpDialog = new BaseBrowser;
    helpDialog->initialise("Help");
    helpDialog->showMaximized();
}


void WelcomeUI::showLogs() {
    //fs::create_directories(dirName.toStdString()+"/logs");
    //QDesktopServices::openUrl(QUrl::fromLocalFile(dirName+"/logs"));
    // Finally Fixed this :)
    //QDesktopServices::openUrl(QUrl::fromUserInput(QDir(dirName+"/logs").absolutePath()));

    QStringList desc;
    QList<QStringList> listData;
    QList<QStringList> actionData;
    QJsonObject logs = gameData->getGameLog();
    QStringList headerButtons = {"neutralOnline"};
    QStringList header = {"Instance", "Checkpoints", "Clicks", "Status", "Level", "Player", "Time Taken"};

    QStringList keys = logs.keys();

    for (const QString &instance : std::as_const(keys)) {
        QJsonObject log = logs.value(instance).toObject();
        QList<QString> logEntry;
        logEntry.append(instance);

        QStringList logKeys = log.keys();
        for (const QString &key : std::as_const(logKeys)) {
            if (key != "log") logEntry.append(log[key].toString());
            else desc.append(QString::fromUtf8(QByteArray::fromBase64(log.value(key).toString().toUtf8())));
        }
        listData.append(logEntry);
        actionData.append(QList<QString> ("history"));
    }

    baselist = new BaseList;
    baselist->setDesc(desc);
    baselist->initList("Logs", "", &header, &headerButtons, &listData, &actionData);
    baselist->show();
}


void WelcomeUI::updateLogs(QString instance, QJsonObject log) {
    gameData->updateGameLog(instance, log);
    if (log.value("gameStatus") == "Win!") gameData->appendPlayerStats(log["level"].toString(), log["playerName"].toString(), log["timeTaken"].toString());
}


void WelcomeUI::clearLogs() {
    gameData->clearLogs();
    QMessageBox::information(this, "wikiLYNX", "Logs cleared!", QMessageBox::Ok);
}


void WelcomeUI::setStatus(int c) {

    if (gameData->version > gameData->getSettings().value("version").toString()) {
        whatsNewDialog = new WhatsNew(gameData->ver);
        whatsNewDialog->show();
        gameData->updateSettings("version", gameData->version);
    }

    ui->status->setText(code[c].split("|")[0]);
    ui->statusIndicator->setIcon(QIcon::fromTheme(code[c].split("|")[1]));
    if (c != 0) checkWorldEvent();
    overview = new StatusOverview;
    overview->initialise(c, gameData->ver);
    connect(overview, &StatusOverview::devEnabled, this, &WelcomeUI::toggleDevOptions);
    return;
}


void WelcomeUI::toggleDevOptions() {
    if (gameData->getSettings().value("debug").toBool()) {
        static QRegularExpression s(" \\(.*\\)");
        ui->version->setText(ui->version->text().remove(s));
        gameData->updateSettings("debug", false);
    }
    else {
        ui->version->setText(ui->version->text() + " (Debug Mode)");
        gameData->updateSettings("debug", true);
    }
}


void WelcomeUI::checkWorldEvent() {

    QString day = QDate::currentDate().toString("ddMM");

    for (auto const& [key, val] : this->worldEvents) {
        if (day == key) {
            ui->status->setText(val.split('|')[1]);
            ui->statusIndicator->setIcon(QIcon::fromTheme(val.split('|')[0]));
        }
    }
}


void WelcomeUI::showStats() {
    statsDialog = new LeaderBoard;
    statsDialog->initialise(gameData);
    statsDialog->showMaximized();
}


void WelcomeUI::launchStatusOverview() {
    if (overview != nullptr) overview->show();
}


void WelcomeUI::showNews() {
    newsDialog = new News;
    newsDialog->initialise();
    newsDialog->show();
}

void WelcomeUI::showAbout() {
    aboutDialog = new About;
    if (gameData->getSettings().value("debug").toBool()) {
        aboutDialog->initDevMode();
        connect(aboutDialog, &About::turnOffDev, this, &WelcomeUI::toggleDevOptions);
    }
    aboutDialog->show();
}
