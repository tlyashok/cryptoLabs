#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include "authwindow.h"
#include "mainwindow.h"
#include "addrecord.h"


class WindowsManager
{
public:
    WindowsManager();
    AuthWindow *AuthW;
    MainWindow *MainW;
    AddRecord *AddW;
    static WindowsManager* w;
    static WindowsManager* get();
    void start();
};

#endif // WINDOWSMANAGER_H
