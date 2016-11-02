#ifndef INVDATABASE_H
#define INVDATABASE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QSqlTableModel;
class QSqlRelationalTableModel;
class ProductDialog;
class QSqlError;
QT_END_NAMESPACE

class Catalogue : public QWidget
{
    Q_OBJECT

public:

    explicit Catalogue(QWidget *parent = 0);

private:

    void initModels();

    void showError(const QSqlError &err);

    // Map a view to the relational database
    QTableView *createView(const QString &title, QSqlTableModel *model);

    // Used to map to Products tables in the relational database
    QSqlRelationalTableModel *prodModel;

    signals:

    public slots:

};

#endif // INVDATABASE_H