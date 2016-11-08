#include "mainwindow.h"
#include "catalogue.h"
#include "productdialog.h"
#include "databasedialog.h"

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
    prodDialog = 0;

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
//    else
//    {
//        QMessageBox::information(this, tr("Success!"),tr("Database successfully opened"));
//        db.close();
//    }

    // do stuff to setup GUI
    createLayout();
    this->setCentralWidget(tabs);

}

void MainWindow::createLayout()
{
    // Instantiate the tab widget
    tabs = new QTabWidget;

    // The interface to the product catalog
    prodDialog = new ProductDialog;

    // Widget for catalog page
    QWidget *catWidget = new QWidget;

    // The layout for this window will be a grid.
    QGridLayout *gLayout = new QGridLayout;

//    if (!tabs->addTab(gLayout,"Catalog")) {
//        QDebug << "addTab operation failed";
//    }

    // These 5 buttons control actions for the catalog page
    catAdd = new QPushButton(tr("&Add"));
//    catEdit = new QPushButton(tr("&Edit"));
    catSave = new QPushButton(tr("&Save"));
    catDelete = new QPushButton(tr("&Delete"));
    catPrev = new QPushButton(tr("&Previous"));
    catNext = new QPushButton(tr("&Next"));

    connect (catAdd, SIGNAL(clicked()), prodDialog, SLOT(newitem()));
    connect (catSave, SIGNAL(clicked()), prodDialog, SLOT(submit()));
    connect (catDelete, SIGNAL(clicked()), prodDialog, SLOT(remove()));
    connect (catPrev, SIGNAL(clicked()), prodDialog, SLOT(previous()));
    connect (catNext, SIGNAL(clicked()), prodDialog, SLOT(next()));

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(catAdd,1);
//    leftLayout->addWidget(catEdit,1);
    leftLayout->addWidget(catSave,1);
    leftLayout->addWidget(catDelete,1);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(catPrev,1);
    bottomLayout->addWidget(catNext,1);

    gLayout->addLayout(leftLayout,0,0,2,1);
    gLayout->addLayout(bottomLayout,1,1);

    gLayout->addWidget(prodDialog,0,1);

    catWidget->setLayout(gLayout);

    tabs->addTab(catWidget,tr("Catalog"));

//    button30 = new QPushButton("Inventory");
//    button40 = new QPushButton("Setup");

    // Add items to a button group which will enforce
    // mutual exclusion, allowing only one of them
    // to be active at a time.
//    col01Group = new QButtonGroup;
//    col01Group->addButton(button10,0);
//    col01Group->addButton(button20,1);
//    col01Group->addButton(button30,2);
//    col01Group->addButton(button40,3);

//    QVBoxLayout *col01Layout = new QVBoxLayout;
//    col01Layout->addWidget(button10);
//    col01Layout->addWidget(button20);
//    col01Layout->addWidget(button30);
//    col01Layout->addWidget(button40);

//    gLayout->addLayout(col01Layout,1,0);

//    button11 = new QPushButton("Edit Catalogue");
//    button11->setCheckable(true);
//    button11->setChecked(false);
//    connect(button11, SIGNAL(clicked()), this, SLOT(showProdDialog()));

//    button12 = new QPushButton("List Catalogue");
//    button12->setCheckable(true);
//    button12->setChecked(false);
//    connect(button12, SIGNAL(clicked()), this, SLOT(showCatalogue()));

//    button13 = new QPushButton("Find Item");
//    button13->setCheckable(true);
//    button13->setChecked(false);
//    button14 = new QPushButton("Delete Item");
//    button14->setCheckable(true);
//    button14->setChecked(false);

//    button21 = new QPushButton("Add to Stock");
//    button21->setCheckable(true);
//    button21->setChecked(true);
//    button22 = new QPushButton("List Stock");
//    button22->setCheckable(true);
//    button22->setChecked(false);
//    button23 = new QPushButton("Find Item");
//    button23->setCheckable(true);
//    button23->setChecked(false);
//    button24 = new QPushButton("Delete Item");
//    button24->setCheckable(true);
//    button24->setChecked(false);

//    button31 = new QPushButton("Start Inventory");
//    button31->setCheckable(true);
//    button31->setChecked(false);
//    button32 = new QPushButton("End Inventory");
//    button32->setCheckable(true);
//    button32->setChecked(false);
//    button33 = new QPushButton("Inventory Report");
//    button33->setCheckable(true);
//    button33->setChecked(false);
//    button34 = new QPushButton("");
//    button34->setCheckable(true);
//    button34->setChecked(false);

//    button41 = new QPushButton("Database");
//    button41->setCheckable(true);
//    button41->setChecked(false);
    // Specify Database connection parameters,
    // test connection and attach
//    connect(button41, SIGNAL(clicked()), this, SLOT(showDbDialog()));

//    button42 = new QPushButton("Not Used");

//    button42->setCheckable(true);
//    button42->setChecked(false);
//    button43 = new QPushButton("Not Used");
//    button43->setCheckable(true);
//    button43->setChecked(false);
//    button44 = new QPushButton("Not Used");
//    button44->setCheckable(true);
//    button44->setChecked(false);

//    page1Group = new QButtonGroup;
//    page1Group->addButton(button11,11);
//    page1Group->addButton(button12,12);
//    page1Group->addButton(button13,13);
//    page1Group->addButton(button14,14);

//    page2Group = new QButtonGroup;
//    page2Group->addButton(button21,21);
//    page2Group->addButton(button22,22);
//    page2Group->addButton(button23,23);
//    page2Group->addButton(button24,24);

//    page3Group = new QButtonGroup;
//    page3Group->addButton(button31,31);
//    page3Group->addButton(button32,32);
//    page3Group->addButton(button33,33);
//    page3Group->addButton(button34,34);

//    page4Group = new QButtonGroup;
//    page4Group->addButton(button41,41);
//    page4Group->addButton(button42,42);
//    page4Group->addButton(button43,43);
//    page4Group->addButton(button44,44);

//    QVBoxLayout *page1Layout = new QVBoxLayout;
//    page1Layout->addWidget(button11);
//    page1Layout->addWidget(button12);
//    page1Layout->addWidget(button13);
//    page1Layout->addWidget(button14);

//    QVBoxLayout *page2Layout = new QVBoxLayout;
//    page2Layout->addWidget(button21);
//    page2Layout->addWidget(button22);
//    page2Layout->addWidget(button23);
//    page2Layout->addWidget(button24);

//    QVBoxLayout *page3Layout = new QVBoxLayout;
//    page3Layout->addWidget(button31);
//    page3Layout->addWidget(button32);
//    page3Layout->addWidget(button33);
//    page3Layout->addWidget(button34);

//    QVBoxLayout *page4Layout = new QVBoxLayout;
//    page4Layout->addWidget(button41);
//    page4Layout->addWidget(button42);
//    page4Layout->addWidget(button43);
//    page4Layout->addWidget(button44);

//    page1 = new QWidget;
//    page1->setLayout(page1Layout);
//    page2 = new QWidget;
//    page2->setLayout(page2Layout);
//    page3 = new QWidget;
//    page3->setLayout(page3Layout);
//    page4 = new QWidget;
//    page4->setLayout(page4Layout);

//    pagesLayout = new QStackedLayout;
//    pagesLayout->addWidget(page1);
//    pagesLayout->addWidget(page2);
//    pagesLayout->addWidget(page3);
//    pagesLayout->addWidget(page4);
//    pagesLayout->setCurrentIndex(0);

//    gLayout->addLayout(pagesLayout,1,1);

//    setLayout(gLayout);

//    connect(button10, SIGNAL(clicked()), this, SLOT(setSubMenu()));
//    connect(button20, SIGNAL(clicked()), this, SLOT(setSubMenu()));
//    connect(button30, SIGNAL(clicked()), this, SLOT(setSubMenu()));
//    connect(button40, SIGNAL(clicked()), this, SLOT(setSubMenu()));
}

//void MainWindow::setSubMenu()
//{
//    int buttonID = col01Group->checkedId();
//    //std::cout << "Button " << buttonID << " clicked.\n";
//    if (buttonID > -1)
//        pagesLayout->setCurrentIndex(buttonID);
//}

//void  MainWindow::showDbDialog()
//{
//    if (!dbDialog) dbDialog = new DatabaseDialog(this);
//    dbDialog->show();
//}

//void  MainWindow::showCatalogue()
//{
//    if (!catalog) catalog = new Catalogue(this);
//    catalog->show();
//}

//void  MainWindow::showProdDialog()
//{
//    if (!prodDialog) prodDialog = new ProductDialog(this);
//    prodDialog->show();
//}

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
