#include "bpscan.h"
#include <QString>
//#include <QTextStream>
//#include <iostream>

BPScan::BPScan(QObject *parent):
    QThread(parent)
{
    barcode = new QString;
}

BPScan::~BPScan()
{
    wait();
}

void BPScan::run()
{
    //QTextStream stream(stdin);
    barcode->append("0082184090008");
    //std::cout << msg.toStdString();
    emit barcodeRead(*barcode);
    return;
}
