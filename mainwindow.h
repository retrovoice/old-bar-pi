#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow();
	MainWindow(QString configFile);

	private slots:
		void openConfigFile();
		//void saveConfigFile();
		//void saveConfigFileAs();
		//void addItem();
		//void delItem();
		//void doReport();

private:
	void createLayout();
	QPushButton *button01;
	QPushButton *button02;
	QPushButton *button03;
	QPushButton *button04;

	QString m_currentConfigFile;
};

#endif // MAINWINDOW_H
