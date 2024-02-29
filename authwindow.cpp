#include "authwindow.h"
#include "ui_authwindow.h"
#include "windowsmanager.h"

AuthWindow::AuthWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    this->show();
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
    if (ui->lineEdit->text()=="123") {
        hide_window();
        WindowsManager::get()->MainW->showWindow();
    } else {
        QMessageBox msgBox;
        msgBox.setText("Неверный пароль");
        msgBox.exec();
        ui->lineEdit->setText("");
    }
}

