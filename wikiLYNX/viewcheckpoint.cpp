#include "include/viewcheckpoint.h"
#include "ui/ui_viewcheckpoint.h"


viewcheckpoint::viewcheckpoint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewcheckpoint)
{
    ui->setupUi(this);
    this->setWindowModality(Qt::ApplicationModal);
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));

}

viewcheckpoint::~viewcheckpoint()
{
    delete ui;
}


void viewcheckpoint::initialise(QJsonObject *data, int *c, int *cStatus) {

    //ui->table->clear();
    ui->table->setRowCount(*c);
    ui->table->setColumnCount(3);
    ui->table->setColumnWidth(1, 100);

    ui->table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    for (int i = 0; i < *c; ++i) {
        QString url = (*data)[QString::number(i)].toObject()["name"].toString();
        QTableWidgetItem* n = new QTableWidgetItem();
        QTableWidgetItem* item = new QTableWidgetItem();
        n->setText(QString::number(i));
        item->setText(url);
        ui->table->setItem(i, 0, n);
        ui->table->setItem(i, 1, item);

    }

    for (int i = 0; i < (*cStatus)+1; ++i) {
        QTableWidgetItem* checkBox = new QTableWidgetItem();
        checkBox->setCheckState(Qt::CheckState::Checked);
        ui->table->setItem(i, 2, checkBox);
    }
}
