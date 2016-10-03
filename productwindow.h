#ifndef PRODUCTWINDOW_H
#define PRODUCTWINDOW_H

#include <QtGui>
#include <QtSql>

class ProductWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ProductWindow(QSqlRelationalTableModel *products, QWidget *parent = 0);

signals:

private slots:
    void newitem();
    void submit();
    void revert();
    void enableButtons(bool enable = true);

private:

    void createButtons();

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
