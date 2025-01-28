#ifndef WHATSNEW_H
#define WHATSNEW_H

#include <QFile>
#include <QDialog>
#include <QDesktopServices>

namespace Ui {
class WhatsNew;
}

class WhatsNew : public QDialog {
    Q_OBJECT

public:
    explicit WhatsNew(QString ver, QWidget *parent = nullptr);
    ~WhatsNew();

private:
    QString ver;
    Ui::WhatsNew *ui;

private slots:
    void launchInfo();
};

#endif // WHATSNEW_H
