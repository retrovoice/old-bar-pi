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
  
  query.exec("create table categories (label varchar(15))");
  
  query.exec("create table reps ("
  "lastname varchar(25),"
  "firstname varchar(20),"
  "email varchar(40),"
  "phone varchar(20)");
  
  query.exec("create table vendors ("
  "rep varchar(25) references reps(lastname),"
  "vendorname varchar(40),"
  "vendorcode varchar(4),"
  "address varchar(30),"
  "city varchar(25),"
  "state varchar(2),"
  "zipcode varchar(10),"
  "phone varchar(20)");
  
  query.exec("create table units ( unit varchar(15))");
  
  query.exec("create table products ("
  "upccode varchar(15) primary key,"
  "label varchar(30),"
  "vendor varchar(4) references vendors(vendorcode),"
  "subfor varchar(15),"
  "category varchar(15) references categories(label),"
  "par real,"
  "price real,"
  "size real,"
  "units varchar(15) references units(unit),"
  "menuorder integer,"
  "density real)");
  
  query.exec("create table inventory ("
  "date integer primary key,"
  "upccode varchar(15) references products(upccode),"
  "quantity real)");
  
  query.exec("create table invoice ("
  "invoicenumber integer primary key,"
  "vendor varchar(4) references vendors(vendorname),"
  "date integer)");
  
  query.exec("create table invoicedetails ("
  "invoice integer references invoice(invoicenumber),"
  "product varchar(30) references products(label),"
  "price real,"
  "discount real,"
  "netprice real,"
  "quantity real)");
  
  query.exec("create table sales ("
  "begindate integer,"
  "enddate integer,"
  "product varchar(30) references products(label),"
  "quantity real)");
  
  query.exec("insert into reps values "
  "('None','','',''),"
  "('McKelvey','Kara','kmckelvey@ujbal.com','256-508-1480'),"
  "('Jones','Will','willj@international-wines.com','256-714-3157'),"
  "('Ponce','Ricardo','','256-429-8432'),"
  "('Swain','Jamie','jswain@supremebeverage.com','256-534-1482'),"
  "('Smith','Colton','CSmith@turnerbeverage.com','256-509-6596')");
  
  query.exec("insert into vendors values "
  "('McKelvey','United Johnson Brothers','UJB','','','','',''),"
  "('Jones','International Wines','INTL','','','','',''),"
  "('Ponce','Alternative Wines','ALT','','','','',''),"
  "('Swain','Supreme Beverage','SBC','5860 Endeavor Way','Tanner','AL','35671'),"
  "('Smith','Turner Beverage','TBC','','','',''),"
  "('None','ABC Wholesale','ABC','2612 Leeman Ferry Rd. SW','Huntsville','AL','35801')");
  
  query.exec("insert into categories values "
  "('red'),('white'),('rose'),('sparkling'),('cooking'),"
  "('gin'),('vodka'),('whiskey'),('rum'),('tequila'),"
  "('scotch'),('cognac'),('liqueur'),('beer'),('mixer')");
  
  query.exec("insert into units values "
  "('liters'),('milliliters'),('fluid ounces'),"
  "('ounces'),('pounds'),('grams'),('kilograms'),"
  "('each')");
  
  query.exec("insert into products values "
  "('822059000035','Dos Fincas Cabernet Sauvignon Malbec','ALT','',1,0,11.25,0.75,'liters',99,1),"
  "('7798152260351','Durigutti  Cabernet Sauvignon','ALT','',1,6,13.5,0.75,'liters',35,1),"
  "('7798152260948','Durigutti Cabernet Franc','ALT','',1,0,13.5,0.75,'liters',99,1),"
  "('736040502869','Durigutti Malbec','ALT','822059000011',1,0,13.5,0.75,'liters',99,1),"
  "('891518001838','Ermisch Pinot Noir','ALT','',1,2,26.25,0.75,'liters',44,1),"
  "('8437007453225','Fernandez Gomez','ALT','',1,0,12.75,0.75,'liters',99,1),"
  "('3760140250570','Jean Sambardier Beaujolias Villages','ALT','',1,0,12,'0.75','liters','99','1'),"
  "('891518002316','La Loyane CDR','ALT','',1,6,12.75,0.75,'liters',39,1),"
  "('891518001654','Val Beylie Bordeaux','ALT','',1,6,11.25,0.75,'liters',24,1),"
  "('822059000011','Vina Amalia Malbec','ALT','',1,6,14.25,0.75,'liters',26,1),"
  "('736040508298','Adesso','INTL','',1,2,10.5,0.75,'liters',99,1),"
  "('97985148940','Chateau Briot','INTL','',1,4,8.25,0.75,'liters',38,1),"
  "('86788018871','Hahn S-L-H Pinot Noir','INTL','',1,6,14.99,0.75,'liters',32,1),"
  "('85124859215','Landmark Pinot Noir','INTL','',1,0,15.37,0.75,'liters',99,1),"
  "('858666003107','Monte Velho Esporao','INTL','',1,0,7.86,0.75,'liters',99,1),"
  "('88110158965','Altos del Plata Malbec','UJB','',1,0,9.16,0.75,'liters',99,1),"
  "('639754000075','Amarone Luigi Righetti Classico','UJB','',1,0,12.87,0.75,'liters',47,1),"
  "('89819003471','Beringer Knights Valley Cab','UJB','',1,2,15.37,0.75,'liters',36,1),"
  "('639754000044','Campolieti Classico','UJB','',1,12,12.87,0.75,'liters',25,1),"
  "('84279992693','Canti Barolo','UJB','',1,2,27.1,0.75,'liters',43,1),"
  "('8032610551070','Cantina Ariano Primitivo','UJB','',1,0,13.82,0.75,'liters',99,1),"
  "('8032610550028','Cantina Ariano Sogno di Volpe','UJB','',1,0,1,0.75,'liters',99,1),"
  "('709793000128','Chateau Vignot','UJB','',1,2,40.37,0.75,'liters',42,1),"
  "('877397001145','Emblem Cab','UJB','',1,4,20.37,0.75,'liters',48,1),"
  "('86891083049','Federalist Zinfandel','UJB','',1,4,11.12,0.75,'liters',30,1),"
  "('633072815021','Hartford Old Vine Zinfandel','UJB','',1,2,24.87,0.75,'liters',46,1),"
  "('633072815298','Hartford Pinot Noir','UJB','',1,0,20.36,0.75,'liters',99,1),"
  "('89121188521','J. Lohr Paso Robles Merlot','UJB','',1,0,13.5,0.75,'liters',99,1),"
  "('188985000137','Joel Gott Cab','UJB','',1,12,12.36,0.75,'liters',34,1),"
  "('894713002488','Levendi Pinot Noir','UJB','',1,2,25.86,0.75,'liters',45,1),"
  "('894713002518','Levendi Pinot Noir','UJB','',1,2,25.86,0.75,'liters',45,1),"
  "('884455000453','Lucente Della Vite','UJB','',1,2,22.86,0.75,'liters',99,1),"
  "('89819705993','Lyric Pinot Noir','UJB','',1,12,12.37,0.75,'liters',31,1),"
  "('83085903053','Modus Sangiovese-Cab','UJB','',1,6,15.37,0.75,'liters',40,1),"
  "('877397000407','Oberon Cab','UJB','',1,12,15.37,0.75,'liters',37,1),"
  "('98137000598','Pepperjack Barossa','UJB','',1,0,10.37,0.75,'liters',99,1),"
  "('89819544370','Quantum','UJB','',1,4,23.37,0.75,'liters',41,1),"
  "('87241660095','Querceto Chianti','UJB','',1,12,8.87,0.75,'liters',27,1),"
  "('87447649320','R Collection Cabernet Sauvignon','UJB','',1,0,8.62,0.75,'liters',99,1),"
  "('89819045853','Stags Leap Petite Sarah','UJB','',1,4,18,0.75,'liters',29,1),"
  "('86891080178','Tamari Cab','UJB','',1,12,9.18,0.75,'liters',33,1),"
  "('86891077109','Tangley Oaks','UJB','',1,12,10.37,0.75,'liters',23,1),"
  "('799987002809','Tenuta Chianti','UJB','',1,4,12.37,0.75,'liters',28,1),"
  "('184745000195','Velvet Devil','UJB','',1,6,8.37,0.75,'liters',22,1),"
  "('86003253025','Velvet Devil','UJB','',1,6,8.37,0.75,'liters',22,1),"
  "('89419386141','Ashbourne Sauvignon Blanc Chard','ALT','',2,0,15.43,0.75,'liters',99,1),"
  "('8437007453034','Fernandez Gomez Rioja','ALT','',2,0,12.75,0.75,'liters',99,1),"
  "('376014025064','Jean Sambardier Beaujolais Blanc','ALT','',2,0,14.25,0.75,'liters',20,1),"
  "('736040012511','La Yunta Torrontes','ALT','',2,0,9.75,0.75,'liters',99,1),"
  "('796126000016','Soave DOP Classico','ALT','',2,4,11.25,0.75,'liters',13,1),"
  "('854837002047','Corvidae Reisling','INTL','768675105023',2,6,1,0.75,'liters',9,1),"
  "('810404020135','DR Reisling Dry','INTL','',2,6,9,0.75,'liters',10,1),"
  "('613649020034','Foxglove Chard','INTL','',2,6,11.25,0.75,'liters',17,1),"
  "('721170004000','Heron Chard','INTL','',2,12,10,0.75,'liters',16,1),"
  "('768675105023','North by Northwest Reisling','INTL','',2,6,6.95,0.75,'liters',9,1),"
  "('89832224273','Pratsch Gruner','INTL','',2,0,9.75,1,'liters',99,1),"
  "('799987003004','Benvolio Pinot Grigio 2017','UJB','89819116621',2,0,5.37,0.75,'liters',11,1),"
  "('8032610551261','Bombino','UJB','',2,0,7.69,0.75,'liters',99,1),"
  "('17224750123','Conundrum White Blend','UJB','',2,0,12,0.75,'liters',99,1),"
  "('83664868544','Donnafugata Anthilia','UJB','',2,0,11.36,0.75,'liters',99,1),"
  "('857488005184','Gerbeaux Pouilly-Fuisse','UJB','',2,0,17.5,0.75,'liters',99,1),"
  "('88586002632','Guado al Tasso','UJB','',2,0,15,0.75,'liters',99,1),"
  "('85000017395','Laguna Chard','UJB','',2,6,14,0.75,'liters',19,1),"
  "('894713002471','Levendi Chardonnay','UJB','',2,0,22.86,0.75,'liters',99,1),"
  "('894713002501','Levendi Chardonnay 2015','UJB','',2,0,22.86,0.75,'liters',99,1),"
  "('689076091411','Lioco Chard','UJB','',2,0,38.64,0.75,'liters',99,1),"
  "('89819545445','Luminus Chardonnay','UJB','',2,12,15.37,0.75,'liters',21,1),"
  "('89819549313','Lyric Chardonnay','UJB','',2,6,12.37,0.75,'liters',18,1),"
  "('89819116621','Promessa P.G.','UJB','',2,12,5.37,0.75,'liters',11,1),"
  "('36287003878','R Pere Macon Village','UJB','',2,0,16.11,0.75,'liters',99,1),"
  "('784585000067','Reverdy Savignon Blanc','UJB','',2,4,18.04,0.75,'liters',15,1),"
  "('89832920212','Senda Verde Godello','UJB','',2,0,9,0.75,'liters',99,1),"
  "('86891084657','Terlato','UJB','',2,6,14.37,0.75,'liters',12,1),"
  "('85000016268','Whitehaven','UJB','',2,12,11,0.75,'liters',14,1),"
  "('85000012017','Whitehaven 2','UJB','',2,12,11,0.75,'liters',14,1),"
  "('3389240013153','Cuvee Van  Gogh','ALT','',3,6,12.75,0.75,'liters',8,1),"
  "('9460000063','Montevina White Zin','UJB','',3,6,4.95,0.75,'liters',7,1),"
  "('727760501935','Bollicini Prosecco','INTL','',4,6,8.75,0.75,'liters',1,1),"
  "('84279990507','Luccio Moscato','UJB','',4,6,7.87,0.75,'liters',4,1),"
  "('86891082240','Nino Franco Prosecco','UJB','',4,6,15.37,0.75,'liters',3,1),"
  "('3049614083891','Veuve Clicquot Brut','UJB','',4,2,41.61,0.75,'liters',6,1),"
  "('850651005042','Villa Sandi Prosecco Split','UJB','',4,18,3.18,0.18,'liters',2,1),"
  "('85000001639','Wycliff','UJB','',4,6,4,0.75,'liters',5,1),"
  "('83804072404','Cribari Marsala','UJB','',5,0,1,0.75,'liters',100,1),"
  "('8553203','Fairbanks Port','UJB','',5,6,1,0.75,'liters',100,1),"
  "('8553708','Fairbanks Sherry','UJB','',5,0,1,0.75,'liters',100,1),"
  "('85000001233','Vella Burgundy','UJB','',5,0,1,5,'liters',100,1),"
  "('85000001202','Vella Chablis','UJB','',5,0,1,5,'liters',100,1),"
  "('89540333489','Beefeater','ABC','',6,0,31.49,1,'liters',300,0.89),"
  "('80480301019','Bombay Sapphire','ABC','',6,0,34.99,1,'liters',300,0.89),"
  "('83664990436','Hendricks','ABC','',6,0,35.99,0.75,'liters',300,0.9),"
  "('88110110406','Tanqueray','ABC','',6,0,32.99,1,'liters',300,0.89),"
  "('835229000407','Absolut','ABC','',7,0,25.99,1,'liters',300,0.91),"
  "('835229001404','Absolut Citron','ABC','',7,0,25.99,1,'liters',300,0.91),"
  "('87116069817','Belvedere','ABC','',7,0,39.99,1,'liters',300,0.91),"
  "('852935001153','Chopin','ABC','',7,0,26.99,0.75,'liters',300,0.91),"
  "('897291000099','Firefly','ABC','',7,0,24.95,1,'liters',300,0.94),"
  "('80480280017','Grey Goose','ABC','',7,0,41.99,1,'liters',300,0.91),"
  "('85156610419','Ketel One','ABC','',7,0,29.99,1,'liters',300,0.91),"
  "('87116019058','Prairie Organic Cucumber','ABC','',7,0,21.99,0.75,'liters',300,0.93),"
  "('82000000051','Smirnoff','ABC','',7,0,17.99,1,'liters',300,0.91),"
  "('811751020021','Stolichnaya','ABC','',7,0,28.99,1,'liters',300,0.91),"
  "('619947000013','Titos','ABC','',7,0,26.99,1,'liters',300,0.91),"
  "('850047003003','Angels Envy','ABC','',8,0,49.99,0.75,'liters',207,1),"
  "('80686012405','Basil Hayden 8 Yr.','ABC','',8,0,41.99,0.75,'liters',212,0.91),"
  "('82000760832','Bulleit 10','ABC','',8,0,49.99,0.75,'liters',208,1),"
  "('8755205','Bulleit Bourbon','ABC','',8,0,29.99,0.75,'liters',216,1),"
  "('82000752967','Bulleit Rye','ABC','',8,0,29.99,0.75,'liters',217,1),"
  "('88076168985','Bushmills','ABC','',8,0,25.99,0.75,'liters',300,1),"
  "('81128001100','Cooper's Craft','ABC','',8,0,32.99,1,'liters',300,0.91),"
  "('87000007246','Crown Royal','ABC','',8,0,36.99,1,'liters',300,0.91),"
  "('82184038734','Gentleman Jack','ABC','',8,0,38.99,1,'liters',300,1),"
  "('82184090442','Jack Daniels','ABC','',8,0,32.99,1,'liters',300,1),"
  "('80432500118','Jameson','ABC','',8,0,37.99,1,'liters',300,1),"
  "('80686001201','Jim Beam','ABC','',8,0,22.99,1,'liters',300,0.91),"
  "('80686016205','Knob Creek','ABC','',8,0,44.99,1,'liters',214,1),"
  "('85246139424','Makers Mark','ABC','',8,0,37.99,1,'liters',300,0.91),"
  "('81128005047','Old Forester','ABC','',8,0,25.99,1,'liters',300,1),"
  "('811538011877','Tin Cup','ABC','',8,0,33.99,0.75,'liters',300,1),"
  "('81128001483','Woodford Reserve','ABC','',8,0,45.99,1,'liters',215,1),"
  "('80480015305','Bacardi Superior','ABC','',9,0,18.99,1,'liters',300,1),"
  "('8771300','Meyers Dark Rum','ABC','',9,0,19.99,0.75,'liters',300,1),"
  "('811538010801','Jose Cuervo Especial','ABC','',10,0,29.49,1,'liters',300,0.91),"
  "('721733000029','Patron Silver','ABC','',10,0,57.49,0.75,'liters',300,0.91),"
  "('83300072106','Ardbeg','ABC','',11,0,52.99,0.75,'liters',202,1),"
  "('80432400432','Chivas Regal','ABC','',11,0,49.99,1,'liters',207,1),"
  "('80480230012','Dewar's White Label','ABC','',11,0,36.99,1,'liters',300,1),"
  "('83664107360','Glenficcich 12','ABC','',11,0,46.99,0.75,'liters',204,1),"
  "('80432400708','Glenlivet 12','ABC','',11,0,48.99,1,'liters',205,1),"
  "('88110011406','Johnnie Walker Black','ABC','',11,0,52.99,1,'liters',206,1),"
  "('80686813019','Lophroaig 10','ABC','',11,0,51.99,0.75,'liters',203,1),"
  "('812066020553','Macallan 12','ABC','',11,0,69.99,0.75,'liters',201,1),"
  "('80480545406','B&B','ABC','',12,0,42.99,0.75,'liters',220,1),"
  "('80686962045','Courvoisier','ABC','',12,0,35.99,0.75,'liters',221,1),"
  "('8541800','E&J Gold','ABC','',12,0,15.59,1,'liters',300,1),"
  "('85000003824','E&J VSOP','ABC','',12,0,15.99,1,'liters',300,1),"
  "('88110151058','Hennessy VSOP','ABC','',12,0,51.99,0.75,'liters',218,1),"
  "('87236001186','Remy Martin VSOP','ABC','',12,0,60.99,1,'liters',219,1),"
  "('721059001311','Aperol','ABC','',13,0,24.99,0.75,'liters',300,1),"
  "('86767210043','Baileys','ABC','',13,0,32.99,1,'liters',300,1),"
  "('721059047500','Campari','ABC','',13,0,29.99,0.75,'liters',300,1),"
  "('83300034531','Chambord Royale','ABC','',13,0,37.99,0.75,'liters',300,1),"
  "('87236565107','Cointreau','ABC','',13,0,39.99,0.75,'liters',300,1),"
  "('80480302023','Drambuie','ABC','',13,0,41.99,0.75,'liters',300,1),"
  "('721059987509','Frangelico','ABC','',13,0,27.99,0.75,'liters',300,1),"
  "('8731108','Godiva Chocolate','ABC','',13,0,31.99,0.75,'liters',300,1),"
  "('8755108','Godiva White','ABC','',13,0,31.99,0.75,'liters',300,1),"
  "('649188900483','Grand Marnier','ABC','',13,0,49.99,1,'liters',222,1),"
  "('89540122700','Kahlua','ABC','',13,0,32.49,1,'liters',300,1),"
  "('88004143541','Pimms Cup','ABC','',13,0,20.99,0.75,'liters',300,1),"
  "('86767221063','Romana Black Sambuca','ABC','',13,0,24.99,0.75,'liters',300,1),"
  "('88544018941','Southern Comfort','ABC','',13,0,20.99,1,'liters',300,1),"
  "('812164000402','Sweet Lucy','ABC','',13,0,24.99,0.75,'liters',300,1),"
  "('83300044776','Tuaca','ABC','',13,0,19.99,0.75,'liters',300,1),"
  "('866871000125','Curlews Coconut Porter','UJB','',14,10,0,0.35,'liters',50,1),"
  "('70491021464','MOM Sweet and Sour','ABC','',15,2,0,1,'liters',101,1),"
  "('74993024770','Barritts Ginger Beer','UJB','',15,0,0,0.36,'liters',101,1),"
  "('91037018519','Dirty Girl Martini Olive Juice','UJB','',15,0,1,0.75,'liters',101,1),"
  "('4124467','Martinellis Sparkling Cider','','',15,0,1,0.75,'liters',101,1)");
  
  
  enableInit(false);
}

void DatabaseDialog::enableInit(bool enable)
{
  initButton->setEnabled(enable);
}
