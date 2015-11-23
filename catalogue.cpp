#include "catalogue.h"

#include <iostream>
#include <QtSql/QtSql>
#include <QString>

Catalogue::Catalogue(QObject *parent) :
    QObject(parent)
{
    myDb = new QSqlDatabase;
    myDb->addDatabase("QPSQL", "conBarpi");
    myDb->setHostName("192.168.1.108");
    myDb->setDatabaseName("barpi");
    myDb->setUserName("juno");
    myDb->setPassword("f4s6n5");
    bool ok = myDb->open();
    if (ok)
    {
        std::cout << "Database successfully opened.\n";
    }
    else
    {
        std::cout << "Database did not open.\n";
    }
}

// Initialize an empty database.  The database
// should not already exist, and if it does, this
// function should return a non-zero value.

int Catalogue::initDatabase(QString url,
                 QString username,
                 QString password)
{
    return 0;
}

// Used to add a new product to the database.
// If the product already exists, this function
// should return a non-zero value.

int Catalogue::addProduct(productPtr product)
{
    return 0;
}

// Used to add a product item to the inventory.
// If the product is not yet in the database, this
// function should return a non-zero value.  The
// GUI should use the return value to prompt the user
// to add the item to the product catalogue.

int Catalogue::addToInventory(inventoryPtr item)
{
    return 0;
}

// Used to record the weight data of an inventory item
// with a date/time stamp.  A non-zero return value indicates
// an error.
/** TODO: define return values for recordReading */

int Catalogue::recordReading(readingPtr data)
{
    return 0;
}

// Used to alter the information on an existing product
// in the catalogue.  Use a findProduct method to return
// a pointer to the desired catalogue item.

int Catalogue::updateProduct(productPtr product)
{
    return 0;
}

// A set of overloaded methods for retrieving an instance
// of a product based on different data

productPtr Catalogue::findProduct(quint32 ID)
{
    return 0;
}

productPtr Catalogue::findProduct(QString field) // Label, ABC Code, or Barcode
{
    return 0;
}
