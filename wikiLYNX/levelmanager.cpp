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


void levelManager::initialise(ScoreSheet **gameData, int launchType) {

    this->gameData = *gameData;

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
        ui->tip->setText("Double click a level to select it");
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
    QJsonObject cLevels = gameData->getLevels("custom");
    auto l = cLevels.keys();

    for (int i = 0; i < l.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        widget->setItem(l[i], \
            QString::number(cLevels[l[i]].toObject()["time"].toDouble()), \
            QString::number(cLevels[l[i]].toObject()["clicks"].toInt()), \
            QString::number(cLevels[l[i]].toObject()["levels"].toString().split(" ").count()), \
            cLevels[l[i]].toObject()["difficulty"].toString(), \
            "edit", "export", "delete");

        connect(widget, &levels::action0, this, &levelManager::launchLevelEditor);
        connect(widget, &levels::action1, this, &levelManager::exportLevels);
        connect(widget, &levels::action2, this, &levelManager::removeLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

    if (ui->showInbuiltLevelToggle->isChecked()) {

        QJsonObject iLevels = gameData->getLevels("inbuilt");
        auto l1 = iLevels.keys();

        for (int i = 0; i < l1.count(); i++) {

            if (!(gameData->getSettings().value("debug").toBool()) && l1[i] == "debug") continue;

            QListWidgetItem *item = new QListWidgetItem();
            auto widget = new levels(this);

            widget->setItem(l1[i], \
                            QString::number(iLevels[l1[i]].toObject()["time"].toDouble()), \
                            QString::number(iLevels[l1[i]].toObject()["clicks"].toInt()), \
                            QString::number(iLevels[l1[i]].toObject()["levels"].toString().split(" ").count()), \
                            iLevels[l1[i]].toObject()["difficulty"].toString(), \
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
    levelEditorDialog->initialise(gameData, lname);
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(updateTable()));
    levelEditorDialog->showMaximized();
}


void levelManager::genRandomLevel() {
    levelEditorDialog = new levelEditor;
    levelEditorDialog->genRandomLevel(gameData, "rand"+QDateTime::currentDateTime().toString("yyMMddHHmmss"));
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(updateTable()));
    levelEditorDialog->showMaximized();
}


void levelManager::launchLevelEditor(QString code) {
    levelEditorDialog = new levelEditor;
    levelEditorDialog->initialise(gameData, code);
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(updateTable()));
    levelEditorDialog->showMaximized();
}


void levelManager::removeLevel(QString code) {
    gameData->removeLevel(code);
    this->updateTable();
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
    gameData->addLevels(QJsonDocument::fromJson(lFile.readAll()).object());
    lFile.close();

    this->updateTable();

}


void levelManager::exportLevels(QString codeName) {

    QJsonObject* exportData = new QJsonObject;
    if (!(codeName.isEmpty())) {
        exportData->insert(codeName, gameData->getLevel(codeName));
    }
    else if (ui->list->selectedItems().count()) {
        QList selection = ui->list->selectedItems();
        for (int i = 0; i < selection.count(); i++) {
            auto item = selection[i];
            auto itemWidget = (levels *) ui->list->itemWidget(item);
            QString code = itemWidget->getItem(0);
            exportData->insert(code, gameData->getLevel(code));
        }
    }
    else {
        *exportData = gameData->getLevels("custom");
    }

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("export.json");
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    if (filename == "") return;

    gameData->saveData(&filename, nullptr, exportData);

}


void levelManager::downloadLevel() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    getLevelDialog = new getLevel;
    getLevelDialog->initialise(gameData);
    connect(getLevelDialog, SIGNAL(levelsUpdated()), this, SLOT(updateTable()));
    getLevelDialog->show();
    QApplication::restoreOverrideCursor();
}
