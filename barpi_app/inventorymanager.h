#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class QString;
class QLayout;
class QLineEdit;
class QLCDNumber;
class QButtonGroup;
class QPushButton;
class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class QHeaderView;
class Catalog;
QT_END_NAMESPACE

class InventoryManager : public QWidget
{
    Q_OBJECT
public:
    explicit InventoryManager(QTabWidget* tabW,
                          Catalog* catalog,
                          QWidget *parent = 0);

signals:

public slots:

private slots:
    void startscanning();
    void grabBarcode();
    void cleartable();
    void commit();
    void report();
    void decrement();
    void refocus();

private:

    void createLayout();
    QLayout* createScanLayout();
    void enableButtons(bool st);

    int scanCount;
    QLineEdit* scanValue;
    QMap<QString,float> scanTally;
    QLCDNumber* scanCounter;
    QButtonGroup* actionGroup;
    QButtonGroup* volumeGroup;
    QTableWidget* tallyTable;
    QHeaderView*  tallyHeader;
    QMap<QString,QTableWidgetItem*> itemMap;
    
    QPushButton* startButton;
    QPushButton* commitButton;
    QPushButton* reportButton;
    QPushButton* resetButton;

    bool checkDB(QString barcode);
    QString getDBField(QString barcode, QString field);
    QString getUPC(QString label);

    Catalog* pCat;
    QTabWidget* tW;

};

#endif // INVENTORYMANAGER_H
