#include <QApplication>
#include "WorkTable.h"
#include "Dialog.h"
#include "OpenCVWindow.h"

WorkTable *table;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	int projectorWidth = 1024, projectorHeight = 768;


	table = new WorkTable(projectorWidth, projectorHeight);
	table->displayMainMenu();
	// table->drawGUI();
	table->show();


//	OpenCVWindow w;
//	w.show();

//	Dialog trackWindow;
//	trackWindow.show();

	return a.exec();
}
