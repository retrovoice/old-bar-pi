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
  dbLabel->setBuddy(dbEdit);
  
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
    QMessageBox::critical(this, tr("Error"), error, QMessageBox::Cancel);
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
  
  //query.exec("PRAGMA foreign_keys=ON");
  
  query.exec("create table categories (id integer primary key, label varchar(15))");
  
  query.exec("create table reps ("
  "id integer primary key,"
  "lastname varchar(25),"
  "firstname varchar(20),"
  "email varchar(40),"
  "phone varchar(20))");
  
  query.exec("create table vendors ("
  "id integer primary key,"
  "rep varchar(25) references reps(id),"
  "vendorname varchar(40),"
  "vendorcode varchar(4),"
  "address varchar(30),"
  "city varchar(25),"
  "state varchar(2),"
  "zipcode varchar(10),"
  "phone varchar(20))");
  
  query.exec("create table units (id integer primary key, unit varchar(15))");
  
  query.exec("create table products ("
  "upccode varchar(15) primary key,"
  "label varchar(30),"
  "vendor varchar(4) references vendors(id),"
  "subfor varchar(15),"
  "category varchar(15) references categories(id),"
  "par real,"
  "price real,"
  "size real,"
  "units varchar(15) references units(id),"
  "menuorder integer,"
  "density real)");
  
  query.exec("create table inventory ("
  "id integer primary key,"
  "date integer,"
  "upccode varchar(15) references products(upccode),"
  "quantity real)");
  
  query.exec("create table invoice ("
  "invoicenumber integer primary key,"
  "vendor varchar(40) references vendorname(id),"
  "date integer)");
  
  query.exec("create table invoicedetails ("
  "id integer primary key,"
  "invoice integer references invoice(invoicenumber),"
  "product varchar(30) references products(upccode),"
  "quantity real,"
  "price real,"
  "discount real,"
  "netprice real)"
  );
  
  query.exec("create table sales ("
  "id integer primary key,"
  "date integer,"
  "product varchar(30) references products(upccode),"
  "quantity real,"
  "netsales real,"
  "units varchar(15) references units(id))");
  
  query.exec("insert into reps values "
  "(NULL,'None',NULL,NULL,NULL),"
  "(NULL,'McKelvey','Kara','kmckelvey@ujbal.com','256-508-1480'),"
  "(NULL,'Jones','Will','willj@international-wines.com','256-714-3157'),"
  "(NULL,'Ponce','Ricardo',NULL,'256-429-8432'),"
  "(NULL,'Swain','Jamie','jswain@supremebeverage.com','256-534-1482'),"
  "(NULL,'Smith','Colton','CSmith@turnerbeverage.com','256-509-6596')");
  
  query.exec("insert into vendors values "
  "(NULL,2,'United Johnson Brothers','UJB',NULL,NULL,NULL,NULL,NULL),"
  "(NULL,3,'International Wines','INTL',NULL,NULL,NULL,NULL,NULL),"
  "(NULL,4,'Alternative Wines','ALT',NULL,NULL,NULL,NULL,NULL),"
  "(NULL,5,'Supreme Beverage','SBC','5860 Endeavor Way','Tanner','AL','35671',NULL),"
  "(NULL,6,'Turner Beverage','TBC',NULL,NULL,NULL,NULL,NULL),"
  "(NULL,1,'ABC Wholesale','ABC','2612 Leeman Ferry Rd. SW','Huntsville','AL','35801',NULL)");
  
  query.exec("insert into categories values "
  "(NULL,'red'),(NULL,'white'),(NULL,'rose'),(NULL,'sparkling'),(NULL,'cooking'),"
  "(NULL,'gin'),(NULL,'vodka'),(NULL,'whiskey'),(NULL,'rum'),(NULL,'tequila'),"
  "(NULL,'scotch'),(NULL,'cognac'),(NULL,'liqueur'),(NULL,'beer'),(NULL,'mixer')");
  
  query.exec("insert into units values "
  "(NULL,'liters'),(NULL,'milliliters'),(NULL,'fluid ounces'),"
  "(NULL,'ounces'),(NULL,'pounds'),(NULL,'grams'),(NULL,'kilograms'),"
  "(NULL,'each')");
  
  //query.exec("insert into invoice values "
  //"(1,0,
  
  query.exec("insert into products values "
  "('822059000035','Dos Fincas Cabernet Sauvignon Malbec',3,'',1,0,11.25,0.75,1,99,1),"
  "('7798152260351','Durigutti Cabernet Sauvignon',3,'',1,6,13.5,0.75,1,35,1),"
  "('7798152260948','Durigutti Cabernet Franc',3,'',1,0,13.5,0.75,1,99,1),"
  "('736040502869','Durigutti Malbec',3,'822059000011',1,0,13.5,0.75,1,99,1),"
  "('891518001838','Ermisch Pinot Noir',3,'',1,2,26.25,0.75,1,44,1),"
  "('8437007453225','Fernandez Gomez',3,'',1,0,12.75,0.75,1,99,1),"
  "('3760140250570','Jean Sambardier Beaujolias Villages',3,'',1,0,12,'0.75',1,'99','1'),"
  "('891518002316','La Loyane CDR',3,'',1,6,12.75,0.75,1,39,1),"
  "('891518001654','Val Beylie Bordeaux',3,'',1,6,11.25,0.75,1,24,1),"
  "('822059000011','Vina Amalia Malbec',3,'',1,6,14.25,0.75,1,26,1),"
  "('736040508298','Adesso',2,'',1,2,10.5,0.75,1,99,1),"
  "('97985148940','Chateau Briot',2,'',1,4,8.25,0.75,1,38,1),"
  "('86788018871','Hahn S-L-H Pinot Noir',2,'',1,6,14.99,0.75,1,32,1),"
  "('85124859215','Landmark Pinot Noir',2,'',1,0,15.37,0.75,1,99,1),"
  "('858666003107','Monte Velho Esporao',2,'',1,0,7.86,0.75,1,99,1),"
  "('88110158965','Altos del Plata Malbec',1,'',1,0,9.16,0.75,1,99,1),"
  "('639754000075','Amarone Luigi Righetti Classico',1,'',1,0,12.87,0.75,1,47,1),"
  "('89819003471','Beringer Knights Valley Cab',1,'',1,2,15.37,0.75,1,36,1),"
  "('639754000044','Campolieti Classico',1,'',1,12,12.87,0.75,1,25,1),"
  "('84279992693','Canti Barolo',1,'',1,2,27.1,0.75,1,43,1),"
  "('8032610551070','Cantina Ariano Primitivo',1,'',1,0,13.82,0.75,1,99,1),"
  "('8032610550028','Cantina Ariano Sogno di Volpe',1,'',1,0,1,0.75,1,99,1),"
  "('709793000128','Chateau Vignot',1,'',1,2,40.37,0.75,1,42,1),"
  "('877397001145','Emblem Cab',1,'',1,4,20.37,0.75,1,48,1),"
  "('86891083049','Federalist Zinfandel',1,'',1,4,11.12,0.75,1,30,1),"
  "('633072815021','Hartford Old Vine Zinfandel',1,'',1,2,24.87,0.75,1,46,1),"
  "('633072815298','Hartford Pinot Noir',1,'',1,0,20.36,0.75,1,99,1),"
  "('89121188521','J. Lohr Paso Robles Merlot',1,'',1,0,13.5,0.75,1,99,1),"
  "('188985000137','Joel Gott Cab',1,'',1,12,12.36,0.75,1,34,1),"
  "('894713002488','Levendi Pinot Noir',1,'',1,2,25.86,0.75,1,45,1),"
  "('894713002518','Levendi Pinot Noir',1,'',1,2,25.86,0.75,1,45,1),"
  "('884455000453','Lucente Della Vite',1,'',1,2,22.86,0.75,1,99,1),"
  "('89819705993','Lyric Pinot Noir',1,'',1,12,12.37,0.75,1,31,1),"
  "('83085903053','Modus Sangiovese-Cab',1,'',1,6,15.37,0.75,1,40,1),"
  "('877397000407','Oberon Cab',1,'',1,12,15.37,0.75,1,37,1),"
  "('98137000598','Pepperjack Barossa',1,'',1,0,10.37,0.75,1,99,1),"
  "('89819544370','Quantum',1,'',1,4,23.37,0.75,1,41,1),"
  "('87241660095','Querceto Chianti',1,'',1,12,8.87,0.75,1,27,1),"
  "('87447649320','R Collection Cabernet Sauvignon',1,'',1,0,8.62,0.75,1,99,1),"
  "('89819045853','Stags Leap Petite Sarah',1,'',1,4,18,0.75,1,29,1),"
  "('86891080178','Tamari Cab',1,'',1,12,9.18,0.75,1,33,1),"
  "('86891077109','Tangley Oaks',1,'',1,12,10.37,0.75,1,23,1),"
  "('799987002809','Tenuta Chianti',1,'',1,4,12.37,0.75,1,28,1),"
  "('184745000195','Velvet Devil',1,'',1,6,8.37,0.75,1,22,1),"
  "('86003253025','Velvet Devil',1,'',1,6,8.37,0.75,1,22,1),"
  "('89419386141','Ashbourne Sauvignon Blanc Chard',3,'',2,0,15.43,0.75,1,99,1),"
  "('8437007453034','Fernandez Gomez Rioja',3,'',2,0,12.75,0.75,1,99,1),"
  "('376014025064','Jean Sambardier Beaujolais Blanc',3,'',2,0,14.25,0.75,1,20,1),"
  "('736040012511','La Yunta Torrontes',3,'',2,0,9.75,0.75,1,99,1),"
  "('796126000016','Soave DOP Classico',3,'',2,4,11.25,0.75,1,13,1),"
  "('854837002047','Corvidae Reisling',2,'768675105023',2,6,1,0.75,1,9,1),"
  "('810404020135','DR Reisling Dry',2,'',2,6,9,0.75,1,10,1),"
  "('613649020034','Foxglove Chard',2,'',2,6,11.25,0.75,1,17,1),"
  "('721170004000','Heron Chard',2,'',2,12,10,0.75,1,16,1),"
  "('768675105023','North by Northwest Reisling',2,'',2,6,6.95,0.75,1,9,1),"
  "('89832224273','Pratsch Gruner',2,'',2,0,9.75,1,1,99,1),"
  "('799987003004','Benvolio Pinot Grigio 2017',1,'89819116621',2,0,5.37,0.75,1,11,1),"
  "('8032610551261','Bombino',1,'',2,0,7.69,0.75,1,99,1),"
  "('17224750123','Conundrum White Blend',1,'',2,0,12,0.75,1,99,1),"
  "('83664868544','Donnafugata Anthilia',1,'',2,0,11.36,0.75,1,99,1),"
  "('857488005184','Gerbeaux Pouilly-Fuisse',1,'',2,0,17.5,0.75,1,99,1),"
  "('88586002632','Guado al Tasso',1,'',2,0,15,0.75,1,99,1),"
  "('85000017395','Laguna Chard',1,'',2,6,14,0.75,1,19,1),"
  "('894713002471','Levendi Chardonnay',1,'',2,0,22.86,0.75,1,99,1),"
  "('894713002501','Levendi Chardonnay 2015',1,'',2,0,22.86,0.75,1,99,1),"
  "('689076091411','Lioco Chard',1,'',2,0,38.64,0.75,1,99,1),"
  "('89819545445','Luminus Chardonnay',1,'',2,12,15.37,0.75,1,21,1),"
  "('89819549313','Lyric Chardonnay',1,'',2,6,12.37,0.75,1,18,1),"
  "('89819116621','Promessa P.G.',1,'',2,12,5.37,0.75,1,11,1),"
  "('36287003878','R Pere Macon Village',1,'',2,0,16.11,0.75,1,99,1),"
  "('784585000067','Reverdy Savignon Blanc',1,'',2,4,18.04,0.75,1,15,1),"
  "('89832920212','Senda Verde Godello',1,'',2,0,9,0.75,1,99,1),"
  "('86891084657','Terlato',1,'',2,6,14.37,0.75,1,12,1),"
  "('85000016268','Whitehaven',1,'',2,12,11,0.75,1,14,1),"
  "('85000012017','Whitehaven 2',1,'',2,12,11,0.75,1,14,1),"
  "('3389240013153','Cuvee Van  Gogh',3,'',3,6,12.75,0.75,1,8,1),"
  "('9460000063','Montevina White Zin',1,'',3,6,4.95,0.75,1,7,1),"
  "('727760501935','Bollicini Prosecco',2,'',4,6,8.75,0.75,1,1,1),"
  "('84279990507','Luccio Moscato',1,'',4,6,7.87,0.75,1,4,1),"
  "('86891082240','Nino Franco Prosecco',1,'',4,6,15.37,0.75,1,3,1),"
  "('3049614083891','Veuve Clicquot Brut',1,'',4,2,41.61,0.75,1,6,1),"
  "('850651005042','Villa Sandi Prosecco Split',1,'',4,18,3.18,0.18,1,2,1),"
  "('85000001639','Wycliff',1,'',4,6,4,0.75,1,5,1),"
  "('83804072404','Cribari Marsala',1,'',5,0,1,0.75,1,100,1),"
  "('8553203','Fairbanks Port',1,'',5,6,1,0.75,1,100,1),"
  "('8553708','Fairbanks Sherry',1,'',5,0,1,0.75,1,100,1),"
  "('85000001233','Vella Burgundy',1,'',5,0,1,5,1,100,1),"
  "('85000001202','Vella Chablis',1,'',5,0,1,5,1,100,1),"
  "('89540333489','Beefeater',6,'',6,0,31.49,1,1,300,0.89),"
  "('80480301019','Bombay Sapphire',6,'',6,0,34.99,1,1,300,0.89),"
  "('83664990436','Hendricks',6,'',6,0,35.99,0.75,1,300,0.9),"
  "('88110110406','Tanqueray',6,'',6,0,32.99,1,1,300,0.89),"
  "('835229000407','Absolut',6,'',7,0,25.99,1,1,300,0.91),"
  "('835229001404','Absolut Citron',6,'',7,0,25.99,1,1,300,0.91),"
  "('87116069817','Belvedere',6,'',7,0,39.99,1,1,300,0.91),"
  "('852935001153','Chopin',6,'',7,0,26.99,0.75,1,300,0.91),"
  "('897291000099','Firefly',6,'',7,0,24.95,1,1,300,0.94),"
  "('80480280017','Grey Goose',6,'',7,0,41.99,1,1,300,0.91),"
  "('85156610419','Ketel One',6,'',7,0,29.99,1,1,300,0.91),"
  "('87116019058','Prairie Organic Cucumber',6,'',7,0,21.99,0.75,1,300,0.93),"
  "('82000000051','Smirnoff',6,'',7,0,17.99,1,1,300,0.91),"
  "('811751020021','Stolichnaya',6,'',7,0,28.99,1,1,300,0.91),"
  "('619947000013','Titos',6,'',7,0,26.99,1,1,300,0.91),"
  "('850047003003','Angels Envy',6,'',8,0,49.99,0.75,1,207,1),"
  "('80686012405','Basil Hayden 8 Yr.',6,'',8,0,41.99,0.75,1,212,0.91),"
  "('82000760832','Bulleit 10',6,'',8,0,49.99,0.75,1,208,1),"
  "('8755205','Bulleit Bourbon',6,'',8,0,29.99,0.75,1,216,1),"
  "('82000752967','Bulleit Rye',6,'',8,0,29.99,0.75,1,217,1),"
  "('88076168985','Bushmills',6,'',8,0,25.99,0.75,1,300,1),"
  "('81128001100','Cooper''s Craft',6,'',8,0,32.99,1,1,300,0.91),"
  "('87000007246','Crown Royal',6,'',8,0,36.99,1,1,300,0.91),"
  "('82184038734','Gentleman Jack',6,'',8,0,38.99,1,1,300,1),"
  "('82184090442','Jack Daniels',6,'',8,0,32.99,1,1,300,1),"
  "('80432500118','Jameson',6,'',8,0,37.99,1,1,300,1),"
  "('80686001201','Jim Beam',6,'',8,0,22.99,1,1,300,0.91),"
  "('80686016205','Knob Creek',6,'',8,0,44.99,1,1,214,1),"
  "('85246139424','Makers Mark',6,'',8,0,37.99,1,1,300,0.91),"
  "('81128005047','Old Forester',6,'',8,0,25.99,1,1,300,1),"
  "('811538011877','Tin Cup',6,'',8,0,33.99,0.75,1,300,1),"
  "('81128001483','Woodford Reserve',6,'',8,0,45.99,1,1,215,1),"
  "('80480015305','Bacardi Superior',6,'',9,0,18.99,1,1,300,1),"
  "('8771300','Meyers Dark Rum',6,'',9,0,19.99,0.75,1,300,1),"
  "('811538010801','Jose Cuervo Especial',6,'',10,0,29.49,1,1,300,0.91),"
  "('721733000029','Patron Silver',6,'',10,0,57.49,0.75,1,300,0.91),"
  "('83300072106','Ardbeg',6,'',11,0,52.99,0.75,1,202,1),"
  "('80432400432','Chivas Regal',6,'',11,0,49.99,1,1,207,1),"
  "('80480230012','Dewar''s White Label',6,'',11,0,36.99,1,1,300,1),"
  "('83664107360','Glenficcich 12',6,'',11,0,46.99,0.75,1,204,1),"
  "('80432400708','Glenlivet 12',6,'',11,0,48.99,1,1,205,1),"
  "('88110011406','Johnnie Walker Black',6,'',11,0,52.99,1,1,206,1),"
  "('80686813019','Lophroaig 10',6,'',11,0,51.99,0.75,1,203,1),"
  "('812066020553','Macallan 12',6,'',11,0,69.99,0.75,1,201,1),"
  "('80480545406','B&B',6,'',12,0,42.99,0.75,1,220,1),"
  "('80686962045','Courvoisier',6,'',12,0,35.99,0.75,1,221,1),"
  "('8541800','E&J Gold',6,'',12,0,15.59,1,1,300,1),"
  "('85000003824','E&J VSOP',6,'',12,0,15.99,1,1,300,1),"
  "('88110151058','Hennessy VSOP',6,'',12,0,51.99,0.75,1,218,1),"
  "('87236001186','Remy Martin VSOP',6,'',12,0,60.99,1,1,219,1),"
  "('721059001311','Aperol',6,'',13,0,24.99,0.75,1,300,1),"
  "('86767210043','Baileys',6,'',13,0,32.99,1,1,300,1),"
  "('721059047500','Campari',6,'',13,0,29.99,0.75,1,300,1),"
  "('83300034531','Chambord Royale',6,'',13,0,37.99,0.75,1,300,1),"
  "('87236565107','Cointreau',6,'',13,0,39.99,0.75,1,300,1),"
  "('80480302023','Drambuie',6,'',13,0,41.99,0.75,1,300,1),"
  "('721059987509','Frangelico',6,'',13,0,27.99,0.75,1,300,1),"
  "('8731108','Godiva Chocolate',6,'',13,0,31.99,0.75,1,300,1),"
  "('8755108','Godiva White',6,'',13,0,31.99,0.75,1,300,1),"
  "('649188900483','Grand Marnier',6,'',13,0,49.99,1,1,222,1),"
  "('89540122700','Kahlua',6,'',13,0,32.49,1,1,300,1),"
  "('88004143541','Pimms Cup',6,'',13,0,20.99,0.75,1,300,1),"
  "('86767221063','Romana Black Sambuca',6,'',13,0,24.99,0.75,1,300,1),"
  "('88544018941','Southern Comfort',6,'',13,0,20.99,1,1,300,1),"
  "('812164000402','Sweet Lucy',6,'',13,0,24.99,0.75,1,300,1),"
  "('83300044776','Tuaca',6,'',13,0,19.99,0.75,1,300,1),"
  "('866871000125','Curlews Coconut Porter',1,'',14,10,0,0.35,1,50,1),"
  "('70491021464','MOM Sweet and Sour',6,'',15,2,0,1,1,101,1),"
  "('74993024770','Barritts Ginger Beer',1,'',15,0,0,0.36,1,101,1),"
  "('91037018519','Dirty Girl Martini Olive Juice',1,'',15,0,1,0.75,1,101,1),"
  "('4124467','Martinellis Sparkling Cider',NULL,'',15,0,1,0.75,1,101,1)");  

  enableInit(false);
}

void DatabaseDialog::enableInit(bool enable)
{
  initButton->setEnabled(enable);
}
