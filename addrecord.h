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
    void setData(Record *record);
    void accept();
    void create();
private slots:
    void changeEchoMode(QWidget *oldFocus, QWidget *newFocus);
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    bool makeNew;
    Record *record;
    Ui::AddRecord *ui;
};

#endif // VIEWRECORDDIALOG_H
