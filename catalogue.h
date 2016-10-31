#ifndef INVDATABASE_H
#define INVDATABASE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QSqlTableModel;
class QSqlRelationalTableModel;
class ProductDialog;
QT_END_NAMESPACE

class Catalogue : public QWidget
{
    Q_OBJECT

public:

    explicit Catalogue(const QString &database, QWidget *parent = 0);

    // The database exists and this call attaches
    // the database to the QSqlRelationalTableModel

    void initModels();


    // Map a view to the relational database

    QTableView *createView(const QString &title, QSqlTableModel *model);


private:

    // Database name passed in with constructor
    const QString *dbName;

    // Used to map to Products tables in the relational database
    QSqlRelationalTableModel *prodModel;

    //ProductDialog *prodDialog;

    signals:

    public slots:

};

#endif // INVDATABASE_H
