#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "record.h"
#include "recordrowwidget.h"
#include "windowsmanager.h"

bool DEBUG = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::tryDecrypt(QByteArray &pin_key)
{
    if (!readFile(pin_key)) {
        return false;
    } else {
        WindowsManager::get()->AddW->addRecord(pin_key);
        if (WindowsManager::get()->AddW->isEditNow == false) {
            resetView(pin_key);
        }
        this->show();
        if (WindowsManager::get()->AddW->isEditNow == true) {
            WindowsManager::get()->AddW->show();
        }
        WindowsManager::get()->AddW->isEditNow = false;
        return true;
    }
}

bool MainWindow::readFile(QByteArray &pin_key) {
    QFile file("records.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return false;
    }
    QByteArray hexEncData = file.readAll();
    QByteArray encData = QByteArray::fromHex(hexEncData);
    QByteArray unEncData;
    int errorcode = decrypt_file(encData, unEncData, pin_key);
    if (errorcode) {
        qDebug() << "Error when decrypting! Errorcode: " << errorcode;
        return false;
    }
    try {
        QJsonDocument data = QJsonDocument::fromJson(unEncData);
        fromJson(data.object(), pin_key);
        return true;
    } catch (...) {
        return false;
    }
}

void MainWindow::saveToFile(QByteArray& pin_key) {
    QFile file("records.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }
    QByteArray unEncData = QJsonDocument(this->toJson(pin_key)).toJson();
    QByteArray encData;
    int errorcode = encrypt_file(unEncData, encData, pin_key);
    if (errorcode) {
        qDebug() << "Error when encrypting! Errorcode: " << errorcode;
        return;
    }
    QByteArray hexEncData = encData.toHex();
    file.write(hexEncData);
    file.close();
}

bool MainWindow::fromJson(QJsonObject json, QByteArray &pin_key) {
    records.clear();
    if (!json.contains("records") || !json["records"].isArray())
        return false;
    QJsonArray jsonRecords = json["records"].toArray();
    for (int i = 0; i < jsonRecords.size(); i++) {
        Record r(
            jsonRecords[i].toObject()["recordName"].toString().toUtf8(),
            jsonRecords[i].toObject()["userName"].toString().toUtf8(),
            jsonRecords[i].toObject()["password"].toString().toUtf8(),
            pin_key
        );
        records.append(r);
    }
    return true;
}

int MainWindow::decrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key) {
    if (DEBUG) {
        out = in;
        return 0;
    }
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

void MainWindow::resetView(QByteArray &pin_key) {
    ui->listWidget->clear();
    populateListWidget(text_search, pin_key);
}

void MainWindow::removeRecord(Record* record) {
    int index = -1;
    for (int i = 0; i < records.size(); i++)
        if (&(records[i]) == record) {
            index = i;
            break;
        }
    if (index != -1) {
        qDebug() << "succesful delete\n";
        qDebug() << records.size();
        records.removeAt(index);
        qDebug() << records.size();
        WindowsManager::get()->AuthW->ask_save_code();
    }
}

void MainWindow::addRecordWidget(int index, QByteArray &pin_key) {
    Record* record = &(records[index]);
    RecordRowWidget* row = new RecordRowWidget(record, index, pin_key, this->ui->listWidget);
    QListWidgetItem* item = new QListWidgetItem(this->ui->listWidget);
    connect(row, &RecordRowWidget::removeRecord, this, &MainWindow::removeRecord);
    this->ui->listWidget->addItem(item);
    item->setSizeHint(row->minimumSizeHint());
    this->ui->listWidget->setItemWidget(item, row);
}

void MainWindow::populateListWidget(QString search_name, QByteArray &pin_key) {
    for (int i = 0; i < records.size(); i++)
        if (records[i].getDatas(pin_key).recordName.toLower().contains(search_name.toLower())) {
            addRecordWidget(i, pin_key);
        }
}

void MainWindow::loadFieldsFromRecord(QByteArray &pin_key)
{
    if (ui->listWidget->currentItem() && WindowsManager::get()->AddW->isEditNow == true) {
        QListWidgetItem *currentItem = ui->listWidget->currentItem();
        QWidget *widget = ui->listWidget->itemWidget(currentItem);
        Record record = *(static_cast<RecordRowWidget*>(widget)->getRecord());
        WindowsManager::get()->AddW->edit(record, pin_key);
        int index = records.indexOf(record);
        if (index != -1) {
            records.removeAt(index);
        }
        saveToFile(pin_key);
    }
}

void MainWindow::checkAuth()
{
    this->setEnabled(false);
    WindowsManager::get()->AuthW->ask_load_code();
}

void MainWindow::authComplete()
{
    this->setEnabled(true);
}

void MainWindow::on_pushButton_clicked()
{
    WindowsManager::get()->AuthW->ask_save_code();
}

QJsonObject MainWindow::toJson(QByteArray& pin_key) {
    QJsonObject data;
    QJsonArray jsonRecords;
    for (int i = 0; i < records.size(); i++) {
        QJsonObject jsonRecord = records[i].toJson(pin_key);
        jsonRecords.append(jsonRecord);
    }
    data["records"] = jsonRecords;
    return data;
}

int MainWindow::encrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key) {
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

void MainWindow::on_lineEdit_textEdited(const QString &text)
{
    this->text_search = text;
    checkAuth();
}

void MainWindow::addRecord(Record rec, QByteArray& pin_key)
{
    records.append(rec);
    saveToFile(pin_key);
}

void MainWindow::on_pushButton_2_clicked()
{
    WindowsManager::get()->AddW->show();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    WindowsManager::get()->AddW->isEditNow = true;
    WindowsManager::get()->AuthW->ask_load_code();
}


void MainWindow::on_reloadButton_clicked()
{
    WindowsManager::get()->AuthW->ask_load_code();
}

