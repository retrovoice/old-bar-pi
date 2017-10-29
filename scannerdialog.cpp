#include "scannerdialog.h"

#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QLineEdit>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>

ScannerDialog::ScannerDialog(QDialog *parent):
    QDialog(parent),
    scanCount(0)
{
    this->createLayout();
    scanList = new QStringList;
}

void ScannerDialog::createLayout()
{
    scanCounter = new QLCDNumber(this);
    scanValue   = new QLineEdit("SCAN ITEM",this);

    connect (scanValue, SIGNAL(returnPressed()), this, SLOT(grabBarcode()));

    QPushButton* startButton  = new QPushButton(tr("Start"));
    QPushButton* finishButton = new QPushButton(tr("Finish"));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));

    connect (startButton,  SIGNAL(clicked(bool)), this, SLOT(startscanning()));
    connect (finishButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect (cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));

    QVBoxLayout* scannerLayout = new QVBoxLayout;

    scannerLayout->addWidget(scanCounter);
    scannerLayout->addWidget(scanValue);
    scannerLayout->addWidget(startButton);
    scannerLayout->addWidget(finishButton);
    scannerLayout->addWidget(cancelButton);

    this->setLayout(scannerLayout);

}

void ScannerDialog::startscanning()
{
    scanValue->setFocus();
    scanValue->clear();
}

void ScannerDialog::grabBarcode()
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

bool ScannerDialog::checkDB(QString barcode)
{
    bool found(false);
    QSqlQuery query("SELECT upccode FROM products;");
    QString upccode;

    while (!found) {
        if (query.next()) {
            upccode = query.value(0).toString();
            if (upccode == barcode) {
                found = true;
                QMessageBox::information(this,"STATUS","Barcode found!");
            }
        } else {
            break;
        }
    }
    return found;
}

void ScannerDialog::accept()
{
    QDialog::accept();
}

void ScannerDialog::done(int r)
{
    QDialog::done(r);
}

void ScannerDialog::reject()
{
    QDialog::reject();
}
