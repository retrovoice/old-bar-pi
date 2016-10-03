#ifndef PRODUCTWINDOW_H
#define PRODUCTWINDOW_H

#include <QtGui>
#include <QtSql>

class ProductWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ProductWindow(int id, QSqlRelationalTableModel *products, QWidget *parent = 0);

    int id();
    
signals:
    //void labelChanged(QString upc, const QString &label);
    //void abccodeChanged(QString upc, const QString &abccode);
    //void volumeChanged(QString upc, const QVariant &volume);
    //void densityChanged(QString upc, const QVariant &density);
    //void categoryChanged(QString upc, const QString &category);
    void infoChanged(int id, const QVariant &info);

private slots:
    void newitem();
    void submit();
    void revert();
    void enableButtons(bool enable = true);

private:

    void createButtons();

    int m_id;

    // Widgets for Products table UI
    QVariant *info;
    QLabel *upcLabel;
    QLabel *nameLabel;
    QLabel *abcCodeLabel;
    QLabel *volumeLabel;
    QLabel *categoryLabel;
    QLabel *densityLabel;
    QLineEdit *upcEdit;
    QLineEdit *nameEdit;
    QLineEdit *abcCodeEdit;
    QLineEdit *volumeEdit;
    QLineEdit *densityEdit;
    QComboBox *categoryCombo;
    QPushButton *newButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *closeButton;
    QDataWidgetMapper *mapper;
    QDialogButtonBox *buttonBox;
};

#endif // PRODUCTWINDOW_H
