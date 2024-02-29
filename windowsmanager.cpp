#include "windowsmanager.h"

WindowsManager::WindowsManager()
{
    this->AuthW = new AuthWindow();
    this->MainW = new MainWindow();
}

void WindowsManager::start()
{
    AuthW->show();
}
