#include "catalogue.h"

#include <QMessageBox>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>

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

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(*dbName);

    if (!db.open())
    {
        QString error("Database open failed.");
        error.append(db.lastError().text());
        QMessageBox::critical(this, tr("Error"),
                              error, QMessageBox::Cancel);
        return;
    }
    else
    {
        QMessageBox::information(this, tr("Success!"),tr("Database successfully opened"));
    }

    prodModel = new QSqlRelationalTableModel;

    this->initModels();

    QTableView *prodView = this->createView(QObject::tr("Barpi Products"), prodModel);
    prodView->show();
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
