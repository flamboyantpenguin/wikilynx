#include "include/getlevel.h"
#include "ui/ui_getlevel.h"


getLevel::getLevel(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::getLevel)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(ui->exitButton, &QPushButton::clicked, this, &getLevel::close);
    connect(ui->list, &QListWidget::clicked, this, &getLevel::setEditStatus);
    connect(ui->helpButton, &QPushButton::clicked, this, &getLevel::launchHelp);
    connect(ui->refreshButton, &QPushButton::clicked, this, &getLevel::updateTable);
}


getLevel::~getLevel()
{
    delete ui;
}


int getLevel::initialise() {

    QFile lFile("./"+dirName+"/gData.json");
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
        ui->status->setText("Unable to reach Archives :(");
        qDebug() << "Can't connect to Archives:" << reply->errorString();
        return 1;
    }

    ui->status->setText("Connected!");
    QByteArray data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = json.object();

    this->levelData = jsonObject;

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new levels(this);

    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);

    widget->setItem("Code Name", \
                    "Time", \
                    "Clicks", \
                    "Checkpoints", \
                    "Difficulty", \
                    "neutralOnline", "", "");

    this->updateTable();

    reply->deleteLater();

    return 0;

}


int getLevel::updateTable() {

    auto l = this->levelData.keys();
    ui->list->clear();

    for (int i = 0; i < this->levelData.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        if (this->iData.contains(l[i])) {
            widget->setItem(l[i], \
                QString::number(this->levelData[l[i]].toObject()["time"].toInt()), \
                QString::number(this->levelData[l[i]].toObject()["clicks"].toInt()), \
                QString::number(this->levelData[l[i]].toObject()["checkpoints"].toInt()), \
                this->levelData[l[i]].toObject()["difficulty"].toString(), \
                "delete", "", "");
        }
        else {
            widget->setItem(l[i], \
                QString::number(this->levelData[l[i]].toObject()["time"].toInt()), \
                QString::number(this->levelData[l[i]].toObject()["clicks"].toInt()), \
                QString::number(this->levelData[l[i]].toObject()["checkpoints"].toInt()), \
                this->levelData[l[i]].toObject()["difficulty"].toString(), \
                "download", "", "");
        }

        connect(widget, &levels::action0, this, &getLevel::downloadLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

    return 0;
}


void getLevel::downloadLevel(QString code) {

    if (this->iData.contains(code)) {
        this->deleteLevel(code);
        return;
    }

    ui->progressBar->show();

    ui->status->setText("Downloading...");

    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url("https://archive.pcland.co.in/DAWN/Projects/wikiLYNX/levels/"+this->levelData[code].toObject()["fname"].toString()); // Replace with the actual URL
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        ui->status->setText("Can't reach Archives!");
        qDebug() << "Can't download level" << reply->errorString();
        return;
    }

    ui->status->setText("Loading Level...");
    QByteArray data = reply->readAll();
    QJsonDocument json = QJsonDocument::fromJson(data);
    QJsonObject jsonObject = json.object();

    QJsonDocument document;
    QJsonObject temp;

    iData.insert(code, jsonObject[code].toObject());

    temp.insert("info", this->cfg);
    temp.insert("data", this->iData);
    document.setObject(temp);

    QFile::remove("./"+dirName+"/gData.json");

    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file("./"+dirName+"/gData.json");
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadWrite);
    QTextStream iStream(&file);
    iStream << bytes;
    file.flush();
    file.close();
    this->updateTable();
    ui->status->setText("Done!");

    ui->progressBar->hide();

}


void getLevel::deleteLevel(QString code) {

    QJsonDocument document;
    QJsonObject temp;

    iData.remove(code);

    temp.insert("info", this->cfg);
    temp.insert("data", this->iData);
    document.setObject(temp);

    QFile::remove("./"+dirName+"/gData.json");

    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file("./"+dirName+"/gData.json");
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadWrite);
    QTextStream iStream(&file);
    iStream << bytes;
    file.flush();
    file.close();
    this->updateTable();
    ui->status->setText("Deleted!");

}


void getLevel::setEditStatus() {
    ui->status->setText("");
}


void getLevel::launchHelp()   {
    QDesktopServices::openUrl(QUrl("https://github.com/flamboyantpenguin/wikilynx/wiki/Gameplay#downloading-levels"));
}

