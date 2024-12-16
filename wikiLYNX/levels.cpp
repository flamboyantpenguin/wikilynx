#include "include/levels.h"
#include "ui/ui_levels.h"

levels::levels(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::levels)
{
    ui->setupUi(this);
    connect(ui->action0, &QPushButton::clicked, this, &levels::initAction0);
    connect(ui->action1, &QPushButton::clicked, this, &levels::initAction1);
    connect(ui->action2, &QPushButton::clicked, this, &levels::initAction2);

    labels.append(ui->label0);
    labels.append(ui->label1);
    labels.append(ui->label2);
    labels.append(ui->label3);

}

levels::~levels()
{

}


void levels::setItem(QString label0, QString label1, QString label2, QString label3, QString label4, QString icon0, QString icon1, QString icon2) {

    ui->label0->setText(label0);
    ui->label1->setText(label1);
    ui->label2->setText(label2);
    ui->label3->setText(label3);
    ui->label4->setText(label4);
    if (!(icon0.isEmpty())) ui->action0->setIcon(QIcon::fromTheme(icon0));
    else ui->action0->hide();
    if (!(icon1.isEmpty())) ui->action1->setIcon(QIcon::fromTheme(icon1));
    else ui->action1->hide();
    if (!(icon2.isEmpty())) ui->action2->setIcon(QIcon::fromTheme(icon2));
    else ui->action2->hide();

}


QString levels::getItem(int n) {
    return labels[n]->text();
}


void levels::initAction0() {
    emit action0(ui->label0->text());
}


void levels::initAction1() {
    emit action1(ui->label0->text());
}


void levels::initAction2() {
    emit action2(ui->label0->text());
}
