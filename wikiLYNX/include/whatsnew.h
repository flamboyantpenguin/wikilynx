#ifndef WHATSNEW_H
#define WHATSNEW_H

#include <QDialog>
#include <QDesktopServices>

namespace Ui {
class WhatsNew;
}

class WhatsNew : public QDialog {
    Q_OBJECT

public:
    explicit WhatsNew(QWidget *parent = nullptr);
    ~WhatsNew();

private:
    Ui::WhatsNew *ui;

private slots:
    void launchInfo();
};

#endif // WHATSNEW_H
