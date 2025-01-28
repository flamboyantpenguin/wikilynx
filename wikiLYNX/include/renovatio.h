#ifndef RENOVATIO_H
#define RENOVATIO_H

#include <QUrl>
#include <QString>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class Renovatio : public QObject  {
    Q_OBJECT

public:
    explicit Renovatio(QString);
    ~Renovatio();

private:
    std::string version;
    std::string lVersion;
    QUrl wikiURL = QUrl::fromUserInput("https://wikipedia.org");
    QUrl versionURL = QUrl::fromUserInput("https://projects.dawn.org.in/wikilynx/.cfg/version.txt");

public slots:
    void process();

signals:
    void finished();
    void status(int);
};

#endif // RENOVATIO_H
