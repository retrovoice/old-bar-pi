#include "invoicemanager.h"
#include "catalog.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSqlQuery>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QSqlError>
#include <QTableView>
#include <QHeaderView>
#include <QDate>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QFile>
#include <QByteArray>
#include <QDateTime>
#include <QVariant>
#include <QCalendarWidget>
#include <iostream>

InvoiceManager::InvoiceManager(QTabWidget *tabW,
                           Catalog *catalog,
                           QWidget *parent) :
    QWidget(parent)
{
    detailsModel = new QSqlRelationalTableModel;
    detailsView  = new QTableView;
    
    invoiceModel = new QSqlRelationalTableModel;
    invoiceView  = new QTableView;
    
    // These must be done in order due to initializaion
    // of access types.
    this->initModel();
    invoiceView = this->createView("Invoices", invoiceModel);
    detailsView  = this->createView("Invoice Details", detailsModel);
    
    this->createLayout();
    
    
    invoiceView->resizeColumnsToContents();
    invoiceView->setSortingEnabled(true);
    //invoiceView->show();
    detailsView->resizeColumnsToContents();
    detailsView->setSortingEnabled(true);
    detailsView->hideColumn(0);
    detailsView->hideColumn(1);
    //invoiceDetailsView->show();
}

void InvoiceManager::initModel()
{
    invoiceModel->setTable("invoice");
    invoiceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    invoiceModel->setRelation(1, QSqlRelation("vendors","id","vendorname"));
    invoiceModel->setHeaderData(0,Qt::Horizontal, QObject::tr("Invoice"));
    invoiceModel->setHeaderData(1,Qt::Horizontal, QObject::tr("Vendor"));
    invoiceModel->setHeaderData(2,Qt::Horizontal, QObject::tr("Date"));
    // Synchronize model with database
    if (invoiceModel->select())
    {
      showError(invoiceModel->lastError());
    }
    
    detailsModel->setTable("invoicedetails");
    detailsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    detailsModel->setRelation(2, QSqlRelation("products", "upccode", "label"));
    detailsModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    detailsModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Invoice"));
    detailsModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Product"));
    detailsModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Qty"));
    detailsModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Price"));
    detailsModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Discount"));
    detailsModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Net Price"));
    
    // Synchronize model with database
    if (detailsModel->select())
    {
      showError(detailsModel->lastError());
    }
}

QTableView* InvoiceManager::createView(const QString &title, QSqlRelationalTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
}


void InvoiceManager::createLayout()
{
    addInvoiceButton = new QPushButton(tr("New Invoice"),this);
    addInvoiceButton->setEnabled(true);
    saveInvoiceButton = new QPushButton(tr("Save"),this);
    saveInvoiceButton->setEnabled(false);
    cancelInvoiceButton = new QPushButton(tr("Cancel"),this);
    cancelInvoiceButton->setEnabled(false);
    deleteInvoiceButton = new QPushButton(tr("Delete"),this);
    deleteInvoiceButton->setEnabled(true);
    
    connect (addInvoiceButton, SIGNAL(clicked()), this, SLOT(newInvoice()));
    connect (saveInvoiceButton, SIGNAL(clicked()), this, SLOT(submitInvoice()));
    connect (cancelInvoiceButton, SIGNAL(clicked()),this, SLOT(cancelInvoice()));
    connect (deleteInvoiceButton, SIGNAL(clicked(bool)), this, SLOT(deleteInvoice()));
        
    addDetailButton = new QPushButton(tr("New Line"),this);
    addDetailButton->setEnabled(true);
    saveDetailsButton = new QPushButton(tr("Save"),this);
    saveDetailsButton->setEnabled(false);
    cancelDetailsButton = new QPushButton(tr("Cancel"),this);
    cancelDetailsButton->setEnabled(false);
    deleteDetailButton = new QPushButton(tr("Delete"),this);
    deleteDetailButton->setEnabled(true);
    
    connect (addDetailButton, SIGNAL(clicked()), this, SLOT(newDetail()));
    connect (saveDetailsButton, SIGNAL(clicked()), this, SLOT(submitDetails()));
    connect (cancelDetailsButton, SIGNAL(clicked()),this, SLOT(cancelDetails()));
    connect (deleteDetailButton, SIGNAL(clicked(bool)), this, SLOT(deleteDetail()));
        
    dateEditBox = new QDateTimeEdit;
    
    // Format string for how date/time will be appear in dateEditBox
    QString format = "MM/dd/yyyy";
    dateEditBox->setDisplayFormat(format);
    QDate currentDate(QDate::currentDate());
    dateEditBox->setDate(currentDate);
    dateEditBox->setCalendarPopup(true);
    QCalendarWidget* calendar = new QCalendarWidget;
    dateEditBox->setCalendarWidget(calendar);
    QLabel* dateLabel = new QLabel(tr("Date"));
    
    vendorBox = new QComboBox;
    QLabel* vendorLabel = new QLabel(tr("Vendor"));
    
    // Get the list of vendors from the database
    // to populate the vendor combobox
    QStringList vendors = this->getVendors();
    QStringList::const_iterator constItr;
    for (constItr = vendors.constBegin(); constItr != vendors.constEnd(); constItr++)
    {
      vendorBox->addItem(*constItr);
    }
    
    invoiceNumberEdit = new QLineEdit;
    QLabel* invoiceNumLabel = new QLabel(tr("Invoice No."));
    
    invoiceCost = new QLineEdit;
    QLabel* invoiceCostLabel = new QLabel(tr("Cost"));
    

    QVBoxLayout* row2_0_Layout = new QVBoxLayout;
    QVBoxLayout* row2_1_Layout = new QVBoxLayout;
    QVBoxLayout* row2_2_Layout = new QVBoxLayout;
    QVBoxLayout* row2_3_Layout = new QVBoxLayout;
    
    row2_0_Layout->addWidget(dateLabel);
    row2_0_Layout->addWidget(dateEditBox);
    row2_0_Layout->addWidget(addDetailButton);
    
    row2_1_Layout->addWidget(vendorLabel);
    row2_1_Layout->addWidget(vendorBox);
    row2_1_Layout->addWidget(saveDetailsButton);
    
    row2_2_Layout->addWidget(invoiceNumLabel);
    row2_2_Layout->addWidget(invoiceNumberEdit);
    row2_2_Layout->addWidget(cancelDetailsButton);
    
    row2_3_Layout->addWidget(invoiceCostLabel);
    row2_3_Layout->addWidget(invoiceCost);
    row2_3_Layout->addWidget(deleteDetailButton);

    QHBoxLayout* detailsHeaderLayout = new QHBoxLayout;
    detailsHeaderLayout->addLayout(row2_0_Layout);
    detailsHeaderLayout->addLayout(row2_1_Layout);
    detailsHeaderLayout->addLayout(row2_2_Layout);
    detailsHeaderLayout->addLayout(row2_3_Layout);
    
    QVBoxLayout* detailsLayout = new QVBoxLayout;
    detailsLayout->addLayout(detailsHeaderLayout);
    detailsLayout->addWidget(detailsView);
    
    QHBoxLayout* invoiceHeaderLayout = new QHBoxLayout;
    invoiceHeaderLayout->addWidget(addInvoiceButton);
    invoiceHeaderLayout->addWidget(saveInvoiceButton);
    invoiceHeaderLayout->addWidget(cancelInvoiceButton);
    invoiceHeaderLayout->addWidget(deleteInvoiceButton);

    QVBoxLayout* invoiceLayout = new QVBoxLayout;
    invoiceLayout->addLayout(invoiceHeaderLayout);
    invoiceLayout->addWidget(invoiceView);
    
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->addLayout(invoiceLayout);
    mainLayout->addLayout(detailsLayout);

    this->setLayout(mainLayout);
    
}


void InvoiceManager::newInvoice()
{
    if (!invoiceModel->insertRows(0,1))
    {
      QMessageBox::warning( this,"InvoiceManager::newInvoice", 
      "insertRows operation failed." );
      this->cancelInvoice();
      return;
    }
    invoiceView->selectRow(0);
    this->invoiceChanged();
}

void InvoiceManager::newDetail()
{
    if (!detailsModel->insertRows(0,1))
    {
      QMessageBox::warning( this,"InvoiceManager::newInvoice", 
      "insertRows operation failed." );
      this->cancelDetails();
      return;
    }
    detailsView->selectRow(0);
    this->detailChanged();
}

void InvoiceManager::cancelInvoice()
{
    invoiceModel->revertAll();
    setInvoiceButtons();
}

void InvoiceManager::cancelDetails()
{
    detailsModel->revertAll();
    setDetailsButtons();
}


void InvoiceManager::submitInvoice()
{   
    invoiceView->setSortingEnabled(false);
    // Update the mapping between the database and the
    // QDataWidgetMapper
    if (!invoiceModel->submitAll()) {
        showError(invoiceModel->lastError());
        this->cancelInvoice();
        return;
    }
    invoiceView->setSortingEnabled(true);
    setInvoiceButtons();
}

void InvoiceManager::submitDetails()
{   
    detailsView->setSortingEnabled(false);
    // Update the mapping between the database and the
    // QDataWidgetMapper
    if (!detailsModel->submitAll()) {
        showError(detailsModel->lastError());
        this->cancelInvoice();
        return;
    }
    detailsView->setSortingEnabled(true);
    setDetailsButtons();
}

void InvoiceManager::deleteInvoice()
{
    // Don't try to remove a row if none exist
    if (invoiceModel->rowCount())
    {
      // Capture the current index of the record being removed
      QModelIndex spot = invoiceView->currentIndex();

      // Remove the row and check result for error.
      if (!invoiceModel->removeRow(spot.row())) {
	  QSqlError err = invoiceModel->lastError();
	  QMessageBox::warning(this, "Error - Remove Row",
                             "Reported Error: " + err.text());
      }

      // Submit the change to the database
      this->submitInvoice();
      setInvoiceButtons();
    }
}

void InvoiceManager::deleteDetail()
{
    // Don't try to remove a row if none exist
    if (detailsModel->rowCount())
    {
      // Capture the current index of the record being removed
      QModelIndex spot = detailsView->currentIndex();

      // Remove the row and check result for error.
      if (!detailsModel->removeRow(spot.row())) {
	  QSqlError err = detailsModel->lastError();
	  QMessageBox::warning(this, "Error - Remove Row",
                             "Reported Error: " + err.text());
      }

      // Submit the change to the database
      this->submitDetails();
      setDetailsButtons();
    }
}

bool InvoiceManager::checkDB(QString barcode)
{
    bool found(false);
    QSqlQuery query("SELECT upccode FROM products;");
    QString upccode;

    while (!found) {
        if (query.next()) {
            upccode = query.value(0).toString();
            if (upccode == barcode) {
                found = true;
                //QMessageBox::information(this,"STATUS","Barcode found!");
            }
        } else {
            break;
        }
    }
    return found;
}

QString InvoiceManager::getDBField(QString barcode, QString field)
{
    QString result("");

    // The barcode is in the products table so use
    // it to lookup the name of the product.
    /// NOTE: double quotes are needed around barcode string
    ///

    QString q1("SELECT ");
    q1.append(field);
    q1.append(" FROM products WHERE upccode = \"");
    q1.append(barcode);
    q1.append("\";");
    QSqlQuery labelQuery(q1);

    if (labelQuery.first()) {
        // Query result is QVariant. Conver to QString
        // and store in result
        result.append(labelQuery.value(0).toString());
    }
    return result;
}

QString InvoiceManager::getUPC(QString label)
{
    QString result("");

    // The barcode is in the products table so use
    // it to lookup the name of the product.
    /// NOTE: double quotes are needed around label string???
    ///

    QString q1("SELECT upccode FROM products WHERE label = \"");
    q1.append(label);
    q1.append("\";");
    QSqlQuery labelQuery(q1);

    if (labelQuery.first()) {
        // Query result is QVariant. Conver to QString
        // and store in result
        result.append(labelQuery.value(0).toString());
    }
    return result;
}

QStringList InvoiceManager::getInvoiceList(const QString& vendor)
{
  QStringList invoiceList;
  return invoiceList;
}

QStringList InvoiceManager::getVendors()
{
  QStringList vendorList;
  QSqlQuery query("SELECT vendorname FROM vendors");
  while (query.next())
  {
    vendorList.append(query.value(0).toString());
  }
  return vendorList;
}

void InvoiceManager::setInvoiceButtons(const bool st)
{
    saveInvoiceButton->setEnabled(!st);
    cancelInvoiceButton->setEnabled(!st);
    addInvoiceButton->setEnabled(st);
}

void InvoiceManager::setDetailsButtons(const bool st)
{
    saveDetailsButton->setEnabled(!st);
    cancelDetailsButton->setEnabled(!st);
    addDetailButton->setEnabled(st);
}

void InvoiceManager::invoiceChanged(const bool st)
{
    cancelInvoiceButton->setEnabled(st);
    saveInvoiceButton->setEnabled(st);
}

void InvoiceManager::detailChanged(const bool st)
{
    cancelDetailsButton->setEnabled(st);
    saveDetailsButton->setEnabled(st);
}

void InvoiceManager::showError(const QSqlError& err)
{
    QMessageBox::critical(this, "Database Error",
      "Reported Error:  " + err.text());
}
