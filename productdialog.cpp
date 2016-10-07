#include "productdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>
#include <QString>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

ProductDialog::ProductDialog(const QString &dbname, QWidget *parent) :
    QDialog(parent)
{

    dbName = new QString(dbname);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(*dbName);

    if (!db.open())
    {
        QString error("Database open failed.");
        error.append(db.lastError().text());
        QMessageBox::critical(this, tr("Error"),
                              error, QMessageBox::Cancel);
        return;
    }
    else
    {
        QMessageBox::information(this, tr("Success!"),tr("Database successfully opened"));
    }

    prodTableModel = new QSqlRelationalTableModel;

    this->initModels();

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

    productCategories = new QStringList;
    for (int i = 0; i < categoryCombo->count(); i++)
    {
        productCategories->append( categoryCombo->itemText(i) );
    }

    createButtons();

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(prodTableModel);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(upcEdit, 0);
    mapper->addMapping(nameEdit, 1);
    mapper->addMapping(abcCodeEdit, 2);
    mapper->addMapping(priceEdit, 3);
    mapper->addMapping(categoryCombo, 4);
    mapper->addMapping(volumeEdit, 5);
    mapper->addMapping(densityEdit, 6);
    mapper->toLast();

    //QString changedText;

    connect(upcEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(nameEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(abcCodeEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(priceEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(volumeEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(densityEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(categoryCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(enableButtons()));

    QFormLayout *productLayout = new QFormLayout;
    productLayout->addRow(upcLabel, upcEdit);
    productLayout->addRow(nameLabel, nameEdit);
    productLayout->addRow(abcCodeLabel, abcCodeEdit);
    productLayout->addRow(priceLabel, priceEdit);
    productLayout->addRow(categoryLabel, categoryCombo);
    productLayout->addRow(volumeLabel, volumeEdit);
    productLayout->addRow(densityLabel, densityEdit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(productLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);
    setWindowTitle(tr("Barpi Product"));

    enableButtons(true);
}

void ProductDialog::initModels()
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
    prodTableModel->select();
}

void ProductDialog::newitem()
{
    // Need to launch a new dialog to capture at a minimum,
    // the actual UPC code of the product.  Once a record
    // is saved to the database, the primary key, in the case
    // the UPC code, cannot be changed

    newProductDialog = new QDialog(this);

    newUpcLabel = new QLabel(tr("&UPC Code:"));
    newUpcEdit = new QLineEdit();
    newUpcLabel->setBuddy(newUpcEdit);

    newNameLabel = new QLabel(tr("Na&me:"));
    newNameEdit = new QLineEdit();
    newNameLabel->setBuddy(newNameEdit);

    newAbcCodeLabel = new QLabel(tr("&ABC Code:"));
    newAbcCodeEdit = new QLineEdit();
    newAbcCodeLabel->setBuddy(newAbcCodeEdit);

    newPriceLabel = new QLabel(tr("&Price:"));
    newPriceEdit = new QLineEdit();
    newPriceLabel->setBuddy(newPriceEdit);

    newVolumeLabel = new QLabel(tr("&Volume"));
    newVolumeEdit = new QLineEdit();
    newVolumeLabel->setBuddy(newVolumeEdit);

    newDensityLabel = new QLabel(tr("&Density:"));
    newDensityEdit = new QLineEdit();
    newDensityLabel->setBuddy(newDensityEdit);

    newCategoryLabel = new QLabel(tr("Category:"));
    newCategoryCombo = new QComboBox();

    for ( int i = 0; i < productCategories->size(); i++ )
    {
        newCategoryCombo->addItem(productCategories->at(i));
    }

    newSaveButton = new QPushButton(tr("&Save"));
    newCancelButton = new QPushButton(tr("&Cancel"));

    connect(newSaveButton, SIGNAL(clicked()), this, SLOT(writenewrecord()));
    connect(newCancelButton, SIGNAL(clicked()), newProductDialog, SLOT(close()));

    newCancelButton->setEnabled(true);
    newSaveButton->setEnabled(false);

    newButtonBox = new QDialogButtonBox(this);
    newButtonBox->addButton(newSaveButton, QDialogButtonBox::AcceptRole);
    newButtonBox->addButton(newCancelButton, QDialogButtonBox::RejectRole);

    connect(newUpcEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enablesavenew()));

    QFormLayout *newProductLayout = new QFormLayout;
    newProductLayout->addRow(newUpcLabel, newUpcEdit);
    newProductLayout->addRow(newNameLabel, newNameEdit);
    newProductLayout->addRow(newAbcCodeLabel, newAbcCodeEdit);
    newProductLayout->addRow(newPriceLabel, newPriceEdit);
    newProductLayout->addRow(newCategoryLabel, newCategoryCombo);
    newProductLayout->addRow(newVolumeLabel, newVolumeEdit);
    newProductLayout->addRow(newDensityLabel, newDensityEdit);

    QVBoxLayout *newLayout = new QVBoxLayout;
    newLayout->addLayout(newProductLayout);
    newLayout->addWidget(newButtonBox);

    newProductDialog->setLayout(newLayout);
    newProductDialog->setWindowTitle(tr("New Barpi Product"));
    newProductDialog->show();

    enableButtons(true);
}

void ProductDialog::writenewrecord()
{
    QString upccode = newUpcEdit->text();
    QString label = newNameEdit->text();
    QString abccode = newAbcCodeEdit->text();
    QString price = newPriceEdit->text();
    QVariant cindex = newCategoryCombo->currentIndex();
    QString volume = newVolumeEdit->text();
    QString density = newDensityEdit->text();

    QString index(cindex.toString());

    //QSqlRecord newRecord;
    //newRecord.insert(0,QSqlField(upccode));
    //newRecord.insert(1,QSqlField(label));
    //newRecord.insert(2,QSqlField(abccode));
    //newRecord.insert(3,QSqlField(price));
    //newRecord.insert(4,QSqlField(index));
    //newRecord.insert(5,QSqlField(volume));
    //newRecord.insert(6,QSqlField(density));

    //prodTableModel->insertRecord(-1,newRecord);
    //prodTableModel->select();

    QString queryText01("\"insert into products values (\'");
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
    queryText01.append(")\"");
    QMessageBox newRecordMessage(this);
    newRecordMessage.setText(queryText01);
    newRecordMessage.exec();

    QSqlQuery insertquery;
    insertquery.exec(queryText01);

}

void ProductDialog::enablesavenew(bool enable)
{
    newSaveButton->setEnabled(enable);
}

void ProductDialog::submit()
{
    mapper->submit();
    enableButtons(false);
}

void ProductDialog::revert()
{
    mapper->revert();
    enableButtons(false);
}

void ProductDialog::createButtons()
{
    newButton = new QPushButton(tr("&New"));
    saveButton = new QPushButton(tr("&Save"));
    cancelButton = new QPushButton(tr("&Cancel"));
    closeButton = new QPushButton(tr("Close"));

    closeButton->setDefault(true);
    newButton->setEnabled(true);

    connect(newButton, SIGNAL(clicked()), this, SLOT(newitem()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(revert()));

    buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(newButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(saveButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);
}

void ProductDialog::enableButtons(bool enable)
{
    cancelButton->setEnabled(enable);
    saveButton->setEnabled(enable);
}
