#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QFile>
#include <QObject>
#include <QDialog>
#include <QThread>

namespace Ui {
class loadingScreen;
}

class loadingScreen : public QDialog
{
    Q_OBJECT

public:
    explicit loadingScreen(QWidget *parent = nullptr);
    ~loadingScreen();

    /*
     *
     * class Worker(QObject):
finished = pyqtSignal()
progress = pyqtSignal(int)
def progressbar(self):
for i in range(101):
self.progress.emit(i)
sleep(progressBarTime)
self.finished.emit()
     * */

private:
    //QThread thread;
    //Worker worker;
    void setPrg(int i);
    Ui::loadingScreen *ui;

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
