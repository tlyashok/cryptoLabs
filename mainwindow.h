#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonArray>
#include <openssl/evp.h>

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

private slots:
    void removeRecord(Record* record);

private:
    QVector<Record> records;

    void fromJson(QJsonObject json);

    void readFile();

    void resetView();

    int decrypt_file(const QByteArray& in, QByteArray& out);

    void addRecordWidget(int index);

    void populateListWidget();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
