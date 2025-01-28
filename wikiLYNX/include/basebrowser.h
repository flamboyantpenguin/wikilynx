#ifndef BASEBROWSER_H
#define BASEBROWSER_H

#include <QDialog>

namespace Ui {
class BaseBrowser;
}

class BaseBrowser : public QDialog {
    Q_OBJECT

public:
    explicit BaseBrowser(QWidget *parent = nullptr);
    ~BaseBrowser();

    void initialise(QString, QString url  = "https://github.com/flamboyantpenguin/wikilynx/wiki/Gameplay");


private:
    Ui::BaseBrowser *ui;
    bool isDarkTheme();

};

#endif // BASEBROWSER_H
