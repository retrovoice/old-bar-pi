#include "catalogue.h"

#include <QMessageBox>
#include <QTableView>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE


Catalogue::Catalogue(QWidget *parent) :
    QWidget(parent)
{
    prodModel = new QSqlRelationalTableModel;

    this->initModels();

    QTableView *prodView = this->createView(QObject::tr("Barpi Products"), prodModel);
    prodView->show();
}

void Catalogue::initModels()
{

    prodModel->setTable("products");
    prodModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    prodModel->setRelation(4, QSqlRelation("categories", "id", "label"));
    prodModel->setHeaderData(0, Qt::Horizontal, QObject::tr("upccode"));
    prodModel->setHeaderData(1, Qt::Horizontal, QObject::tr("label"));
    prodModel->setHeaderData(2, Qt::Horizontal, QObject::tr("abccode"));
    prodModel->setHeaderData(3, Qt::Horizontal, QObject::tr("price"));
    prodModel->setHeaderData(4, Qt::Horizontal, QObject::tr("category"));
    prodModel->setHeaderData(5, Qt::Horizontal, QObject::tr("volume"));
    prodModel->setHeaderData(6, Qt::Horizontal, QObject::tr("density"));
    // Populate the model
    if (!prodModel->select()) {
        showError(prodModel->lastError());
        return;
    }

}

QTableView *Catalogue::createView(const QString &title, QSqlTableModel *model)
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setItemDelegate(new QSqlRelationalDelegate(view));
    view->setWindowTitle(title);
    return view;
}

void Catalogue::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Error Mapping Database",
                          "Reported Error: " + err.text());
}
