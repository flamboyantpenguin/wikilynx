#include "include/renovatio.h"


Renovatio::Renovatio(QString version) {
    qDebug() << version;
    this->version = version.toStdString();
    this->lVersion = version.toStdString();
    connect(this, &Renovatio::finished, this, &Renovatio::deleteLater);
}


Renovatio::~Renovatio() {
    //Cleanup
}


void Renovatio::process() { // Process. Start processing data.

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(wikiURL));

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit status(0);
        return;
    }
    reply->deleteLater();

    reply = manager.get(QNetworkRequest(versionURL));

    QEventLoop loop2;
    QObject::connect(reply, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
    loop2.exec();

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        qDebug() << "Error fetching latest version information:" << reply->errorString();
        emit status(3);
        return;
    }
    else {
        QByteArray data = reply->read(8);
        qDebug() << QString::fromLocal8Bit(data);
        lVersion = QString::fromLocal8Bit(data).toStdString();
    }
    reply->deleteLater();

    if (strcmp(lVersion.c_str(), version.c_str()) > 0) emit status(2);
    else if (strcmp(lVersion.c_str(), version.c_str()) < 0) emit status(4);
    else emit status(1);

    emit finished();

}
