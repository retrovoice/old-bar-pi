#include "inventory.h"

#include <QMessageBox>
#include <QLabel>
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
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

Inventory::Inventory(QWidget *parent) :
    QWidget(parent)
{
    invTableModel = new QSqlRelationalTableModel;
    invTableView = new QTableView;
    invLayout = new QGridLayout;

    // These must be done in order due to initializaion
    // of access types.
    this->initModel();
    invTableView = this->createView("Barpi Inventory", invTableModel);
    invLayout->addWidget(invTableView, 0, 0);
    this->setLayout(invLayout);
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

QTableView* Inventory::createView(const QString &title, QSqlTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
}

/*void Inventory::newitem()
{
    spot = mapper->currentIndex();
    upcEdit->clear();
    nameEdit->clear();
    abcCodeEdit->clear();
    priceEdit->clear();
    volumeEdit->clear();
    densityEdit->clear();
    categoryCombo->setCurrentIndex(0);
    isNew = true;
    enableButtons(false);
}

void Inventory::submit()
{
    spot = mapper->currentIndex();
    // If this is a new record, then construct the SQL
    // command to insert it into the database, based on
    // the values in the LineEdit widgets.
    if (isNew) {
        QString upccode = upcEdit->text();
        QString label = nameEdit->text();
        QString abccode = abcCodeEdit->text();
        QString price = priceEdit->text();
        QVariant cindex = categoryCombo->currentIndex() + 1;
        QString volume = volumeEdit->text();
        QString density = densityEdit->text();

        // Convert integer index to string for use in
        // SQL command.
        QString index(cindex.toString());

        // The database was previously opened, so attach
        // to it with default connection name.
        QSqlDatabase db = QSqlDatabase::database();

        QString queryText01("INSERT INTO products VALUES (\'");
        queryText01.append(upccode);
        queryText01.append("\',\'");
        queryText01.append(label);
        queryText01.append("\',\'");
        queryText01.append(abccode);
        queryText01.append("\',");
        queryText01.append(price);
        queryText01.append(",");
        queryText01.append(index);
        queryText01.append(",");
        queryText01.append(volume);
        queryText01.append(",");
        queryText01.append(density);
        queryText01.append(")");

        // Create a query object and execute it
        QSqlQuery query;
        if (!query.exec(queryText01)) {
            showError(query.lastError());
            return;
        }
    }
    // Submit the change to the database
    if (!mapper->submit()) {
        showError(invTableModel->lastError());
        this->cancel();
        return;
    }
    // Update the mapping between the database and the
    // QDataWidgetMapper
    if (!invTableModel->submitAll()) {
        showError(invTableModel->lastError());
        this->cancel();
        return;
    }

    if (isNew) {
        mapper->toLast();
        isNew = false;
        spot = mapper->currentIndex();
    } else {
        mapper->setCurrentIndex(spot);
    }
    enableButtons(false);
}

void Inventory::cancel()
{
    mapper->setCurrentIndex(spot);
    enableButtons(false);
}

void Inventory::previous()
{
    mapper->toPrevious();
    spot = mapper->currentIndex();
    enableButtons(false);
}

void Inventory::next()
{
    mapper->toNext();
    spot = mapper->currentIndex();
    enableButtons(false);
}

void Inventory::remove()
{
    // Capture the current index of the record being removed
    spot = mapper->currentIndex();

    // Remove the row and check result for error.
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
    // Move the mapper to the index just before the
    // deleted record, or the first record in index
    // is 1 or less
    if ((spot-1) > 0) {
        mapper->setCurrentIndex(spot-1);
        spot--;
    }
    else {
        mapper->toFirst();
        spot = mapper->currentIndex();
    }
    enableButtons(false);
}

void Inventory::enableButtons(const bool st)
{
    saveButton->setEnabled(st);
    cancelButton->setEnabled(st);
}
*/
void Inventory::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Database Error",
                          "Reported Error: " + err.text());
}

