#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QJsonObject>

#include "levels.h"

#include "levels.h"

namespace Ui {
class levelEditor;
}

class levelEditor : public QDialog
{
    Q_OBJECT

public:
    explicit levelEditor(QWidget *parent = nullptr);
    QString code;
    QJsonObject levelInfo;
    QStringList chkData;
    QJsonObject *gameData;
    void initialise(QJsonObject *, QString);
    ~levelEditor();

protected:
    void closeEvent(QCloseEvent *event) override {
        //this->saveData();
        //this->saveData();
        QDialog::closeEvent(event);
    }


private:
    Ui::levelEditor *ui;

private slots:
    void addChk();
    void updateBrowser();
    void updateChkList();
    void saveData();
    //void launchHelp();
    void updateHeader();
    void removeChk(QString chk);

};

#endif // LEVELEDITOR_H
