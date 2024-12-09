#include "include/welcome.h"
#include "ui/ui_welcome.h"

namespace fs = std::filesystem;


welcomeUI::welcomeUI(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::welcomeDialog)
{
    ui->setupUi(this);
    ui->editLevelButton->setEnabled(false);

    this->checkStatus();
    connect(ui->initButton, &QPushButton::clicked, this, &welcomeUI::startGame);
    connect(ui->passcodeInput, SIGNAL(currentIndexChanged(int)), this, SLOT(showLevelInfo()));
    connect(ui->aboutButton, &QPushButton::clicked, this, &welcomeUI::showAbout);
    connect(ui->helpButton, &QPushButton::clicked, this, &welcomeUI::showRules);
    connect(ui->cLogsButton, &QPushButton::clicked, this, &welcomeUI::clearLogs);
    connect(ui->sLogsButton, &QPushButton::clicked, this, &welcomeUI::showLogs);
    connect(ui->editLevelButton, &QPushButton::clicked, this, &welcomeUI::addCustom);
    connect(ui->refreshButton, &QPushButton::clicked, this, &welcomeUI::loadSettings);
    connect(ui->refreshButton, &QPushButton::clicked, this, &welcomeUI::updateUI);
    connect(ui->statsButton, &QPushButton::clicked, this, &welcomeUI::showStats);
    connect(ui->newsButton, &QPushButton::clicked, this, &welcomeUI::showNews);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &welcomeUI::checkStatus);
    connect(ui->statusIndicator, &QPushButton::clicked, this, &welcomeUI::launchStatusOverview);

    ui->initButton->setFocus();

}


int welcomeUI::initialise(int *totem) {

    fs::create_directories("./"+dirName.toStdString());
    this->totemofUndying = totem;

    loadSettings();
    updateUI();

    // Set icon theme
    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    theme = this->cfg["iconTheme"].toString() + theme;
    QIcon::setThemeName(theme);
    qDebug() << "Current Icon Theme:" << QIcon::themeName();

    connect(ui->killToggle, &QCheckBox::checkStateChanged, this, &welcomeUI::updateSettings);
    connect(ui->allowSitesToggle, &QCheckBox::checkStateChanged, this, &welcomeUI::updateSettings);
    connect(ui->iconThemeSelect, SIGNAL(currentTextChanged(QString)), this, SLOT(updateSettings()));

    return 0;
}


void welcomeUI::checkStatus() {

    thread = new QThread();
    checkUpdateWorker* worker = new checkUpdateWorker();
    worker->moveToThread(thread);

    connect( thread, &QThread::started, worker, &checkUpdateWorker::process);
    connect( worker, &checkUpdateWorker::finished, thread, &QThread::quit);
    connect( worker, &checkUpdateWorker::finished, worker, &checkUpdateWorker::deleteLater);
    connect(worker, SIGNAL(status(int)), this, SLOT(setStatus(int)));
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();

}


bool welcomeUI::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void welcomeUI::showLevelInfo() {

    QString lName = ui->passcodeInput->currentText();
    auto level = data[lName].toObject();
    ui->chk->setText(QString::number(level["checkpoints"].toInt()));
    ui->difficulty->setText(level["difficulty"].toString());

}


int welcomeUI::startGame() {

    QString passcode = ui->passcodeInput->currentText();
    if (!data.contains(passcode)) {
        QMessageBox::critical(this, "wikiLYNX", "Invalid Code!", QMessageBox::Ok);
        return 1;
    }

    this->game = new GameWindow;
    connect(&(this->game->congratsView), &congrats::closed, this, &welcomeUI::reset);
    this->grabKeyboard();
    this->hide();
    auto temp = data[passcode].toObject();
    game->initialise(&temp, dontKillParse0, "https://en.wikipedia.org", this->cfg["allowReference"].toInt(), ui->playerName->text(), passcode);
    *dontKillParse0 = 0;
    QThread::msleep(500);
    game->showFullScreen();
    QApplication::processEvents();
    return 0;

}


void welcomeUI::reset() {
    this->show();
    this->releaseKeyboard();
    *dontKillParse0 = 1;
    this->checkStatus();
}


void welcomeUI::loadSettings() {

    checkCustom();

    QFile cFile(":/cfg/gameData.json");
    ui->editLevelButton->setEnabled(true);
    cFile.open(QIODevice::ReadOnly);
    auto iData = QJsonDocument::fromJson(cFile.readAll()).object();
    iData = iData["data"].toObject();
    QFile sFile("./"+dirName+"/gData.json");
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

    ui->iconThemeSelect->addItems(this->cfg["availableIconThemes"].toString().split("|"));
    ui->iconThemeSelect->setCurrentText(this->cfg["iconTheme"].toString());

    ui->allowSitesToggle->setChecked(this->cfg["allowReference"].toInt());

    *totemofUndying = cfg["totemOfUndying"].toInt();
    ui->killToggle->setChecked(cfg["totemOfUndying"].toInt());

    ui->passcodeInput->clear();
    ui->passcodeInput->addItems(data.keys());
    ui->passcodeInput->removeItem(ui->passcodeInput->findText("debug"));
}


void welcomeUI::updateSettings() {
    *totemofUndying = ui->killToggle->isChecked();
    this->cfg["allowReference"] = ui->allowSitesToggle->isChecked();
    this->cfg["iconTheme"] = ui->iconThemeSelect->currentText();
    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    theme = this->cfg["iconTheme"].toString() + theme;
    QIcon::setThemeName(theme);
    QWidget::update();
    //QApplication::processEvents();
    saveSettings();
}


void welcomeUI::saveSettings() {

    QFile lFile("./"+dirName+"/gData.json");
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

    QFile::remove("./"+dirName+"/gData.json");
    QFile file("./"+dirName+"/gData.json");
    file.open(QIODevice::ReadWrite);
    file.write(document.toJson());
    file.flush();
    file.close();
}


void welcomeUI::checkCustom() {

    const char* file = ("./.wikilynx/gData.json");
    struct stat sb;

    if (stat(file, &sb) != 0 && !(sb.st_mode & S_IFDIR)) {

        QFile cFile(":/cfg/gameData.json");
        ui->editLevelButton->setEnabled(true);
        cFile.open(QIODevice::ReadOnly);
        auto iData = QJsonDocument::fromJson(cFile.readAll()).object();
        this->cfg = iData = iData["info"].toObject();

        QJsonDocument document;
        QJsonObject temp;

        temp.insert("info", this->cfg);
        document.setObject(temp);

        QFile::remove("./"+dirName+"/gData.json");

        QByteArray bytes = document.toJson( QJsonDocument::Indented );
        QFile file("./"+dirName+"/gData.json");
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


void welcomeUI::showRules() {
    helpDialog.show();
}


void welcomeUI::showLogs() {
    fs::create_directories("./"+dirName.toStdString()+"/logs");
    //QDesktopServices::openUrl(QUrl::fromLocalFile("./"+dirName+"/logs"));
    // Finally Fixed this :)
    QDesktopServices::openUrl(QUrl::fromUserInput(QDir("./"+dirName+"/logs").absolutePath()));
}


void welcomeUI::clearLogs() {
    QDir("./"+dirName+"/logs").removeRecursively();
    QMessageBox::information(this, "wikiLYNX", "Logs cleared successfully!", QMessageBox::Ok);
}


void welcomeUI::setStatus(int c) {

    ui->status->setText(code[c].split("|")[0]);
    ui->statusIndicator->setIcon(QIcon::fromTheme(code[c].split("|")[1]));
    if (c != 0)  checkWorldEvent();
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
    statsDialog.show();
}


void welcomeUI::launchStatusOverview() {
    //overview.initialise();
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

    int flg;
    QNetworkAccessManager manager;
    QUrl url("https://wikipedia.org");
    QNetworkReply *reply = manager.get(QNetworkRequest(url));

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        emit status(0);
        flg = 1;
    }
    reply->deleteLater();

    url = QUrl("https://repo.pcland.co.in/QtOnline/wikiLYNX/.info/version.txt");
    reply = manager.get(QNetworkRequest(url));

    QEventLoop loop2;
    QObject::connect(reply, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
    loop2.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->read(7);
        lVersion = QString::fromLocal8Bit(data).toStdString();
    } else {
        qDebug() << "Error fetching latest version information:" << reply->errorString();
        if (!flg) emit status(3);
        flg = 1;
    }
    reply->deleteLater();


    if (!flg) {
        if (strcmp(lVersion.c_str(), version.c_str()) > 0) emit status(2);
        else if (strcmp(lVersion.c_str(), version.c_str()) < 0) emit status(4);
        else emit status(1);
    }

    emit finished();

}
