#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <QDialog>
#include <QString>
#include <QListWidget>
#include <QStringList>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QDesktopServices>

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
    void genRandomLevel(QJsonObject*, QString);
    void initialise(QJsonObject *, QString);
    ~levelEditor();

protected:
    void closeEvent(QCloseEvent *event) override {
        this->saveData();
        emit this->closed();
        QDialog::closeEvent(event);
    }



private:
    int genRandom = 0;
    Ui::levelEditor *ui;

private slots:
    void addChk();
    void genRandomReload();
    void previewArticle(QListWidgetItem *item);
    void getRandomArticle();
    void updateBrowser();
    void updateChkList();
    void saveData();
    void updateIndex();
    void updateCodeName();
    void updateExtras();
    void updateIsWiki();
    void loadURL();
    void resetBrowser();
    void launchHelp();
    void updateHeader();
    void removeChk(QString chk);

signals:
    void closed();
    void itemMoved();
    void genRandomFinished();


};

#endif // LEVELEDITOR_H
