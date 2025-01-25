#ifndef TERMS_H
#define TERMS_H

#include <QDialog>
#include <QFile>

namespace Ui {
class Terms;
}

class Terms : public QDialog {
    Q_OBJECT

public:
    explicit Terms(QWidget *parent = nullptr);
    ~Terms();

private:
    bool isDarkTheme();
    void initialise();
    Ui::Terms *ui;
};

#endif // TERMS_H
