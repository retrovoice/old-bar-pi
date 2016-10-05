#include "catalogue.h"

#include <QtSql>
#include <QtGui>
#include "connection.h"
#include "productdialog.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE


Catalogue::Catalogue(const QString &database, QWidget *parent) :
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

    //createConnection();

    dbName = new QString(database);

    prodModel = new QSqlRelationalTableModel;

    this->initModels();

    QTableView *prodView = this->createView(QObject::tr("Barpi Products"), prodModel);
    prodView->show();

    //prodDialog = new ProductDialog(*dbName,this);
    //prodDialog->show();
}

// The database exists and this call attaches
// the database to the QSqlRelationalTableModel

void Catalogue::initModels()
{

    prodModel->setTable("products");
    prodModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    prodModel->setRelation(4, QSqlRelation("categories", "id", "label"));
    prodModel->setHeaderData(0, Qt::Horizontal, QObject::tr("upccode"));
    prodModel->setHeaderData(1, Qt::Horizontal, QObject::tr("label"));
    prodModel->setHeaderData(2, Qt::Horizontal, QObject::tr("abccode"));
    prodModel->setHeaderData(3, Qt::Horizontal, QObject::tr("price"));
    prodModel->setHeaderData(4, Qt::Horizontal, QObject::tr("category"));
    prodModel->setHeaderData(5, Qt::Horizontal, QObject::tr("volume"));
    prodModel->setHeaderData(6, Qt::Horizontal, QObject::tr("density"));
    prodModel->select();

}

QTableView *Catalogue::createView(const QString &title, QSqlTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
}

// Display the Product table in a window
void Catalogue::showCatalog(QSqlRelationalTableModel *model)
{

}

// Display a dialog to edit catalog entries and
// add new records
void Catalogue::editCatalog()
{

}
