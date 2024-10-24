#ifndef EDITLEVEL_H
#define EDITLEVEL_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileDialog>

#include "editchk.h"
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
    //std::map<QString, QJsonObject> uData;
    QJsonObject cfg, iData, uData;
    void saveData(QString fname = "./gData/gData.json", int mode = 0);
    //void collectData();
    void initialise();
    void updateTable(QJsonObject);


protected:
    void closeEvent(QCloseEvent *event) override {
        this->saveData();
        QDialog::closeEvent(event);
    }

private:
    Ui::editLevel *ui;
    editChk editChkDialog;
    getLevel getLevelDialog;


private slots:
    void addLevel();
    void removeLevel();
    void importLevels();
    void exportLevels();
    void editChkPoint();
    void downloadLevel();
    void setEditStatus();

};

#endif // EDITLEVEL_H
