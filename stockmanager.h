#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class QString;
class QLayout;
class QLineEdit;
class QLCDNumber;
class QTableWidget;
class QTableWidgetItem;
QT_END_NAMESPACE

class StockManager : public QWidget
{
    Q_OBJECT
public:
    explicit StockManager(QWidget *parent = 0);

signals:

public slots:

private slots:
    void addtostock();
    void countstock();
    void startscanning();
    void grabBarcode();

private:

    void createLayout();
    QLayout* createScanLayout();

    int scanCount;
    QLineEdit* scanValue;
    QMap<QString,int> scanTally;
    QLCDNumber* scanCounter;
    QTableWidget* tallyTable;
    QMap<QString,QTableWidgetItem*> itemMap;

    bool checkDB(QString barcode);
    QString getProductLabel(QString barcode);

};

#endif // STOCKMANAGER_H
