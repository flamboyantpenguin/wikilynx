#ifndef LEVELS_H
#define LEVELS_H

#include <QWidget>

namespace Ui {
class levels;
}

class levels : public QWidget
{
    Q_OBJECT

public:
    explicit levels(QWidget *parent = nullptr);
    ~levels();

    int initialise();
    void setItem(QString, QString, QString, QString, QString, QString, QString);

private:
    Ui::levels *ui;

private slots:
    void initAction0();
    void initAction1();
    void initAction2();

signals:
    void action0(QString);
    void action1(QString);
    void action2(QString);

};

#endif // LEVELS_H
