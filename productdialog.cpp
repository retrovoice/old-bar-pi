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
#include <QSqlRecord>
#include <QSqlField>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>

ProductDialog::ProductDialog(QWidget *parent) :
    QDialog(parent),
    isNew(false)
{

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

    QStringList productCategories;
    for (int i = 0; i < categoryCombo->count(); i++)
    {
        productCategories.append( categoryCombo->itemText(i) );
    }

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
    mapper->toLast();

    createButtons();

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
    setWindowTitle(tr("Catalog Edit"));

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
    // Populate the model
    if (!prodTableModel->select()) {
        showError(prodTableModel->lastError());
        return;
    }
}

void ProductDialog::newitem()
{
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

void ProductDialog::submit()
{
    if (isNew) {
        QString upccode = upcEdit->text();
        QString label = nameEdit->text();
        QString abccode = abcCodeEdit->text();
        QString price = priceEdit->text();
        QVariant cindex = categoryCombo->currentIndex() + 1;
        QString volume = volumeEdit->text();
        QString density = densityEdit->text();

        QString index(cindex.toString());

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

        isNew = false;

        QSqlQuery query;
        if (!query.exec(queryText01)) {
            showError(query.lastError());
            return;
        }
    }
    mapper->submit();
    enableButtons(false);
}

void ProductDialog::createButtons()
{
    newButton = new QPushButton(tr("New"));
    saveButton = new QPushButton(tr("Save"));
    prevButton = new QPushButton(tr("Previous"));
    nextButton = new QPushButton(tr("Next"));
    closeButton = new QPushButton(tr("Close"));

    //closeButton->setDefault(true);
    newButton->setEnabled(true);

    connect(newButton, SIGNAL(clicked()), this, SLOT(newitem()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(prevButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));

    buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(newButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(saveButton, QDialogButtonBox::ApplyRole);
    buttonBox->addButton(prevButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(nextButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);
}

void ProductDialog::enableButtons(bool enable)
{
    saveButton->setEnabled(enable);
}

void ProductDialog::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Database Error",
                          "Reported Error: " + err.text());
}