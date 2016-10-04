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

    volumeLabel = new QLabel(tr("&Volume"));
    volumeEdit = new QLineEdit();
    volumeLabel->setBuddy(volumeEdit);

    densityLabel = new QLabel(tr("&Density:"));
    densityEdit = new QLineEdit();
    densityLabel->setBuddy(densityEdit);

    categoryLabel = new QLabel(tr("Category:"));
    categoryCombo = new QComboBox();
    categoryCombo->setModel(products->relationModel(3));
    categoryCombo->setModelColumn(products->relationModel(3)->fieldIndex("label"));

    createButtons();

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(products);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(upcEdit, 0);
    mapper->addMapping(nameEdit, 1);
    mapper->addMapping(abcCodeEdit, 2);
    mapper->addMapping(categoryCombo, 3);
    mapper->addMapping(volumeEdit, 4);
    mapper->addMapping(densityEdit, 5);
    mapper->toFirst();

    //QString changedText;

    connect(upcEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(nameEdit, SIGNAL(textEdited(QString)),
            this, SLOT(enableButtons()));
    connect(abcCodeEdit, SIGNAL(textEdited(QString)),
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
    productLayout->addRow(categoryLabel, categoryCombo);
    productLayout->addRow(volumeLabel, volumeEdit);
    productLayout->addRow(densityLabel, densityEdit);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(productLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);

    enableButtons(false);
    setWindowTitle(tr("Barpi Product"));
}

void ProductWindow::newitem()
{
    upcEdit->clear();
    nameEdit->clear();
    abcCodeEdit->clear();
    volumeEdit->clear();
    densityEdit->clear();
    categoryCombo->setCurrentIndex(0);
    enableButtons(false);
    newButton->setEnabled(false);

    // Need to launch a new dialog to capture at a minimum,
    // the actual UPC code of the product.  Once a record
    // is saved to the database, the primary key, in the case
    // the UPC code, cannot be changed

    enableButtons(true);
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
