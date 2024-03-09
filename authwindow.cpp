#include "authwindow.h"
#include "ui_authwindow.h"
#include "windowsmanager.h"

AuthWindow::AuthWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    ask_load_code();
}

void AuthWindow::hide_window()
{
    this->hide();
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

void AuthWindow::on_joinButton_clicked()
{
    QByteArray hash = QCryptographicHash::hash(
                ui->lineEdit->text().toUtf8(),
                QCryptographicHash::Sha256
    );

    if (WindowsManager::get()->MainW->tryDecrypt(hash)) {
        QMessageBox msgBox;
        msgBox.setText("Успешно");
        msgBox.exec();
        ui->lineEdit->setText("");
        WindowsManager::get()->MainW->authComplete();
        hide_window();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Неверный пароль");
        msgBox.exec();
        ui->lineEdit->setText("");
    }

    ui->lineEdit->setText(QString().fill('*', ui->lineEdit->text().size()));
    ui->lineEdit->clear();

    hash.setRawData(
                const_cast<const char*>( QByteArray().fill('*', 32).data()),
                32
    );
}



void AuthWindow::on_saveButton_clicked()
{
    QByteArray hash = QCryptographicHash::hash(
        ui->lineEdit_2->text().toUtf8(),
        QCryptographicHash::Sha256
    );
    WindowsManager::get()->MainW->saveToFile(hash);
    WindowsManager::get()->MainW->tryDecrypt(hash);
    WindowsManager::get()->MainW->authComplete();
    hide_window();

    QMessageBox mb;
    mb.setText("Сохранено!");
    mb.exec();

    ui->lineEdit_2->setText(QString().fill('*', ui->lineEdit_2->text().size()));
    ui->lineEdit_2->clear();

    hash.setRawData(
        const_cast<const char*>( QByteArray().fill('*', 32).data()),
        32
    );
}

void AuthWindow::ask_save_code()
{
    this->ui->stackedWidget->setCurrentIndex(1);
    this->show();
}

void AuthWindow::ask_load_code()
{
    this->ui->stackedWidget->setCurrentIndex(0);
    this->show();
}

