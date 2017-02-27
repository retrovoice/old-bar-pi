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
    void previous();
    void next();
    void retire();

private:

    void initModel();
    void mapModel();
    void createLayout();
    void showError(const QSqlError &err);
    double readScale();

    // Relational table model used to display product catalog
    QSqlRelationalTableModel *invTableModel;

    // Widgets for Products table UI
    QLabel *retiredLabel;
    QCheckBox *Edit;
    QDataWidgetMapper *mapper;

    // Flag to determine is adding record to database
    bool isNew;
    // An index used to keep track of current record in the database
    int spot;
};

#endif // INVENTORY_H
