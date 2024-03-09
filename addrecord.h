#ifndef VIEWRECORDDIALOG_H
#define VIEWRECORDDIALOG_H

#include "record.h"
#include <QDialog>

namespace Ui {
class AddRecord;
}

class AddRecord : public QDialog
{
    Q_OBJECT

public:
    explicit AddRecord(QWidget *parent = nullptr);
    ~AddRecord();
    void accept();
    void create();
    void edit(Record& rec, QByteArray &pin_key);
    void addRecord(QByteArray &pin_key);
    bool isEditNow = false;
private slots:
    void changeEchoMode(QWidget *oldFocus, QWidget *newFocus);
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    bool makeNew;
    decryptedDatas *record = nullptr;
    Ui::AddRecord *ui;
};

#endif // VIEWRECORDDIALOG_H
