//#include "mainwindow.h"
#include "catalogue.h"
#include <QApplication>
#include <iostream>
#include <QtSql/QtSql>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    Catalogue myCat;
    QSqlDatabase myDb = QSqlDatabase::addDatabase("QPSQL", "mainBarpi");
    myDb.setHostName("localhost");
    myDb.setDatabaseName("barpi");
    myDb.setUserName("juno");
    myDb.setPassword("f4s6n5");
    bool ok = myDb.open();
    if (ok)
    {
        std::cout << "Database successfully opened in main.\n";
        myDb.close();
    }
    else
    {
        std::cout << "Database did not open.\n";
    }

    return 0; //a.exec();
}
