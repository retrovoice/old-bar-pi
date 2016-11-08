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
