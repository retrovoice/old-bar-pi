#ifndef INVDATABASE_H
#define INVDATABASE_H

#include <QObject>
#include <QtSql/QtSql>
#include "appStructs.h"

class QString;
class QTableView;

class Catalogue : public QObject
{
    Q_OBJECT

public:

    explicit Catalogue(QObject *parent = 0);

    // The database exists and this call attaches
    // the database to the QSqlRelationalTableModel

    void initModel();


    // Map a view to the relational database

    QTableView *createView(const QString &title, QSqlTableModel *model);

    // Used to add a new product to the database.
    // If the product already exists, this function
    // should return a non-zero value.

    int addProduct(productPtr product);

    // Used to add a product item to the inventory.
    // If the product is not yet in the database, this
    // function should return a non-zero value.  The
    // GUI should use the return value to prompt the user
    // to add the item to the product catalogue.

    int addToInventory(inventoryPtr item);

    // Used to record the weight data of an inventory item
    // with a date/time stamp.  A non-zero return value indicates
    // an error.
    /** TODO: define return values for recordReading */

    int recordReading(readingPtr data);

    // Used to alter the information on an existing product
    // in the catalogue.  Use a findProduct method to return
    // a pointer to the desired catalogue item.

    int updateProduct(productPtr product);

    // A set of overloaded methods for retrieving an instance
    // of a product based on different data

    productPtr findProduct(quint32 ID);
    productPtr findProduct(QString field); // Label, ABC Code, or Barcode

private:

    QSqlRelationalTableModel *dbModel;

    signals:

    public slots:

};

#endif // INVDATABASE_H
