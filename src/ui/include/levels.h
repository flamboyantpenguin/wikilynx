#ifndef LEVELS_H
#define LEVELS_H

#include <QList>
#include <QLabel>
#include <QWidget>
#include <QSpacerItem>
#include <QPushButton>


namespace Ui {
class Levels;
}

class Levels : public QWidget {
    Q_OBJECT

public:
    explicit Levels(QWidget *parent = nullptr);
    ~Levels();

    int initialise();
    QString getItem(int n = 0);
    void setItem(QStringList, QStringList);
    void setLabelSize(int, int);

private:
    Ui::Levels *ui;
    QList <QLabel*> labelUI;
    QList <QSpacerItem*> spacers;
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
