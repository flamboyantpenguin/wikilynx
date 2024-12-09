#include "include/leveleditor.h"
#include "ui/ui_leveleditor.h"

levelEditor::levelEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::levelEditor)
{
    ui->setupUi(this);
    ui->table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    connect(ui->addButton, &QPushButton::clicked, this, &levelEditor::addChk);
    connect(ui->removeButton, &QPushButton::clicked, this, &levelEditor::removeChk);
    connect(ui->closeButton, &QPushButton::clicked, this, &levelEditor::close);

}

levelEditor::~levelEditor()
{
    delete ui;
}


void levelEditor::initialise(QJsonObject *chk, QString cde) {

    this->chkData = chk;
    this->code = cde;
    auto d = chk->value(cde).toObject();


    auto l = d.keys();
    ui->table->setRowCount(l.count());

    for (int i = 0; i < l.count(); i++) {

        QTableWidgetItem* nChk = new QTableWidgetItem();
        QTableWidgetItem* chkName = new QTableWidgetItem();
        QTableWidgetItem* chkURL = new QTableWidgetItem();
        nChk->setText(QString::number(i));
        chkName->setText(d[QString::number(i)].toObject().value("name").toString());
        chkURL->setText(d[QString::number(i)].toObject().value("url").toString());
        ui->table->setItem(i, 0, nChk);
        ui->table->setItem(i, 1, chkName);
        ui->table->setItem(i, 2, chkURL);

    }
}


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


void levelEditor::addChk() {
    ui->table->setRowCount(ui->table->rowCount()+1);
    QTableWidgetItem* i = new QTableWidgetItem();
    i->setText(QString::number(ui->table->rowCount() - 1));
    ui->table->setItem(ui->table->rowCount() - 1, 0, i);
}


void levelEditor::removeChk() {
    ui->table->removeRow(ui->table->rowAt(ui->table->rowCount() - 1));
}
