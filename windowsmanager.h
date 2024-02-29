#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include "authwindow.h"
#include "mainwindow.h"


class WindowsManager
{
public:
    WindowsManager();
    AuthWindow *AuthW;
    MainWindow *MainW;

    void start();
};

#endif // WINDOWSMANAGER_H
