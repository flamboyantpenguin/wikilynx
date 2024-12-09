#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QJsonObject>

namespace Ui {
class levelEditor;
}

class levelEditor : public QDialog
{
    Q_OBJECT

public:
    explicit levelEditor(QWidget *parent = nullptr);
    QString code;
    QJsonObject *chkData;
    void saveData();
    void initialise(QJsonObject *, QString);
    ~levelEditor();

protected:
    void closeEvent(QCloseEvent *event) override {
        this->saveData();
        QDialog::closeEvent(event);

    }


private:
    Ui::levelEditor *ui;

private slots:
    void addChk();
    void removeChk();

};

#endif // LEVELEDITOR_H
