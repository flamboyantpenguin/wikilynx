#ifndef HELP_H
#define HELP_H

#include <QDialog>

namespace Ui {
class help;
}

class help : public QDialog
{
    Q_OBJECT

public:
    explicit help(QWidget *parent = nullptr);
    ~help();

    void initialise(QString url  = "https://github.com/flamboyantpenguin/wikilynx/wiki/Gameplay");


private:
    Ui::help *ui;
    bool isDarkTheme();

};

#endif // HELP_H
