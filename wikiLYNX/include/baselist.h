#ifndef BASELIST_H
#define BASELIST_H

#include "levels.h"
#include <QDialog>
#include <QTextBrowser>


namespace Ui {
class BaseList;
}

class BaseList : public QDialog {
    Q_OBJECT

public:
    int *dontKillMe = nullptr;
    QStringList desc0;
    void setDesc(QStringList);
    int initList(QString title, QString tip, QStringList *header = nullptr, QStringList *headerButtons = nullptr, QList<QStringList>* data = nullptr, QList<QStringList>* actions = nullptr);
    explicit BaseList(QWidget *parent = nullptr);
    ~BaseList();

private:
    int dockFontSize = 8;
    QList<QStringList> data;
    Ui::BaseList *ui;

protected:
    void closeEvent(QCloseEvent *event) override {
        if (this->dontKillMe != nullptr) *(this->dontKillMe) = 1;
        QDialog::closeEvent(event);
        if (this->dontKillMe != nullptr) *(this->dontKillMe) = 0;
    }

private slots:
    void increaseDockFont();
    void decreaseDockFont();
    void launchTop(QString);
};

#endif // BASELIST_H
