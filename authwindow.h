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
    void ask_save_code();
    void ask_load_code();
    ~AuthWindow();

private slots:
    void on_joinButton_clicked();

    void on_saveButton_clicked();

    void hide_window();
private:
    Ui::AuthWindow *ui;
};

#endif // AUTHWINDOW_H
