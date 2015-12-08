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
	button01 = new QPushButton("Catalogue");
	button01->setCheckable(true);
	button01->setChecked(true);
	button02 = new QPushButton("Iventory");
	button02->setCheckable(true);
	button02->setChecked(false);
	button03 = new QPushButton("Measure");
	button03->setCheckable(true);
	button03->setChecked(false);
	button04 = new QPushButton("Setup");
	button04->setCheckable(true);
	button04->setChecked(false);

	QGridLayout *gLayout = new QGridLayout;
	QButtonGroup *row01Group = new QButtonGroup;

	QLabel *banner = new QLabel("BarPi");
	gLayout->addWidget(banner,0,0,1,3);


	row01Group->addButton(button01,1);
	row01Group->addButton(button02,2);
	row01Group->addButton(button03,3);
	row01Group->addButton(button04,4);

	gLayout->addWidget(button01,1,0);
	gLayout->addWidget(button02,2,0);
	gLayout->addWidget(button03,3,0);
	gLayout->addWidget(button04,4,0);

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
