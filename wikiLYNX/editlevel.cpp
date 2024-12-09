#include "include/editlevel.h"
#include "ui/ui_editlevel.h"


#include <QMessageBox>


editLevel::editLevel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editLevel)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &editLevel::addLevel);
    connect(ui->loadButton, &QPushButton::clicked, this, &editLevel::importLevels);
    connect(ui->exportButton, &QPushButton::clicked, this, &editLevel::exportLevels);
    connect(ui->downloadButton, &QPushButton::clicked, this, &editLevel::downloadLevel);
    connect(&getLevelDialog, &getLevel::exitWindow, this, &editLevel::initialise);
    connect(ui->closeButton, &QPushButton::clicked, this, &editLevel::close);

}


editLevel::~editLevel()
{
    delete ui;
}


void editLevel::initialise() {

    QFile lFile("./"+dirName+"/gData.json");
    if (lFile.isOpen())
        lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    this->cfg = temp["info"].toObject();
    this->iData = temp["data"].toObject();

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new levels(this);

    widget->setItem("Code", \
                    "Time Taken (Seconds)", \
                    "Checkpoints", \
                    "Difficulty", \
                    "neutralOnline", "neutralOnline", "");

    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);
    this->updateTable();

}


void editLevel::updateTable() {

    ui->list->clear();
    auto l = this->iData.keys();

    for (int i = 0; i < l.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        widget->setItem(l[i], \
            QString::number(iData[l[i]].toObject()["time"].toInt()), \
            QString::number(iData[l[i]].toObject()["checkpoints"].toInt()), \
            iData[l[i]].toObject()["difficulty"].toString(), \
            "edit", "delete", "");

        connect(widget, &levels::action0, this, &editLevel::launchLevelEditor);
        connect(widget, &levels::action1, this, &editLevel::removeLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }
}


void editLevel::addLevel() {
    /*
    ui->table->setRowCount(ui->table->rowCount()+1);
    */
}


void editLevel::launchLevelEditor(QString code) {
    /*
    auto t = ui->table->currentIndex();
    auto cde = ui->table->item(t.row(), 0)->text();
    editChkDialog.initialise(&uData, cde);
    editChkDialog.show();
    */
}


void editLevel::removeLevel(QString code) {

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
    this->initialise();

}


void editLevel::importLevels() {

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    QFile lFile(filename);
    if (lFile.isOpen()) lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto importData = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    QVariantMap map = this->iData.toVariantMap();
    map.insert(importData["data"].toObject().toVariantMap());
    this->iData = QJsonObject::fromVariantMap(map);

    QJsonObject nSaveData;
    QJsonDocument document;
    nSaveData.insert("info", this->cfg);
    nSaveData.insert("data", this->iData);
    document.setObject(nSaveData);
    QFile::remove("./"+dirName+"/gData.json");
    QFile file("./"+dirName+"/gData.json");
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadWrite);
    QTextStream iStream(&file);
    iStream << document.toJson( QJsonDocument::Indented );;
    file.flush();
    file.close();
    this->updateTable();

}


void editLevel::exportLevels() {

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("export.json");
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    QJsonDocument document;
    QJsonObject temp;

    temp.insert("info", this->cfg);
    temp.insert("data", iData);
    document.setObject(temp);
    QFile::remove(filename);
    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file(filename);
    if (file.isOpen()) file.close();
    file.open(QIODevice::ReadWrite);
    QTextStream iStream(&file);
    iStream << bytes;
    file.flush();
    file.close();

}


void editLevel::downloadLevel() {
    getLevelDialog.initialise();
    getLevelDialog.show();
}
