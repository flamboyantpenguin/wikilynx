#ifndef ABOUT_H
#define ABOUT_H

#include <QDir>
#include <QUrl>
#include <QDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDesktopServices>

#include "terms.h"

namespace Ui {
class about;
}

class about : public QDialog
{
    Q_OBJECT

public:
    void initDevMode();
    explicit about(QWidget *parent = nullptr);
    ~about();

private:
    Ui::about *ui;
    terms *termsDialog;
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

private slots:
    void showTerms();
    bool isDarkTheme();
    void showAppData();
    void deleteAllData();
    void turnOffDevMode();
    void launchFeedback();
    void toggleLoadingScreen();

signals:
    void turnOffDev();

};

#endif // ABOUT_H
