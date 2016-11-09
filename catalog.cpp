#include "productdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>
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
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

ProductDialog::ProductDialog(QWidget *parent) :
    QWidget(parent),
    isNew(false)
{

    prodTableModel = new QSqlRelationalTableModel;

    // These must be done in order due to initializaion
    // of access types.
    this->initModel();
    this->createLayout();
    this->mapModel();

    //    enableButtons(true);
}

void ProductDialog::initModel()
{
    prodTableModel->setTable("products");
    prodTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    prodTableModel->setRelation(4, QSqlRelation("categories", "id", "label"));
    prodTableModel->setHeaderData(0, Qt::Horizontal, QObject::tr("upccode"));
    prodTableModel->setHeaderData(1, Qt::Horizontal, QObject::tr("label"));
    prodTableModel->setHeaderData(2, Qt::Horizontal, QObject::tr("abccode"));
    prodTableModel->setHeaderData(3, Qt::Horizontal, QObject::tr("price"));
    prodTableModel->setHeaderData(4, Qt::Horizontal, QObject::tr("category"));
    prodTableModel->setHeaderData(5, Qt::Horizontal, QObject::tr("volume"));
    prodTableModel->setHeaderData(6, Qt::Horizontal, QObject::tr("density"));
    // Synchronize model with database
    if (!prodTableModel->select()) {
        showError(prodTableModel->lastError());
        return;
    }
}

void ProductDialog::mapModel()
{
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(prodTableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(upcEdit, 0);
    mapper->addMapping(nameEdit, 1);
    mapper->addMapping(abcCodeEdit, 2);
    mapper->addMapping(priceEdit, 3);
    mapper->addMapping(categoryCombo, 4);
    mapper->addMapping(volumeEdit, 5);
    mapper->addMapping(densityEdit, 6);
    mapper->toFirst();
}

void ProductDialog::createLayout()
{
    upcLabel = new QLabel(tr("&UPC Code:"));
    upcEdit = new QLineEdit();
    upcLabel->setBuddy(upcEdit);

    nameLabel = new QLabel(tr("Na&me:"));
    nameEdit = new QLineEdit();
    nameLabel->setBuddy(nameEdit);

    abcCodeLabel = new QLabel(tr("&ABC Code:"));
    abcCodeEdit = new QLineEdit();
    abcCodeLabel->setBuddy(abcCodeEdit);

    priceLabel = new QLabel(tr("&Price"));
    priceEdit = new QLineEdit();
    priceLabel->setBuddy(priceEdit);

    volumeLabel = new QLabel(tr("&Volume"));
    volumeEdit = new QLineEdit();
    volumeLabel->setBuddy(volumeEdit);

    densityLabel = new QLabel(tr("&Density:"));
    densityEdit = new QLineEdit();
    densityLabel->setBuddy(densityEdit);

    categoryLabel = new QLabel(tr("Category:"));
    categoryCombo = new QComboBox();
    categoryCombo->setModel(prodTableModel->relationModel(4));
    categoryCombo->setModelColumn(prodTableModel->relationModel(4)->fieldIndex("label"));

    QStringList productCategories;
    for (int i = 0; i < categoryCombo->count(); i++)
    {
        productCategories.append( categoryCombo->itemText(i) );
    }

    QFormLayout *productLayout = new QFormLayout;
    productLayout->addRow(upcLabel, upcEdit);
    productLayout->addRow(nameLabel, nameEdit);
    productLayout->addRow(abcCodeLabel, abcCodeEdit);
    productLayout->addRow(priceLabel, priceEdit);
    productLayout->addRow(categoryLabel, categoryCombo);
    productLayout->addRow(volumeLabel, volumeEdit);
    productLayout->addRow(densityLabel, densityEdit);

    // These 5 buttons control actions for the catalog page
    newButton = new QPushButton(tr("&New"));
    cancelButton = new QPushButton(tr("&Cancel"));
    saveButton = new QPushButton(tr("&Save"));
    deleteButton = new QPushButton(tr("&Delete"));
    prevButton = new QPushButton(tr("&Previous"));
    nextButton = new QPushButton(tr("Next"));

    connect (newButton, SIGNAL(clicked()), this, SLOT(newitem()));
    connect (saveButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect (cancelButton, SIGNAL(clicked()),this, SLOT(cancel()));
    connect (deleteButton, SIGNAL(clicked()), this, SLOT(remove()));
    connect (prevButton, SIGNAL(clicked()), this, SLOT(previous()));
    connect (nextButton, SIGNAL(clicked()), this, SLOT(next()));

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(newButton,1);
    leftLayout->addWidget(saveButton,1);
    leftLayout->addWidget(cancelButton,1);
    leftLayout->addWidget(deleteButton,1);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(prevButton,1);
    bottomLayout->addWidget(nextButton,1);

    // The layout for this window will be a grid.
    QGridLayout *gLayout = new QGridLayout;
    gLayout->addLayout(leftLayout,0,0,2,1);
    gLayout->addLayout(bottomLayout,1,1);
    gLayout->addLayout(productLayout,0,1);

    this->setLayout(gLayout);
}

void ProductDialog::newitem()
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
}

void ProductDialog::submit()
{
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

        // Reset the isNew flag prior to executing the
        // database query.
        isNew = false;
        spot = mapper->currentIndex();

        // Create a query object and execute it
        QSqlQuery query;
        if (!query.exec(queryText01)) {
            showError(query.lastError());
            return;
        }
    }
    // Submit the change to the database
    mapper->submit();
    // Update the mapping between the database and the
    // QDataWidgetMapper
    prodTableModel->select();
    // Set the mapper index to last, i.e. the new record
    mapper->toLast();
    spot = mapper->currentIndex();
}

void ProductDialog::cancel()
{
    mapper->setCurrentIndex(spot);
}

void ProductDialog::previous()
{
    mapper->toPrevious();
    spot = mapper->currentIndex();
}

void ProductDialog::next()
{
    mapper->toNext();
    spot = mapper->currentIndex();
}

void ProductDialog::remove()
{
    // Capture the current index of the record being removed
    spot = mapper->currentIndex();

    // Remove the row and check result for error.
    if (!prodTableModel->removeRows(spot,1)) {
        QSqlError err = prodTableModel->lastError();
        QMessageBox::warning(this, "Error - Remove Row",
                             "Reported Error: " + err.text());
        return;
    }

    // Submit the change to the database
    prodTableModel->submitAll();
    // Update the mapping between the database and the
    // QDataWidgetMapper
    prodTableModel->select();
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
}

void ProductDialog::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Database Error",
                          "Reported Error: " + err.text());
}
