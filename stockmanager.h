#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QString;
class QStringList;
class QLCDNumber;
class QLineEdit;
class QVBoxLayout;
class QSqlRelationalTableModel;
class QSqlError;
class QTableView;
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
    void createScanLayout();
    int scanCount;
    QVBoxLayout* scanLayout;
    QStringList scanList;
    QLCDNumber* scanCounter;
    QLineEdit*  scanValue;
    bool checkDB(QString barcode);

    void createCatTableLayout();
    void initCatModel();
    QTableView* createCatView(const QString &title, QSqlRelationalTableModel *model);
    // Relational database classes
    QSqlRelationalTableModel *catTableModel;
    QTableView* catTableView;

    void createInvTableLayout();
    // Relational database classes
    QSqlRelationalTableModel *invTableModel;
    QTableView* invTableView;

    QLabel*      stockLabel;
    QPushButton* addtostockButton;
    QPushButton* countstockButton;
    QPushButton* delstockButton;
    QPushButton* reportCount;
    QPushButton* reportInv;

};

#endif // STOCKMANAGER_H
