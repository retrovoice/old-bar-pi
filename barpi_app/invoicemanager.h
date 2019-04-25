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
class QDateTimeEdit;
class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
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
    void newinvoice();
    void reset();
    void enterinvoice();
    void refocus();

private:

    void createLayout();
    QStringList getVendors();
    QStringList getInvoiceList(const QString& vendor);
    void recallInvoice();

    QPushButton*   addInvoiceButton;
    QPushButton*   commitButton;
    QPushButton*   recallButton;
    QPushButton*   cancelButton;
    QDateTimeEdit* dateEditBox;
    QComboBox*     vendorBox;
    QLineEdit*     invoiceNumberEdit;
    QLineEdit*     invoiceCost;
    QTableWidget*  invoiceDetailsTable;
    QHeaderView*   invoiceDetailsHeader;
    QMap<QString,QTableWidgetItem*> itemMap;
    
    bool checkDB(QString barcode);
    QString getDBField(QString barcode, QString field);
    QString getUPC(QString label);

};

#endif // INVOICEMANAGER_H