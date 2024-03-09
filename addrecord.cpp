#include "addrecord.h"
#include "ui_addrecord.h"
#include "windowsmanager.h"
#include <QDebug>

AddRecord::AddRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRecord) {
    ui->setupUi(this);
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    connect((QApplication*) QApplication::instance(), &QApplication::focusChanged, this, &AddRecord::changeEchoMode);
}

void AddRecord::changeEchoMode(QWidget *oldFocus, QWidget *newFocus) {
    if(oldFocus == this->ui->passwordLineEdit)
        this->ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
    else if(newFocus ==this->ui->passwordLineEdit)
        this->ui->passwordLineEdit->setEchoMode(QLineEdit::Normal);
}

void AddRecord::on_buttonBox_accepted()
{
    accept();
}

AddRecord::~AddRecord() {
    delete ui;
}

void AddRecord::accept() {
    this->hide();
    this->record = new decryptedDatas(
        ui->recordNameLineEdit->text().toUtf8(),
        ui->userNameLineEdit->text().toUtf8(),
        ui->passwordLineEdit->text().toUtf8()
    );
    ui->recordNameLineEdit->setText("");
    ui->userNameLineEdit->setText("");
    ui->passwordLineEdit->setText("");
    WindowsManager::get()->AuthW->ask_load_code();
}

void AddRecord::create()
{

}

void AddRecord::edit(Record& rec, QByteArray &pin_key)
{
    if (isEditNow) {
        ui->recordNameLineEdit->setText(rec.getDatas(pin_key).recordName);
        ui->userNameLineEdit->setText(rec.getDatas(pin_key).userName);
        ui->passwordLineEdit->setText(rec.getDatas(pin_key).password);
    }
}

void AddRecord::on_buttonBox_rejected()
{
    this->hide();
    ui->recordNameLineEdit->setText("");
    ui->userNameLineEdit->setText("");
    ui->passwordLineEdit->setText("");
}

void AddRecord::addRecord(QByteArray &pin_key)
{
     if (record == nullptr) {
        WindowsManager::get()->MainW->loadFieldsFromRecord(pin_key);
        return;
    }
    Record *encrypted_record = new Record(
        record->recordName,
        record->userName,
        record->password,
        pin_key
    );
    WindowsManager::get()->MainW->addRecord(*encrypted_record, pin_key);
    record = nullptr;
    delete encrypted_record;

}

