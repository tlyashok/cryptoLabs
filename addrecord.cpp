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

void AddRecord::setData(Record *record) {
    this->record = record;
    ui->recordNameLineEdit->setText(record->recordName);
    ui->userNameLineEdit->setText(record->userName);
    ui->passwordLineEdit->setText(record->password);
}

void AddRecord::accept() {
    this->hide();
    record->recordName = ui->recordNameLineEdit->text();
    record->userName = ui->userNameLineEdit->text();
    record->password= ui->passwordLineEdit->text();
    ui->recordNameLineEdit->setText("");
    ui->userNameLineEdit->setText("");
    ui->passwordLineEdit->setText("");
    if (makeNew)
        WindowsManager::get()->MainW->addRecord(*(this->record));
}

void AddRecord::create()
{
    this->record = new Record();
    makeNew = true;
}

void AddRecord::edit()
{

}

void AddRecord::on_buttonBox_rejected()
{
    this->hide();
    ui->recordNameLineEdit->setText("");
    ui->userNameLineEdit->setText("");
    ui->passwordLineEdit->setText("");
}

