#include "include/leveleditor.h"
#include "ui/ui_leveleditor.h"

levelEditor::levelEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::levelEditor)
{
    ui->setupUi(this);
    connect(ui->addButton, &QPushButton::clicked, this, &levelEditor::addChk);
    connect(ui->closeButton, &QPushButton::clicked, this, &levelEditor::close);
    connect(ui->webEngineView, &QWebEngineView::urlChanged, this, &levelEditor::updateBrowser);
    connect(ui->refreshButton, &QPushButton::clicked, ui->webEngineView, &QWebEngineView::reload);
    connect(ui->webEngineView, &QWebEngineView::urlChanged, this, &levelEditor::updateBrowser);
    connect(ui->refreshButton, &QPushButton::clicked, ui->webEngineView, &QWebEngineView::reload);

}


levelEditor::~levelEditor()
{
    delete ui;
}


void levelEditor::initialise(QJsonObject *lData, QString cde) {
void levelEditor::initialise(QJsonObject *lData, QString cde) {

    this->code = cde;
    this->gameData = lData;
    this->levelInfo = (*this->gameData)[code].toObject();
    this->chkData = this->levelInfo["levels"].toString().split(" ");

    this->updateHeader();
    this->updateChkList();
    this->updateBrowser();

    ui->code->setText(code);
    ui->difficulty->setText(this->levelInfo["difficulty"].toString());
    ui->timeTaken->setText(QString::number(this->levelInfo["time"].toInt()));
    ui->clicks->setText(QString::number(this->levelInfo["clicks"].toInt()));

}


void levelEditor::updateHeader() {

    ui->header->clear();
    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new levels(this);

    QString s("URL ($count)");
    s.replace("$count", QString::number(this->chkData.count()));
    widget->setItem(s, \
                    "", \
                    "", \
                    "", \
                    "neutralOnline", "", "");

    //connect(widget, &levels::action0, this, &levelEditor::removeChks);

    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);


}


void levelEditor::updateChkList() {

    ui->list->clear();
    auto chk = this->chkData;

    for (int i = 0; i < this->chkData.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        widget->setItem(QString::number(i), \
                        this->chkData[i], \
                        "", \
                        "", \
                        "delete", "", "");

        connect(widget, &levels::action0, this, &levelEditor::removeChk);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

}


void levelEditor::updateBrowser() {
    ui->url->setText(ui->webEngineView->url().toString());
}



void levelEditor::saveData() {

    this->levelInfo["levels"] = this->chkData.join(" ");
    (*(this->gameData))[this->code] = this->levelInfo;

}

*/


void levelEditor::addChk() {

    int c = this->chkData.count();


    QString url = ui->url->text();

    this->chkData.append(url);
    this->updateHeader();
    this->updateChkList();
    this->saveData();

}


void levelEditor::removeChk(QString chk) {

    this->chkData.removeAt(chk.toInt());
    this->updateChkList();
    this->updateHeader();
    this->saveData();

}
