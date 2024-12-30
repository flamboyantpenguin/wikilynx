#ifndef ABOUT_H
#define ABOUT_H

#include <QUrl>
#include <QDialog>
#include <QDesktopServices>

#include "terms.h"

namespace Ui {
class about;
}

class about : public QDialog
{
    Q_OBJECT

public:
    explicit about(QWidget *parent = nullptr);
    ~about();

private:
    Ui::about *ui;
    terms termsDialog;

private slots:
    void showTerms();
    bool isDarkTheme();
    void launchFeedback();
};

#endif // ABOUT_H
