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
class LevelManager;
}

class LevelManager : public QDialog {
    Q_OBJECT

public:
    explicit LevelManager(QWidget *parent = nullptr);
    ~LevelManager();
    void initialise(ScoreSheet **gameData, int launchType = 0);

protected:
    void closeEvent(QCloseEvent *event) override {
        QDialog::closeEvent(event);
    }

private:
    ScoreSheet *gameData;
    Ui::LevelManager *ui;
    LevelEditor *levelEditorDialog;
    GetLevel *GetLevelDialog;


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
