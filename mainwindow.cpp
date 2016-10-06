#include <QtGui>
#include <QtSql>
#include "mainwindow.h"
#include "catalogue.h"
#include "productdialog.h"
#include "databasedialog.h"

MainWindow::MainWindow()
{
    // Initialize dialog pointers to NULL
    catalog = 0;
    dbDialog = 0;
    prodDialog = 0;

    dbName = new QString("bp001");

    // do stuff to setup GUI
    createLayout();
}

MainWindow::MainWindow(QString configFile):
    currentConfigFile(configFile)
{
    // Initialize dialog pointers to NULL
    catalog = 0;
    dbDialog = 0;
    prodDialog = 0;

    dbName = new QString("bp001");

    // do stuff to setup GUI
	createLayout();
}

void MainWindow::createLayout()
{
	// The layout for this window will be a grid.
	QGridLayout *gLayout = new QGridLayout;

	QLabel *banner = new QLabel();
	banner->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//banner->setTextFormat(Qt::RichText);
	banner->setText("Barpi");

    // Specify row 0, col 0, to span 1 row and 3 columns
	gLayout->addWidget(banner,0,0,1,3);

	// These 4 buttons make up the main menu of the app.
	// Set the buttons Checkable, and make one of
	// them Checked for mutual exclusive operation
	// when added to the button group.
    button10 = new QPushButton("Catalogue");
    button10->setCheckable(true);
    button10->setChecked(false);
    button20 = new QPushButton("Stock");
    button20->setCheckable(true);
    button20->setChecked(false);
    button30 = new QPushButton("Inventory");
    button30->setCheckable(true);
    button30->setChecked(false);
    button40 = new QPushButton("Setup");
    button40->setCheckable(true);
    button40->setChecked(false);

	// Add items to a button group which will enforce
	// mutual exclusion, allowing only one of them
	// to be active at a time.
    col01Group = new QButtonGroup;
    col01Group->addButton(button10,0);
    col01Group->addButton(button20,1);
    col01Group->addButton(button30,2);
    col01Group->addButton(button40,3);

	QVBoxLayout *col01Layout = new QVBoxLayout;
    col01Layout->addWidget(button10);
    col01Layout->addWidget(button20);
    col01Layout->addWidget(button30);
    col01Layout->addWidget(button40);

	gLayout->addLayout(col01Layout,1,0);

    button11 = new QPushButton("Add to Catalogue");
    button11->setCheckable(true);
    button11->setChecked(false);
    connect(button11, SIGNAL(clicked()), this, SLOT(showProdDialog()));

    button12 = new QPushButton("List Catalogue");
    button12->setCheckable(true);
    button12->setChecked(false);
    connect(button12, SIGNAL(clicked()), this, SLOT(showCatalogue()));

    button13 = new QPushButton("Find Item");
    button13->setCheckable(true);
    button13->setChecked(false);
    button14 = new QPushButton("Delete Item");
    button14->setCheckable(true);
    button14->setChecked(false);

    button21 = new QPushButton("Add to Stock");
    button21->setCheckable(true);
    button21->setChecked(true);
    button22 = new QPushButton("List Stock");
    button22->setCheckable(true);
    button22->setChecked(false);
    button23 = new QPushButton("Find Item");
    button23->setCheckable(true);
    button23->setChecked(false);
    button24 = new QPushButton("Delete Item");
    button24->setCheckable(true);
    button24->setChecked(false);

    button31 = new QPushButton("Start Inventory");
    button31->setCheckable(true);
    button31->setChecked(false);
    button32 = new QPushButton("End Inventory");
    button32->setCheckable(true);
    button32->setChecked(false);
    button33 = new QPushButton("Inventory Report");
    button33->setCheckable(true);
    button33->setChecked(false);
    button34 = new QPushButton("");
    button34->setCheckable(true);
    button34->setChecked(false);

    button41 = new QPushButton("Database");
    button41->setCheckable(true);
    button41->setChecked(false);
    // Specify Database connection parameters,
    // test connection and attach
    connect(button41, SIGNAL(clicked()), this, SLOT(showDbDialog()));

    button42 = new QPushButton("Not Used");

    button42->setCheckable(true);
    button42->setChecked(false);
    button43 = new QPushButton("Not Used");
    button43->setCheckable(true);
    button43->setChecked(false);
    button44 = new QPushButton("Not Used");
    button44->setCheckable(true);
    button44->setChecked(false);

    page1Group = new QButtonGroup;
    page1Group->addButton(button11,11);
    page1Group->addButton(button12,12);
    page1Group->addButton(button13,13);
    page1Group->addButton(button14,14);

    page2Group = new QButtonGroup;
    page2Group->addButton(button21,21);
    page2Group->addButton(button22,22);
    page2Group->addButton(button23,23);
    page2Group->addButton(button24,24);

    page3Group = new QButtonGroup;
    page3Group->addButton(button31,31);
    page3Group->addButton(button32,32);
    page3Group->addButton(button33,33);
    page3Group->addButton(button34,34);

    page4Group = new QButtonGroup;
    page4Group->addButton(button41,41);
    page4Group->addButton(button42,42);
    page4Group->addButton(button43,43);
    page4Group->addButton(button44,44);

	QVBoxLayout *page1Layout = new QVBoxLayout;
    page1Layout->addWidget(button11);
    page1Layout->addWidget(button12);
    page1Layout->addWidget(button13);
    page1Layout->addWidget(button14);

	QVBoxLayout *page2Layout = new QVBoxLayout;
    page2Layout->addWidget(button21);
    page2Layout->addWidget(button22);
    page2Layout->addWidget(button23);
    page2Layout->addWidget(button24);

	QVBoxLayout *page3Layout = new QVBoxLayout;
    page3Layout->addWidget(button31);
    page3Layout->addWidget(button32);
    page3Layout->addWidget(button33);
    page3Layout->addWidget(button34);

	QVBoxLayout *page4Layout = new QVBoxLayout;
    page4Layout->addWidget(button41);
    page4Layout->addWidget(button42);
    page4Layout->addWidget(button43);
    page4Layout->addWidget(button44);

    page1 = new QWidget;
    page1->setLayout(page1Layout);
    page2 = new QWidget;
    page2->setLayout(page2Layout);
    page3 = new QWidget;
    page3->setLayout(page3Layout);
    page4 = new QWidget;
    page4->setLayout(page4Layout);
	
    pagesLayout = new QStackedLayout;
    pagesLayout->addWidget(page1);
    pagesLayout->addWidget(page2);
    pagesLayout->addWidget(page3);
    pagesLayout->addWidget(page4);
    pagesLayout->setCurrentIndex(0);

    gLayout->addLayout(pagesLayout,1,1);

	setLayout(gLayout);

    connect(button10, SIGNAL(clicked()), this, SLOT(setSubMenu()));
    connect(button20, SIGNAL(clicked()), this, SLOT(setSubMenu()));
    connect(button30, SIGNAL(clicked()), this, SLOT(setSubMenu()));
    connect(button40, SIGNAL(clicked()), this, SLOT(setSubMenu()));
}

void MainWindow::openConfigFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Select Simulation Input file") );
	if (!fileName.isEmpty())
	{
        currentConfigFile = fileName;
	}
}

void MainWindow::setSubMenu()
{
    int buttonID = col01Group->checkedId();
	//std::cout << "Button " << buttonID << " clicked.\n";
	if (buttonID > -1)
        pagesLayout->setCurrentIndex(buttonID);
}

void  MainWindow::showDbDialog()
{
    if (!dbDialog) dbDialog = new DatabaseDialog(this);
    dbDialog->show();
}

void  MainWindow::showCatalogue()
{
    if (!catalog) catalog = new Catalogue(*dbName,this);
    catalog->show();
}

void  MainWindow::showProdDialog()
{
    if (!prodDialog) prodDialog = new ProductDialog(*dbName,this);
    prodDialog->show();
}
