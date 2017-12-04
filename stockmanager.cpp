#include "stockmanager.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QButtonGroup>
#include <QLCDNumber>
#include <QLineEdit>
#include <QSqlQuery>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QFile>
#include <QByteArray>
#include "catalog.h"

StockManager::StockManager(QTabWidget *tabW,
                           Catalog *catalog,
                           QWidget *parent) :
    QWidget(parent),
    scanCount(0)
{
    pCat = catalog;
    tW = tabW;
    tallyTable = new QTableWidget(0, 5, this);
    QStringList labels;
    labels << "Count" << "Code" << "Item" << "Category" << "Volume";
    tallyTable->setHorizontalHeaderLabels(labels);
    tallyTable->setMinimumWidth(480);
    this->createLayout();
}

void StockManager::createLayout()
{
    QPushButton* addtostockButton = new QPushButton(tr("Add to Stock"),this);
    addtostockButton->setCheckable(true);
    addtostockButton->setChecked(true);

    QPushButton* countstockButton = new QPushButton(tr("Count Stock"),this);
    countstockButton->setCheckable(true);

    QLabel* stockLabel = new QLabel(tr("Manage Stock"),this);
    stockLabel->setAlignment(Qt::AlignHCenter);

    actionGroup = new QButtonGroup;
    actionGroup->addButton(addtostockButton);
    actionGroup->setId(addtostockButton,0);
    actionGroup->addButton(countstockButton);
    actionGroup->setId(countstockButton,1);

    QLabel* directions = new QLabel(tr("Select operation above "
                                       "then click:\n\"Start\" "
                                       "to begin "
                                       "scanning barcodes.\n"
                                       "\"Finish\" when done.\n"
                                       "\"Cancel\" to abort.\n"),this);
    directions->setWordWrap(true);

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(stockLabel);
    buttonLayout->addWidget(addtostockButton);
    buttonLayout->addWidget(countstockButton);
    buttonLayout->addWidget(directions);
    buttonLayout->addSpacing(3);

    QGridLayout* smLayout = new QGridLayout;
    smLayout->addLayout(buttonLayout, 0, 0);
    smLayout->addLayout(this->createScanLayout(), 1, 0);
    smLayout->addWidget(tallyTable, 0, 1, 2, 1);
    smLayout->setColumnStretch(1,3);
    this->setLayout(smLayout);
}

QLayout *StockManager::createScanLayout()
{
    QLabel* lcdLabel = new QLabel("Scan Counter\n(counts successful scans)");
    lcdLabel->setAlignment(Qt::AlignCenter);
    scanCounter = new QLCDNumber;
    QLabel* bcLabel = new QLabel("Barcode");
    bcLabel->setAlignment(Qt::AlignCenter);
    scanValue   = new QLineEdit("---");
    scanValue->setAlignment(Qt::AlignHCenter);
    scanValue->setReadOnly(true);

    connect (scanValue, SIGNAL(returnPressed()), this, SLOT(grabBarcode()));

    QPushButton* startButton  = new QPushButton(tr("Start"));
    QPushButton* finishButton = new QPushButton(tr("Finish"));
    QPushButton* clearButton = new QPushButton(tr("Clear"));
    QPushButton* decrementButton = new QPushButton(tr("Decrement"));

    connect (startButton,  SIGNAL(clicked(bool)), this, SLOT(startscanning()));
    connect (finishButton, SIGNAL(clicked(bool)), this, SLOT(finish()));
    connect (clearButton, SIGNAL(clicked(bool)), this, SLOT(cleartable()));
    connect (decrementButton, SIGNAL(clicked(bool)), this, SLOT(decrement()));

    QVBoxLayout* scanLayout = new QVBoxLayout;

    scanLayout->addWidget(lcdLabel);
    scanLayout->addWidget(scanCounter);
    scanLayout->addWidget(bcLabel);
    scanLayout->addWidget(scanValue);
    scanLayout->addWidget(startButton);
    scanLayout->addWidget(finishButton);
    scanLayout->addWidget(clearButton);
    scanLayout->addWidget(decrementButton);
    scanLayout->addStretch(2);

    return scanLayout;
}

void StockManager::startscanning()
{
    scanValue->setFocus();
    scanValue->clear();
    scanValue->setReadOnly(false);
}

void StockManager::grabBarcode()
{
    QString barcode = scanValue->text();

    if (this->checkDB(barcode)) {

        QString itemLabel = this->getDBField(barcode, "label");

        // Check the QMap to see if this product has
        // already been scanned.  If so, increment the count.
        if (scanTally.contains(barcode)) {

            scanTally[barcode] = scanTally.value(barcode) + 1;

            // This item is already in the table, so need the row
            // number of the table displayed in the GUI in order
            // to increment the count.  Use the itemMap to
            // retreive the previously created QTableWidgetItem
            // to locate the correct row in the table.
            int i = tallyTable->row(itemMap.value(barcode));
            QTableWidgetItem *newCount =
                    new QTableWidgetItem(tr("%1").arg(scanTally.value(barcode)));
            tallyTable->setItem(i,0,newCount);

        } else {

            // First scan of this product so create an instance
            // in the scan tally map.

            scanTally[barcode] = 1;

            // Insert a new row at the top of the table for the
            // first instance of this product being scanned
            tallyTable->insertRow(0);

            // Create a table item based on the query result
            QTableWidgetItem *product  = new QTableWidgetItem(itemLabel);
            QTableWidgetItem *abccode  = new QTableWidgetItem(this->getDBField(barcode,"abccode"));
            QTableWidgetItem *category = new QTableWidgetItem(this->getDBField(barcode,"category"));
            QTableWidgetItem *volume   = new QTableWidgetItem(this->getDBField(barcode,"volume"));
            QTableWidgetItem *newCount = new QTableWidgetItem(tr("%1").arg(1));
            tallyTable->setItem(0,0,newCount);
            tallyTable->setItem(0,1,abccode);
            tallyTable->setItem(0,2,product);
            tallyTable->setItem(0,3,category);
            tallyTable->setItem(0,4,volume);

            // Map the product widget item for lookup when incrementing count
            itemMap[barcode] = product;
        }

        scanCount += 1;
        if (!scanCounter->checkOverflow(scanCount)) {
            scanCounter->display(scanCount);
        }
    } else {
        // Dialog to add product to catalog
        QMessageBox dBox;
        dBox.setText("UPC not in Catalog.");
        dBox.setInformativeText("Add this Item to the Catalog?");
        dBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        dBox.setDefaultButton(QMessageBox::Ok);
        int ret = dBox.exec();
        switch (ret) {
        case QMessageBox::Ok:
            tW->setCurrentIndex(1);
            pCat->addItem(barcode);
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
    scanValue->clear();
}

void StockManager::cleartable()
{
    int r = tallyTable->rowCount();
    for (r--; r >= 0; r--) {
        tallyTable->removeRow(r);
    }
    scanCount = 0;
    scanCounter->display(scanCount);
    scanValue->setText("---");
    scanValue->setReadOnly(true);
    scanTally.clear();
    itemMap.clear();
}

void StockManager::finish()
{
    QStringList* itemList = new QStringList;
    QString tempStr;
    QString filename;
    QTableWidgetItem* count    = new QTableWidgetItem;
    QTableWidgetItem* code     = new QTableWidgetItem;
    QTableWidgetItem* product  = new QTableWidgetItem;
    QTableWidgetItem* category = new QTableWidgetItem;
    QTableWidgetItem* volume   = new QTableWidgetItem;

    // Label for file based on operation selected
    if (actionGroup->checkedId()) {
        itemList->append("Stock Count");
        filename.append("Inventory_");
    } else {
        itemList->append("Stock Received");
        filename.append("Received_");
    }

    // Date/Time stamp for file
    QDateTime currentDateTime(QDate::currentDate(),QTime::currentTime());

    // Format string for how date/time will be written for the file
    QString format = "MM/dd/yyyy,hh:mm:ss ";
    itemList->append(currentDateTime.toString(format));

    // Format string for how date/time will be written for the filename
    format.clear();
    format.append("MM-dd-yyyy_hh-mm-ss ");
    filename.append(currentDateTime.toString(format));
    filename.append(".csv");

    // Number of rows of data
    int r = tallyTable->rowCount();

    // Add column headings
    itemList->append("Count,Code,Description,Category,Volume");

    // Loop through table, adding data to list
    for (int i = 0; i < r; i++) {
        count    = tallyTable->item(i,0);
        code     = tallyTable->item(i,1);
        product  = tallyTable->item(i,2);
        category = tallyTable->item(i,3);
        volume   = tallyTable->item(i,4);
        tempStr.append(count->text());
        tempStr.append(",");
        tempStr.append(code->text());
        tempStr.append(",");
        tempStr.append(product->text());
        tempStr.append(",");
        tempStr.append(category->text());
        tempStr.append(",");
        tempStr.append(volume->text());
        itemList->append(tempStr);
        tempStr.clear();
    }
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
    this->cleartable();
}

void StockManager::decrement()
{
    int rows = tallyTable->rowCount();
    int i = tallyTable->currentRow();

    if (i < 0 || i > rows - 1) {
        // invalid row selected
        return;
    }

    QTableWidgetItem* item = new QTableWidgetItem;
    item = tallyTable->item(i,2);
    QString itemLabel = item->text();
    QString barcode = this->getUPC(itemLabel);

    // Check that the item count doesn't go below zero
    if (scanTally.value(barcode) <= 0) {
        return;
    } else {
        scanTally[barcode] = scanTally.value(barcode) - 1;
    }

    QTableWidgetItem *newCount = new QTableWidgetItem(tr("%1").arg(scanTally.value(barcode)));
    tallyTable->setItem(i,0,newCount);
    scanCount -= 1;
    scanCounter->display(scanCount);
}

bool StockManager::checkDB(QString barcode)
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

QString StockManager::getDBField(QString barcode, QString field)
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

QString StockManager::getUPC(QString label)
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
