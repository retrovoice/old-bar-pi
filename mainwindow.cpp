#include "mainwindow.h"

#include <QGridLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QStackedLayout>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlError>
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
    // Initialize dialog pointers to 0
    catalog = 0;
    dbDialog = 0;
    prodCatalog = 0;

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
        QString error("Database open failed.");
        QSqlError dberror(db.lastError());
        error.append(dberror.text());
        QMessageBox::critical(this, tr("Error"),
                              error, QMessageBox::Cancel);
        return;
    }
    // do stuff to setup GUI
    createTabs();
    this->setCentralWidget(tabs);

}

void MainWindow::createTabs()
{
    // Instantiate the tab widget
    tabs = new QTabWidget;

    // The interface to the product catalog
    prodCatalog = new Catalog(this);

//    // Widget for catalog page
//    QWidget *catWidget = new QWidget;

//    // The layout for this window will be a grid.
//    QGridLayout *gLayout = new QGridLayout;


//    // These 5 buttons control actions for the catalog page
//    catNew = new QPushButton(tr("&New"));
//    catCancel = new QPushButton(tr("&Cancel"));
//    catSave = new QPushButton(tr("&Save"));
//    catDelete = new QPushButton(tr("&Delete"));
//    catPrev = new QPushButton(tr("&Previous"));
//    catNext = new QPushButton(tr("Next"));

//    connect (catNew, SIGNAL(clicked()), prodDialog, SLOT(newitem()));
//    connect (catSave, SIGNAL(clicked()), prodDialog, SLOT(submit()));
//    connect (catCancel, SIGNAL(clicked()),prodDialog, SLOT(cancel()));
//    connect (catDelete, SIGNAL(clicked()), prodDialog, SLOT(remove()));
//    connect (catPrev, SIGNAL(clicked()), prodDialog, SLOT(previous()));
//    connect (catNext, SIGNAL(clicked()), prodDialog, SLOT(next()));

//    QVBoxLayout *leftLayout = new QVBoxLayout;
//    leftLayout->addWidget(catNew,1);
//    leftLayout->addWidget(catSave,1);
//    leftLayout->addWidget(catCancel,1);
//    leftLayout->addWidget(catDelete,1);

//    QHBoxLayout *bottomLayout = new QHBoxLayout;
//    bottomLayout->addWidget(catPrev,1);
//    bottomLayout->addWidget(catNext,1);

//    gLayout->addLayout(leftLayout,0,0,2,1);
//    gLayout->addLayout(bottomLayout,1,1);

//    gLayout->addWidget(prodDialog,0,1);

//    catWidget->setLayout(gLayout);

    tabs->addTab(prodCatalog,tr("Catalog"));

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
        QMessageBox::information(this, tr("Create Directory"), tr(".barpi dir created"));
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
