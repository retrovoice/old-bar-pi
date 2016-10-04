#include "catalogue.h"

#include <iostream>
#include <QTableView>
#include <QtGui>
#include "connection.h"

Catalogue::Catalogue(QWidget *parent) :
    QWidget(parent)
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

    categoryModel = new QSqlRelationalTableModel;
    prodModel = new QSqlRelationalTableModel;
    invModel = new QSqlRelationalTableModel;
    readModel = new QSqlRelationalTableModel;

    this->initModels();

    QTableView *categoryView = createView(QObject::tr("Barpi Categories"), categoryModel);
    categoryView->show();
    QTableView *prodView = this->createView(QObject::tr("Barpi Products"), prodModel);
    prodView->show();
    QTableView *invView = this->createView(QObject::tr("Barpi Inventory"), invModel);
    invView->show();
    QTableView *readView = this->createView(QObject::tr("Barpi Readings"), readModel);
    readView->show();

    prodDialog = new ProductWindow(prodModel,this);
    prodDialog->show();
}

// The database exists and this call attaches
// the database to the QSqlRelationalTableModel

void Catalogue::initModels()
{
    categoryModel->setTable("categories");
    categoryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    categoryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    categoryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("label"));
    categoryModel->select();

    prodModel->setTable("products");
    prodModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    prodModel->setRelation(3, QSqlRelation("categories", "id", "label"));
    prodModel->setHeaderData(0, Qt::Horizontal, QObject::tr("upccode"));
    prodModel->setHeaderData(1, Qt::Horizontal, QObject::tr("label"));
    prodModel->setHeaderData(2, Qt::Horizontal, QObject::tr("abccode"));
    prodModel->setHeaderData(3, Qt::Horizontal, QObject::tr("category"));
    prodModel->setHeaderData(4, Qt::Horizontal, QObject::tr("volume"));
    prodModel->setHeaderData(5, Qt::Horizontal, QObject::tr("density"));
    prodModel->select();

    invModel->setTable("inventory");
    invModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    invModel->setRelation(1, QSqlRelation("products", "upccode", "label"));
    invModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    invModel->setHeaderData(1, Qt::Horizontal, QObject::tr("upc"));
    invModel->setHeaderData(2, Qt::Horizontal, QObject::tr("barcode"));
    invModel->setHeaderData(3, Qt::Horizontal, QObject::tr("retired"));
    invModel->setHeaderData(4, Qt::Horizontal, QObject::tr("gross"));
    invModel->setHeaderData(5, Qt::Horizontal, QObject::tr("tare"));
    invModel->setHeaderData(6, Qt::Horizontal, QObject::tr("arrival"));
    invModel->setHeaderData(7, Qt::Horizontal, QObject::tr("departure"));
    invModel->select();

    readModel->setTable("readings");
    readModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    readModel->setHeaderData(0, Qt::Horizontal, QObject::tr("id"));
    readModel->setHeaderData(1, Qt::Horizontal, QObject::tr("item"));
    readModel->setHeaderData(2, Qt::Horizontal, QObject::tr("stamp"));
    readModel->setHeaderData(3, Qt::Horizontal, QObject::tr("weight"));
    readModel->select();
}

QTableView *Catalogue::createView(const QString &title, QSqlTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
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
