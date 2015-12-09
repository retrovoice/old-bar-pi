#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>

class QStackedLayout;
class QButtonGroup;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow();
	MainWindow(QString configFile);

	private slots:
		void openConfigFile();
		void setSubMenu();
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

	QWidget *mp_page1;
	QWidget *mp_page2;
	QWidget *mp_page3;
	QWidget *mp_page4;

	QButtonGroup *mp_col01Group;
	QButtonGroup *mp_page1Group;
	QButtonGroup *mp_page2Group;
	QButtonGroup *mp_page3Group;
	QButtonGroup *mp_page4Group;

	QStackedLayout *mp_pagesLayout;

	QString m_currentConfigFile;
};

#endif // MAINWINDOW_H
