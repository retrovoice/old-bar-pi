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
	QButtonGroup *col01Group = new QButtonGroup;
	col01Group->addButton(mp_button10,1);
	col01Group->addButton(mp_button20,2);
	col01Group->addButton(mp_button30,3);
	col01Group->addButton(mp_button40,4);

	mp_button11 = new QPushButton("Option 11");
	mp_button11->setCheckable(true);
	mp_button11->setChecked(true);
	mp_button12 = new QPushButton("Option 12");
	mp_button12->setCheckable(true);
	mp_button12->setChecked(false);
	mp_button13 = new QPushButton("Option 13");
	mp_button13->setCheckable(true);
	mp_button13->setChecked(false);
	mp_button14 = new QPushButton("Option 14");
	mp_button14->setCheckable(true);
	mp_button14->setChecked(false);

	mp_button21 = new QPushButton("Option 21");
	mp_button21->setCheckable(true);
	mp_button21->setChecked(true);
	mp_button22 = new QPushButton("Option 22");
	mp_button22->setCheckable(true);
	mp_button22->setChecked(false);
	mp_button23 = new QPushButton("Option 23");
	mp_button23->setCheckable(true);
	mp_button23->setChecked(false);
	mp_button24 = new QPushButton("Option 24");
	mp_button24->setCheckable(true);
	mp_button24->setChecked(false);

	mp_button31 = new QPushButton("Option 31");
	mp_button31->setCheckable(true);
	mp_button31->setChecked(true);
	mp_button32 = new QPushButton("Option 32");
	mp_button32->setCheckable(true);
	mp_button32->setChecked(false);
	mp_button33 = new QPushButton("Option 33");
	mp_button33->setCheckable(true);
	mp_button33->setChecked(false);
	mp_button34 = new QPushButton("Option 34");
	mp_button34->setCheckable(true);
	mp_button34->setChecked(false);

	mp_button41 = new QPushButton("Option 41");
	mp_button41->setCheckable(true);
	mp_button41->setChecked(true);
	mp_button42 = new QPushButton("Option 42");
	mp_button42->setCheckable(true);
	mp_button42->setChecked(false);
	mp_button43 = new QPushButton("Option 43");
	mp_button43->setCheckable(true);
	mp_button43->setChecked(false);
	mp_button44 = new QPushButton("Option 44");
	mp_button44->setCheckable(true);
	mp_button44->setChecked(false);

	QButtonGroup *page1Group = new QButtonGroup;
	page1Group->addButton(mp_button11,11);
	page1Group->addButton(mp_button12,12);
	page1Group->addButton(mp_button13,13);
	page1Group->addButton(mp_button14,14);

	QButtonGroup *page2Group = new QButtonGroup;
	page2Group->addButton(mp_button21,21);
	page2Group->addButton(mp_button22,22);
	page2Group->addButton(mp_button23,23);
	page2Group->addButton(mp_button24,24);

	QButtonGroup *page3Group = new QButtonGroup;
	page3Group->addButton(mp_button31,31);
	page3Group->addButton(mp_button32,32);
	page3Group->addButton(mp_button33,33);
	page3Group->addButton(mp_button34,34);

	QButtonGroup *page4Group = new QButtonGroup;
	page4Group->addButton(mp_button41,41);
	page4Group->addButton(mp_button42,42);
	page4Group->addButton(mp_button43,43);
	page4Group->addButton(mp_button44,44);

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

	QWidget *page1 = new QWidget;
	page1->setLayout(page1Layout);
	QWidget *page2 = new QWidget;
	page1->setLayout(page2Layout);
	QWidget *page3 = new QWidget;
	page1->setLayout(page3Layout);
	QWidget *page4 = new QWidget;
	page1->setLayout(page4Layout);
	
	QStackedLayout *pagesLayout = new QStackedLayout;
	pagesLayout->addWidget(page1);
	pagesLayout->addWidget(page2);
	pagesLayout->addWidget(page3);
	pagesLayout->addWidget(page4);

	gLayout->addLayout(pagesLayout,1,1,4,1);

	setLayout(gLayout);

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
