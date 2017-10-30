#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QString;
class QLayout;
class QLineEdit;
class QStringList;
class QLCDNumber;
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
    QStringList* scanList;
    QLCDNumber* scanCounter;
    bool checkDB(QString barcode);

};

#endif // STOCKMANAGER_H
