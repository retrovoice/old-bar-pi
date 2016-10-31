#ifndef DATABASEDIALOG_H
#define DATABASEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QStringList;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QDialogButtonBox;
QT_END_NAMESPACE

class DatabaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DatabaseDialog(QWidget *parent = 0);

signals:
    
public slots:

    void testConnection();
    void initdb();
    void enablebuttons(bool enable = true);

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
    QPushButton *closeButton;
    QDialogButtonBox *bBox;

    QString *dbconn;

};

#endif // DATABASEDIALOG_H
