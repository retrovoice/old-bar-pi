#include "stockmanager.h"
#include "scannerdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

StockManager::StockManager(QWidget *parent) :
    QWidget(parent)
{
    this->createLayout();
}

void StockManager::createLayout()
{
    addtostockButton = new QPushButton(tr("Add to Stock"),this);
    countstockButton = new QPushButton(tr("Count Stock"),this);

    connect (addtostockButton, SIGNAL(clicked()), this, SLOT(addtostock()));
    connect (countstockButton, SIGNAL(clicked()), this, SLOT(countstock()));

    addtostockLabel = new QLabel(tr("Add Stock to Inventory"),this);
    stockLabel = new QLabel(tr("Count Current Stock"),this);

    QVBoxLayout* smLayout = new QVBoxLayout;
    smLayout->addWidget(addtostockLabel);
    smLayout->addWidget(addtostockButton);
    smLayout->addWidget(stockLabel);
    smLayout->addWidget(countstockButton);

    this->setLayout(smLayout);
}

void StockManager::createScanLayout()
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

    scanLayout = new QVBoxLayout;

    scanLayout->addWidget(scanCounter);
    scanLayout->addWidget(scanValue);
    scanLayout->addWidget(startButton);
    scanLayout->addWidget(finishButton);
    scanLayout->addWidget(cancelButton);
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
                QMessageBox::information(this,"STATUS","Barcode found!");
            }
        } else {
            break;
        }
    }
    return found;
}

void StockManager::initCatModel()
{
    catTableModel->setTable("products");
    catTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    catTableModel->setRelation(4, QSqlRelation("categories", "id", "label"));
    catTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("UPC Code"));
    catTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Product"));
    catTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("ABC Code"));
    catTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Price"));
    catTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Category"));
    catTableModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Volume"));
    catTableModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Density"));
    // Synchronize model with database
    if (!catTableModel->select()) {
        showError(catTableModel->lastError());
    }
}

QTableView* StockManager::createCatView(const QString &title, QSqlRelationalTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
}


