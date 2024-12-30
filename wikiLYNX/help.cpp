#include "include/help.h"
#include "ui/ui_help.h"

help::help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::help)
{
    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &help::close);

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    QString logo = ui->appLogo->document()->toHtml();
    if (theme == "Light") logo.replace("#181818", "#ffffff");
    ui->appLogo->setHtml(logo);


}

help::~help()
{
    delete ui;
}


bool help::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void help::initialise(QString url) {
    ui->webEngineView->load(QUrl::fromUserInput(url));
}
