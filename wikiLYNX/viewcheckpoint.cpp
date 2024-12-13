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


void viewcheckpoint::initialise(QStringList *data, int *cStatus) {

    //ui->table->clear();
    ui->table->setRowCount(data->count());
    ui->table->setColumnCount(3);
    ui->table->setColumnWidth(1, 100);

    ui->table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    for (int i = 0; i < data->count(); ++i) {
        QTableWidgetItem* n = new QTableWidgetItem();
        QTableWidgetItem* item = new QTableWidgetItem();
        QTableWidgetItem* checkBox = new QTableWidgetItem();

        if (i <= *cStatus) checkBox->setCheckState(Qt::CheckState::Checked);
        n->setText(QString::number(i));
        item->setText(data->at(i));
        ui->table->setItem(i, 0, n);
        ui->table->setItem(i, 1, item);
        ui->table->setItem(i, 2, checkBox);

    }

}
