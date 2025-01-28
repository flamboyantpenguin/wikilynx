#include "include/levels.h"
#include "ui/ui_levels.h"

Levels::Levels(QWidget *parent) : QWidget(parent), ui(new Ui::Levels) {
    ui->setupUi(this);
    connect(ui->action0, &QPushButton::clicked, this, &Levels::initAction0);
    connect(ui->action1, &QPushButton::clicked, this, &Levels::initAction1);
    connect(ui->action2, &QPushButton::clicked, this, &Levels::initAction2);

    labelUI.append(ui->label0);
    labelUI.append(ui->label1);
    labelUI.append(ui->label2);
    labelUI.append(ui->label3);
    labelUI.append(ui->label4);
    labelUI.append(ui->label5);
    labelUI.append(ui->label6);

}

Levels::~Levels() {
    delete ui;
}


void Levels::setItem(QStringList labels, QStringList icons) {

    if (labels.value(0).isEmpty()) ui->label0->hide();
    else ui->label0->setText(labels.value(0));
    if (labels.value(1).isEmpty()) ui->label1->hide();
    else ui->label1->setText(labels.value(1));
    if (labels.value(2).isEmpty()) ui->label2->hide();
    else ui->label2->setText(labels.value(2));
    if (labels.value(3).isEmpty()) ui->label3->hide();
    else ui->label3->setText(labels.value(3));
    if (labels.value(4).isEmpty()) ui->label4->hide();
    else ui->label4->setText(labels.value(4));
    if (labels.value(5).isEmpty()) ui->label5->hide();
    else ui->label5->setText(labels.value(5));
    if (labels.value(6).isEmpty()) ui->label6->hide();
    else ui->label6->setText(labels.value(6));
    //ui->label1->setText(label1);
    //ui->label2->setText(label2);
    //ui->label3->setText(label3);
    //ui->label4->setText(label4);
    if (!(icons.value(0).isEmpty())) ui->action0->setIcon(QIcon::fromTheme(icons.value(0)));
    else ui->action0->hide();
    if (!(icons.value(1).isEmpty())) ui->action1->setIcon(QIcon::fromTheme(icons.value(1)));
    else ui->action1->hide();
    if (!(icons.value(2).isEmpty())) ui->action2->setIcon(QIcon::fromTheme(icons.value(2)));
    else ui->action2->hide();

}


QString Levels::getItem(int n) {
    return labelUI[n]->text();
}


void Levels::initAction0() {
    emit action0(ui->label0->text());
}


void Levels::initAction1() {
    emit action1(ui->label0->text());
}


void Levels::initAction2() {
    emit action2(ui->label0->text());
}
