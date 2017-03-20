#ifndef CATALOG_H
#define CATALOG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QSqlRelationalTableModel;
class QSqlError;
class QTableView;
QT_END_NAMESPACE

class Catalog : public QWidget
{
    Q_OBJECT
public:
    explicit Catalog(QWidget *parent = 0);

signals:

private slots:
    void newitem();
    void submit();
    void cancel();
    void remove();
    void updateButtons(const bool st = TRUE);

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
