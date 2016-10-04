#include "productwindow.h"

ProductWindow::ProductWindow(QSqlRelationalTableModel *products, QWidget *parent) :
    QDialog(parent)
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
    categoryCombo->setModel(products->relationModel(4));
    categoryCombo->setModelColumn(products->relationModel(4)->fieldIndex("label"));

    productCategories = new QStringList;
    for (int i = 0; i < categoryCombo->count(); i++)
    {
        productCategories->append( categoryCombo->itemText(i) );
    }

    createButtons();

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(products);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(upcEdit, 0);
    mapper->addMapping(nameEdit, 1);
    mapper->addMapping(abcCodeEdit, 2);
    mapper->addMapping(priceEdit, 3);
    mapper->addMapping(categoryCombo, 4);
    mapper->addMapping(volumeEdit, 5);
    mapper->addMapping(densityEdit, 6);
    mapper->toFirst();

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

void ProductWindow::newitem()
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

void ProductWindow::writenewrecord()
{
    QString upccode = newUpcEdit->text();
    QString label = newNameEdit->text();
    QString abccode = newAbcCodeEdit->text();
    QString price = newPriceEdit->text();
    QVariant cindex = newCategoryCombo->currentIndex();
    QString volume = newVolumeEdit->text();
    QString density = newDensityEdit->text();

    QString index(cindex.toString());

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

void ProductWindow::enablesavenew(bool enable)
{
    newSaveButton->setEnabled(enable);
}

void ProductWindow::submit()
{
    mapper->submit();
    enableButtons(false);
}

void ProductWindow::revert()
{
    mapper->revert();
    enableButtons(false);
}

void ProductWindow::createButtons()
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

void ProductWindow::enableButtons(bool enable)
{
    cancelButton->setEnabled(enable);
    saveButton->setEnabled(enable);
}
