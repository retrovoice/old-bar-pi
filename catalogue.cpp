#include "catalogue.h"

#include <iostream>
#include <QtSql/QtSql>
#include <QString>
#include <QTableView>
#include "connection.h"

Catalogue::Catalogue(QObject *parent) :
    QObject(parent)
{
    /* myDb = QSqlDatabase::addDatabase("QSQLITE", "conBarpi");
    //myDb.setHostName("localhost");
    myDb.setDatabaseName("barpi");
    //myDb.setUserName("juno");
    //myDb.setPassword("f4s6n5");
    bool ok = myDb.open();
    if (ok)
    {
        std::cout << "Database successfully opened from catalogue.\n";
        myDb.close();
    }
    else
    {
        std::cout << "Database did not open.\n";
    } */

    createConnection();
    dbModel = new QSqlRelationalTableModel;
    this->initModel();
}

// Initialize an empty database.  The database
// should not already exist, and if it does, this
// function should return a non-zero value.

void Catalogue::initModel()
{
    dbModel->setTable("products");
    dbModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dbModel->setHeaderData(0, Qt::Horizontal, QObject::tr("upccode"));
    dbModel->setHeaderData(1, Qt::Horizontal, QObject::tr("label"));
    dbModel->setHeaderData(2, Qt::Horizontal, QObject::tr("abccode"));
    dbModel->setHeaderData(3, Qt::Horizontal, QObject::tr("volume"));
    dbModel->setHeaderData(4, Qt::Horizontal, QObject::tr("type"));
    dbModel->setHeaderData(5, Qt::Horizontal, QObject::tr("density"));

    dbModel->setTable("inventory");
    dbModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dbModel->setRelation(1, QSqlRelation("products", "upccode", "upc"));
    dbModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    dbModel->setHeaderData(1, Qt::Horizontal, QObject::tr("upc"));
    dbModel->setHeaderData(2, Qt::Horizontal, QObject::tr("barcode"));
    dbModel->setHeaderData(3, Qt::Horizontal, QObject::tr("retired"));
    dbModel->setHeaderData(4, Qt::Horizontal, QObject::tr("gross"));
    dbModel->setHeaderData(5, Qt::Horizontal, QObject::tr("tare"));
    dbModel->setHeaderData(6, Qt::Horizontal, QObject::tr("arrival"));
    dbModel->setHeaderData(7, Qt::Horizontal, QObject::tr("departure"));

    dbModel->setTable("readings");
    dbModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dbModel->setRelation(1, QSqlRelation("inventory", "id", "item"));
    dbModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    dbModel->setHeaderData(1, Qt::Horizontal, QObject::tr("item"));
    dbModel->setHeaderData(2, Qt::Horizontal, QObject::tr("stamp"));
    dbModel->setHeaderData(3, Qt::Horizontal, QObject::tr("weight"));

    dbModel->select();
}

QTableView *Catalogue::createView(const QString &title, QSqlTableModel *model)
{
    QTableView *view = new QTableView;
    //view->
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
