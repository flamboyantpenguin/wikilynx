#include "include/basebrowser.h"
#include "forms/ui_basebrowser.h"

BaseBrowser::BaseBrowser(QWidget *parent) : QDialog(parent), ui(new Ui::BaseBrowser) {

    ui->setupUi(this);
    connect(ui->closeButton, &QPushButton::clicked, this, &BaseBrowser::close);

    QString theme = (isDarkTheme()) ? "Dark" : "Light";
    ui->appLogo->setIcon(QIcon(":/base/images/wikiLYNX_" + theme + ".svg"));
    ui->appLogo->update();

}

BaseBrowser::~BaseBrowser() {
    delete ui;
}


bool BaseBrowser::isDarkTheme() {
    QColor backgroundColor = qApp->palette().color(QPalette::Window);
    int luminance = (0.299 * backgroundColor.red() +
                     0.587 * backgroundColor.green() +
                     0.114 * backgroundColor.blue());
    return luminance < 128;  // If luminance is low, it's likely a dark theme.
}


void BaseBrowser::initialise(QString title, QString url) {
    this->setWindowTitle(title);
    ui->webEngineView->load(QUrl::fromUserInput(url));
}
