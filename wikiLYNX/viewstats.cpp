#include "include/viewstats.h"
#include "ui/ui_viewstats.h"

viewStats::viewStats(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::viewStats)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &viewStats::close);
    connect(ui->levelSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(loadData(int)));

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    QString logo = ui->appLogo->document()->toHtml();
    if (theme == "Light") logo.replace("#181818", "#ffffff");
    ui->appLogo->setHtml(logo);

}

viewStats::~viewStats()
{
    delete ui;
}


bool viewStats::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


int viewStats::initialise() {

    ui->levelSelect->clear();
    QFile statFile("./"+dirName+"/.stat");
    statFile.open(QIODevice::ReadOnly);
    if (statFile.isOpen()) {
        this->data = QJsonDocument::fromJson(statFile.readAll()).object();
        ui->levelSelect->addItems(data.keys());
    }
    statFile.close();
    return 0;
}


void viewStats::loadData(int s) {

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
