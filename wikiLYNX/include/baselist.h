#ifndef BASELIST_H
#define BASELIST_H

#include "levels.h"
#include <QDialog>

namespace Ui {
class baseList;
}

class baseList : public QDialog
{
    Q_OBJECT

public:
    int *dontKillMe = nullptr;
    int initList(QString title, QString tip, QList<QString> *header, QList<QList<QString>>* data = nullptr, QList<QList<QString>>* actions = nullptr);
    explicit baseList(QWidget *parent = nullptr);
    ~baseList();

private:
    Ui::baseList *ui;

protected:
    void closeEvent(QCloseEvent *event) override {
        if (this->dontKillMe != nullptr) *(this->dontKillMe) = 1;
        QDialog::closeEvent(event);
        if (this->dontKillMe != nullptr) *(this->dontKillMe) = 0;
    }
};

#endif // BASELIST_H
