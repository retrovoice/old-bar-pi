#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bpscan.h"

QT_BEGIN_NAMESPACE
class QStackedLayout;
class QButtonGroup;
class DatabaseDialog;
class Catalog;
class Inventory;
class DatabaseDialog;
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

protected slots:

    void acceptBarcode(const QString& barcode);

private:

    BPScan scanner;

    QString *dbName;
    QString *connection;
    QString *dbType;
    QString *hostname;
    QString *username;
    QString *password;
    Catalog *prodCatalog;
    Inventory *prodInventory;
    DatabaseDialog *dbDialog;


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
