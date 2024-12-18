#ifndef VIEWHISTORY_H
#define VIEWHISTORY_H

#include <QDialog>
#include <QCloseEvent>
#include <QEvent>


namespace Ui {
class viewHistory;
}

class viewHistory : public QDialog
{
    Q_OBJECT


public:
    explicit viewHistory(QWidget *parent = nullptr);
    ~viewHistory();
    int *dontKillMe;
    void initialise(QString*);

protected:
    void closeEvent(QCloseEvent *event) override
    {
        if (this->dontKillMe != nullptr) *(this->dontKillMe) = 1;
        QDialog::closeEvent(event);
        if (this->dontKillMe != nullptr) *(this->dontKillMe) = 0;
    }

private:
    Ui::viewHistory *ui;

};

#endif // VIEWHISTORY_H
