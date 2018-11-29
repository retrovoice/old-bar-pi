#ifndef DATABASEDIALOG_H
#define DATABASEDIALOG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStringList;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QDialogButtonBox;
QT_END_NAMESPACE

class DatabaseDialog : public QWidget
{
    Q_OBJECT
public:
    explicit DatabaseDialog(QWidget *parent = 0);

signals:
    
public slots:

    void testConnection();
    void initdb();
    void enableInit(bool enable = true);

private:
    QLabel *dbLabel;
    QLabel *hostLabel;
    QLabel *userLabel;
    QLabel *passwordLabel;
    QLabel *typeLabel;
    QLineEdit *dbEdit;
    QLineEdit *hostEdit;
    QLineEdit *userEdit;
    QLineEdit *passwordEdit;
    QComboBox *typeCombo;
    QPushButton *testButton;
    QPushButton *initButton;
    QDialogButtonBox *bBox;
};

#endif // DATABASEDIALOG_H
