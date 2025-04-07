#include "include/getlevel.h"
#include "forms/ui_getlevel.h"


GetLevel::GetLevel(QWidget *parent) : QDialog(parent), ui(new Ui::GetLevel) {
    ui->setupUi(this);
    ui->progressBar->hide();
    connect(ui->exitButton, &QPushButton::clicked, this, &GetLevel::close);
    connect(ui->list, &QListWidget::clicked, this, &GetLevel::setEditStatus);
    connect(ui->helpButton, &QPushButton::clicked, this, &GetLevel::launchHelp);
    connect(ui->refreshButton, &QPushButton::clicked, this, &GetLevel::updateTable);

}


GetLevel::~GetLevel() {
    delete ui;
}


int GetLevel::initialise(ScoreSheet* gameData) {

    this->gameData = gameData;
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url("https://archive.dawn.org.in/DAWN/Projects/wikiLYNX/levels/.info");
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

    this->levelData = QJsonDocument::fromJson(reply->readAll()).object();

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new Levels(this);

    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);

    QStringList itemData = {"Code Name", "Time", "Clicks", "Checkpoints", "Difficulty"};
    QStringList icons = {"neutralOnline"};

    widget->setItem(itemData, icons);

    this->updateTable();

    reply->deleteLater();

    return 0;

}


int GetLevel::updateTable() {

    auto l = this->levelData.keys();
    ui->list->clear();

    for (int i = 0; i < this->levelData.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new Levels(this);

        QStringList itemData = {
            l[i],
            QString::number(this->levelData[l[i]].toObject()["time"].toInt()),
            QString::number(this->levelData[l[i]].toObject()["clicks"].toInt()),
            QString::number(this->levelData[l[i]].toObject()["checkpoints"].toInt()),
            this->levelData[l[i]].toObject()["difficulty"].toString()
        };

        if (!(gameData->getLevelPresence(l[i]).isEmpty())) {
            QStringList icons = {"delete"};
            widget->setItem(itemData, icons);
        }
        else {
            QStringList icons = {"download"};
            widget->setItem(itemData, icons);
        }

        connect(widget, &Levels::action0, this, &GetLevel::downloadLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

    return 0;
}


void GetLevel::downloadLevel(QString code) {

    ui->list->setDisabled(true);

    if (!(gameData->getLevelPresence(code).isEmpty())) {
        gameData->removeLevel(code);
        ui->status->setText("Deleted!");
        emit levelsUpdated();
        this->updateTable();
        ui->list->setEnabled(true);
        return;
    }

    ui->progressBar->show();
    ui->status->setText("Downloading...");

    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QUrl url("https://archive.dawn.org.in/DAWN/Projects/wikiLYNX/levels/"+this->levelData[code].toObject()["fname"].toString()); // Replace with the actual URL
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

    QJsonObject jsonObject = QJsonDocument::fromJson(reply->readAll()).object();

    gameData->updateLevel(code, jsonObject[code].toObject());

    this->updateTable();
    ui->status->setText("Done!");

    ui->progressBar->hide();
    emit levelsUpdated();

    ui->list->setEnabled(true);

}


void GetLevel::setEditStatus() {
    ui->status->setText("");
}


void GetLevel::launchHelp()   {
    QDesktopServices::openUrl(QUrl("https://github.com/flamboyantpenguin/wikilynx/wiki/Gameplay#downloading-levels"));
}

