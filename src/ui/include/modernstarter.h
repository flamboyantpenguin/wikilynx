#ifndef MODERNSTARTER_H
#define MODERNSTARTER_H

#include <QMainWindow>

namespace Ui {
class ModernStarter;
}

class ModernStarter : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModernStarter(QWidget *parent = nullptr);
    ~ModernStarter();

private:
    Ui::ModernStarter *ui;
};

#endif // MODERNSTARTER_H
