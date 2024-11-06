#include "include/news.h"
#include "ui/ui_news.h"

news::news(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::news)
{
    ui->setupUi(this);

    connect(&manager, &QNetworkAccessManager::finished, this, &news::finished);

    connect(ui->treeWidget, &QTreeWidget::itemActivated, this, [](QTreeWidgetItem *item) { QDesktopServices::openUrl(QUrl(item->text(1))); });
    ui->treeWidget->setHeaderLabels(QStringList { tr("Title"), tr("Link"), tr("Date"), tr("Description") });
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->initialise();
}


news::~news()
{
    delete ui;
}


void news::initialise() {
    this->fetch();
}


void news::fetch() {
    ui->treeWidget->clear();
    get(this->url);
}


void news::consumeData()
{
    int statusCode = currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300)
        parseXml();
}


void news::error(QNetworkReply::NetworkError)
{
    qWarning("error retrieving RSS feed");
    xml.clear();
    currentReply->disconnect(this);
    currentReply->deleteLater();
    currentReply = nullptr;
}

void news::finished(QNetworkReply *reply)
{
    Q_UNUSED(reply);
}


void news::get(const QUrl &url)
{
    if (currentReply) {
        currentReply->disconnect(this);
        currentReply->deleteLater();
    }
    currentReply = url.isValid() ? manager.get(QNetworkRequest(url)) : nullptr;
    if (currentReply) {
        connect(currentReply, &QNetworkReply::readyRead, this, &news::consumeData);
        connect(currentReply, &QNetworkReply::errorOccurred, this, &news::error);

    }
    xml.setDevice(currentReply);
}


void news::parseXml()
{
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == u"item") {
                linkString = xml.attributes().value("rss:about").toString();
                titleString.clear();
                pubDate.clear();
                description.clear();
            }
            currentTag = xml.name().toString();
        } else if (xml.isEndElement()) {
            if (xml.name() == u"item") {
                QTreeWidgetItem *item = new QTreeWidgetItem;
                item->setText(0, titleString);
                item->setText(1, linkString);
                item->setText(2, pubDate);
                item->setText(3, description);
                ui->treeWidget->addTopLevelItem(item);
            }
        } else if (xml.isCharacters() && !xml.isWhitespace()) {
            if (currentTag == "title")
                titleString += xml.text();
            else if (currentTag == "link")
                linkString += xml.text();
            else if (currentTag == "pubDate")
                pubDate += xml.text();
            else if (currentTag == "description")
                description += xml.text();
        }
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError)
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();

}
