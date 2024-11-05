#include "include/welcome.h"
#include "ui/ui_welcome.h"

namespace fs = std::filesystem;


welcomeUI::welcomeUI(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::welcomeDialog)
{
    ui->setupUi(this);
    ui->editLevelButton->setEnabled(false);

    ui->initButton->setIcon(QIcon::fromTheme("play"));
    connect(ui->initButton, &QPushButton::clicked, this, &welcomeUI::startGame);
    connect(ui->passcodeInput, &QComboBox::currentIndexChanged, this, &welcomeUI::showLevelInfo);
    connect(ui->aboutButton, &QPushButton::clicked, this, &welcomeUI::showAbout);
    connect(ui->helpButton, &QPushButton::clicked, this, &welcomeUI::showRules);
    connect(ui->cLogsButton, &QPushButton::clicked, this, &welcomeUI::clearLogs);
    connect(ui->sLogsButton, &QPushButton::clicked, this, &welcomeUI::showLogs);
    connect(ui->editLevelButton, &QPushButton::clicked, this, &welcomeUI::addCustom);
    connect(ui->refreshButton, &QPushButton::clicked, this, &welcomeUI::loadSettings);
    connect(ui->refreshButton, &QPushButton::clicked, this, &welcomeUI::updateUI);
    connect(ui->statsButton, &QPushButton::clicked, this, &welcomeUI::showStats);

    ui->initButton->setFocus();

}


int welcomeUI::initialise(int *totem) {

    fs::create_directories("./"+dirName.toStdString());
    this->totemofUndying = totem;

    loadSettings();
    updateUI();

    connect(ui->killToggle, &QCheckBox::checkStateChanged, this, &welcomeUI::updateSettings);
    connect(ui->allowSitesToggle, &QCheckBox::checkStateChanged, this, &welcomeUI::updateSettings);

    return 0;
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
    game->initialise(&temp, dontKillParse0, "https://en.wikipedia.org", this->aRD, ui->playerName->text(), passcode);
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

    this->aRD = cfg["allowReference"].toInt();
    ui->allowSitesToggle->setChecked(aRD);

    *totemofUndying = cfg["totemOfUndying"].toInt();
    ui->killToggle->setChecked(cfg["totemOfUndying"].toInt());

    ui->passcodeInput->clear();
    ui->passcodeInput->addItems(data.keys());
    ui->passcodeInput->removeItem(ui->passcodeInput->findText("debug"));
}


void welcomeUI::updateSettings() {
    *totemofUndying = ui->killToggle->isChecked();
    this->aRD = ui->allowSitesToggle->isChecked();
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
    this->cfg["allowReference"] = this->aRD;
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

    const char* file = ("./.wLnKMeow/gData.json");
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
    QDesktopServices::openUrl(QUrl::fromLocalFile("./"+dirName+"/logs"));
}


void welcomeUI::clearLogs() {
    QDir("./"+dirName+"/logs").removeRecursively();
    QMessageBox::information(this, "wikiLYNX", "Logs cleared successfully!", QMessageBox::Ok);
}


void welcomeUI::showStats() {

    statsDialog.initialise();
    statsDialog.show();
}

void welcomeUI::showAbout() {
    aboutDialog.show();
}
