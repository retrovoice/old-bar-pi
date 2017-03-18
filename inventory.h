#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QCheckBox;
class QPushButton;
class QSqlRelationalTableModel;
class QSqlError;
class QTableView;
class QLineEdit;
QT_END_NAMESPACE

class Inventory : public QWidget
{
    Q_OBJECT
public:
    explicit Inventory(QWidget *parent = 0);
signals:

private slots:
    void additem();
    void submit();
    void cancel();
    void retire();
    void enableButtons(const bool st = true);

private:

    void initModel();
    QTableView* createView(const QString &title, QSqlRelationalTableModel *model);
    void createLayout();
    void showError(const QSqlError &err);
//    double readScale();

    // Relational database classes
    QSqlRelationalTableModel *invTableModel;
    QTableView* invTableView;

    // Widgets for Products table UI
    QCheckBox* retiredCB;
    QPushButton* addButton;
    QPushButton* saveButton;

    // Widgets for adding item to inventory
    QLineEdit* upcEdit;
    QLineEdit* nameEdit;
    QLineEdit* barcodeEdit;
    QLineEdit* grossEdit;
    QLineEdit* tareEdit;


    // Flag to determine is adding record to database
    bool isNew;
    // An index used to keep track of current record in the database
    int spot;
};

#endif // INVENTORY_H
