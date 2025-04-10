#include "include/gamewindow.h"
#include "forms/ui_gamewindow.h"


namespace fs = std::filesystem;


GameWindow::GameWindow(GameBoi *gameSystem, int *dontKillMeParse, QString prgHex,
                       QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow) {

    ui->setupUi(this);

    // Parse GameBoi instance
    this->gameSystem = gameSystem;

    // Connect Signals of UI Elements to Slots
    connect(ui->showHistory, SIGNAL(clicked()), this, SLOT(launchLogs()));
    connect(ui->exitButton, SIGNAL(clicked()), gameSystem, SLOT(endGame()));
    connect(ui->viewChkButton, SIGNAL(clicked()), this, SLOT(viewCheckPoints()));
    //connect(ui->viewChkButton, SIGNAL(clicked()), this, SLOT(viewCheckPoints()));

    // Link GameBoi Subsystem to UI elements
    connect(gameSystem, SIGNAL(chkSuccess(int)), this, SLOT(updatePRG(int)));
    connect(gameSystem, SIGNAL(counter(QString)), this, SLOT(updateCountdown(QString)));
    connect(gameSystem, SIGNAL(sendMessage(QString)), this, SLOT(showMessage(QString)));
    connect(gameSystem, SIGNAL(end(QJsonObject,int)), this, SLOT(endGame(QJsonObject,int)));
    connect(gameSystem, SIGNAL(clicked(QString, QString)), this, SLOT(updateClicks(QString,QString)));


    // Set Logo according to Theme
    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();


    // Set Progress Bar Theme
    QFile plt(":/base/pallete/bannerPrg.plt");
    plt.open(QIODevice::ReadOnly);
    QString styleSheet = plt.readAll();
    plt.close();
    styleSheet.replace("#39ff14", prgHex.split("|")[0]);
    styleSheet.replace("#181818", prgHex.split("|")[1]);
    ui->progressBar->setStyleSheet(styleSheet);

    // Parse dontKillMe Pointer
    this->dontKillMe = dontKillMeParse;

    // Initialise Progress Bar as 0
    ui->progressBar->setValue(0);

    // Load Checkpoint[0]
    ui->field->load(QUrl::fromUserInput(gameSystem->getCurrentChk()));
    // This ensures the game starts only when the page loads
    connect(ui->field, &QWebEngineView::loadFinished, this, &GameWindow::startGame);


}


GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::startGame() {
    gameSystem->startGame();
    connect(ui->field, &QWebEngineView::urlChanged, this, &GameWindow::click);
    disconnect(ui->field, &QWebEngineView::loadFinished, this, &GameWindow::startGame);
}

void GameWindow::click(QUrl url) {
    gameSystem->click(url.toString());
}

void GameWindow::showMessage(QString message) {
    *dontKillMe = 1;
    QMessageBox::critical(this, "wikiLYNX", messageCodes[message], QMessageBox::Ok);
    *dontKillMe = 0;
    if (message == "RLV") ui->field->load(QUrl::fromUserInput(gameSystem->getCurrentChk()));
}

bool GameWindow::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}

// Update UI Aspects

// Update Click Count and Next Checkpoint URL
void GameWindow::updateClicks(QString clicks, QString level) {

    // Update UI
    ui->clicks->setText(clicks);
    ui->statusbar->showMessage("Next Checkpoint: "+level);

}

// Update Countdown/Countup
void GameWindow::updateCountdown(QString counter) {

    ui->counter->setText(counter);
    ui->clock->setText(QTime::currentTime().toString("hh:mm:ss"));

}

// Update Progress Bar
void GameWindow::updatePRG(int value) {
    ui->progressBar->setValue(value);
}


int GameWindow::endGame(QJsonObject gameStat, int code) {

    *dontKillMe = 1;
    ui->statusbar->showMessage(endCodes[code]);
    if (code != 3) QMessageBox::information(this, "wikiLYNX", endCodes[code], QMessageBox::Ok);

    congratsView.initialise(gameStat, code);
    congratsView.show();
    ui->field->close();
    close();
    return 0;
}


void GameWindow::launchLogs() {

    QStringList header = {"Logs"};
    QList<QStringList> listData;
    QList<QStringList> actionData;
    QStringList headerButtons = {"neutralOnline"};
    for (int i = 0; i < this->gameSystem->logs.count(); i++) {
        listData.append(QStringList(this->gameSystem->logs.value(i)));
        actionData.append(QStringList ("history"));
    }

    baselist = new BaseList;
    baselist->dontKillMe = (this->dontKillMe);
    baselist->initList("Logs", "", &header, &headerButtons, &listData, &actionData);
    baselist->show();

}


void GameWindow::viewCheckPoints() {

    QStringList header = {"Checkpoints"};
    QList<QStringList> listData;
    QList<QStringList> actionData;
    QStringList headerButtons = {"neutralOnline"};
    for (int i = 0; i < this->gameSystem->getLevels().count(); i++) {
        listData.append(QStringList (this->gameSystem->getLevels().value(i)));
        QStringList tmp = (i <= gameSystem->getChk()) ? QStringList("online") : QStringList("offline");
        actionData.append(tmp);
    }

    baselist = new BaseList;
    baselist->dontKillMe = (this->dontKillMe);
    baselist->initList("Checkpoints", "", &header, &headerButtons, &listData, &actionData);
    baselist->show();

}
