#include "include/news.h"
#include "ui/ui_news.h"


news::news(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::news)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->treeWidget->setHeaderLabels(QStringList { tr("Title"), tr("Link"), tr("Date"), tr("Description") });
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}


news::~news()
{
    delete ui;
}


bool news::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void news::initialise() {
    connect(&manager, &QNetworkAccessManager::finished, this, &news::finished);
    connect(ui->treeWidget, &QTreeWidget::itemActivated, this, [](QTreeWidgetItem *item) { QDesktopServices::openUrl(QUrl(item->text(1))); });
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
    //if (currentReply) {
     //   currentReply->disconnect(this);
     //   currentReply->deleteLater();
    //}
    ui->progressBar->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    currentReply = manager.get(QNetworkRequest(this->url));
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
    ui->progressBar->hide();
    QApplication::restoreOverrideCursor();

}
