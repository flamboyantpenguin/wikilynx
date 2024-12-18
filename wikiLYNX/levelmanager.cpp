#include "include/levelmanager.h"
#include "ui/ui_levelmanager.h"


#include <QMessageBox>


levelManager::levelManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::levelManager)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &levelManager::addLevel);
    connect(ui->randomButton, &QPushButton::clicked, this, &levelManager::genRandomLevel);
    connect(ui->loadButton, &QPushButton::clicked, this, &levelManager::importLevels);
    connect(ui->exportButton, &QPushButton::clicked, this, &levelManager::exportLevels);
    connect(ui->downloadButton, &QPushButton::clicked, this, &levelManager::downloadLevel);
    connect(&getLevelDialog, &getLevel::closed, this, &levelManager::initialise);
    connect(&levelEditorDialog, SIGNAL(closed()), this, SLOT(saveData()));
    connect(ui->closeButton, &QPushButton::clicked, this, &levelManager::close);

}


levelManager::~levelManager()
{
    delete ui;
}


void levelManager::initialise() {

    QFile lFile("./"+dirName+"/gData.json");
    if (lFile.isOpen()) lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    this->cfg = temp["info"].toObject();
    this->iData = temp["data"].toObject();

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new levels(this);

    widget->setItem("Code", \
                    "Time (Seconds)", \
                    "Clicks", \
                    "Checkpoints", \
                    "Difficulty", \
                    "neutralOnline", "neutralOnline", "");

    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);
    this->updateTable();

}


void levelManager::updateTable() {

    ui->list->clear();
    auto l = this->iData.keys();

    for (int i = 0; i < l.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        widget->setItem(l[i], \
            QString::number(iData[l[i]].toObject()["time"].toDouble()), \
            QString::number(iData[l[i]].toObject()["clicks"].toInt()), \
            QString::number(iData[l[i]].toObject()["levels"].toString().split(" ").count()), \
            iData[l[i]].toObject()["difficulty"].toString(), \
            "edit", "delete", "");

        connect(widget, &levels::action0, this, &levelManager::launchLevelEditor);
        connect(widget, &levels::action1, this, &levelManager::removeLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }
}


void levelManager::addLevel() {
    QString lname = "level$date";
    lname.replace("$date", QDateTime::currentDateTime().toString("yyMMddHHmm"));
    levelEditorDialog.initialise(&(this->iData), lname);
    levelEditorDialog.showMaximized();
}


void levelManager::genRandomLevel() {
    levelEditorDialog.genRandomLevel(&(this->iData), "rand"+QDateTime::currentDateTime().toString("yyMMddHHmmss"));
    levelEditorDialog.showMaximized();
}


void levelManager::saveData(QString fname) {

    QJsonDocument document;
    QJsonObject temp;

    temp.insert("info", this->cfg);
    temp.insert("data", this->iData);
    document.setObject(temp);

    QFile::remove(fname);

    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file(fname);
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadWrite);
    QTextStream iStream(&file);
    iStream << bytes;
    file.flush();
    file.close();
    this->initialise();

}


void levelManager::launchLevelEditor(QString code) {
    levelEditorDialog.initialise(&(iData), code);
    levelEditorDialog.showMaximized();
}


void levelManager::removeLevel(QString code) {
    iData.remove(code);
    this->saveData();

}


void levelManager::importLevels() {

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    if (filename == "") return;

    QFile lFile(filename);
    if (lFile.isOpen()) lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto importData = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    QVariantMap map = this->iData.toVariantMap();
    map.insert(importData["data"].toObject().toVariantMap());
    this->iData = QJsonObject::fromVariantMap(map);

    this->saveData();
    this->updateTable();

}


void levelManager::exportLevels() {

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("export.json");
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    if (filename == "") return;

    this->saveData(filename);

}


void levelManager::downloadLevel() {
    getLevelDialog.initialise();
    getLevelDialog.show();
}
