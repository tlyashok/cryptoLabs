#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "record.h"
#include "recordrowwidget.h"
#include "windowsmanager.h"

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

void MainWindow::showWindow()
{
    this->show();
    readFile();
    resetView();
}

void MainWindow::readFile() {
    QFile file("records.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }
    QByteArray hexEncData = file.readAll();
    QByteArray encData = QByteArray::fromHex(hexEncData);
    QByteArray unEncData;
    int errorcode = decrypt_file(encData, unEncData);
    if (errorcode) {
        qDebug() << "Error when decrypting! Errorcode: " << errorcode;
        return;
    }
    QJsonDocument data = QJsonDocument::fromJson(unEncData);
    fromJson(data.object());
}

void MainWindow::fromJson(QJsonObject json) {
    if (!json.contains("records") || !json["records"].isArray())
        return;
    QJsonArray jsonRecords = json["records"].toArray();
    records.clear();
    for (int i = 0; i < jsonRecords.size(); i++) {
        Record r;
        r.recordName = jsonRecords[i].toObject()["recordName"].toString();
        r.userName = jsonRecords[i].toObject()["userName"].toString();
        r.password = jsonRecords[i].toObject()["password"].toString();
        records.append(r);
    }
}

int MainWindow::decrypt_file(const QByteArray& in, QByteArray& out) {
    QByteArray key_qba = QByteArray::fromHex("ebbc302ee102a698092424f16221330da793ddc2ffb24ad0ff88cff7bc4e4324");
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

void MainWindow::resetView() {
    ui->listWidget->clear();
    populateListWidget(text_search);
}

void MainWindow::removeRecord(Record* record) {
    int index = -1;
    for (int i = 0; i < records.size(); i++)
        if (&(records[i]) == record) {
            index = i;
            break;
        }
    if (index != -1) {
        records.remove(index);
        resetView();
    }
}

void MainWindow::addRecordWidget(int index) {
    Record* record = &(records[index]);
    RecordRowWidget* row = new RecordRowWidget(record, index, this->ui->listWidget);
    QListWidgetItem* item = new QListWidgetItem(this->ui->listWidget);
    connect(row, &RecordRowWidget::removeRecord, this, &MainWindow::removeRecord);
    this->ui->listWidget->addItem(item);
    item->setSizeHint(row->minimumSizeHint());
    this->ui->listWidget->setItemWidget(item, row);
}

void MainWindow::populateListWidget(QString search_name) {
    for (int i = 0; i < records.size(); i++)
        if (records[i].recordName.toLower().contains(search_name.toLower())) {
            addRecordWidget(i);
        }
}

void MainWindow::on_pushButton_clicked()
{
    saveToFile();
    QMessageBox mb;
    mb.setText("Сохранено!");
    mb.exec();
}

void MainWindow::saveToFile() {
    QFile file("records.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return;
    }
    QJsonObject data = this->toJson();
    QByteArray unEncData = QJsonDocument(this->toJson()).toJson();
    QByteArray encData;
    int errorcode = encrypt_file(unEncData, encData);
    if (errorcode) {
        qDebug() << "Error when encrypting! Errorcode: " << errorcode;
        return;
    }
    QByteArray hexEncData = encData.toHex();
    file.write(hexEncData);
    file.close();
}

QJsonObject MainWindow::toJson() {
    QJsonObject data;
    QJsonArray jsonRecords;
    for (int i = 0; i < records.size(); i++) {
        QJsonObject jsonRecord = records[i].toJson();
        jsonRecords.append(jsonRecord);
    }
    data["records"] = jsonRecords;
    return data;
}

int MainWindow::encrypt_file(const QByteArray& in, QByteArray& out) {
    QByteArray key_qba = QByteArray::fromHex("ebbc302ee102a698092424f16221330da793ddc2ffb24ad0ff88cff7bc4e4324");
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
    resetView();
}

void MainWindow::addRecord(Record rec)
{
    records.append(rec);
    resetView();
}

void MainWindow::on_pushButton_2_clicked()
{
    WindowsManager::get()->AddW->create();
    WindowsManager::get()->AddW->show();
}


