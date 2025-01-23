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
#include "scoresheet.h"


namespace Ui {
class levelManager;
}

class levelManager : public QDialog
{
    Q_OBJECT

public:
    explicit levelManager(QWidget *parent = nullptr);
    ~levelManager();
    void initialise(ScoreSheet **gameData, int launchType = 0);
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);


protected:
    void closeEvent(QCloseEvent *event) override {
        QDialog::closeEvent(event);
    }

private:
    ScoreSheet *gameData;
    Ui::levelManager *ui;
    levelEditor *levelEditorDialog;
    getLevel *getLevelDialog;


private slots:
    void addLevel();
    void updateTable();
    void importLevels();
    void downloadLevel();
    void genRandomLevel();
    void removeLevel(QString);
    void launchLevelEditor(QString);
    void exportLevels(QString codeName = "");
    void listDoubleClickEmitter(QListWidgetItem*);


signals:
    void listDoubleClicked(QString);

};

#endif // LEVELMANAGER_H
