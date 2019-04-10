#include "catalog.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlRelationalDelegate>
#include <QTableView>
#include <QModelIndex>

Catalog::Catalog(QWidget *parent) :
    QWidget(parent)
{
    prodTableModel = new QSqlRelationalTableModel;
    prodTableView  = new QTableView;

    // These must be done in order due to initializaion
    // of access types.
    this->initModel();
    prodTableView = this->createView("Barpi Catalog", prodTableModel);
    this->createLayout();
    prodTableView->resizeColumnsToContents();
    prodTableView->setSortingEnabled(true);
    prodTableView->show();
    //updateButtons(false);
}

void Catalog::initModel()
{
    prodTableModel->setTable("products");
    prodTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    prodTableModel->setRelation(7, QSqlRelation("categories", "id", "label"));
    prodTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("UPC Code"));
    prodTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Product"));
    prodTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Vendor"));
    prodTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Par"));
    prodTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("List Order"));
    prodTableModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Zone"));
    prodTableModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Price"));
    prodTableModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Category"));
    prodTableModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Volume"));
    prodTableModel->setHeaderData(9, Qt::Horizontal, QObject::tr("Density"));
    // Synchronize model with database
    if (!prodTableModel->select()) {
        showError(prodTableModel->lastError());
    }
}

QTableView* Catalog::createView(const QString &title, QSqlRelationalTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
}

void Catalog::createLayout()
{
    // These buttons control actions for the catalog page
    newButton = new QPushButton(tr("&New"),this);
    cancelButton = new QPushButton(tr("&Cancel"),this);
    cancelButton->setEnabled(false);
    saveButton = new QPushButton(tr("&Save"),this);
    saveButton->setEnabled(false);
    deleteButton = new QPushButton(tr("&Delete"),this);

    connect (newButton, SIGNAL(clicked()), this, SLOT(newitem()));
    connect (saveButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect (cancelButton, SIGNAL(clicked()),this, SLOT(cancel()));
    connect (deleteButton, SIGNAL(clicked()), this, SLOT(remove()));

    connect (prodTableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
             this, SLOT(dataChanged()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(newButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch(5);

    QGridLayout* prodLayout = new QGridLayout;
    prodLayout->addLayout(buttonLayout, 0, 0);
    prodLayout->addWidget(prodTableView, 1, 0);

    this->setLayout(prodLayout);
}

void Catalog::newitem()
{
    prodTableView->setSortingEnabled(false);
    QVariant row = prodTableModel->rowCount();
    QString msg = "Row Index [";
    msg.append( row.toString());
    msg.append("] out of bounds.");

    int iRow = row.toInt();

    if (!prodTableModel->insertRows(iRow, 1)) {
        QMessageBox::warning( this,"Inventory::additem", msg );
        this->cancel();
    }
    prodTableView->setSortingEnabled(true);
}

void Catalog::submit()
{
    // Update the mapping between the database and the
    // QDataWidgetMapper
    if (!prodTableModel->submitAll()) {
        showError(prodTableModel->lastError());
        this->cancel();
        return;
    }
    setButtons();
}

void Catalog::cancel()
{
    prodTableView->setSortingEnabled(false);
    prodTableModel->revertAll();
    setButtons();
    prodTableView->setSortingEnabled(true);
}

void Catalog::remove()
{
    prodTableView->setSortingEnabled(false);
    // Capture the current index of the record being removed
    QModelIndex spot = prodTableView->currentIndex();

    // Remove the row and check result for error.
    if (!prodTableModel->removeRow(spot.row())) {
        QSqlError err = prodTableModel->lastError();
        QMessageBox::warning(this, "Error - Remove Row",
                             "Reported Error: " + err.text());
    }

    // Submit the change to the database
    this->submit();
    setButtons();
    prodTableView->setSortingEnabled(true);
}

void Catalog::addItem(const QString &barcode)
{
    prodTableView->setSortingEnabled(false);
    QVariant row = prodTableModel->rowCount();
    QString msg = "Row Index [";
    msg.append( row.toString());
    msg.append("] out of bounds.");

    int iRow = row.toInt();

    if (!prodTableModel->insertRows(iRow, 1)) {
        QMessageBox::warning( this,"Inventory::additem", msg );
        this->cancel();
    }

    int col = prodTableModel->fieldIndex("upccode");
    QModelIndex mIndex = prodTableModel->index(iRow,col);
    if (!prodTableModel->setData(mIndex,QVariant(barcode))) {
        QSqlError err = prodTableModel->lastError();
        QMessageBox::warning(this, "Error - Remove Row",
                             "Reported Error: " + err.text());
        this->cancel();
    }
    this->dataChanged();
    prodTableView->setSortingEnabled(true);
}

void Catalog::setButtons(const bool st)
{
    saveButton->setEnabled(!st);
    cancelButton->setEnabled(!st);
    newButton->setEnabled(st);
    deleteButton->setEnabled(st);
}

void Catalog::dataChanged(const bool st)
{
    cancelButton->setEnabled(st);
    saveButton->setEnabled(st);
}

void Catalog::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Database Error",
                          "Reported Error: " + err.text());
}
