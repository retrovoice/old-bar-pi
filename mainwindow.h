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
	QPushButton *mp_button10;
	QPushButton *mp_button20;
	QPushButton *mp_button30;
	QPushButton *mp_button40;

	QPushButton *mp_button11;
	QPushButton *mp_button12;
	QPushButton *mp_button13;
	QPushButton *mp_button14;

	QPushButton *mp_button21;
	QPushButton *mp_button22;
	QPushButton *mp_button23;
	QPushButton *mp_button24;

	QPushButton *mp_button31;
	QPushButton *mp_button32;
	QPushButton *mp_button33;
	QPushButton *mp_button34;

	QPushButton *mp_button41;
	QPushButton *mp_button42;
	QPushButton *mp_button43;
	QPushButton *mp_button44;

	QString m_currentConfigFile;
};

#endif // MAINWINDOW_H
