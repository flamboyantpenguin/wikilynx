#include "include/getlevel.h"
#include "ui/ui_getlevel.h"


getLevel::getLevel(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getLevel)
{
    ui->setupUi(this);
    ui->downloadButton->setEnabled(false);
    connect(ui->table, &QTableWidget::clicked, this, &getLevel::setEditStatus);
    connect(ui->downloadButton, &QPushButton::clicked, this, &getLevel::downloadLevel);
    connect(ui->exitButton, &QPushButton::clicked, this, &getLevel::close);

}


getLevel::~getLevel()
{
    delete ui;
}


int getLevel::initialise() {

    QFile lFile("./gData/gData.json");
    if (lFile.isOpen()) lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    this->cfg = temp["info"].toObject();
    this->iData = temp["data"].toObject();

    QNetworkAccessManager *manager = new QNetworkAccessManager();


    QUrl url("https://archive.pcland.co.in/DAWN/Projects/wikiLYNX/levels/.info");
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        ui->status->setText("Unable to reach Archives!");
        qDebug() << "Can't connect to Archive:" << reply->errorString();
        return 1;
    }

    ui->status->setText("Connected!");
    QByteArray data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = json.object();

    this->levelData = jsonObject;

    this->updateTable();

    reply->deleteLater();

    return 0;

}


int getLevel::updateTable() {

    auto l = this->levelData.keys();
    ui->table->setRowCount(l.count());

    ui->table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    for (int i = 0; i < this->levelData.count(); i++) {

        QTableWidgetItem* code = new QTableWidgetItem();
        QTableWidgetItem* time = new QTableWidgetItem();
        QTableWidgetItem* chk = new QTableWidgetItem();
        QTableWidgetItem* difficulty = new QTableWidgetItem();
        QTableWidgetItem* status = new QTableWidgetItem();
        status->setText("X");
        code->setText(l[i]);
        time->setText(QString::number(this->levelData[l[i]].toObject()["time"].toInt()));
        chk->setText(QString::number(this->levelData[l[i]].toObject()["checkpoints"].toInt()));
        difficulty->setText(this->levelData[l[i]].toObject()["difficulty"].toString());
        ui->table->setItem(i, 0, code);
        ui->table->setItem(i, 1, time);
        ui->table->setItem(i, 2, chk);
        ui->table->setItem(i, 3, difficulty);

        if (this->iData.contains(l[i])) {
            status->setText("Y");
        }
        ui->table->setItem(i, 4, status);

    }

    return 0;
}


void getLevel::downloadLevel() {

    auto t = ui->table->currentIndex();
    auto cde = ui->table->item(t.row(), 0)->text();

    QNetworkAccessManager *manager = new QNetworkAccessManager();


    QUrl url("https://archive.pcland.co.in/DAWN/Projects/wikiLYNX/levels/"+this->levelData[cde].toObject()["fname"].toString()); // Replace with the actual URL
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        ui->status->setText("Unable to reach Archives!");
        qDebug() << "Can't download level" << reply->errorString();
        return;
    }

    ui->status->setText("Loading Level...");
    QByteArray data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = json.object();

    QJsonDocument document;
    QJsonObject temp;

    iData.insert(cde, jsonObject[cde].toObject());

    temp.insert("info", this->cfg);
    temp.insert("data", this->iData);
    document.setObject(temp);

    QFile::remove("./gData/gData.json");

    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file("./gData/gData.json");
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadWrite);
    QTextStream iStream(&file);
    iStream << bytes;
    file.flush();
    file.close();
    this->updateTable();
    ui->status->setText("Done!");

}


void getLevel::setEditStatus() {
    if (ui->table->item(ui->table->currentIndex().row(), 1) && !ui->table->item(ui->table->currentIndex().row(), 1)->text().isEmpty())
        ui->downloadButton->setEnabled(true);
}
