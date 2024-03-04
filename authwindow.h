#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include <QCryptographicHash>

namespace Ui {
class AuthWindow;
}

class AuthWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AuthWindow(QWidget *parent = nullptr);
    void hide_window();
    ~AuthWindow();

private slots:
    void on_joinButton_clicked();

private:
    Ui::AuthWindow *ui;
};

#endif // AUTHWINDOW_H
