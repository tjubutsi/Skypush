#include "skypush.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Skypush skypush;

    if (!skypush.registerHotkeys())
    {
        qDebug() << "registering failed";
        qApp->quit();
    }

    return a.exec();
}
