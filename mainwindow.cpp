#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags):
	QMainWindow(parent, flags),
	m_currentConfigFile("")
{
	// do stuff to setup GUI
}

MainWindow::MainWindow(QString configFile,
					   QWidget* parent,
					   Qt::WindowFlags flags):
QMainWindow(parent, flags),
	m_currentConfigFile(configFile)
{
	// do stuff to setup GUI
}

MainWindow::~MainWindow()
{

}

void MainWindow::createLayout()
{
	QPushButton *button01 = new QPushButton("Catalogue");
	QPushButton *button02 = new QPushButton("Iventory");
	QPushButton *button03 = new QPushButton("Measure");
	QPushButton *button04 = new QPushButton("Setup");

	QStackedWidget *stackedWidget = new QStackedWidget;
	setCentralWidget(stackedWidget);
	stackedWidget->addWidget(button01);
	stackedWidget->addWidget(button02);
	stackedWidget->addWidget(button03);
	stackedWidget->addWidget(button04);

	//this->openConfigFile();

	//QVBoxLayout *layout = new QVBoxLayout;
	//layout->addWidget(stackedWidget);
	//setLayout(layout);

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
