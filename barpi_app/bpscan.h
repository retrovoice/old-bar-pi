#ifndef BPSCAN_H
#define BPSCAN_H

#include <QThread>

class BPScan : public QThread
{
    Q_OBJECT
public:
    explicit BPScan(QObject *parent);
    ~BPScan();

signals:
    void barcodeRead();

protected:
    void run();

private:
};

#endif // BPSCAN_H
