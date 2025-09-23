#include "include/levels.h"
#include "forms/ui_levels.h"

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

    spacers.append(ui->horizontalSpacer);
    spacers.append(ui->horizontalSpacer_2);
    spacers.append(ui->horizontalSpacer_3);
    spacers.append(ui->horizontalSpacer_4);
    spacers.append(ui->horizontalSpacer_5);
    spacers.append(ui->horizontalSpacer_6);
    //spacers.append(ui->buttonSpacer);

}

Levels::~Levels() {
    delete ui;
}


void Levels::setItem(QStringList labels, QStringList icons) {

    for (int i = 0; i < labels.count(); i++) {
        labelUI[i]->setText(labels.value(i));
    }

    for (int i = labels.count(); i < labelUI.count(); i++) {
        labelUI[i]->hide();
        labelUI[i]->setMinimumWidth(0);
        if (i != labels.count() && i < spacers.length()) spacers[i]->changeSize(0, 0);
    }

    if (labels.count() == 1) labelUI[0]->setMaximumWidth(16777215);
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


void Levels::setLabelSize(int label, int size) {
    if (label < labelUI.length()) {
        labelUI[label]->setMinimumWidth(size);
        labelUI[label]->setMaximumWidth(size);
    }
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
