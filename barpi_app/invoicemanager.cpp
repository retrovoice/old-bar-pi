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
    invoiceDetailsTable = new QTableWidget(0, 5, this);
    QStringList labels;
    labels << "Item" << "Qty" << "Price" << "Description" << "Net";
    invoiceDetailsTable->setHorizontalHeaderLabels(labels);
    invoiceDetailsTable->setMinimumWidth(480);
    invoiceDetailsTable->setSortingEnabled(true);
    invoiceDetailsHeader = invoiceDetailsTable->horizontalHeader();
    this->createLayout();
}

void InvoiceManager::createLayout()
{
    addInvoiceButton = new QPushButton(tr("New Invoice"),this);
    commitButton = new QPushButton(tr("Commit"),this);
    recallButton = new QPushButton(tr("Recall"),this);
    cancelButton = new QPushButton(tr("Cancel"),this);
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
    vendorLabel->setBuddy(vendorBox);
    
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
    invoiceNumLabel->setBuddy(invoiceNumberEdit);
    
    invoiceCost = new QLineEdit;
    QLabel* invoiceCostLabel = new QLabel(tr("Cost"));
    invoiceCostLabel->setBuddy(invoiceCost);
    
    addInvoiceButton->setEnabled(true);
    commitButton->setEnabled(false);

    QVBoxLayout* row2_0_Layout = new QVBoxLayout;
    QVBoxLayout* row2_1_Layout = new QVBoxLayout;
    QVBoxLayout* row2_2_Layout = new QVBoxLayout;
    QVBoxLayout* row2_3_Layout = new QVBoxLayout;
    
    row2_0_Layout->addWidget(addInvoiceButton);
    row2_0_Layout->addWidget(dateLabel);
    row2_0_Layout->addWidget(dateEditBox);
    
    row2_1_Layout->addWidget(commitButton);
    row2_1_Layout->addWidget(vendorLabel);
    row2_1_Layout->addWidget(vendorBox);
    
    row2_2_Layout->addWidget(recallButton);
    row2_2_Layout->addWidget(invoiceNumLabel);
    row2_2_Layout->addWidget(invoiceNumberEdit);
    
    row2_3_Layout->addWidget(cancelButton);
    row2_3_Layout->addWidget(invoiceCostLabel);
    row2_3_Layout->addWidget(invoiceCost);

    QHBoxLayout* headerLayout = new QHBoxLayout;
    headerLayout->addLayout(row2_0_Layout);
    headerLayout->addLayout(row2_1_Layout);
    headerLayout->addLayout(row2_2_Layout);
    headerLayout->addLayout(row2_3_Layout);
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(invoiceDetailsTable);

    this->setLayout(mainLayout);
    
}


void InvoiceManager::newinvoice()
{
    invoiceNumberEdit->setFocus();
    invoiceNumberEdit->clear();
    invoiceNumberEdit->setReadOnly(false);
}

void InvoiceManager::reset()
{
    invoiceDetailsTable->setSortingEnabled(false);
    int r = invoiceDetailsTable->rowCount();
    for (r--; r >= 0; r--) {
        invoiceDetailsTable->removeRow(r);
    }
    invoiceNumberEdit->setText("---");
    invoiceNumberEdit->setReadOnly(true);
    itemMap.clear();
    invoiceDetailsTable->setSortingEnabled(true);
}

void InvoiceManager::enterinvoice()
{
    invoiceDetailsTable->setSortingEnabled(false);

    QStringList* itemList = new QStringList;
    QString tempStr;
    QString tableStr;
    QString filename;
    QTableWidgetItem* count     = new QTableWidgetItem;
    QTableWidgetItem* vendor    = new QTableWidgetItem;
    QTableWidgetItem* product   = new QTableWidgetItem;
    QTableWidgetItem* menuorder = new QTableWidgetItem;
    QTableWidgetItem* category  = new QTableWidgetItem;
    QTableWidgetItem* price     = new QTableWidgetItem;
    QTableWidgetItem* upc      = new QTableWidgetItem;

    // Label for file based on operation selected
        itemList->append("Stock Received");
        filename.append("Received_");
	tableStr.append(" invoicedetails ");

    // Date/Time stamp for file
    QDateTime currentDateTime(QDate::currentDate(),QTime::currentTime());

    // Integer value of date/time used for database record.
    QVariant datetimeInt = currentDateTime.toTime_t();
    
    // Create a query used to write records to the database.
    QSqlQuery query;
    QString querytext;
    // Set query command string for appropriate table
    // Will later append lines for each record
    querytext.append("INSERT INTO" + tableStr + "VALUES ");


    // Format string for how date/time will be written for the file
    QString format = "MM/dd/yyyy,hh:mm:ss";
    itemList->append(currentDateTime.toString(format));

    // Format string for how date/time will be written for the filename
    format.clear();
    format.append("yyyy-MM-dd_hh-mm-ss");
    filename.append(currentDateTime.toString(format));
    filename.append(".csv");

    // Number of rows of data
    int r = invoiceDetailsTable->rowCount();

    // Add column headings
    //itemList->append("Count,Vendor,Description,Index,Category,Price,Value");
    itemList->append("Count,Vendor,Description,Menu Order,Price,Value");

    float invTotal = 0.;
    // Loop through table, adding data to list
    for (int i = 0; i < r; i++) {
        count    = invoiceDetailsTable->item(i,0);
        vendor     = invoiceDetailsTable->item(i,1);
        product  = invoiceDetailsTable->item(i,2);
        menuorder   = invoiceDetailsTable->item(i,3);
        //category = tallyTable->item(i,4);
        price = invoiceDetailsTable->item(i,5);
	float stockvalue = count->text().toFloat() * price->text().toFloat();
	invTotal += stockvalue;
	QString valuetext;
	valuetext.setNum(stockvalue);
	upc  = invoiceDetailsTable->item(i,6);
	
	// build the string that will be written to the CSV file
        tempStr.append(count->text());
        tempStr.append(",");
        tempStr.append(vendor->text());
        tempStr.append(",");
        tempStr.append(product->text());
        tempStr.append(",");
        tempStr.append(menuorder->text());
        tempStr.append(",");
        //tempStr.append(category->text());
        //tempStr.append(",");
        tempStr.append(price->text());
        tempStr.append(",");
	tempStr.append(valuetext);
        //tempStr.append(",");
        //tempStr.append(upc->text());
        itemList->append(tempStr);
        tempStr.clear();
	
	// Build line of query for the current record
	querytext.append("(" + datetimeInt.toString() + ",'" + upc->text() + "'," + count->text() + "),");
    }
    // The trailing comma of the querystring needs to be removed
    int stLength = querytext.length();
    querytext.resize(stLength - 1);
    std::cout << "StockManager::finish - SQL String is:\n";
    std::cout << "\t" << querytext.toStdString() << std::endl; 
    // Execute the query
    query.exec(querytext);

    QString fivecommas(",,,,,");
    QString valueTotal;
    valueTotal.setNum(invTotal);
    itemList->append(fivecommas + valueTotal);
    QFile results(filename);
    if (!results.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             "File Write Error",
                             "Could not open file to write results");
    } else {
        QStringList::ConstIterator constItr;
        QByteArray aline;
        for (constItr = itemList->constBegin();
             constItr != itemList->constEnd();
             constItr++)
        {
            aline.append(*constItr);
            aline.append("\n");
            results.write(aline);
            aline.clear();
        }
    }
    this->reset();
    invoiceDetailsTable->setSortingEnabled(true);
}

void InvoiceManager::refocus()
{
    invoiceNumberEdit->setFocus();
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

void InvoiceManager::recallInvoice()
{

}
