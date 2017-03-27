#ifndef BPSCAN_H
#define BPSCAN_H

#include <QThread>

class QString;

class BPScan : public QThread
{
    Q_OBJECT
public:
    BPScan(QObject *parent = 0);
    ~BPScan();

signals:
    void barcodeRead(const QString& barcode);

protected:
    void run();

private:
    QString* barcode;
};

#endif // BPSCAN_H
