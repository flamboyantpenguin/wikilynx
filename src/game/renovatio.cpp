#include "include/renovatio.h"

std::map<int, QString> Renovatio::statuscode = {
    { 0, "Offline|offline" }, // Can't connect to wikipedia.org
    { 1, "Online|online" }, // App is on the latest version
    { 2, "Update Available|upgrade" }, // Update available
    { 3, "Update Check Failed|neutralOnline" }, // Can't connect to wikiLYNX website
    { 4, "Meow|meow" }, // App version is higher than current stable version
};


Renovatio::Renovatio(QString version) {
    this->version = version.toStdString();
    this->lVersion = version.toStdString();
    connect(this, &Renovatio::finished, this, &Renovatio::deleteLater);
}


Renovatio::~Renovatio() {
    //Cleanup
}


void Renovatio::process() {

    // Connect to wikipedia.org. This serves as the internet check for the app
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(wikiURL));

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // Check for errors
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        emit status(0);
        return;
    }
    reply->deleteLater();

    // Get latest wikiLYNX version from website
    reply = manager.get(QNetworkRequest(versionURL));

    QEventLoop loop2;
    QObject::connect(reply, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
    loop2.exec();

    // Check for errors
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        qDebug() << "Error fetching latest version information:" << reply->errorString();
        emit status(3);
        return;
    }
    else {
        QByteArray data = reply->read(8);
        lVersion = QString::fromLocal8Bit(data).toStdString();
    }
    reply->deleteLater();

    // Check headerfile for more info on status codes
    if (strcmp(lVersion.c_str(), version.c_str()) > 0) emit status(2);
    else if (strcmp(lVersion.c_str(), version.c_str()) < 0) emit status(4);
    else emit status(1);

    emit finished();

}
