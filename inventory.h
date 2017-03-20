#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QSqlRelationalTableModel;
class QSqlError;
class QTableView;
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
    void enableButtons(const bool st = true);

private:

    void initModel();
    QTableView* createView(const QString &title, QSqlRelationalTableModel *model);
    void createLayout();
    QString createBarcode(int id);
    void showError(const QSqlError &err);
//    double readScale();

    // Relational database classes
    QSqlRelationalTableModel *invTableModel;
    QTableView* invTableView;

    // Widgets for Products table UI
    QPushButton* addButton;
    QPushButton* saveButton;
    QPushButton* cancelButton;
};

#endif // INVENTORY_H
