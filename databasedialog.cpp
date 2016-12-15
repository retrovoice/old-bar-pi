#include "databasedialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QStringList>
#include <QString>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlError>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSqlQuery>

DatabaseDialog::DatabaseDialog(QWidget *parent) :
    QWidget(parent)
{    
    QSqlDatabase db = QSqlDatabase::database();

    dbLabel = new QLabel(tr("Database Name:"));
    dbEdit = new QLineEdit;
    dbEdit->insert(db.databaseName());
    dbLabel->setBuddy(hostEdit);

    hostLabel = new QLabel(tr("&Hostname:"));
    hostEdit = new QLineEdit;
    hostEdit->insert(db.hostName());
    hostLabel->setBuddy(hostEdit);

    userLabel = new QLabel(tr("&Username:"));
    userEdit = new QLineEdit;
    userEdit->insert(db.userName());
    userLabel->setBuddy(userEdit);

    passwordLabel = new QLabel(tr("&Password:"));
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    passwordEdit->insert(db.password());
    passwordLabel->setBuddy(passwordEdit);

    typeLabel = new QLabel(tr("Database Type:"));
    typeCombo = new QComboBox();

    QStringList dbTypes;
    dbTypes = QSqlDatabase::drivers();
    for (int i = 0; i < dbTypes.size(); i++)
    {
        typeCombo->addItem(dbTypes.at(i));
    }
    int cbindex = typeCombo->findText(db.driverName());
    if (cbindex > -1)
    {
        typeCombo->setCurrentIndex(cbindex);
    }


    testButton = new QPushButton(tr("&Test"));
    initButton = new QPushButton(tr("&Initialize"));

    testButton->setEnabled(true);
    initButton->setEnabled(false);

    connect(testButton, SIGNAL(clicked()), this, SLOT(testConnection()));
    connect(initButton, SIGNAL(clicked()), this, SLOT(initdb()));

    bBox = new QDialogButtonBox(this);
    bBox->addButton(testButton, QDialogButtonBox::ActionRole);
    bBox->addButton(initButton, QDialogButtonBox::AcceptRole);

    QFormLayout *layout = new QFormLayout;
    layout->addRow(typeLabel,typeCombo);
    layout->addRow(dbLabel,dbEdit);
    layout->addRow(hostLabel,hostEdit);
    layout->addRow(userLabel,userEdit);
    layout->addRow(passwordLabel,passwordEdit);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(layout);
    vlayout->addWidget(bBox);
    setLayout(vlayout);
    setWindowTitle(tr("Database Setup"));

    enableInit(false);
}

void DatabaseDialog::testConnection()
{
    QString dbType = typeCombo->itemText(typeCombo->currentIndex());
    QString dbName(dbEdit->text());
    QString hostname(hostEdit->text());
    QString username(userEdit->text());
    QString password(passwordEdit->text());

    QSqlDatabase db = QSqlDatabase::addDatabase(dbType,"dbtest");
    db.setDatabaseName(dbName);

    if (hostname.size()) db.setHostName(hostname);
    if (username.size()) db.setUserName(username);
    if (password.size()) db.setPassword(password);

    if (!db.open())
    {
        QString error("Database open failed.");
        error.append(db.lastError().text());
        QMessageBox::critical(this, tr("Error"),
                              error, QMessageBox::Cancel);
        return;
    }
    else
    {
        QMessageBox::information(this, tr("Success!"),tr("Database successfully opened"));
        enableInit();
    }
}

void DatabaseDialog::initdb()
{
    QSqlQuery query;
    query.exec("create table categories (id integer primary key, label varchar(15))");
    query.exec("create table products ("
               "upccode varchar(15) primary key,"
               "label varchar(30),"
               "abccode varchar(8),"
               "price real,"
               "category integer references categories(id),"
               "volume real,"
               "density real)");
    query.exec("create table inventory ("
               "id integer primary key autoincrement,"
               "upc varchar(15) references products(upccode),"
               "barcode varchar(30) unique,"
               "retired boolean,"
               "gross real,"
               "tare  real,"
               "arrival timestamp,"
               "departure timestamp )");
    query.exec("create table readings ("
               "id integer primary key autoincrement,"
               "item varchar(30),"
               "stamp timestamp,"
               "weight real )");

    query.exec("insert into categories values (1,'bourbon')");
    query.exec("insert into categories values (2,'brandy')");
    query.exec("insert into categories values (3,'cognac')");
    query.exec("insert into categories values (4,'gin')");
    query.exec("insert into categories values (5,'liqueur')");
    query.exec("insert into categories values (6,'rum')");
    query.exec("insert into categories values (7,'scotch')");
    query.exec("insert into categories values (8,'tequila')");
    query.exec("insert into categories values (9,'vodka')");
    query.exec("insert into categories values (10,'whiskey')");

    query.exec("insert into products values ("
               "'0082184090008', 'Jack Daniels No. 7',"
               "'E305', 31.49, 10, 1.0, 0.916)");
    query.exec("insert into products values ("
               "'0830895501098', 'Grey Goose',"
               "'E1400', 39.99, 9, 1.0, 0.916)");
    query.exec("insert into products values ("
               "'0083664868780', 'Hendricks',"
               "'A723',35.99, 4, 0.75, 0.9076)");

    QString msg("Last reported error: ");
    msg.append(query.lastError().text());
    QMessageBox::information(this, tr("Database Initialized"),msg);
    enableInit(false);
}

void DatabaseDialog::enableInit(bool enable)
{
    initButton->setEnabled(enable);
}
