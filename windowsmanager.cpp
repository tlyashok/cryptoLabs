#include "windowsmanager.h"

WindowsManager* WindowsManager::w = nullptr;

WindowsManager::WindowsManager()
{
    this->AuthW = new AuthWindow();
    this->MainW = new MainWindow();
    this->AddW = new AddRecord();
}

WindowsManager* WindowsManager::get()
{
    if (w == nullptr) {
        w = new WindowsManager();
    }
    return w;
}

void WindowsManager::start()
{
    AuthW->show();
}
