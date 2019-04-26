#ifndef INVOICEMANAGER_H
#define INVOICEMANAGER_H

#include <QWidget>
#include <QMap>

#include "catalog.h"

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
class QLabel;
class QLayout;
class QLineEdit;
class QComboBox;
class QPushButton;
class QDateEdit;
class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class QSqlRelationalTableModel;
class QSqlError;
class QTableView;
QT_END_NAMESPACE

class InvoiceManager : public QWidget
{
    Q_OBJECT
public:
    explicit InvoiceManager(QTabWidget* tabW,
                          Catalog* catalog,
                          QWidget *parent = 0);

signals:

public slots:

private slots:
    void newInvoice();
    void submitInvoice();
    void cancelInvoice();
    void deleteInvoice();
    void newDetail();
    void submitDetails();
    void cancelDetails();
    void deleteDetail();
    void invoiceChanged(const bool st = true);
    void detailChanged(const bool st = true);

private:

    void initModel();
    void createLayout();
    QTableView* createView(const QString &title, QSqlRelationalTableModel *model);
    QStringList getVendors();
    QStringList getInvoiceList(const QString& vendor);
    void setInvoiceButtons(const bool st = true);
    void setDetailsButtons(const bool st = true);
    void showError(const QSqlError &err);

    QPushButton*   addInvoiceButton;
    QPushButton*   saveInvoiceButton;
    QPushButton*   cancelInvoiceButton;
    QPushButton*   deleteInvoiceButton;
    QPushButton*   addDetailButton;
    QPushButton*   saveDetailsButton;
    QPushButton*   cancelDetailsButton;
    QPushButton*   deleteDetailButton;
    QDateEdit*     dateEditBox;
    QComboBox*     vendorBox;
    QLineEdit*     invoiceNumberEdit;
    QLineEdit*     invoiceCost;
    QSqlRelationalTableModel* invoiceModel;
    QSqlRelationalTableModel* detailsModel;
    QTableView*               invoiceView;
    QTableView*               detailsView;
    QHeaderView*   invoiceDetailsHeader;
    QMap<QString,QTableWidgetItem*> itemMap;
    
    bool checkDB(QString barcode);
    QString getDBField(QString barcode, QString field);
    QString getUPC(QString label);

};

#endif // INVOICEMANAGER_H
