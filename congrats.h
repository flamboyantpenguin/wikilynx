#ifndef CONGRATS_H
#define CONGRATS_H

#include <QFile>
#include <QDialog>

#include "viewhistory.h"
#include "viewcheckpoint.h"

namespace Ui {
class congrats;
}

class congrats : public QDialog
{
    Q_OBJECT

public:
    explicit congrats(QWidget *parent = nullptr);
    ~congrats();

    //int chk;
    //int tChk;
    int *dontKillMe;
    std::string instanceName;
    void initialise(QString, QString, QString, std::string, int*, int);


private:
    viewHistory hView;
    viewcheckpoint cView;
    Ui::congrats *ui;

private slots:
    void viewhistory();
    void viewCheckPoints();
};

#endif // CONGRATS_H
