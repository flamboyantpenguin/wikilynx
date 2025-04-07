#include "include/leaderboard.h"
#include "forms/ui_leaderboard.h"


LeaderBoard::LeaderBoard(QWidget *parent) : QDialog(parent), ui(new Ui::LeaderBoard) {
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &LeaderBoard::close);
    connect(ui->levelSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(loadData(int)));

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}

LeaderBoard::~LeaderBoard() {
    delete ui;
}


bool LeaderBoard::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


int LeaderBoard::initialise(ScoreSheet *gameData) {

    ui->levelSelect->clear();
    this->gameData = gameData;
    ui->levelSelect->addItems(gameData->getStatLevels());
    return 0;
}


void LeaderBoard::loadData(int s) {

    QString level = ui->levelSelect->currentText();

    QJsonObject leaderBoard = gameData->getLeaderBoard(level);

    ui->table->setRowCount(leaderBoard.count());

    int i = 0;
    QStringList keys = leaderBoard.keys();
    for (const QString& key : keys) {
        QTableWidgetItem* timeTaken = new QTableWidgetItem();
        QTableWidgetItem* playerName = new QTableWidgetItem();
        timeTaken->setText(key);
        playerName->setText(leaderBoard[key].toString());
        ui->table->setItem(i, 0, timeTaken);
        ui->table->setItem(i, 1, playerName);
        i++;
    }

    ui->table->sortByColumn(ui->table->columnAt(0), Qt::AscendingOrder);

}
