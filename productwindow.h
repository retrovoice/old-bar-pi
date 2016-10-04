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
    void writenewrecord();
    void enablesavenew(bool enable = true);
    void submit();
    void revert();
    void enableButtons(bool enable = true);

private:

    void createButtons();

    // Widgets for Products table UI
    QLabel *upcLabel;
    QLabel *nameLabel;
    QLabel *abcCodeLabel;
    QLabel *priceLabel;
    QLabel *volumeLabel;
    QLabel *categoryLabel;
    QLabel *densityLabel;
    QLineEdit *upcEdit;
    QLineEdit *nameEdit;
    QLineEdit *abcCodeEdit;
    QLineEdit *priceEdit;
    QLineEdit *volumeEdit;
    QLineEdit *densityEdit;
    QComboBox *categoryCombo;
    QPushButton *newButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *closeButton;
    QDataWidgetMapper *mapper;
    QDialogButtonBox *buttonBox;

    // Widgets for adding new Product record
    // to database
    QDialog *newProductDialog;
    QStringList *productCategories;
    QLabel *newUpcLabel;
    QLabel *newNameLabel;
    QLabel *newAbcCodeLabel;
    QLabel *newPriceLabel;
    QLabel *newVolumeLabel;
    QLabel *newCategoryLabel;
    QLabel *newDensityLabel;
    QLineEdit *newUpcEdit;
    QLineEdit *newNameEdit;
    QLineEdit *newAbcCodeEdit;
    QLineEdit *newPriceEdit;
    QLineEdit *newVolumeEdit;
    QLineEdit *newDensityEdit;
    QComboBox *newCategoryCombo;
    QPushButton *newSaveButton;
    QPushButton *newCancelButton;
    QDialogButtonBox *newButtonBox;

};

#endif // PRODUCTWINDOW_H
