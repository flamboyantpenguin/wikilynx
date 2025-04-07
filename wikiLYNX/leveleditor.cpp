#include "include/leveleditor.h"
#include "ui/ui_leveleditor.h"


LevelEditor::LevelEditor(QWidget *parent) : QDialog(parent), ui(new Ui::LevelEditor) {
    ui->setupUi(this);
    ui->isWiki->setChecked(true);
    ui->genRandIndicatorPrg->hide();
    ui->genRandIndicatorText->hide();
    connect(ui->homeButton, &QPushButton::clicked, this, &LevelEditor::resetBrowser);
    connect(ui->backButton, &QPushButton::clicked, ui->webEngineView, &QWebEngineView::back);
    connect(ui->list, &QListWidget::itemDoubleClicked, this, &LevelEditor::previewArticle);
    connect(ui->getRandomButton, &QPushButton::clicked, this, &LevelEditor::getRandomArticle);
    connect(ui->addButton, &QPushButton::clicked, this, &LevelEditor::addChk);
    connect(ui->closeButton, &QPushButton::clicked, this, &LevelEditor::close);
    connect(ui->webEngineView, &QWebEngineView::urlChanged, this, &LevelEditor::updateBrowser);
    connect(ui->refreshButton, &QPushButton::clicked, ui->webEngineView, &QWebEngineView::reload);
    connect(ui->url, &QLineEdit::returnPressed, this, &LevelEditor::loadURL);
    connect(ui->webEngineView, &QWebEngineView::urlChanged, this, &LevelEditor::updateBrowser);
    connect(ui->webEngineView, &QWebEngineView::loadFinished, this, &LevelEditor::genRandomReload);
    connect(ui->refreshButton, &QPushButton::clicked, ui->webEngineView, &QWebEngineView::reload);
    connect(ui->clicks, &QLineEdit::textChanged, this, &LevelEditor::updateExtras);
    connect(ui->code, &QLineEdit::textChanged, this, &LevelEditor::updateCodeName);
    connect(ui->timeTaken, &QLineEdit::textChanged, this, &LevelEditor::updateExtras);
    connect(ui->difficulty, &QLineEdit::textChanged, this, &LevelEditor::updateExtras);
    connect(ui->isWiki, &QCheckBox::checkStateChanged, this, &LevelEditor::updateIsWiki);

}


LevelEditor::~LevelEditor()
{
    delete ui;
}


void LevelEditor::initialise(ScoreSheet *gameData, QString cde) {

    this->code = cde;
    this->genRandom = -1;
    this->gameData = gameData;
    this->levelInfo = this->gameData->getLevel(cde);
    this->chkData = this->levelInfo["levels"].toString().split(" ");

    if (this->levelInfo["levels"].toString().isEmpty()) this->chkData.pop_front();

    ui->code->setText(code);
    ui->difficulty->setText(levelInfo["difficulty"].toString());
    this->levelInfo["clicks"] = this->levelInfo["clicks"].toInt();
    ui->clicks->setText(QString::number(this->levelInfo["clicks"].toInt()));
    this->levelInfo["time"] = this->levelInfo["time"].toDouble();
    ui->timeTaken->setText(QString::number(this->levelInfo["time"].toDouble()));

    this->updateHeader();
    this->updateChkList();
    this->updateBrowser();

    if (!this->levelInfo.contains("wiki?")) ui->isWiki->setChecked(true);
    else if (!(this->levelInfo["wiki?"].toBool())) ui->isWiki->setChecked(false);
    else ui->isWiki->setChecked(true);

}


void LevelEditor::genRandomLevel(ScoreSheet *gameData, QString code) {

    ui->genRandIndicatorPrg->show();
    ui->genRandIndicatorText->show();

    this->chkData.clear();
    this->levelInfo = QJsonObject();

    QRandomGenerator *generator = QRandomGenerator::global();

    genRandom = (generator->bounded(2, 11)) + 1;

    this->code = code;
    this->gameData = gameData;

    ui->code->setText(code);
    ui->difficulty->setText("genRandom");
    ui->clicks->setText(QString::number(0));
    ui->timeTaken->setText(QString::number(0));

    this->levelInfo["wiki?"] = true;
    ui->isWiki->setChecked(true);

    this->updateHeader();
    this->updateChkList();
    this->updateBrowser();

    ui->webEngineView->load(QUrl::fromUserInput("https://wikipedia.org/wiki/Special:Random"));

}


void LevelEditor::genRandomReload() {
    if (genRandom == -1) return;
    if (!genRandom) {
        ui->genRandIndicatorPrg->hide();
        ui->genRandIndicatorText->hide();
        this->update();
        emit genRandomFinished();
        return;
    }
    ui->addButton->click();
    ui->webEngineView->load(QUrl::fromUserInput("https://wikipedia.org/wiki/Special:Random"));
    genRandom--;
}


void LevelEditor::updateHeader() {

    ui->header->clear();
    QListWidgetItem *item = new QListWidgetItem();
    auto widget = new Levels(this);

    QString s("URL ($count)");
    s.replace("$count", QString::number(this->chkData.count()));

    QStringList itemData = {s};
    QStringList icons = {"neutralOnline"};

    widget->setItem(itemData, icons);

    item->setSizeHint(widget->sizeHint());
    ui->header->addItem(item);
    ui->header->setItemWidget(item, widget);


}


void LevelEditor::updateExtras() {

    this->levelInfo["difficulty"] = ui->difficulty->text();
    this->levelInfo["time"] = ui->timeTaken->text().toDouble();
    this->levelInfo["clicks"] = ui->clicks->text().toInt();
    //this->saveData();

}


void LevelEditor::updateCodeName() {

    this->gameData->removeLevel(code);
    this->code = ui->code->text();
    this->gameData->updateLevel(code, this->levelInfo);
    //this->saveData();

}


void LevelEditor::updateIsWiki() {

    this->levelInfo["wiki?"] = ui->isWiki->isChecked();
    //this->saveData();

}


void LevelEditor::updateChkList() {

    ui->list->clear();

    for (int i = 0; i < this->chkData.count(); i++) {

        QListWidgetItem *item = new QListWidgetItem();
        auto widget = new Levels(this);

        QStringList itemData = {this->chkData.value(i)};
        QStringList icons = {"delete"};

        widget->setItem(itemData, icons);

        connect(widget, &Levels::action0, this, &LevelEditor::removeChk);

        auto tmp = ui->header->sizeHint();
        tmp.setHeight(widget->sizeHint().height());
        item->setSizeHint(tmp);
        ui->list->addItem(item);
        ui->list->setItemWidget(item, widget);

    }

    if (ui->list->count()) ui->isWiki->setEnabled(false);
    else ui->isWiki->setEnabled(true);

}


void LevelEditor::getRandomArticle() {
    ui->webEngineView->load(QUrl::fromUserInput("https://wikipedia.org/wiki/Special:Random"));
}


void LevelEditor::updateBrowser() {
    ui->url->setText(ui->webEngineView->url().toString());
}


void LevelEditor::loadURL() {
    ui->webEngineView->load(QUrl::fromUserInput(ui->url->text()));
}


void LevelEditor::previewArticle(QListWidgetItem *i) {
    auto itemWidget = (Levels *) ui->list->itemWidget(i);
    if (this->levelInfo["wiki?"].toBool()) {
        ui->webEngineView->load(QUrl::fromUserInput("https://wikipedia.org/wiki/"+itemWidget->getItem(0)));
    }
    else {
        ui->webEngineView->load(QUrl::fromUserInput(itemWidget->getItem(0)));
    }
}


void LevelEditor::resetBrowser() {
    ui->webEngineView->load(QUrl::fromUserInput("https://wikipedia.org"));
}


void LevelEditor::launchHelp()   {
    QDesktopServices::openUrl(QUrl("https://github.com/flamboyantpenguin/wikilynx/wiki/Gameplay#level-editor"));
}


void LevelEditor::updateIndex() {

    for (int i = 0; i < ui->list->count(); i++) {
        auto item = ui->list->item(i);
        auto itemWidget = (Levels *) ui->list->itemWidget(item);
        this->chkData[i] = itemWidget->getItem(0);
    }

}



void LevelEditor::saveData() {
    this->updateIndex();
    this->levelInfo["levels"] = this->chkData.join(" ");
    this->gameData->updateLevel(this->code, this->levelInfo);
}


void LevelEditor::addChk() {

    QString url = ui->url->text();

    if (!QUrl::fromUserInput(ui->url->text()).isValid()) return;

    if (this->levelInfo["wiki?"].toBool()) {
        if (url.contains("wikipedia.org/wiki")) url = url.split("wikipedia.org/wiki/")[1];
        else ui->isWiki->setChecked(false);
    }

    this->chkData.append(url);
    this->updateHeader();
    this->updateChkList();
    this->saveData();

}


void LevelEditor::removeChk(QString chk) {
    this->chkData.removeAll(chk);
    //this->chkData.removeAt(chk.toInt());
    this->updateChkList();
    this->updateHeader();
    this->saveData();
}
