#include "include/leaderboard.h"
#include "ui/ui_leaderboard.h"

leaderboard::leaderboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::leaderboard)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &leaderboard::close);
    connect(ui->levelSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(loadData(int)));

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}

leaderboard::~leaderboard()
{
    delete ui;
}


bool leaderboard::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


int leaderboard::initialise() {

    ui->levelSelect->clear();
    QFile statFile(dirName+"/.stat");
    if (!(statFile.exists())) return 0;
    statFile.open(QIODevice::ReadOnly);
    if (statFile.isOpen()) {
        this->data = QJsonDocument::fromJson(statFile.readAll()).object();
        ui->levelSelect->addItems(data.keys());
    }
    statFile.close();
    return 0;
}


void leaderboard::loadData(int s) {

    auto level = ui->levelSelect->currentText();

    QJsonObject leaderBoard = this->data.value(level).toObject();

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
