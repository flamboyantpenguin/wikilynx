#pragma once

#include <QDialog>

#include "util.h"

namespace Ui {
class Terms;
}

class Terms : public QDialog {
    Q_OBJECT

public:
    explicit Terms(QWidget *parent = nullptr);
    ~Terms();

private:
    bool isDarkTheme();
    void initialise();
    Ui::Terms *ui;
};
