#include "JAProjekt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JAProjekt w;
    w.show();
    return a.exec();
	
}
