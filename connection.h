
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
    query.exec("create table category (id integer primary key, label varchar(15))");
    query.exec("create table products ("
               "upccode varchar(15) primary key,"
               "label varchar(30),"
               "abccode varchar(8) unique,"
               "volume real,"
               "type integer references category(id),"
               "density real)");
    query.exec("create table inventory ("
               "id integer primary key autoincrement,"
               "upc varchar(15) references products(upccode),"
               "barcode varchar(30) unique,"
               "retired boolean,"
               "gross real,"
               "tare  real,"
               "arrival timestamp,"
               "departure timestamp )");
    query.exec("create table readings ("
               "id integer primary key autoincrement,"
               "item varchar(30),"
               "stamp timestamp,"
               "weight real )");

    query.exec("insert into category values (1,'bourbon')");
    query.exec("insert into category values (2,'brandy')");
    query.exec("insert into category values (3,'cognac')");
    query.exec("insert into category values (4,'gin')");
    query.exec("insert into category values (5,'liqueur')");
    query.exec("insert into category values (6,'rum')");
    query.exec("insert into category values (7,'scotch')");
    query.exec("insert into category values (8,'tequila')");
    query.exec("insert into category values (9,'vodka')");
    query.exec("insert into category values (10,'whiskey')");

    query.exec("insert into products values ("
               "'0082184090008', 'Jack Daniels No. 7',"
               "'A305', 0.75, 10, 0.916)");
    query.exec("insert into products values ("
               "'0830895501098', 'Grey Goose',"
               "'E1400', 1.0, 9, 0.916)");
    query.exec("insert into products values ("
               "'0083664868780', 'Hendricks',"
               "'A723', 0.75, 4, 0.9076)");

    return true;
}
//! [0]

#endif
