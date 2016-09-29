
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/
//! [0]
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;
    query.exec("create type category as enum ("
               "'bourbon','brandy','cognac','gin',"
               "'liqueur','rum','scotch','tequila',"
               "'vodka','whiskey')");
    query.exec("create table products ("
               "upccode varchar(15) primary key,label varchar(30),"
               "abccode varchar(8) unique,volume real,type category,"
               "density real)");
    query.exec("create table inventory ("
               "id serial primary key,"
               "upc varchar(15) references products(upccode),"
               "barcode varchar(30) unique,"
               "retired boolean,"
               "gross real,"
               "tare  real,"
               "arrival timestamp,"
               "departure timestamp )");
    query.exec("create table readings ("
               "id serial primary key,"
               "item integer references inventory(id),"
               "stamp timestamp,"
               "weight real )");

    query.exec("insert into products values("
               "'0082184090008', 'Jack Daniels No. 7',"
               "'A305', '0.75', 'whiskey', '0.916')");
    query.exec("insert into products values("
               "'0830895501098', 'Grey Goose',"
               "'E1400', '1.0', 'vodka', '0.916')");
    query.exec("insert into products values("
               "'0083664868780', 'Hendricks',"
               "'A723', '0.75', 'gin', '0.9076')");

    return true;
}
//! [0]

#endif
