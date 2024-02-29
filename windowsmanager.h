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
    static WindowsManager* w;
    static WindowsManager* get();
    void start();
};

#endif // WINDOWSMANAGER_H
