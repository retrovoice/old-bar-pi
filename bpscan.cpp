#include "bpscan.h"
#include <QString>
#include <QTextStream>

BPScan::BPScan(QObject *parent):
    QThread(parent)
{
}

BPScan::~BPScan()
{
}

void BPScan::run()
{
    QTextStream stream(stdin);
    return;
}
