#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStackedLayout;
class QButtonGroup;
class Catalogue;
class DatabaseDialog;
class ProductDialog;
class QPushButton;
class QString;
template <class Key, class T> class QMap;
QT_END_NAMESPACE

typedef QMap<QString,QString> paramMap;

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QString configFile);

private slots:
    void setSubMenu();
    void showDbDialog();
    void showCatalogue();
    void showProdDialog();

private:

    QString *dbName;
    QString *connection;
    QString *dbType;
    QString *hostname;
    QString *username;
    QString *password;
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

    paramMap *paramvalues;

    void readconfigfile(const QString filename, paramMap *params);
    void writeconfigfile(const QString filename, const paramMap &values);
    void createdefaultconfig(const QString filename);

};

#endif // MAINWINDOW_H
