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
    labels << "Item" << "Qty" << "Price" << "Disc" << "Net";
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
    dateEditBox = new QDateTimeEdit;
    
    // Format string for how date/time will be appear in dateEditBox
    QString format = "MM/dd/yyyy";
    dateEditBox->setDisplayFormat(format);
    QDate currentDate(QDate::currentDate());
    dateEditBox->setDate(currentDate);
    dateEditBox->setCalendarPopup(true);
    QCalendarWidget* calendar = new QCalendarWidget;
    dateEditBox->setCalendarWidget(calendar);
    
    vendorBox = new QComboBox;
    vendorLabel = new QLabel(tr("Vendor"));
    vendorLabel->setBuddy(vendorBox);
    
    invoiceNumberEdit = new QLineEdit;
    invoiceNumLabel = new QLabel(tr("Invoice No."));
    invoiceNumLabel->setBuddy(invoiceNumberEdit);
    
    invoiceCost = new QLineEdit;
    invoiceCostLabel = new QLabel(tr("Cost"));
    invoiceCostLabel->setBuddy(invoiceCost);
    
    addInvoiceButton->setEnabled(true);
    commitButton->setEnabled(false);

    QLabel* stockLabel = new QLabel(tr("Invoice Management"),this);
    stockLabel->setAlignment(Qt::AlignHCenter);

    QHBoxLayout* buttonRowOneLayout = new QHBoxLayout;
    buttonRowOneLayout->addWidget(addInvoiceButton);
    buttonRowOneLayout->addWidget(commitButton);
    buttonRowOneLayout->addWidget(recallButton);
    
    QHBoxLayout* rowTwoLabelLayout = new QHBoxLayout;
    rowTwoLabelLayout->addWidget(vendorLabel);
    rowTwoLabelLayout->addWidget(invoiceNumLabel);
    rowTwoLabelLayout->addWidget(invoiceCostLabel);
    
    QHBoxLayout* buttonRowTwoLayout = new QHBoxLayout;
    buttonRowTwoLayout->addWidget(dateEditBox);
    buttonRowTwoLayout->addWidget(vendorBox);
    buttonRowTwoLayout->addWidget(invoiceNumberEdit);
    buttonRowTwoLayout->addWidget(invoiceCost);
    

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stockLabel);
    mainLayout->addLayout(buttonRowOneLayout);
    mainLayout->addLayout(rowTwoLabelLayout);
    mainLayout->addLayout(buttonRowTwoLayout);
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
