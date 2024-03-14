#include "record.h"

Record::Record() {
    this->recordName = "";
    this->userName = "";
    this->password = "";
}

Record::Record(QString recordName, QString userName, QString password, QByteArray& pin_key) {
    encrypt_file(recordName.toUtf8(), this->recordName, pin_key);
    encrypt_file(userName.toUtf8(), this->userName, pin_key);
    encrypt_file(password.toUtf8(), this->password, pin_key);
}

Record::Record(QByteArray recordName, QByteArray userName, QByteArray password, QByteArray& pin_key) {
    this->recordName = recordName;
    this->userName = userName;
    this->password = password;
}

QJsonObject Record::toJson(QByteArray& pin_key) {
    QJsonObject result {
        {"recordName", QString(this->recordName.toHex())},
        {"userName",  QString(this->userName.toHex())},
        {"password", QString(this->password.toHex())},
   };
    return result;
}

decryptedDatas Record::getDatas(QByteArray& pin_key )
{
    return decryptedDatas(this->recordName, this->userName, this->password, pin_key);
}

int Record::encrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key) {
    QByteArray key_qba = pin_key;
    QByteArray iv_qba = QByteArray::fromHex("1be1e6411665696b56e284a50deb15bb");
    QDataStream decryptedStream(in);
    QDataStream encryptedStream(&out, QIODevice::ReadWrite);
    const int bufferLen = 256;
    int encryptedLen, decryptedLen;
    unsigned char key[32], iv[16];
    unsigned char encryptedBuffer[bufferLen] = {0}, decryptedBuffer[bufferLen] = {0};

    memcpy(key, key_qba.data(), 32);
    memcpy(iv, iv_qba.data(), 16);

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_EncryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }
    do {
        decryptedLen = decryptedStream.readRawData(reinterpret_cast<char*>(decryptedBuffer), bufferLen);
        if (!EVP_EncryptUpdate(ctx, encryptedBuffer, &encryptedLen, decryptedBuffer, decryptedLen)) {
            EVP_CIPHER_CTX_free(ctx);
            return 2;
        }
        encryptedStream.writeRawData(reinterpret_cast<char*>(encryptedBuffer), encryptedLen);
    } while (decryptedLen > 0);
    if (!EVP_EncryptFinal_ex(ctx, encryptedBuffer, &encryptedLen)) {
        EVP_CIPHER_CTX_free(ctx);
        return 3;
    }
    encryptedStream.writeRawData(reinterpret_cast<char*>(encryptedBuffer), encryptedLen);
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

int Record::decrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key) {
    QByteArray key_qba = pin_key;
    QByteArray iv_qba = QByteArray::fromHex("1be1e6411665696b56e284a50deb15bb");
    QDataStream encryptedStream(in);
    QDataStream decryptedStream(&out, QIODevice::ReadWrite);
    const int bufferLen = 256;
    int encryptedLen, decryptedLen, tmplen;
    unsigned char key[32], iv[16];
    unsigned char encryptedBuffer[bufferLen] = {0}, decryptedBuffer[bufferLen] = {0};

    memcpy(key, key_qba.data(), 32);
    memcpy(iv, iv_qba.data(), 16);

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }
    do {
        encryptedLen = encryptedStream.readRawData(reinterpret_cast<char*>(encryptedBuffer), bufferLen);
        if (!EVP_DecryptUpdate(ctx, decryptedBuffer, &decryptedLen, encryptedBuffer, encryptedLen)) {
            EVP_CIPHER_CTX_free(ctx);
            return 2;
        }
        decryptedStream.writeRawData(reinterpret_cast<char*>(decryptedBuffer), decryptedLen);
    } while (encryptedLen > 0);

    if (!EVP_DecryptFinal_ex(ctx, decryptedBuffer, &decryptedLen)) {
        EVP_CIPHER_CTX_free(ctx);
        return 3;
    }
    decryptedStream.writeRawData(reinterpret_cast<char*>(decryptedBuffer), decryptedLen);
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

int decryptedDatas::decrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key) {
    QByteArray key_qba = pin_key;
    QByteArray iv_qba = QByteArray::fromHex("1be1e6411665696b56e284a50deb15bb");
    QDataStream encryptedStream(in);
    QDataStream decryptedStream(&out, QIODevice::ReadWrite);
    const int bufferLen = 256;
    int encryptedLen, decryptedLen, tmplen;
    unsigned char key[32], iv[16];
    unsigned char encryptedBuffer[bufferLen] = {0}, decryptedBuffer[bufferLen] = {0};

    memcpy(key, key_qba.data(), 32);
    memcpy(iv, iv_qba.data(), 16);

    EVP_CIPHER_CTX *ctx;
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return 1;
    }
    do {
        encryptedLen = encryptedStream.readRawData(reinterpret_cast<char*>(encryptedBuffer), bufferLen);
        if (!EVP_DecryptUpdate(ctx, decryptedBuffer, &decryptedLen, encryptedBuffer, encryptedLen)) {
            EVP_CIPHER_CTX_free(ctx);
            return 2;
        }
        decryptedStream.writeRawData(reinterpret_cast<char*>(decryptedBuffer), decryptedLen);
    } while (encryptedLen > 0);

    if (!EVP_DecryptFinal_ex(ctx, decryptedBuffer, &decryptedLen)) {
        EVP_CIPHER_CTX_free(ctx);
        return 3;
    }
    decryptedStream.writeRawData(reinterpret_cast<char*>(decryptedBuffer), decryptedLen);
    EVP_CIPHER_CTX_free(ctx);

    return 0;
}

decryptedDatas::decryptedDatas(const QByteArray& recordName, const QByteArray& userName, const QByteArray& password, QByteArray& pin_key)
{
    QByteArray recordNameDecrypted, userNameDecrypted, passwordDecrypted;
    decrypt_file(recordName, recordNameDecrypted, pin_key);
    decrypt_file(userName, userNameDecrypted, pin_key);
    decrypt_file(password, passwordDecrypted, pin_key);

    this->recordName = QString::fromUtf8(recordNameDecrypted);
    this->userName = QString::fromUtf8(userNameDecrypted);
    this->password = QString::fromUtf8(passwordDecrypted);
}


decryptedDatas::decryptedDatas(const QString& recordName, const QString& userName, const QString& password)
{
    this->recordName = recordName;
    this->userName = userName;
    this->password = password;
}
