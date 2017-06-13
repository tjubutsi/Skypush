#include "Skypush.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Skypush w;
	w.show();
	return a.exec();
}
