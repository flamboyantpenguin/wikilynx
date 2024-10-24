#include "include/editlevel.h"
#include "ui/ui_editlevel.h"

#include <QMessageBox>

editLevel::editLevel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editLevel)
{
    ui->setupUi(this);
    ui->editButton->setEnabled(false);
    ui->table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    connect(ui->addButton, &QPushButton::clicked, this, &editLevel::addLevel);
    connect(ui->removeButton, &QPushButton::clicked, this, &editLevel::removeLevel);
    connect(ui->editButton, &QPushButton::clicked, this, &editLevel::editChkPoint);
    connect(ui->table, &QTableWidget::clicked, this, &editLevel::setEditStatus);
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

    QFile lFile("./gData/gData.json");
    if (lFile.isOpen())
        lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    this->cfg = temp["info"].toObject();
    this->iData = temp["data"].toObject();
    this->updateTable(this->iData);


    QStringList keys = this->iData.keys();
    for (const QString& key : keys) this->uData.insert(key, this->iData[key].toObject()["data"].toObject());

}


void editLevel::updateTable(QJsonObject data) {


    auto l = data.keys();
    ui->table->setRowCount(l.count());

    for (int i = 0; i < l.count(); i++) {

        QTableWidgetItem* code = new QTableWidgetItem();
        QTableWidgetItem* time = new QTableWidgetItem();
        QTableWidgetItem* chk = new QTableWidgetItem();
        QTableWidgetItem* difficulty = new QTableWidgetItem();
        code->setText(l[i]);
        time->setText(QString::number(data[l[i]].toObject()["time"].toInt()));
        chk->setText(QString::number(data[l[i]].toObject()["checkpoints"].toInt()));
        difficulty->setText(data[l[i]].toObject()["difficulty"].toString());
        ui->table->setItem(i, 0, code);
        ui->table->setItem(i, 1, time);
        ui->table->setItem(i, 2, chk);
        ui->table->setItem(i, 3, difficulty);

    }
}


void editLevel::addLevel() {
    ui->table->setRowCount(ui->table->rowCount()+1);
}


void editLevel::removeLevel() {
    ui->table->removeRow(ui->table->currentIndex().row());
}


void editLevel::editChkPoint() {

    auto t = ui->table->currentIndex();
    auto cde = ui->table->item(t.row(), 0)->text();
    //QMessageBox::information(this, "wikiLYNX", QString::number(t.row()), QMessageBox::Ok);
    //this->uData.insert(cde, data[cde].toObject()["data"].toObject());
    //QMessageBox::information(this, "wikiLYNX", ui->table->item(t.row(), 0)->text(), QMessageBox::Ok);
    editChkDialog.initialise(&uData, cde);
    editChkDialog.show();

}


void editLevel::setEditStatus() {
    if (ui->table->item(ui->table->currentIndex().row(), 1) && !ui->table->item(ui->table->currentIndex().row(), 1)->text().isEmpty())
        ui->editButton->setEnabled(true);
}


void editLevel::saveData(QString fname, int mode) {

    QJsonObject nData;

    for (int i = 0; i < ui->table->rowCount(); ++i) {
        QJsonObject t;
        if (ui->table->item(i, 1) && !ui->table->item(i, 1)->text().isEmpty())
            t.insert("time", ui->table->item(i, 1)->text().toInt());
        if (ui->table->item(i, 2) && !ui->table->item(i, 2)->text().isEmpty())
            t.insert("checkpoints", ui->table->item(i, 2)->text().toInt());
        if (ui->table->item(i, 3) && !ui->table->item(i, 3)->text().isEmpty())
            t.insert("difficulty", ui->table->item(i, 3)->text());
        if (ui->table->item(i, 0) && !ui->table->item(i, 0)->text().isEmpty()) {
            t.insert("data", this->uData.value(ui->table->item(i, 0)->text()));
            nData.insert(ui->table->item(i, 0)->text(), t);
        }
    }

    QJsonDocument document;
    QJsonObject temp;

    if (!mode) temp.insert("info", this->cfg);
    temp.insert("data", nData);
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
}


void editLevel::importLevels() {

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) {
        filename = dialog.selectedFiles()[0];
    }

    QFile lFile(filename);
    if (lFile.isOpen()) lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    QVariantMap map = this->iData.toVariantMap();
    map.insert(temp["data"].toObject().toVariantMap());
    this->iData = QJsonObject::fromVariantMap(map);
    this->updateTable(this->iData);

    QStringList keys = this->iData.keys();
    for (const QString& key : keys) this->uData.insert(key, this->iData[key].toObject()["data"].toObject());

}


void editLevel::exportLevels() {


    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("export.json");
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) {
        filename = dialog.selectedFiles()[0];
    }
    //if (filename.isEmpty()) return
    this->saveData(filename, 1);
}


void editLevel::downloadLevel() {

    getLevelDialog.initialise();
    getLevelDialog.show();
}
