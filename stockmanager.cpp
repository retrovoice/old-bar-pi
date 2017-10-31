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
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QFile>
#include <QByteArray>

StockManager::StockManager(QWidget *parent) :
    QWidget(parent),
    scanCount(0)
{
    tallyTable = new QTableWidget(0, 2, this);
    QStringList labels;
    labels << "Count" << "Item";
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

    connect (addtostockButton, SIGNAL(clicked()), this, SLOT(addtostock()));
    connect (countstockButton, SIGNAL(clicked()), this, SLOT(countstock()));

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

    connect (startButton,  SIGNAL(clicked(bool)), this, SLOT(startscanning()));
    connect (finishButton, SIGNAL(clicked(bool)), this, SLOT(finish()));
    connect (clearButton, SIGNAL(clicked(bool)), this, SLOT(cleartable()));

    QVBoxLayout* scanLayout = new QVBoxLayout;

    scanLayout->addWidget(lcdLabel);
    scanLayout->addWidget(scanCounter);
    scanLayout->addWidget(bcLabel);
    scanLayout->addWidget(scanValue);
    scanLayout->addWidget(startButton);
    scanLayout->addWidget(finishButton);
    scanLayout->addWidget(clearButton);
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

        QString itemLabel = this->getProductLabel(barcode);

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
            QTableWidgetItem *product = new QTableWidgetItem(itemLabel);
            QTableWidgetItem *newCount = new QTableWidgetItem(tr("%1").arg(1));
            tallyTable->setItem(0,0,newCount);
            tallyTable->setItem(0,1,product);

            // Map the product widget item for lookup when incrementing count
            itemMap[barcode] = product;
        }

        scanCount += 1;
        if (!scanCounter->checkOverflow(scanCount)) {
            scanCounter->display(scanCount);
        }
    } else {
        // Dialog to add product to catalog
        QMessageBox::warning(this, tr("UPC NOt Found"),tr("UPC not in Catalog. Please Add."));
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
}

void StockManager::finish()
{
    QStringList* itemList = new QStringList;
    QString tempStr;
    QString filename;
    QTableWidgetItem* count = new QTableWidgetItem;
    QTableWidgetItem* product = new QTableWidgetItem;

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
    itemList->append(currentDateTime.toString());
    filename.append(currentDateTime.toString());

    // Number of rows of data
    int r = tallyTable->rowCount();

    // Loop through table, adding data to list
    for (int i = 0; i < r; i++) {
        count = tallyTable->item(i,0);
        product = tallyTable->item(i,1);
        tempStr.append(count->text());
        tempStr.append(",");
        tempStr.append(product->text());
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

QString StockManager::getProductLabel(QString barcode)
{
    QString result("");

    // The barcode is in the products table so use
    // it to lookup the name of the product.
    /// NOTE: double quotes are needed around barcode string
    ///

    QString q1("SELECT label FROM products WHERE upccode = \"");
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
