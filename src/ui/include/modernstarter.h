#ifndef MODERNSTARTER_H
#define MODERNSTARTER_H

#include <QMainWindow>

#include <include/scoresheet.h>

namespace Ui {
class ModernStarter;
}

class ModernStarter : public QMainWindow
{
    Q_OBJECT

public:
    int initialise(int*, int*);
    explicit ModernStarter(QWidget *parent = nullptr);
    ~ModernStarter();

private:
    int *dontKillParse0;
    int *totemofUndying;
    ScoreSheet *gameData;
    Ui::ModernStarter *ui;
};

#endif // MODERNSTARTER_H
