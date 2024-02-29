#include "record.h"

Record::Record() {
    this->recordName = "";
    this->userName = "";
    this->password = "";
}

Record::Record(QString recordName, QString userName, QString password) {
    this->recordName = recordName;
    this->userName = userName;
    this->password = password;
}

QJsonObject Record::toJson() {
    QJsonObject result {
                       {"recordName", recordName},
                       {"userName", userName},
                       {"password", password},
                       };
    return result;
}
