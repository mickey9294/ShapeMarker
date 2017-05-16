#include "ShapeMarker.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ShapeMarker w;
	w.show();
	return a.exec();
}
