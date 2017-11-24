#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class QString;
class QLayout;
class QLineEdit;
class QLCDNumber;
class QButtonGroup;
class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class Catalog;
QT_END_NAMESPACE

class StockManager : public QWidget
{
    Q_OBJECT
public:
    explicit StockManager(QTabWidget* tabW,
                          Catalog* catalog,
                          QWidget *parent = 0);

signals:

public slots:

private slots:
    void startscanning();
    void grabBarcode();
    void cleartable();
    void finish();

private:

    void createLayout();
    QLayout* createScanLayout();

    int scanCount;
    QLineEdit* scanValue;
    QMap<QString,int> scanTally;
    QLCDNumber* scanCounter;
    QButtonGroup* actionGroup;
    QTableWidget* tallyTable;
    QMap<QString,QTableWidgetItem*> itemMap;

    bool checkDB(QString barcode);
    QString getProductLabel(QString barcode);

    Catalog* pCat;
    QTabWidget* tW;

};

#endif // STOCKMANAGER_H
