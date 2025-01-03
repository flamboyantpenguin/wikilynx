#ifndef VIEWSTATS_H
#define VIEWSTATS_H

#include <QFile>
#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace Ui {
class viewStats;
}

class viewStats : public QDialog
{
    Q_OBJECT

public:
    int initialise();
    explicit viewStats(QWidget *parent = nullptr);
    ~viewStats();

private:
    bool isDarkTheme();
    QJsonObject data;
    Ui::viewStats *ui;
    QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

private slots:
    void loadData(int s = 0);
};

#endif // VIEWSTATS_H
