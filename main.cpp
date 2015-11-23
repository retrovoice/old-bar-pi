#include "mainwindow.h"
#include "catalogue.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    Catalogue* myCat = new Catalogue();

    return 0; //a.exec();
}
