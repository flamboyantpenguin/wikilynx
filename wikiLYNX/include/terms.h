#ifndef TERMS_H
#define TERMS_H

#include <QDialog>
#include <QFile>

namespace Ui {
class terms;
}

class terms : public QDialog
{
    Q_OBJECT

public:
    explicit terms(QWidget *parent = nullptr);
    ~terms();

private:
    bool isDarkTheme();
    void initialise();
    Ui::terms *ui;
};

#endif // TERMS_H
