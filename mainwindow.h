#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>

QT_BEGIN_NAMESPACE
class QStackedLayout;
class QButtonGroup;
class Catalogue;
class DatabaseDialog;
class ProductDialog;
QT_END_NAMESPACE


class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow();
	MainWindow(QString configFile);

private slots:
    void openConfigFile();
    void setSubMenu();
    void showDbDialog();
    void showCatalogue();
    void showProdDialog();

private:

    QString *dbName;
    Catalogue *catalog;
    DatabaseDialog *dbDialog;
    ProductDialog *prodDialog;

	void createLayout();
    QPushButton *button10;
    QPushButton *button20;
    QPushButton *button30;
    QPushButton *button40;

    QPushButton *button11;
    QPushButton *button12;
    QPushButton *button13;
    QPushButton *button14;

    QPushButton *button21;
    QPushButton *button22;
    QPushButton *button23;
    QPushButton *button24;

    QPushButton *button31;
    QPushButton *button32;
    QPushButton *button33;
    QPushButton *button34;

    QPushButton *button41;
    QPushButton *button42;
    QPushButton *button43;
    QPushButton *button44;

    QWidget *page1;
    QWidget *page2;
    QWidget *page3;
    QWidget *page4;

    QButtonGroup *col01Group;
    QButtonGroup *page1Group;
    QButtonGroup *page2Group;
    QButtonGroup *page3Group;
    QButtonGroup *page4Group;

    QStackedLayout *pagesLayout;

    QString currentConfigFile;
};

#endif // MAINWINDOW_H
