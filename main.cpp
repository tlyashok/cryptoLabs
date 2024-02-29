#include <QApplication>
#include "windowsmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowsManager::get()->start();
    return a.exec();
}
