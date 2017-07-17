#include "skypush.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Skyweb");
    QCoreApplication::setOrganizationDomain("skyweb.nu");
    QCoreApplication::setApplicationName("Skypush");
    Skypush skypush;

    qApp->setQuitOnLastWindowClosed(false);
    return a.exec();
}
