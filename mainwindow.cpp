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
	button02 = new QPushButton("Iventory");
	button03 = new QPushButton("Measure");
	button04 = new QPushButton("Setup");

	QGridLayout *gLayout = new QGridLayout;
	gLayout->addWidget(button01,0,0);
	gLayout->addWidget(button02,1,0);
	gLayout->addWidget(button03,2,0);
	gLayout->addWidget(button04,3,0);
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
