#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QJsonObject>

struct Record {
    QString recordName;
    QString userName;
    QString password;
    Record();
    Record(QString recordName, QString userName, QString password);
    QJsonObject toJson();
};

#endif // RECORD_H
