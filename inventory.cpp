#include "inventory.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QString>
#include <QGridLayout>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QTableView>
#include <QSqlRelationalDelegate>
#include <QDate>
#include <QTime>
#include <QDateTime>

Inventory::Inventory(QWidget *parent) :
    QWidget(parent)
{
    invTableModel = new QSqlRelationalTableModel;
    invTableView = new QTableView;

    // These must be done in order due to initializaion
    // of access types.
    this->initModel();
    invTableView = this->createView("Barpi Inventory", invTableModel);
    createLayout();
    invTableView->show();
}

void Inventory::initModel()
{
    invTableModel->setTable("inventory");
    invTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    invTableModel->setRelation(1, QSqlRelation("products", "upccode", "label"));
    invTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    invTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    invTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Barcode"));
    invTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Retired?"));
    invTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Gross"));
    invTableModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Tare"));
    invTableModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Arrival"));
    invTableModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Departure"));
    // Synchronize model with database
    if (!invTableModel->select()) {
        showError(invTableModel->lastError());
    }
}

QTableView* Inventory::createView(const QString &title, QSqlRelationalTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
}

void Inventory::createLayout()
{
    retiredCB = new QCheckBox;
    retiredCB->setText("Retired");
    addButton = new QPushButton("&Add");
    saveButton = new QPushButton("&Save");

    upcEdit = new QLineEdit;
    nameEdit = new QLineEdit;
    barcodeEdit = new QLineEdit;
    grossEdit = new QLineEdit;
    tareEdit = new QLineEdit;

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(retiredCB);

    connect (addButton, SIGNAL(clicked()), this, SLOT(additem()));
    connect (saveButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect (retiredCB, SIGNAL(stateChanged(int)), this, SLOT(retire()));

    QGridLayout* invLayout = new QGridLayout;
    invLayout->addLayout(buttonLayout, 0, 0);
    invLayout->addWidget(invTableView, 1, 0);

    this->setLayout(invLayout);
}

void Inventory::additem()
{
    //SqlRecord newRecord;
    QDateTime currentDateTime(QDate::currentDate(),QTime::currentTime());
    int datetimeInt = currentDateTime.toTime_t();

    QVariant row = invTableModel->rowCount();
    QString msg = "Row Index [";
    msg.append( row.toString());
    msg.append("] out of bounds.");

    int iRow = row.toInt();

    if (!invTableModel->insertRows(iRow, 1)) {
        QMessageBox::warning( this,"Inventory::additem", msg );
        this->cancel();
    }

    if (!invTableModel->setData(invTableModel->index(iRow, 6),datetimeInt)) {
        QMessageBox::warning( this,"Inventory::additem", msg );
        this->cancel();
    }
    enableButtons(true);
}

void Inventory::submit()
{
/*    // If this is a new record, then construct the SQL
    // command to insert it into the database, based on
    // the values in the LineEdit widgets.
    if (isNew) {
        QString upccode = upcEdit->text();
        QString label = nameEdit->text();
        QString barcode = barcodeEdit->text();
        QString gross = grossEdit->text();
        QString tare = tareEdit->text();

        // The database was previously opened, so attach
        // to it with default connection name.
        QSqlDatabase db = QSqlDatabase::database();

        QString queryText01("INSERT INTO products VALUES (\'");
        queryText01.append(upccode);
        queryText01.append("\',\'");
        queryText01.append(label);
        queryText01.append("\',\'");
        queryText01.append(barcode);
        queryText01.append("\',");
        queryText01.append(gross);
        queryText01.append(",");
        queryText01.append(tare);
        queryText01.append(")");

        // Create a query object and execute it
        QSqlQuery query;
        if (!query.exec(queryText01)) {
            showError(query.lastError());
            return;
        }
    }*/
    // Update the mapping between the database and the
    // QDataWidgetMapper
    if (!invTableModel->submitAll()) {
        showError(invTableModel->lastError());
        this->cancel();
        return;
    }
    enableButtons();
}

void Inventory::cancel()
{
    invTableModel->revertAll();
    enableButtons(TRUE);
}

void Inventory::retire()
{
    // State has changed on the Retired checkbox
    // Determine new state
    if (retiredCB->isChecked())
    if (!invTableModel->removeRows(spot,1)) {
        QSqlError err = invTableModel->lastError();
        QMessageBox::warning(this, "Error - Remove Row",
                             "Reported Error: " + err.text());
        return;
    }

    // Submit the change to the database
    invTableModel->submitAll();
    // Update the mapping between the database and the
    // QDataWidgetMapper
    invTableModel->select();
    enableButtons(false);
}

void Inventory::enableButtons(const bool st)
{
    saveButton->setEnabled(st);
    addButton->setEnabled(st);
}

void Inventory::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Database Error",
                          "Reported Error: " + err.text());
}

