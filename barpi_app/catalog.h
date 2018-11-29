#ifndef CATALOG_H
#define CATALOG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QSqlRelationalTableModel;
class QSqlError;
class QTableView;
class QString;
QT_END_NAMESPACE

class Catalog : public QWidget
{
    Q_OBJECT
public:
    explicit Catalog(QWidget *parent = 0);
    void addItem(const QString& barcode);

private slots:
    void newitem();
    void submit();
    void cancel();
    void remove();
    void setButtons(const bool st = true);
    void dataChanged(const bool st = true);


private:

    void initModel();
    QTableView* createView(const QString &title, QSqlRelationalTableModel *model);
    void createLayout();
    void showError(const QSqlError &err);

    // Relational database classes
    QSqlRelationalTableModel *prodTableModel;
    QTableView* prodTableView;

    // Widgets for Products table UI
    QPushButton *newButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *deleteButton;
};

#endif // PRODUCTWINDOW_H
