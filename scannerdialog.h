#ifndef SCANNERDIALOG_H
#define SCANNERDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QString;
class QStringList;
class QDialog;
class QLCDNumber;
class QLineEdit;
QT_END_NAMESPACE

class ScannerDialog : public QDialog
{

    Q_OBJECT
public:
    explicit ScannerDialog(QDialog *parent = 0);

public slots:
    void accept();
    void done(int r);
    void reject();

private slots:
    void startscanning();

private:

    void createLayout();

    int scanCount;
    QLCDNumber* scanCounter;
    QLineEdit*  scanValue;

};

#endif // SCANNERDIALOG_H
