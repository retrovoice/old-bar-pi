#include <QApplication>
#include <iostream>
#include <QString>
#include <QStringList>
#include <iostream>
#include "mainwindow.h"
#include "catalogue.h"
#include "commandlineparser.h"
#include <QGridLayout>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Store the command line arguments in a list
	QStringList *args = new QStringList(QApplication::arguments());

	// Create a list to contain "valid" command line argument
	// options.  Here, -F and --file are added to the list.
	QStringList cpOptions;
	cpOptions.append("-F");
	cpOptions.append("--file");

	// Create a command line parser object, using the list
	// of options just created.
	CommandLineParser *cp = new CommandLineParser(cpOptions);

	// Now pass the command line arguments to be proccessed,
	// which will store tokens that are associated with valid
	// options.
	cp->processArgvList(*args);

	// Iterate through the list of valid options to see whether
	// any were found on the command line.
	QStringList::const_iterator itr = cpOptions.constBegin();
	QString configFile;
	while (itr != cpOptions.constEnd())
	{
		//p NOTE:  If the same option is set multiple times,
		// the last instance is used.
		if (*itr == "-F")
			configFile = cp->optionValue(*itr);
		else if (*itr == "--file")
			configFile = cp->optionValue(*itr);
		else
			configFile = "barpi.conf";
		itr++;
	}

	Catalogue myCat;

	MainWindow window;
	window.show();
	return a.exec();
}
