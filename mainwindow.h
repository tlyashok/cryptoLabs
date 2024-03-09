#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QListWidget>
#include <openssl/evp.h>

#include "addrecord.h"

class Record;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addRecord(Record rec, QByteArray& pin_key);

    bool tryDecrypt(QByteArray &pin_key);

    void saveToFile(QByteArray& pin_key);

    void checkAuth();

    void authComplete();

    void loadFieldsFromRecord(QByteArray &pin_key);

private slots:
    void removeRecord(Record* record);

    void on_pushButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_reloadButton_clicked();

private:
    QVector<Record> records;

    bool fromJson(QJsonObject json, QByteArray &pin_key);

    bool readFile(QByteArray &pin_key);

    void resetView(QByteArray &pin_key);

    int decrypt_file(const QByteArray& in, QByteArray& out, QByteArray &pin_key);

    void addRecordWidget(int index, QByteArray &pin_key);

    void populateListWidget(QString search_name, QByteArray &pin_key);

    QString text_search;

    QJsonObject toJson(QByteArray &pin_key);

    int encrypt_file(const QByteArray& in, QByteArray& out, QByteArray& pin_key);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
