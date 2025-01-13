#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <QStandardPaths>
#include "levels.h"
#include "leveleditor.h"
#include "getlevel.h"


namespace Ui {
class levelManager;
}

class levelManager : public QDialog
{
    Q_OBJECT

public:
    explicit levelManager(QWidget *parent = nullptr);
    ~levelManager();
    QJsonObject cfg, cData, iData, uData;
    void initialise(int launchType = 0);
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);


protected:
    void closeEvent(QCloseEvent *event) override {
        QDialog::closeEvent(event);
    }

private:
    Ui::levelManager *ui;
    levelEditor *levelEditorDialog;
    getLevel *getLevelDialog;


private slots:
    void listDoubleClickEmitter(QListWidgetItem*);
    void addLevel();
    void updateTable();
    void importLevels();
    void exportLevels(QString codeName = "");
    void downloadLevel();
    void genRandomLevel();
    void removeLevel(QString code);
    void saveData(QString fname = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/gData.json", QJsonObject* data = nullptr);
    void launchLevelEditor(QString code);

signals:
    void listDoubleClicked(QString lname);

};

#endif // LEVELMANAGER_H
