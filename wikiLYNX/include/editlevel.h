#ifndef EDITLEVEL_H
#define EDITLEVEL_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileDialog>
#include <QProcess>

#include "levels.h"
#include "leveleditor.h"
#include "getlevel.h"


namespace Ui {
class editLevel;
}

class editLevel : public QDialog
{
    Q_OBJECT

public:
    QString dirName = ".wikilynx";
    explicit editLevel(QWidget *parent = nullptr);
    ~editLevel();
    QJsonObject cfg, iData, uData;
    void initialise();


protected:
    void closeEvent(QCloseEvent *event) override {
        QDialog::closeEvent(event);
    }

private:
    Ui::editLevel *ui;
    levelEditor levelEditorDialog;
    getLevel getLevelDialog;


private slots:
    void addLevel();
    void updateTable();
    void importLevels();
    void exportLevels();
    void downloadLevel();
    void genRandomLevel();
    void removeLevel(QString code);
    void launchLevelEditor(QString code);
    void saveData(QString fname = "./.wikilynx/gData.json");


};

#endif // EDITLEVEL_H
