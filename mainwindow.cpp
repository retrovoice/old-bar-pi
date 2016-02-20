#include <iostream>
#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow()
{
	// do stuff to setup GUI
	createLayout();
}

MainWindow::MainWindow(QString configFile):
	m_currentConfigFile(configFile)
{
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

	gLayout->addWidget(banner,0,0,1,3);

	// These 4 buttons make up the main menu of the app.
	// Set the buttons Checkable, and make one of
	// them Checked for mutual exclusive operation
	// when added to the button group.
	mp_button10 = new QPushButton("Catalogue");
	mp_button10->setCheckable(true);
	mp_button10->setChecked(true);
	mp_button20 = new QPushButton("Stock");
	mp_button20->setCheckable(true);
	mp_button20->setChecked(false);
	mp_button30 = new QPushButton("Inventory");
	mp_button30->setCheckable(true);
	mp_button30->setChecked(false);
	mp_button40 = new QPushButton("Setup");
	mp_button40->setCheckable(true);
	mp_button40->setChecked(false);

	// Add items to a button group which will enforce
	// mutual exclusion, allowing only one of them
	// to be active at a time.
	mp_col01Group = new QButtonGroup;
	mp_col01Group->addButton(mp_button10,0);
	mp_col01Group->addButton(mp_button20,1);
	mp_col01Group->addButton(mp_button30,2);
	mp_col01Group->addButton(mp_button40,3);

	QVBoxLayout *col01Layout = new QVBoxLayout;
	col01Layout->addWidget(mp_button10);
	col01Layout->addWidget(mp_button20);
	col01Layout->addWidget(mp_button30);
	col01Layout->addWidget(mp_button40);

	gLayout->addLayout(col01Layout,1,0);

	mp_button11 = new QPushButton("Add to Catalogue");
	mp_button11->setCheckable(true);
	mp_button11->setChecked(true);
	mp_button12 = new QPushButton("List Catalogue");
	mp_button12->setCheckable(true);
	mp_button12->setChecked(false);
	mp_button13 = new QPushButton("Find Item");
	mp_button13->setCheckable(true);
	mp_button13->setChecked(false);
	mp_button14 = new QPushButton("Delete Item");
	mp_button14->setCheckable(true);
	mp_button14->setChecked(false);

	mp_button21 = new QPushButton("Add to Stock");
	mp_button21->setCheckable(true);
	mp_button21->setChecked(true);
	mp_button22 = new QPushButton("List Stock");
	mp_button22->setCheckable(true);
	mp_button22->setChecked(false);
	mp_button23 = new QPushButton("Find Item");
	mp_button23->setCheckable(true);
	mp_button23->setChecked(false);
	mp_button24 = new QPushButton("Delete Item");
	mp_button24->setCheckable(true);
	mp_button24->setChecked(false);

	mp_button31 = new QPushButton("Start Inventory");
	mp_button31->setCheckable(true);
	mp_button31->setChecked(true);
	mp_button32 = new QPushButton("End Inventory");
	mp_button32->setCheckable(true);
	mp_button32->setChecked(false);
	mp_button33 = new QPushButton("Inventory Report");
	mp_button33->setCheckable(true);
	mp_button33->setChecked(false);
	mp_button34 = new QPushButton("");
	mp_button34->setCheckable(true);
	mp_button34->setChecked(false);

	mp_button41 = new QPushButton("Database");
	mp_button41->setCheckable(true);
	mp_button41->setChecked(true);
	mp_button42 = new QPushButton("Load");
	mp_button42->setCheckable(true);
	mp_button42->setChecked(false);
	mp_button43 = new QPushButton("Save");
	mp_button43->setCheckable(true);
	mp_button43->setChecked(false);
	mp_button44 = new QPushButton("Save As");
	mp_button44->setCheckable(true);
	mp_button44->setChecked(false);

	mp_page1Group = new QButtonGroup;
	mp_page1Group->addButton(mp_button11,11);
	mp_page1Group->addButton(mp_button12,12);
	mp_page1Group->addButton(mp_button13,13);
	mp_page1Group->addButton(mp_button14,14);

	mp_page2Group = new QButtonGroup;
	mp_page2Group->addButton(mp_button21,21);
	mp_page2Group->addButton(mp_button22,22);
	mp_page2Group->addButton(mp_button23,23);
	mp_page2Group->addButton(mp_button24,24);

	mp_page3Group = new QButtonGroup;
	mp_page3Group->addButton(mp_button31,31);
	mp_page3Group->addButton(mp_button32,32);
	mp_page3Group->addButton(mp_button33,33);
	mp_page3Group->addButton(mp_button34,34);

	mp_page4Group = new QButtonGroup;
	mp_page4Group->addButton(mp_button41,41);
	mp_page4Group->addButton(mp_button42,42);
	mp_page4Group->addButton(mp_button43,43);
	mp_page4Group->addButton(mp_button44,44);

	QVBoxLayout *page1Layout = new QVBoxLayout;
	page1Layout->addWidget(mp_button11);
	page1Layout->addWidget(mp_button12);
	page1Layout->addWidget(mp_button13);
	page1Layout->addWidget(mp_button14);

	QVBoxLayout *page2Layout = new QVBoxLayout;
	page2Layout->addWidget(mp_button21);
	page2Layout->addWidget(mp_button22);
	page2Layout->addWidget(mp_button23);
	page2Layout->addWidget(mp_button24);

	QVBoxLayout *page3Layout = new QVBoxLayout;
	page3Layout->addWidget(mp_button31);
	page3Layout->addWidget(mp_button32);
	page3Layout->addWidget(mp_button33);
	page3Layout->addWidget(mp_button34);

	QVBoxLayout *page4Layout = new QVBoxLayout;
	page4Layout->addWidget(mp_button41);
	page4Layout->addWidget(mp_button42);
	page4Layout->addWidget(mp_button43);
	page4Layout->addWidget(mp_button44);

	mp_page1 = new QWidget;
	mp_page1->setLayout(page1Layout);
	mp_page2 = new QWidget;
	mp_page2->setLayout(page2Layout);
	mp_page3 = new QWidget;
	mp_page3->setLayout(page3Layout);
	mp_page4 = new QWidget;
	mp_page4->setLayout(page4Layout);
	
	mp_pagesLayout = new QStackedLayout;
	mp_pagesLayout->addWidget(mp_page1);
	mp_pagesLayout->addWidget(mp_page2);
	mp_pagesLayout->addWidget(mp_page3);
	mp_pagesLayout->addWidget(mp_page4);
	mp_pagesLayout->setCurrentIndex(0);

	gLayout->addLayout(mp_pagesLayout,1,1);

	setLayout(gLayout);

	connect(mp_button10, SIGNAL(clicked()), this, SLOT(setSubMenu()));
	connect(mp_button20, SIGNAL(clicked()), this, SLOT(setSubMenu()));
	connect(mp_button30, SIGNAL(clicked()), this, SLOT(setSubMenu()));
	connect(mp_button40, SIGNAL(clicked()), this, SLOT(setSubMenu()));
}

void MainWindow::openConfigFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Select Simulation Input file") );
	if (!fileName.isEmpty())
	{
		m_currentConfigFile = fileName;
	}
}

void MainWindow::setSubMenu()
{
	int buttonID = mp_col01Group->checkedId();
	//std::cout << "Button " << buttonID << " clicked.\n";
	if (buttonID > -1)
		mp_pagesLayout->setCurrentIndex(buttonID);
}