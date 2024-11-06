#ifndef NEWS_H
#define NEWS_H

#include <QDialog>
#include <QDesktopServices>
#include <QXmlStreamReader>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>

namespace Ui {
class news;
}

class news : public QDialog
{
    Q_OBJECT

public:
    QUrl url = QUrl::fromUserInput("https://feeds.pcland.co.in/Projects/wikiLYNX/RSS.xml");
    explicit news(QWidget *parent = nullptr);
    ~news();

private:
    Ui::news *ui;
    void parseXml();
    void get(const QUrl &url);

    // Parser state:
    QXmlStreamReader xml;
    QString currentTag;
    QString linkString;
    QString titleString;
    QString pubDate;
    QString description;

    // Network state:
    QNetworkAccessManager manager;
    QNetworkReply *currentReply;

    // UI elements:

public slots:
    void fetch();
    void initialise();
    void finished(QNetworkReply *reply);
    void consumeData();
    void error(QNetworkReply::NetworkError);
};

#endif // NEWS_H
