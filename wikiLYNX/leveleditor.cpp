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

}

levelEditor::~levelEditor()
{
    delete ui;
}


void levelEditor::initialise(QJsonObject *lData, QString cde) {

    this->code = cde;
    this->gameData = lData;
    this->levelInfo = (*this->gameData)[code].toObject();
    this->chkData = this->levelInfo["data"].toObject();

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

    QString s("Url ($count)");
    s.replace("$count", QString::number(this->chkData.keys().count()));
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
    auto l = chk.keys();

    for (int i = 0; i < l.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        widget->setItem(chk[l[i]].toObject()["url"].toString(), \
                        "", \
                        "", \
                        "", \
                        "delete", "", "");

        //connect(widget, &levels::action0, this, &levelEditor::removeChks);

        item->setSizeHint(widget->sizeHint());
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

}


void levelEditor::updateBrowser() {
    ui->url->setText(ui->webEngineView->url().toString());
}

/*
void levelEditor::saveData() {



    QJsonObject nData;

    for (int i = 0; i < ui->table->rowCount(); ++i) {
        QJsonObject t;
        if (ui->table->item(i, 1) && !ui->table->item(i, 1)->text().isEmpty()) {
            t.insert("name", ui->table->item(i, 1)->text());

        }
        if (ui->table->item(i, 2) && !ui->table->item(i, 2)->text().isEmpty()) {
            t.insert("url", ui->table->item(i, 2)->text());
            nData.insert(QString::number(i), t);

        }
    }


    this->chkData->insert(this->code, nData);

}

*/


void levelEditor::addChk() {

    QString c = QString::number(this->levelInfo["data"].toObject().keys().count());


    QString url = ui->url->text();

    QJsonObject chk, temp;
    chk["url"] = url;
    chk["name"] = url.mid(30);
    ///this->chkData[c] = chk;
    this->updateHeader();
    this->updateChkList();


}


void levelEditor::removeChk(QString chk) {

}
