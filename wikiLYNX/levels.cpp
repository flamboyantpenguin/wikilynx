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

}

levels::~levels()
{

}


void levels::setItem(QString code, QString time, QString chk, QString difficulty, QString icon0, QString icon1, QString icon2) {

    ui->code->setText(code);
    ui->time->setText(time);
    ui->chk->setText(chk);
    ui->difficulty->setText(difficulty);
    if (!(icon0.isEmpty())) ui->action0->setIcon(QIcon::fromTheme(icon0));
    else ui->action0->hide();
    if (!(icon1.isEmpty())) ui->action1->setIcon(QIcon::fromTheme(icon1));
    else ui->action1->hide();
    if (!(icon2.isEmpty())) ui->action2->setIcon(QIcon::fromTheme(icon2));
    else ui->action2->hide();

}


void levels::initAction0() {
    QString code = ui->code->text();
    emit action0(code);
}


void levels::initAction1() {
    QString code = ui->code->text();
    emit action1(code);
}


void levels::initAction2() {
    QString code = ui->code->text();
    emit action2(code);
}
