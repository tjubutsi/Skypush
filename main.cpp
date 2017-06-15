#include "trayhandler.h"
#include "helpers.h"
#include <QApplication>
#include <QDebug>

#include <QHotkey>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrayHandler t;
    Helpers h;

    if (!h.registerHotkeys())
    {
        qDebug() << "registering failed";
        qApp->quit();
    }

    return a.exec();
}
