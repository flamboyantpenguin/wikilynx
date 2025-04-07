#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QFile>
#include <QObject>
#include <QDialog>
#include <QThread>

namespace Ui {
class LoadingScreen;
}

class LoadingScreen : public QDialog {
    Q_OBJECT

public:
    explicit LoadingScreen(QWidget *parent = nullptr);
    ~LoadingScreen();

private:
    void setPrg(int i);
    Ui::LoadingScreen *ui;

};

class Worker : public QObject  {
    Q_OBJECT

public slots:
    void process();

signals:
    void finished();
    void progress(int);
};

#endif // LOADINGSCREEN_H
