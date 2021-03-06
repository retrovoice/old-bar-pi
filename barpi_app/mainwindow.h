#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QButtonGroup;
class DatabaseDialog;
class Catalog;
class Inventory;
class DatabaseDialog;
class StockManager;
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

protected:

    Catalog *prodCatalog;
    Inventory *prodInventory;
    StockManager *stockMgr;
    DatabaseDialog *dbDialog;

private:

    QString *dbName;
    QString *connection;
    QString *dbType;
    QString *hostname;
    QString *username;
    QString *password;


    void createTabs();
    QPushButton *catNew;
    QPushButton *catSave;
    QPushButton *catCancel;
    QPushButton *catDelete;
    QPushButton *catPrev;
    QPushButton *catNext;
    QTabWidget *tabs;

    paramMap *paramvalues;

    void readconfigfile(const QString filename, paramMap *params);
    void writeconfigfile(const QString filename, const paramMap &values);
    void createdefaultconfig(const QString filename);
};

#endif // MAINWINDOW_H
