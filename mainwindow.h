#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
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
    void showWindow();

    void addRecord(Record rec);

private slots:
    void removeRecord(Record* record);

    void on_pushButton_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    QVector<Record> records;

    void fromJson(QJsonObject json);

    void readFile();

    void resetView();

    int decrypt_file(const QByteArray& in, QByteArray& out);

    void addRecordWidget(int index);

    void populateListWidget(QString search_name);

    void saveToFile();

    QString text_search;

    QJsonObject toJson();

    int encrypt_file(const QByteArray& in, QByteArray& out);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
