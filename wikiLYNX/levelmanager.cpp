#include "include/levelmanager.h"
#include "ui/ui_levelmanager.h"


#include <QMessageBox>


LevelManager::LevelManager(QWidget *parent) : QDialog(parent), ui(new Ui::LevelManager) {
    ui->setupUi(this);
    ui->status->setText("");
    connect(ui->addButton, &QPushButton::clicked, this, &LevelManager::addLevel);
    connect(ui->randomButton, &QPushButton::clicked, this, &LevelManager::genRandomLevel);
    connect(ui->loadButton, &QPushButton::clicked, this, &LevelManager::importLevels);
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(exportLevels()));
    connect(ui->downloadButton, &QPushButton::clicked, this, &LevelManager::downloadLevel);
    connect(ui->showInbuiltLevelToggle, &QCheckBox::checkStateChanged, this, &LevelManager::updateTable);
    connect(ui->closeButton, &QPushButton::clicked, this, &LevelManager::close);

}


LevelManager::~LevelManager()
{
    delete ui;
}


void LevelManager::initialise(ScoreSheet **gameData, int launchType) {

    this->gameData = *gameData;

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new Levels(this);

    QStringList itemData = {"Code", "Time (s)", "Clicks", "Checkpoints", "Difficulty"};
    QStringList icons = {"neutralOnline", "neutralOnline", "neutralOnline"};

    widget->setItem(itemData, icons);
    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);

    if (launchType) {
        ui->title->setText("Select Level");
        ui->tip->setText("Double click a level to select it");
        ui->showInbuiltLevelToggle->setChecked(true);
        updateTable();
        connect(ui->list, &QListWidget::itemDoubleClicked, this, &LevelManager::listDoubleClickEmitter);
        return;
    }
    updateTable();

}


void LevelManager::listDoubleClickEmitter(QListWidgetItem* item) {
    auto itemWidget = (Levels *) ui->list->itemWidget(item);
    emit listDoubleClicked(itemWidget->getItem());
}


void LevelManager::updateTable() {

    ui->list->clear();
    QJsonObject cLevels = gameData->GetLevels("custom");

    for (QString cKey : cLevels.keys()) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new Levels(this);

        QStringList itemData {
            cKey,
            QString::number(cLevels[cKey].toObject()["time"].toDouble()),
            QString::number(cLevels[cKey].toObject()["clicks"].toInt()),
            QString::number(cLevels[cKey].toObject()["levels"].toString().split(" ").count()),
            cLevels[cKey].toObject()["difficulty"].toString(),
        };
        QStringList icons = {"edit", "export", "delete"};

        widget->setItem(itemData, icons);

        connect(widget, &Levels::action0, this, &LevelManager::launchLevelEditor);
        connect(widget, &Levels::action1, this, &LevelManager::exportLevels);
        connect(widget, &Levels::action2, this, &LevelManager::removeLevel);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

    if (ui->showInbuiltLevelToggle->isChecked()) {

        QJsonObject iLevels = gameData->GetLevels("inbuilt");

        for (QString lKey : iLevels.keys()) {

            if (!(gameData->getSettings().value("debug").toBool()) && lKey == "debug") continue;

            QListWidgetItem *item = new QListWidgetItem();
            auto widget = new Levels(this);

            QStringList itemData {
                lKey,
                QString::number(iLevels[lKey].toObject()["time"].toDouble()),
                QString::number(iLevels[lKey].toObject()["clicks"].toInt()),
                QString::number(iLevels[lKey].toObject()["levels"].toString().split(" ").count()),
                iLevels[lKey].toObject()["difficulty"].toString()
            };
            QStringList icons = {"neutralOnline", "export", "neutralOnline"};

            widget->setItem(itemData, icons);

            //connect(widget, &levels::action0, this, &LevelManager::launchLevelEditor);
            connect(widget, &Levels::action1, this, &LevelManager::exportLevels);
            //connect(widget, &levels::action2, this, &LevelManager::removeLevel);

            auto tmp = ui->header->sizeHint();
            tmp.setHeight(widget->sizeHint().height());
            item->setSizeHint(tmp);
            ui->list->addItem(item);
            ui->list->setItemWidget(item, widget);

        }
    }
}


void LevelManager::addLevel() {
    QString lname = "level$date";
    lname.replace("$date", QDateTime::currentDateTime().toString("yyMMddHHmm"));
    levelEditorDialog = new LevelEditor;
    levelEditorDialog->initialise(gameData, lname);
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(updateTable()));
    levelEditorDialog->showMaximized();
}


void LevelManager::genRandomLevel() {
    levelEditorDialog = new LevelEditor;
    levelEditorDialog->genRandomLevel(gameData, "rand"+QDateTime::currentDateTime().toString("yyMMddHHmmss"));
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(updateTable()));
    levelEditorDialog->showMaximized();
}


void LevelManager::launchLevelEditor(QString code) {
    levelEditorDialog = new LevelEditor;
    levelEditorDialog->initialise(gameData, code);
    connect(levelEditorDialog, SIGNAL(closed()), this, SLOT(updateTable()));
    levelEditorDialog->showMaximized();
}


void LevelManager::removeLevel(QString code) {
    gameData->removeLevel(code);
    this->updateTable();
}


void LevelManager::importLevels() {

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    if (filename == "") return;

    gameData->addLevels(gameData->readTxtFile(filename));

    this->updateTable();

}


void LevelManager::exportLevels(QString codeName) {

    QJsonObject* exportData = new QJsonObject;
    if (!(codeName.isEmpty())) {
        exportData->insert(codeName, gameData->GetLevel(codeName));
    }
    else if (ui->list->selectedItems().count()) {
        QList selection = ui->list->selectedItems();
        for (int i = 0; i < selection.count(); i++) {
            auto item = selection[i];
            auto itemWidget = (Levels *) ui->list->itemWidget(item);
            QString code = itemWidget->getItem(0);
            exportData->insert(code, gameData->GetLevel(code));
        }
    }
    else {
        *exportData = gameData->GetLevels("custom");
    }

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("export.json");
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles()[0];

    if (filename == "") return;

    gameData->writeTxtFile(filename, *exportData);

}


void LevelManager::downloadLevel() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    GetLevelDialog = new GetLevel;
    GetLevelDialog->initialise(gameData);
    connect(GetLevelDialog, SIGNAL(levelsUpdated()), this, SLOT(updateTable()));
    GetLevelDialog->show();
    QApplication::restoreOverrideCursor();
}
