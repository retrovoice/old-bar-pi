#include "stockmanager.h"
#include "scannerdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QButtonGroup>
#include <QLCDNumber>
#include <QLineEdit>
#include <QSqlQuery>

StockManager::StockManager(QWidget *parent) :
    QWidget(parent),
    scanCount(0)
{
    this->createLayout();
    scanList = new QStringList;
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

    QButtonGroup* buttonGroup = new QButtonGroup;
    buttonGroup->addButton(addtostockButton);
    buttonGroup->setId(addtostockButton,0);
    buttonGroup->addButton(countstockButton);
    buttonGroup->setId(countstockButton,1);

    QVBoxLayout* buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(stockLabel);
    buttonLayout->addWidget(addtostockButton);
    buttonLayout->addWidget(countstockButton);
    buttonLayout->addStretch(4);

    QHBoxLayout* smLayout = new QHBoxLayout;
    smLayout->addLayout(buttonLayout);
    smLayout->addLayout(this->createScanLayout());
    smLayout->addStretch(5);
    this->setLayout(smLayout);
}

QLayout *StockManager::createScanLayout()
{
    QLabel* lcdLabel = new QLabel("Scan Counter");
    lcdLabel->setAlignment(Qt::AlignCenter);
    scanCounter = new QLCDNumber;
    QLabel* bcLabel = new QLabel("Barcode");
    bcLabel->setAlignment(Qt::AlignCenter);
    scanValue   = new QLineEdit("SCAN ITEM");

    connect (scanValue, SIGNAL(returnPressed()), this, SLOT(grabBarcode()));

    QPushButton* startButton  = new QPushButton(tr("Start"));
    QPushButton* finishButton = new QPushButton(tr("Finish"));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));

    connect (startButton,  SIGNAL(clicked(bool)), this, SLOT(startscanning()));
//    connect (finishButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
//    connect (cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    QVBoxLayout* scanLayout = new QVBoxLayout;

    scanLayout->addWidget(lcdLabel);
    scanLayout->addWidget(scanCounter);
    scanLayout->addWidget(bcLabel);
    scanLayout->addWidget(scanValue);
    scanLayout->addWidget(startButton);
    scanLayout->addWidget(finishButton);
    scanLayout->addWidget(cancelButton);
    scanLayout->addStretch(4);

    return scanLayout;
}

void StockManager::addtostock()
{
    ScannerDialog* addtoDialog = new ScannerDialog;
    addtoDialog->show();
    addtoDialog->raise();
    addtoDialog->activateWindow();
}

void StockManager::countstock()
{
    ScannerDialog* countDialog = new ScannerDialog;
    countDialog->show();
    countDialog->raise();
    countDialog->activateWindow();
}

void StockManager::startscanning()
{
    scanValue->setFocus();
    scanValue->clear();
}

void StockManager::grabBarcode()
{
    QString barcode = scanValue->text();
    if (this->checkDB(barcode)) {
        scanList->append(scanValue->text());
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


