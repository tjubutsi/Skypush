#include "skypush.h"
#include "systemtray.h"
#include <QApplication>
#include <QDebug>

#include <QHotkey>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Skypush skypush;
    SystemTray s;

    if (!skypush.registerHotkeys())
    {
        qDebug() << "registering failed";
        qApp->quit();
    }

    return a.exec();
}
