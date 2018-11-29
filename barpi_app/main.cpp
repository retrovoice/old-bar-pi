#include <iostream>
#include <QApplication>
//#include <QGuiApplication>
#include <QString>
#include <QStringList>
#include <QFont>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QFont sansFont("Sans [DejaVu]",12);

    QApplication::setFont(sansFont);

    // Default name of configuration file
    QString configFile("barpi.conf");

    MainWindow window(configFile);
	window.show();
	return a.exec();
}
