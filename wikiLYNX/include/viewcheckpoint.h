#ifndef VIEWCHECKPOINT_H
#define VIEWCHECKPOINT_H

#include <QDialog>
#include <QCloseEvent>
#include <QHeaderView>

namespace Ui {
class viewcheckpoint;
}

class viewcheckpoint : public QDialog
{
    Q_OBJECT

public:
    explicit viewcheckpoint(QWidget *parent = nullptr);
    ~viewcheckpoint();
    int *dontKillMe;
    void initialise(QStringList*, int*);

protected:
    void closeEvent(QCloseEvent *event) override
    {
        *(this->dontKillMe) = 1;
        QDialog::closeEvent(event);
        *(this->dontKillMe) = 0;
    }

private:
    Ui::viewcheckpoint *ui;

};

#endif // VIEWCHECKPOINT_H
