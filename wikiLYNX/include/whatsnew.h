#ifndef WHATSNEW_H
#define WHATSNEW_H

#include <QDialog>
#include <QDesktopServices>

namespace Ui {
class whatsNew;
}

class whatsNew : public QDialog
{
    Q_OBJECT

public:
    explicit whatsNew(QWidget *parent = nullptr);
    ~whatsNew();

private:
    Ui::whatsNew *ui;

private slots:
    void launchInfo();
};

#endif // WHATSNEW_H
