#ifndef ABOUT_H
#define ABOUT_H

#include <QDir>
#include <QUrl>
#include <QDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDesktopServices>

#include "terms.h"
#include "basebrowser.h"

namespace Ui {
class About;
}

class About : public QDialog {
    Q_OBJECT

public:
    void initDevMode();
    explicit About(QWidget *parent = nullptr);
    ~About();

private:
    Ui::About *ui;
    Terms *termsDialog = nullptr;
    BaseBrowser *feedbackBrowser = nullptr;
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
