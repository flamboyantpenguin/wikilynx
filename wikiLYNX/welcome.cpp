#include "include/welcome.h"
#include "ui/ui_welcome.h"

namespace fs = std::filesystem;


welcomeUI::welcomeUI(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::welcomeDialog)
{

    ui->setupUi(this);
    ui->genRandTxt->hide();
    ui->genRandPrg->hide();
    ui->editLevelButton->setEnabled(false);
    connect(ui->newsButton, &QPushButton::clicked, this, &welcomeUI::showNews);
    connect(ui->initButton, &QPushButton::clicked, this, &welcomeUI::startGame);
    connect(ui->helpButton, &QPushButton::clicked, this, &welcomeUI::showRules);
    connect(ui->sLogsButton, &QPushButton::clicked, this, &welcomeUI::showLogs);
    connect(ui->aboutButton, &QPushButton::clicked, this, &welcomeUI::showAbout);
    connect(ui->cLogsButton, &QPushButton::clicked, this, &welcomeUI::clearLogs);
    connect(ui->statsButton, &QPushButton::clicked, this, &welcomeUI::showStats);
    connect(ui->refreshButton, &QPushButton::clicked, this, &welcomeUI::updateUI);
    connect(ui->editLevelButton, &QPushButton::clicked, this, &welcomeUI::addCustom);
    connect(ui->refreshButton, &QPushButton::clicked, this, &welcomeUI::loadSettings);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &welcomeUI::checkStatus);
    connect(ui->genRandomLevel, &QPushButton::clicked, this, &welcomeUI::genRandomLevel);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &welcomeUI::launchStatusOverview);
    connect(ui->passcodeInput, SIGNAL(activated(int)), this, SLOT(showLevelInfo(int)));

    ui->initButton->setFocus();

    this->checkStatus();

}


int welcomeUI::initialise(int *totem) {

    fs::create_directories(dirName.toStdString());
    this->totemofUndying = totem;

    loadSettings();
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

    theme = this->cfg["iconTheme"].toString() + theme;
    QIcon::setThemeName(theme);
    qDebug() << "Current Icon Theme:" << QIcon::themeName();
    this->update();

    //qDebug() << logo;

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


void welcomeUI::showLevelInfo(int s) {

    QString lName = ui->passcodeInput->currentText();
    auto level = data[lName].toObject();
    ui->chk->setText(QString::number(level["levels"].toString().split(" ").count()));
    ui->difficulty->setText(level["difficulty"].toString());

}


int welcomeUI::startGame() {

    QString hex = "#" +this->base["availableIconThemes"].toString().split(",")[ui->iconThemeSelect->currentIndex()].split("#")[!(this->isDarkTheme()) + 1];
    QString bHex = (isDarkTheme()) ? "#000000" : "#ffffff";

    QString passcode = ui->passcodeInput->currentText();
    if (!data.contains(passcode)) {
        QMessageBox::critical(this, "wikiLYNX", "Invalid Code!", QMessageBox::Ok);
        return 1;
    }

    this->game = new GameWindow;
    connect(&(this->game->congratsView), &congrats::closed, this, &welcomeUI::reset);
    if (!(ui->keyboardToggle->isChecked())) this->grabKeyboard();
    this->hide();
    auto gData = data[passcode].toObject();
    game->initialise(&gData, dontKillParse0, hex+"|"+bHex, this->cfg["allowReference"].toInt(), ui->playerName->text(), passcode);
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


void welcomeUI::loadSettings() {

    this->checkCustom();

    QFile cFile(":/cfg/gameData.json");
    ui->editLevelButton->setEnabled(true);
    cFile.open(QIODevice::ReadOnly);
    auto iData = QJsonDocument::fromJson(cFile.readAll()).object();
    this->base = iData["base"].toObject();
    iData = iData["data"].toObject();
    QFile sFile(dirName+"/gData.json");
    sFile.open(QIODevice::ReadOnly);
    auto nData = QJsonDocument::fromJson(sFile.readAll()).object();
    this->cfg = nData["info"].toObject();
    nData = nData["data"].toObject();
    auto d = iData.toVariantMap();
    d.insert(nData.toVariantMap());
    this->data = QJsonObject::fromVariantMap(d);
    sFile.flush();
    sFile.close();
    cFile.close();

}


void welcomeUI::updateUI() {

    //ui->iconThemeSelect->clear();

    for (int i = 0; i < this->base["availableIconThemes"].toString().split(",").count(); ++i) {
        ui->iconThemeSelect->addItem(this->base["availableIconThemes"].toString().split(",")[i].split("#")[0]);
    }
    ui->iconThemeSelect->setCurrentText(this->cfg["iconTheme"].toString());

    ui->allowSitesToggle->setChecked(this->cfg["allowReference"].toInt());

    *totemofUndying = cfg["totemOfUndying"].toInt();
    ui->killToggle->setChecked(cfg["totemOfUndying"].toInt());

    ui->passcodeInput->clear();
    ui->passcodeInput->addItems(this->data.keys());
    ui->passcodeInput->removeItem(ui->passcodeInput->findText("debug"));
}


void welcomeUI::updateSettings() {
    *totemofUndying = ui->killToggle->isChecked();
    this->cfg["allowReference"] = ui->allowSitesToggle->isChecked();
    this->cfg["iconTheme"] = ui->iconThemeSelect->currentText();
    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    theme = this->cfg["iconTheme"].toString() + theme;
    QIcon::setThemeName(theme);
    this->update();
    saveSettings();
}


void welcomeUI::saveSettings() {

    QFile lFile(dirName+"/gData.json");
    if (lFile.isOpen())
        lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.flush();
    lFile.close();

    QJsonObject d = temp["data"].toObject();

    //this->cfg = this->cfg;
    this->cfg["totemOfUndying"] = *totemofUndying;

    QJsonDocument document;
    QJsonObject nContent;

    nContent.insert("info", this->cfg);
    nContent.insert("data", d);

    document.setObject(nContent);

    QFile::remove(dirName+"/gData.json");
    QFile file(dirName+"/gData.json");
    file.open(QIODevice::ReadWrite);
    file.write(document.toJson());
    file.flush();
    file.close();
}


void welcomeUI::checkCustom() {

    const char* file = (dirName+"/gData.json").toStdString().c_str();
    struct stat sb;

    if (!(QFile(dirName+"/gData.json").exists())) {

        QFile cFile(":/cfg/gameData.json");
        ui->editLevelButton->setEnabled(true);
        cFile.open(QIODevice::ReadOnly);
        auto iData = QJsonDocument::fromJson(cFile.readAll()).object();
        this->cfg = iData["info"].toObject();
        //this->cfg["iconTheme"] = "green";

        QJsonDocument document;
        QJsonObject temp;

        temp.insert("info", this->cfg);
        document.setObject(temp);

        QFile::remove(dirName+"/gData.json");

        QByteArray bytes = document.toJson(QJsonDocument::Indented);
        QFile file(dirName+"/gData.json");
        file.open(QIODevice::ReadWrite);
        QTextStream iStream(&file);
        iStream << bytes;
        file.flush();
        file.close();
    }
}


void welcomeUI::addCustom() {
    editDialog.initialise();
    editDialog.show();
}


void welcomeUI::genRandomLevel() {
    this->setCursor(Qt::WaitCursor);
    ui->genRandTxt->show();
    ui->genRandPrg->show();
    disconnect(&this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandTxt, &QProgressBar::hide);
    disconnect(&this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandPrg, &QProgressBar::hide);
    disconnect(&this->levelEditorDlg, &levelEditor::genRandomFinished, this, &welcomeUI::startGame);
    disconnect(&this->levelEditorDlg, &levelEditor::genRandomFinished, &this->levelEditorDlg, &levelEditor::close);
    levelEditorDlg.genRandomLevel(&(this->data), "random");
    levelEditorDlg.showMaximized();
    levelEditorDlg.hide();
    this->setFocus();
    ui->passcodeInput->addItem("random");
    ui->passcodeInput->setCurrentText("random");
    connect(&this->levelEditorDlg, &levelEditor::genRandomFinished, this, &welcomeUI::unsetCursor);
    connect(&this->levelEditorDlg, &levelEditor::genRandomFinished, this, &welcomeUI::startGame);
    connect(&this->levelEditorDlg, &levelEditor::genRandomFinished, &this->levelEditorDlg, &levelEditor::close);
    connect(&this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandTxt, &QProgressBar::hide);
    connect(&this->levelEditorDlg, &levelEditor::genRandomFinished, ui->genRandPrg, &QProgressBar::hide);

}


void welcomeUI::showRules() {
    helpDialog.initialise();
    helpDialog.showMaximized();
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

    if (this->base["version"].toString() > this->cfg["version"].toString()) {
        whatsNewDialog.show();
        this->cfg["version"] = this->base["version"].toString();
        this->saveSettings();
    }

    ui->status->setText(code[c].split("|")[0]);
    ui->statusIndicator->setIcon(QIcon::fromTheme(code[c].split("|")[1]));
    if (c != 0) checkWorldEvent();
    overview.initialise(c);
    return;
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
    statsDialog.initialise();
    statsDialog.showMaximized();
}


void welcomeUI::launchStatusOverview() {
    //overiDataview.initialise();
    overview.show();
}


void welcomeUI::showNews() {
    newsDialog.initialise();
    newsDialog.show();
}

void welcomeUI::showAbout() {
    aboutDialog.show();
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
