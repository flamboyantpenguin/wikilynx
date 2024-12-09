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
    explicit editLevel(QWidget *parent = nullptr);
    ~editLevel();
    QJsonObject cfg, iData, uData;
    void initialise();
    void updateTable();


protected:
    void closeEvent(QCloseEvent *event) override {
        QDialog::closeEvent(event);
    }

private:
    Ui::editLevel *ui;
    levelEditor levelEditorDialog;
    getLevel getLevelDialog;
    QString dirName = ".wikilynx";


private slots:
    void addLevel();
    void removeLevel(QString code);
    void importLevels();
    void exportLevels();
    void downloadLevel();
    void launchLevelEditor(QString code);

};

#endif // EDITLEVEL_H
