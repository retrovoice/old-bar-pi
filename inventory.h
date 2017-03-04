#ifndef INVENTORY_H
#define INVENTORY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QCheckBox;
class QPushButton;
class QDataWidgetMapper;
class QSqlRelationalTableModel;
class QSqlError;
class QTableView;
class QSqlTableModel;
class QGridLayout;
QT_END_NAMESPACE

class Inventory : public QWidget
{
    Q_OBJECT
public:
    explicit Inventory(QWidget *parent = 0);
signals:

private slots:
//    void additem();
//    void submit();
//    void cancel();
//    void previous();
//    void next();
//    void retire();

private:

    void initModel();
    QTableView* createView(const QString &title, QSqlTableModel *model);
    void showError(const QSqlError &err);
//    double readScale();

    // Relational table model used to access inventory database
    QSqlRelationalTableModel *invTableModel;
    QTableView* invTableView;

    // Layout to contain table view
    QGridLayout* invLayout;

    // Widgets for Products table UI
//    QLabel *retiredLabel;
//    QCheckBox *Edit;
//    QDataWidgetMapper *mapper;

    // Flag to determine is adding record to database
//    bool isNew;
    // An index used to keep track of current record in the database
//    int spot;
};

#endif // INVENTORY_H
