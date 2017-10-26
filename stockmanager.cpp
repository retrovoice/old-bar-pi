#include "stockmanager.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

StockManager::StockManager(QWidget *parent) : QWidget(parent)
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
    countstockLabel = new QLabel(tr("Count Current Stock"),this);

    QVBoxLayout* smLayout = new QVBoxLayout;
    smLayout->addWidget(addtostockLabel);
    smLayout->addWidget(addtostockButton);
    smLayout->addWidget(countstockLabel);
    smLayout->addWidget(countstockButton);

    this->setLayout(smLayout);
}

void StockManager::addtostock()
{


}

void StockManager::countstock()
{

}

bool StockManager::inCatalog(QString barcode)
{
    return true;
}

