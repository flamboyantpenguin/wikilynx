#include "include/loadingscreen.h"
#include "ui/ui_loadingscreen.h"

LoadingScreen::LoadingScreen(QWidget *parent) : QDialog(parent), ui(new Ui::LoadingScreen) {
    ui->setupUi(this);


    QFile plt(":/base/pallete/bannerPrg.plt");
    plt.open(QIODevice::ReadOnly);
    auto styleSheet = plt.readAll();
    plt.close();

    QThread* thread = new QThread();
    Worker* worker = new Worker();
    worker->moveToThread(thread);

    connect( thread, &QThread::started, worker, &Worker::process);
    connect( worker, &Worker::finished, thread, &QThread::quit);
    connect( worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(worker, &Worker::progress, this, &LoadingScreen::setPrg);
    connect( thread, &QThread::finished, thread, &QThread::deleteLater);
    connect( thread, &QThread::finished, this, &LoadingScreen::close);
    thread->start();

    ui->progressBar->setStyleSheet(styleSheet);


}

LoadingScreen::~LoadingScreen() {
    delete ui;
}


void LoadingScreen::setPrg(int i) {
    ui->progressBar->setValue(i);
}


void Worker::process() { // Process. Start processing data.
    for (int i = 0; i < 100; i++) {
        emit progress(i);
        QThread::msleep(10);
    }
    emit finished();
}
