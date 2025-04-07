#include "include/levelmanager.h"
#include "forms/ui_levelmanager.h"


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
    //QStringList icons = {"neutralOnline", "neutralOnline", "neutralOnline"};
    QStringList icons;

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

    QJsonObject gameLevels;
    if (ui->showInbuiltLevelToggle->isChecked()) gameLevels = gameData->getLevels();
    else gameLevels = gameData->getLevels("custom");

    ui->list->clear();

    QStringList levelNames = gameLevels.keys();
    for (const QString &levelName : std::as_const(levelNames)) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new Levels(this);

        QStringList itemData {
            levelName,
            !(gameLevels[levelName].toObject()["time"].toDouble()) ? "nolimit" : QString::number(gameLevels[levelName].toObject()["time"].toDouble()),
            !(gameLevels[levelName].toObject()["clicks"].toInt()) ? "nolimit" : QString::number(gameLevels[levelName].toObject()["clicks"].toInt()),
            QString::number(gameLevels[levelName].toObject()["levels"].toString().split(" ").count()),
            gameLevels[levelName].toObject()["difficulty"].toString(),
        };


        QStringList icons;
        if (gameData->getLevelPresence(levelName) == "inbuilt") {
            //QStringList icons = {"export", "neutralOnline", "neutralOnline"};
            icons = {"export"};
            connect(widget, &Levels::action0, this, &LevelManager::exportLevels);

        }
        else {
            icons = {"edit", "export", "delete"};
            connect(widget, &Levels::action0, this, &LevelManager::launchLevelEditor);
            connect(widget, &Levels::action1, this, &LevelManager::exportLevels);
            connect(widget, &Levels::action2, this, &LevelManager::removeLevel);
        }

        //qDebug() << ui->list->width();
        //widget->setLabelSize(0, (int) ui->list->width());
        widget->setItem(itemData, icons);
        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);


        auto tmp = ui->header->sizeHint();
        tmp.setHeight(widget->sizeHint().height());
        item->setSizeHint(tmp);
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);


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
    if (dialog.exec()) filename = dialog.selectedFiles().value(0);

    if (filename == "") return;

    gameData->addLevels(gameData->readTxtFile(filename));

    this->updateTable();

}


void LevelManager::exportLevels(QString codeName) {

    QFileDialog dialog(this);
    QString filename;
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("export.json");
    dialog.setNameFilter(tr("JSON Files (*.json)"));
    if (dialog.exec()) filename = dialog.selectedFiles().value(0);

    if (filename == "") return;

    QJsonObject* exportData = new QJsonObject;
    if (!(codeName.isEmpty())) {
        exportData->insert(codeName, gameData->getLevel(codeName));
    }
    else if (ui->list->selectedItems().count()) {
        QList selection = ui->list->selectedItems();
        for (int i = 0; i < selection.count(); i++) {
            auto item = selection[i];
            auto itemWidget = (Levels *) ui->list->itemWidget(item);
            QString code = itemWidget->getItem(0);
            exportData->insert(code, gameData->getLevel(code));
        }
    }
    else {
        *exportData = gameData->getLevels("custom");
    }

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
