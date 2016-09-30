#ifndef INVDATABASE_H
#define INVDATABASE_H

#include <QWidget>
#include <QtSql>
#include "appStructs.h"

QT_BEGIN_NAMESPACE
class QString;
class QTableView;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
QT_END_NAMESPACE

class Catalogue : public QObject
{
    Q_OBJECT

public:

    explicit Catalogue(QObject *parent = 0);

    // The database exists and this call attaches
    // the database to the QSqlRelationalTableModel

    void initModels();


    // Map a view to the relational database

    QTableView *createView(const QString &title, QSqlTableModel *model);

    // Creates the UI window for the Products table
    void makeProductUI();

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

    // These are the models that map to the Products, Inventory,
    // and Readings tables in the relational database
    QSqlRelationalTableModel *prodModel;
    QSqlRelationalTableModel *invModel;
    QSqlRelationalTableModel *readModel;
    QSqlRelationalTableModel *categoryModel;

    // Widgets for Products table UI
    QLabel *p_upcLabel;
    QLabel *p_nameLael;
    QLabel *p_abcCodeLabel;
    QLabel *p_volumeLabel;
    QLabel *p_categoryLabel;
    QLabel *p_densityLabel;
    QLineEdit *p_upcEdit;
    QLineEdit *p_nameEdit;
    QLineEdit *p_abcCodeEdit;
    QLineEdit *p_volumeEdit;
    QLineEdit *p_densityEdit;
    QComboBox *p_categoryCombo;
    QPushButton *p_newButton;
    QPushButton *p_saveButton;
    QPushButton *p_cancelButton;
    signals:

    public slots:

};

#endif // INVDATABASE_H
