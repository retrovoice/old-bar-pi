#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

class QString;
class QStringList;
template<class K, class T>
class QMap;

class CommandLineParser
{

public:
   CommandLineParser(QStringList& options);
   virtual ~CommandLineParser();
    
   void processArgvList(QStringList &argv);
   QString optionValue(QString opt);
   const QStringList getList() { return *optionList; }
   
private:
   QMap<QString,QString> *tokenDict;
   QStringList *optionList;
   void addParserOptions(QStringList& opts);
};

#endif // COMMANDLINEPARSER_H
