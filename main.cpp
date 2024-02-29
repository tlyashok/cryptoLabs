#include <QApplication>
#include "windowsmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowsManager *w = new WindowsManager();
    w->start();
    return a.exec();
}
