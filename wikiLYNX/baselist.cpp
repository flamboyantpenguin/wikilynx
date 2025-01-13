#include "include/baselist.h"
#include "ui/ui_baselist.h"

baseList::baseList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::baseList)
{
    ui->setupUi(this);
    ui->action1->hide();
    ui->action2->hide();
    connect(ui->closeButton, &QPushButton::clicked, this, &baseList::close);
}

baseList::~baseList()
{
    delete ui;
}


int baseList::initList(QString title, QString tip, QList<QString> *header, QList<QList<QString>> *data, QList<QList<QString>> *actions) {

    ui->list->clear();
    ui->tip->setText(tip);
    ui->title->hide();
    ui->title->setText(title);
    this->setWindowTitle(title);

    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new levels(this);

    widget->setItem(header->value(0), \
                    header->value(1), \
                    header->value(2), \
                    header->value(3), \
                    header->value(4), \
                    actions->value(0).count() > 0 ? "neutralOnline" : "", \
                    actions->value(0).count() > 1 ? "neutralOnline" : "", \
                    actions->value(0).count() > 2 ? "neutralOnline" : "");

    //connect(widget, &levels::action0, this, &levelEditor::removeChk);

    auto tmp = ui->header->sizeHint();
    tmp.setHeight(widget->sizeHint().height());
    item->setSizeHint(tmp);
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);

    for (int i = 0; i < data->count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new levels(this);

        widget->setItem(data->value(i).value(0), \
                        data->value(i).value(1), \
                        data->value(i).value(2), \
                        data->value(i).value(3), \
                        data->value(i).value(4), \
                        actions->value(i).value(0), actions->value(i).value(1), actions->value(i).value(2));

        //connect(widget, &levels::action0, this, &levelEditor::removeChk);

        auto tmp = ui->header->sizeHint();
        tmp.setHeight(widget->sizeHint().height());
        item->setSizeHint(tmp);
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);
    }

    return 0;

}
