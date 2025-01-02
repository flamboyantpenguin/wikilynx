#include "include/about.h"
#include "ui/ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &about::close);
    connect(ui->showLicense, &QPushButton::clicked, this, &about::showTerms);
    connect(ui->feedbackButton, &QPushButton::clicked, this, &about::launchFeedback);

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}

about::~about()
{
    delete ui;
}


bool about::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void about::showTerms() {
    termsDialog.show();
}


void about::launchFeedback() {
    QDesktopServices::openUrl(QUrl("https://forms.gle/SScZKbFLFBffdVay8"));
}
