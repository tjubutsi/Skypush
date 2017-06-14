#include "trayhandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TrayHandler t;

    return a.exec();
}
