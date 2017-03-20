#ifndef CATALOG_H
#define CATALOG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStringList;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QDialogButtonBox;
class QDataWidgetMapper;
class QSqlRelationalTableModel;
class QSqlError;
QT_END_NAMESPACE

class Catalog : public QWidget
{
    Q_OBJECT
public:
    explicit Catalog(QWidget *parent = 0);

signals:

private slots:
    void newitem();
    void submit();
    void cancel();
    void previous();
    void next();
    void remove();
    void updateButtons(const bool st = TRUE);
    void updateNextPrev(int row);

private:

    void initModel();
    void mapModel();
    void createLayout();
    void showError(const QSqlError &err);

    // Relational table model used to display product catalog
    QSqlRelationalTableModel *prodTableModel;

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
    QDataWidgetMapper *mapper;

    QPushButton *newButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QPushButton *deleteButton;
    QPushButton *prevButton;
    QPushButton *nextButton;

    // Flag to determine is adding record to database
    bool isNew;
    // An index used to keep track of current record in the database
    int spot;
};

#endif // PRODUCTWINDOW_H
