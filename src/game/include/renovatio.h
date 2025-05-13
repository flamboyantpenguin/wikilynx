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

    static std::map<int, QString> statuscode;

private:
    std::string version;
    std::string lVersion;
    QUrl wikiURL = QUrl::fromUserInput("https://wikipedia.org"); // The URL of Wikipedia, the free encyclopedia
    QUrl versionURL = QUrl::fromUserInput("https://projects.dawn.org.in/wikilynx/.cfg/version.txt"); // The latest available version is fetched from here

public slots:
    void process();

signals:
    void finished();
    void status(int); // The integer emitted here denotes the connectivity and update status
};

#endif // RENOVATIO_H
