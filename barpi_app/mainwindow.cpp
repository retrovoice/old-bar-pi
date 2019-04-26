#include "mainwindow.h"
#include "catalog.h"
#include "inventory.h"
#include "databasedialog.h"
#include "inventorymanager.h"
#include "invoicemanager.h"

#include <QGridLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>
#include <QVariant>

MainWindow::MainWindow(QString configFile,
                       QWidget* parent,
                       Qt::WindowFlags flags):
    QMainWindow(parent, flags)
{
    // Size the window so that tables are readable
    this->resize(1200,600);

    paramvalues = new paramMap();
    readconfigfile(configFile,paramvalues);

    if (paramvalues->contains("dbname"))
        dbName = new QString(paramvalues->value("dbname"));
    else
        dbName = new QString("bpdata");

    if (paramvalues->contains("connection"))
        connection = new QString(paramvalues->value("connection"));
    else
        connection = new QString("barpi");

    if (paramvalues->contains("dbtype"))
        dbType = new QString(paramvalues->value("dbtype"));
    else
        dbType = new QString("QSQLITE");

    if (paramvalues->contains("hostname"))
        hostname = new QString(paramvalues->value("hostname"));
    else
        hostname = new QString;

    if (paramvalues->contains("username"))
        username = new QString(paramvalues->value("username"));
    else
        username = new QString;

    if (paramvalues->contains("password"))
        password = new QString(paramvalues->value("password"));
    else
        password = new QString;

    QSqlDatabase db = QSqlDatabase::addDatabase(*dbType);
    db.setDatabaseName(*dbName);

    if (hostname->size()) db.setHostName(*hostname);
    if (username->size()) db.setUserName(*username);
    if (password->size()) db.setPassword(*password);

    if (!db.open())
    {
        QSqlError dberror(db.lastError());
        QString dbError(dberror.text());
        QMessageBox::critical(this,"Database Open Failed",dbError);
    }

    // do stuff to setup GUI
    createTabs();
    this->setCentralWidget(tabs);

}

void MainWindow::createTabs()
{
    // Check to see if the database contains the products table.
    // If not, then the database hasn't yet been initialized.

    // The database setting widget.  Create here so that
    // the database gets attached to.
    dbDialog = new DatabaseDialog(this);

    QSqlTableModel checkModel;
    checkModel.setTable("products");
    if (!checkModel.select()) {
        dbDialog->initdb();
    }

    // Instantiate the tab widget
    tabs = new QTabWidget;

    // The interface to the product catalog
    prodCatalog = new Catalog(this);

    // The interface to the product inventory
    // prodInventory = new Inventory(this);

    // The interface for managing inventory
    inventoryMgr = new InventoryManager(tabs, prodCatalog, this);

    // The interface for managing invoices
    invoiceMgr = new InvoiceManager(tabs, prodCatalog, this);

    tabs->addTab(inventoryMgr,tr("Inventory Manager"));
    tabs->addTab(invoiceMgr,tr("Invoice Manager"));
    tabs->addTab(prodCatalog,tr("Catalog"));
    // tabs->addTab(dbDialog,tr("Database"));
}

void MainWindow::readconfigfile(const QString filename, paramMap *params)
{
    // Set homedir to point to user's home directory
    QDir homedir(QDir::home());

    // Check to see if .barpi directory exists and
    // if not, create it.  If it exists, cd to .barpi
    // will work, and skip directory creation.
    if (!homedir.cd(".barpi")) {
        homedir.mkdir(".barpi");
        homedir.cd(".barpi");
        QMessageBox::information(this, tr("Create Directory"),
                                 tr(".barpi dir created"));
    }

    QString filepath = homedir.path();
    filepath.append("/");
    filepath.append(filename);
    QFile conf(filepath);

    if (!conf.exists()) {
        QString msg(filepath);
        msg.append(" does not exist!\n Creating default config file.");
        QMessageBox::information(this, tr("Create Default Config"), msg);
        this->createdefaultconfig(filepath);
    }

    if (!conf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString msg;
        msg.append("Cannot open ");
        msg.append(filename);
        QMessageBox::critical(this, tr("File Open Error"),
                              msg, QMessageBox::Cancel);
        return;
    }

    const QChar comment('#');
    const QChar delimiter('=');

    QTextStream in(&conf);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(QRegExp("\\s+"));
        int dindex = 0;
        for (int i = 0; i < parts.size(); i++) {

            if (parts[i][0] == comment) break;

            QStringList segments = parts[i].split(delimiter,QString::SkipEmptyParts);

            if (segments.size() == 2) {
                params->insert(segments[0],segments[1]);
            }
            if (parts[i] == delimiter) {
                dindex = i;
            }
        }
        if (dindex > 0 && parts.size() > 2) {
            params->insert(parts[dindex-1],parts[dindex+1]);
        }
    }
}


void MainWindow::writeconfigfile(const QString filename, const paramMap &values)
{

}

void MainWindow::createdefaultconfig(const QString filename)
{
    QFile conf(filename);

    if (!conf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QString msg;
        msg.append("Cannot open ");
        msg.append(filename);
        msg.append(" for writing.");
        QMessageBox::critical(this, tr("File Open Error"),
                              msg, QMessageBox::Cancel);
        return;
    }
    else {
        QTextStream output(&conf);
        output << "# This is the default barpi configuration file.\n";
        output << "dbname = bpdata\n";
        output << "connection = barpi\n";
        output << "dbtype = QSQLITE\n";
        output.flush();
        conf.close();
    }
}
