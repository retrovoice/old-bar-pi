#include "scannerdialog.h"

#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QLineEdit>

ScannerDialog::ScannerDialog(QDialog *parent):
    QDialog(parent),
    scanCount(0)
{
    this->createLayout();
}

void ScannerDialog::createLayout()
{
    scanCounter = new QLCDNumber(this);
    scanValue   = new QLineEdit("SCAN ITEM",this);

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
    scanCount += 1;
    if (!scanCounter->checkOverflow(scanCount)) {
        scanCounter->display(scanCount);
    }
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
