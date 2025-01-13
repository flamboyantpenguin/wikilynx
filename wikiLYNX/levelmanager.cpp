#include "include/levelmanager.h"
#include "ui/ui_levelmanager.h"


#include <QMessageBox>


levelManager::levelManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::levelManager)
{
    ui->setupUi(this);
    ui->status->setText("");
    connect(ui->addButton, &QPushButton::clicked, this, &levelManager::addLevel);
    connect(ui->randomButton, &QPushButton::clicked, this, &levelManager::genRandomLevel);
    connect(ui->loadButton, &QPushButton::clicked, this, &levelManager::importLevels);
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(exportLevels()));
    connect(ui->downloadButton, &QPushButton::clicked, this, &levelManager::downloadLevel);
    connect(ui->showInbuiltLevelToggle, &QCheckBox::checkStateChanged, this, &levelManager::updateTable);
    connect(ui->closeButton, &QPushButton::clicked, this, &levelManager::close);

}


levelManager::~levelManager()
{
    delete ui;
}


void levelManager::initialise(int launchType) {

    QFile lFile(dirName+"/gData.json");
    if (lFile.isOpen()) lFile.close();
    lFile.open(QIODevice::ReadOnly);
    auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
    lFile.close();

    cfg = temp["info"].toObject();
    cData = temp["data"].toObject();

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new levels(this);

    widget->setItem("Code", \
                    "Time (s)", \
                    "Clicks", \
                    "Checkpoints", \
                    "Difficulty", \
                    "neutralOnline", "neutralOnline", "neutralOnline");

    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);

    if (launchType) {
        ui->title->setText("Select Level");
        ui->tip->setText("Double a click a level to select it");
        ui->showInbuiltLevelToggle->setChecked(true);
        updateTable();
        connect(ui->list, &QListWidget::itemDoubleClicked, this, &levelManager::listDoubleClickEmitter);
        return;
    }
    updateTable();

}


void levelManager::listDoubleClickEmitter(QListWidgetItem* item) {
    auto itemWidget = (levels *) ui->list->itemWidget(item);
    emit listDoubleClicked(itemWidget->getItem());
}


void levelManager::updateTable() {

    ui->list->clear();
    auto l = this->cData.keys();

    for (int i = 0; i < l.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        widget->setItem(l[i], \
            QString::number(cData[l[i]].toObject()["time"].toDouble()), \
            QString::number(cData[l[i]].toObject()["clicks"].toInt()), \
            QString::number(cData[l[i]].toObject()["levels"].toString().split(" ").count()), \
            cData[l[i]].toObject()["difficulty"].toString(), \
            "edit", "export", "delete");

        connect(widget, &levels::action0, this, &levelManager::launchLevelEditor);
        connect(widget, &levels::action1, this, &levelManager::exportLevels);
        connect(widget, &levels::action2, this, &levelManager::removeLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

    if (ui->showInbuiltLevelToggle->isChecked()) {

        QFile lFile(":/cfg/gameData.json");
        if (lFile.isOpen()) lFile.close();
        lFile.open(QIODevice::ReadOnly);
        auto temp = QJsonDocument::fromJson(lFile.readAll()).object();
        lFile.close();
        iData = temp["data"].toObject();

        auto l1 = iData.keys();

        for (int i = 0; i < l1.count(); i++) {

            if (!(cfg.value("debug").toBool()) && l1[i] == "debug") continue;

            QListWidgetItem *item = new QListWidgetItem();
            auto widget = new levels(this);

            widget->setItem(l1[i], \
                            QString::number(iData[l1[i]].toObject()["time"].toDouble()), \
                            QString::number(iData[l1[i]].toObject()["clicks"].toInt()), \
                            QString::number(iData[l1[i]].toObject()["levels"].toString().split(" ").count()), \
                            iData[l1[i]].toObject()["difficulty"].toString(), \
                            "neutralOnline", "export", "neutralOnline");

            //connect(widget, &levels::action0, this, &levelManager::launchLevelEditor);
            connect(widget, &levels::action1, this, &levelManager::exportLevels);
            //connect(widget, &levels::action2, this, &levelManager::removeLevel);

            auto tmp = ui->header->sizeHint();
            tmp.setHeight(widget->sizeHint().height());
            item->setSizeHint(tmp);
            ui->list->addItem(item);
            ui->list->setItemWidget(item, widget);

        }
    }
}


void levelManager::addLevel() {
    QString lname = "level$date";
    lname.replace("$date", QDateTime::currentDateTime().toString("yyMMddHHmm"));
    levelEditorDialog = new levelEditor;
    levelEditorDialog->initialise(&(this->cData), lname);
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(saveData()));
    levelEditorDialog->showMaximized();
}


void levelManager::genRandomLevel() {
    levelEditorDialog = new levelEditor;
    levelEditorDialog->genRandomLevel(&(this->cData), "rand"+QDateTime::currentDateTime().toString("yyMMddHHmmss"));
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(saveData()));
    levelEditorDialog->showMaximized();
}


void levelManager::saveData(QString fname, QJsonObject* data) {

    if (data == nullptr) {
        data = &cData;
    }

    QJsonDocument document;
    QJsonObject temp;

    temp.insert("info", cfg);
    temp.insert("data", *data);
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
    levelEditorDialog = new levelEditor;
    levelEditorDialog->initialise(&(cData), code);
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(saveData()));
    levelEditorDialog->showMaximized();
}


void levelManager::removeLevel(QString code) {
    cData.remove(code);
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

    QVariantMap map = cData.toVariantMap();
    map.insert(importData["data"].toObject().toVariantMap());
    this->cData = QJsonObject::fromVariantMap(map);

    this->saveData();
    this->updateTable();

}


void levelManager::exportLevels(QString codeName) {

    QJsonObject* exportData = new QJsonObject;
    if (!(codeName.isEmpty())) {
        if (iData.contains(codeName)) exportData->insert(codeName, iData[codeName].toObject());
        else if (cData.contains(codeName)) exportData->insert(codeName, cData[codeName].toObject());
    }
    else {
        QList selection = ui->list->selectedItems();
        for (int i = 0; i < selection.count(); i++) {
            auto item = selection[i];
            auto itemWidget = (levels *) ui->list->itemWidget(item);
            QString code = itemWidget->getItem(0);
            if (iData.contains(code)) exportData->insert(code, iData[code].toObject());
            else if (cData.contains(code)) exportData->insert(code, cData[code].toObject());
        }
    }

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("export.json");
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    if (filename == "") return;

    this->saveData(filename, exportData);

}


void levelManager::downloadLevel() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    getLevelDialog = new getLevel;
    getLevelDialog->initialise(&cData);
    connect(getLevelDialog, SIGNAL(closed()), this, SLOT(saveData()));
    getLevelDialog->show();
    QApplication::restoreOverrideCursor();
}
