#include "include/util.h"


QString Util::justReadThisFile(const QString &path) {

    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QString text = QString(file.readAll());
        file.close();
        file.deleteLater();
        return text;
    }
    return "There was an error reading this text";
}
