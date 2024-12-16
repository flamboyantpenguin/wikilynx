#include "include/editlevel.h"
#include "ui/ui_editlevel.h"


#include <QMessageBox>


editLevel::editLevel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editLevel)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &editLevel::addLevel);
    connect(ui->randomButton, &QPushButton::clicked, this, &editLevel::genRandomLevel);
    connect(ui->loadButton, &QPushButton::clicked, this, &editLevel::importLevels);
    connect(ui->exportButton, &QPushButton::clicked, this, &editLevel::exportLevels);
    connect(ui->downloadButton, &QPushButton::clicked, this, &editLevel::downloadLevel);
    connect(&getLevelDialog, &getLevel::closed, this, &editLevel::initialise);
    connect(&levelEditorDialog, SIGNAL(closed()), this, SLOT(saveData()));
    connect(ui->closeButton, &QPushButton::clicked, this, &editLevel::close);

}


editLevel::~editLevel()
{
    delete ui;
}


void editLevel::initialise() {

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


void editLevel::updateTable() {

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

        connect(widget, &levels::action0, this, &editLevel::launchLevelEditor);
        connect(widget, &levels::action1, this, &editLevel::removeLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }
}


void editLevel::addLevel() {
    QString lname = "level$date";
    lname.replace("$date", QDateTime::currentDateTime().toString("yyMMddHHmm"));
    levelEditorDialog.initialise(&(this->iData), lname);
    levelEditorDialog.showMaximized();
}


void editLevel::genRandomLevel() {
    levelEditorDialog.genRandomLevel(&(this->iData));
    levelEditorDialog.showMaximized();
}


void editLevel::saveData(QString fname) {

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


void editLevel::launchLevelEditor(QString code) {
    levelEditorDialog.initialise(&(iData), code);
    levelEditorDialog.showMaximized();
}


void editLevel::removeLevel(QString code) {
    iData.remove(code);
    this->saveData();

}


void editLevel::importLevels() {

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


void editLevel::exportLevels() {

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


void editLevel::downloadLevel() {
    getLevelDialog.initialise();
    getLevelDialog.show();
}
