#ifndef STOCKMANAGER_H
#define STOCKMANAGER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QString;
class QStringList;
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

private:

    QStringList scanList;

    bool inCatalog(QString barcode);
    void createLayout();

    QLabel*      addtostockLabel;
    QLabel*      countstockLabel;

    QPushButton* addtostockButton;
    QPushButton* countstockButton;
};

#endif // STOCKMANAGER_H
