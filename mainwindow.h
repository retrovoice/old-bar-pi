#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QStackedLayout;
class QButtonGroup;
class Catalogue;
class DatabaseDialog;
class ProductDialog;
class QPushButton;
class QString;
class QTabWidget;
template <class Key, class T> class QMap;
QT_END_NAMESPACE

typedef QMap<QString,QString> paramMap;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(QString configFile,
               QWidget* parent = 0,
               Qt::WindowFlags flags = 0);

    const QString dbname() { return *dbName; }

private slots:
//    void setSubMenu();
//    void showDbDialog();
//    void showCatalogue();
//    void showProdDialog();

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
    QPushButton *catAdd;
    QPushButton *catEdit;
    QPushButton *catSave;
    QPushButton *catDelete;
    QPushButton *catPrev;
    QPushButton *catNext;
//    QPushButton *button12;
//    QPushButton *button13;
//    QPushButton *button14;

//    QPushButton *button21;
//    QPushButton *button22;
//    QPushButton *button23;
//    QPushButton *button24;

//    QPushButton *button31;
//    QPushButton *button32;
//    QPushButton *button33;
//    QPushButton *button34;

//    QPushButton *button41;
//    QPushButton *button42;
//    QPushButton *button43;
//    QPushButton *button44;

//    QWidget *page1;
//    QWidget *page2;
//    QWidget *page3;
//    QWidget *page4;

//    QButtonGroup *col01Group;
//    QButtonGroup *page1Group;
//    QButtonGroup *page2Group;
//    QButtonGroup *page3Group;
//    QButtonGroup *page4Group;

//    QStackedLayout *pagesLayout;
    QTabWidget *tabs;

    paramMap *paramvalues;

    void readconfigfile(const QString filename, paramMap *params);
    void writeconfigfile(const QString filename, const paramMap &values);
    void createdefaultconfig(const QString filename);
};

#endif // MAINWINDOW_H
