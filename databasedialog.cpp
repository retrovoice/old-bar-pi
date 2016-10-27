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
    QDialog(parent)
{    
    dbName = new QString("barpi.sql");
    dbConnection = new QString("barpi");

    dbLabel = new QLabel(tr("Database &Name:"));
    dbEdit = new QLineEdit;
    dbLabel->setBuddy(hostEdit);

    hostLabel = new QLabel(tr("&Hostname:"));
    hostEdit = new QLineEdit;
    hostLabel->setBuddy(hostEdit);

    userLabel = new QLabel(tr("&Username:"));
    userEdit = new QLineEdit;
    userLabel->setBuddy(userEdit);

    passwordLabel = new QLabel(tr("&Password:"));
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    passwordLabel->setBuddy(passwordEdit);

    typeLabel = new QLabel(tr("Database Type:"));
    typeCombo = new QComboBox();

    QStringList dbTypes;
    dbTypes = QSqlDatabase::drivers();
    for (int i = 0; i < dbTypes.size(); i++)
    {
        typeCombo->addItem(dbTypes.at(i));
    }

    testButton = new QPushButton(tr("&Test"));
    initButton = new QPushButton(tr("&Initialize"));
    closeButton = new QPushButton(tr("Close"));

    testButton->setEnabled(false);
    initButton->setEnabled(false);
    closeButton->setEnabled(true);

    connect(testButton, SIGNAL(clicked()), this, SLOT(testConnection()));
    connect(initButton, SIGNAL(clicked()), this, SLOT(initdb()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    bBox = new QDialogButtonBox(this);
    bBox->addButton(testButton, QDialogButtonBox::ActionRole);
    bBox->addButton(initButton, QDialogButtonBox::AcceptRole);
    bBox->addButton(closeButton, QDialogButtonBox::RejectRole);

    connect(dbEdit, SIGNAL(textEdited(QString)), this, SLOT(enablebuttons()));
    connect(hostEdit, SIGNAL(textEdited(QString)), this, SLOT(enablebuttons()));
    connect(userEdit, SIGNAL(textEdited(QString)), this, SLOT(enablebuttons()));
    connect(passwordEdit, SIGNAL(textEdited(QString)), this, SLOT(enablebuttons()));

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

    enablebuttons(true);
}

void DatabaseDialog::testConnection()
{
    QString dbType = typeCombo->itemText(typeCombo->currentIndex());
    QString hostname(hostEdit->text());
    QString username(userEdit->text());
    QString password(passwordEdit->text());

    QSqlDatabase db;
    if (QSqlDatabase::contains(*dbConnection))
    {
        QMessageBox::information(this, tr("Contains Connection"),*dbConnection);
        db = QSqlDatabase::database(*dbConnection);
        dbName->clear();
        dbName->append(db.databaseName());
        dbEdit->insert(*dbName);
        db.setDatabaseName(dbEdit->text());
    }
    else
    {
        QMessageBox::information(this, tr("Connection Added"),*dbConnection);
        db = QSqlDatabase::addDatabase(dbType,*dbConnection);
        db.setDatabaseName(dbEdit->text());
    }

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
    }
}

void DatabaseDialog::initdb()
{
    QString dbType = typeCombo->itemText(typeCombo->currentIndex());
    QString hostname(hostEdit->text());
    QString username(userEdit->text());
    QString password(passwordEdit->text());

    QSqlDatabase db;
    if (QSqlDatabase::contains(*dbConnection))
    {
        QMessageBox::information(this, tr("Contains Connection"),*dbConnection);
        db = QSqlDatabase::database(*dbConnection);
        dbName->clear();
        dbName->append(db.databaseName());
        dbEdit->insert(*dbName);
        db.setDatabaseName(dbEdit->text());
    }
    else
    {
        QMessageBox::information(this, tr("Connection Added"),*dbConnection);
        db = QSqlDatabase::addDatabase(dbType,*dbConnection);
        db.setDatabaseName(dbEdit->text());
    }

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
        QSqlQuery query(db);
        query.exec("create table categories (id integer primary key, label varchar(15))");
        query.exec("create table products ("
                   "upccode varchar(15) primary key,"
                   "label varchar(30),"
                   "abccode varchar(8) unique,"
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
        msg.append(db.lastError().text());
        QMessageBox::information(this, tr("Database Initialized"),msg);
    }
}

void DatabaseDialog::enablebuttons(bool enable)
{
    initButton->setEnabled(enable);
    testButton->setEnabled(enable);
}
