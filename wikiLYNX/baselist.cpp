#include "include/baselist.h"
#include "ui/ui_baselist.h"

BaseList::BaseList(QWidget *parent) : QDialog(parent), ui(new Ui::BaseList) {
    ui->setupUi(this);
    ui->dock->hide();
    ui->action1->hide();
    ui->action2->hide();
    connect(ui->closeButton, &QPushButton::clicked, this, &BaseList::close);
    connect(ui->increaseFontSize, &QPushButton::clicked, this, &BaseList::increaseDockFont);
    connect(ui->decreaseFontSize, &QPushButton::clicked, this, &BaseList::decreaseDockFont);
}


BaseList::~BaseList() {
    delete ui;
}


int BaseList::initList(QString title, QString tip, QStringList *header, QStringList *headerButtons, QList<QStringList> *data, QList<QStringList> *actions) {

    this->data = *data;

    ui->list->clear();
    ui->tip->setText(tip);
    ui->title->hide();
    ui->title->setText(title);
    this->setWindowTitle(title);

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new Levels(this);

    widget->setItem(*header, *headerButtons);

    auto tmp = ui->header->sizeHint();
    tmp.setHeight(widget->sizeHint().height());
    item->setSizeHint(tmp);
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);

    for (int i = 0; i < data->count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new Levels(this);

        widget->setItem(data->value(i), actions->value(i));

        if (!(desc0.isEmpty())) connect(widget, &Levels::action0, this, &BaseList::launchTop);

        auto tmp = ui->header->sizeHint();
        tmp.setHeight(widget->sizeHint().height());
        item->setSizeHint(tmp);
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);
    }

    return 0;

}


void BaseList::launchTop(QString instance) {
    int i = 0;
    for (i = 0; i < this->data.count(); i++) {
        if (data.value(i).value(0) == instance) break;
    }
    ui->dock->show();
    ui->desc->setText(this->desc0.value(i));
}


void BaseList::setDesc(QStringList desc) {
    this->desc0 = desc;
}


void BaseList::increaseDockFont() {
    this->dockFontSize++;
    ui->desc->setFontPointSize(this->dockFontSize);
    ui->desc->setText(ui->desc->toPlainText());
}


void BaseList::decreaseDockFont() {
    this->dockFontSize--;
    ui->desc->setFontPointSize(this->dockFontSize);
    ui->desc->setText(ui->desc->toPlainText());
}
