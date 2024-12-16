#ifndef LEVELS_H
#define LEVELS_H

#include <QList>
#include <QLabel>
#include <QWidget>
#include <QPushButton>


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
    QString getItem(int n = 0);
    void setItem(QString, QString, QString, QString, QString, QString, QString, QString);

private:
    Ui::levels *ui;
    QList <QLabel*> labels;
    QList <QPushButton*> actions;

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
