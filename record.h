#ifndef RECORD_H
#define RECORD_H

#include <QString>
#include <QJsonObject>
#include <QIODevice>
#include <openssl/evp.h>

struct decryptedDatas {
    QString recordName;
    QString userName;
    QString password;
    decryptedDatas(const QByteArray& recordName, const QByteArray& userName, const QByteArray& password, QByteArray& pin_key);
    decryptedDatas(const QString& recordName, const QString& userName, const QString& password);
private:
    int decrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key);
};

struct Record {
    QByteArray recordName;
    QByteArray userName;
    QByteArray password;
    Record();
    Record(QString recordName, QString userName, QString password, QByteArray& pin_key);
    Record(QByteArray recordName, QByteArray userName, QByteArray password, QByteArray &pin_key);
    QJsonObject toJson(QByteArray& pin_key);
    decryptedDatas getDatas(QByteArray& pin_key);

    bool operator==(const Record& other) const {
        return (this->recordName == other.recordName &&
                this->userName == other.userName &&
                this->password == other.password);
    }

    bool operator!=(const Record& other) const {
        return !(*this == other);
    }

private:
    int decrypt_file(const QByteArray &in, QByteArray &out, QByteArray &pin_key);
    int encrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key);
};

#endif // RECORD_H
