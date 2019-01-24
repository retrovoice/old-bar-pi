#include <QString>
#include <QStringList>
#include <QMap>

#include "commandlineparser.h"

CommandLineParser::CommandLineParser(QStringList& options)
{
   tokenDict = new QMap<QString,QString>;
   optionList = new QStringList;
   this->addParserOptions(options);
}

CommandLineParser::~CommandLineParser()
{
   delete tokenDict;
   delete optionList;
}

void CommandLineParser::processArgvList(QStringList& argv)
{
   QStringList newArgs(argv);
   while (!newArgs.isEmpty())
   {
      QString token = newArgs.takeFirst();
      if (optionList->contains(token))
      {
         if (!newArgs.isEmpty() && !optionList->contains(newArgs.at(0)))
         {
            tokenDict->operator[](token) = newArgs.takeFirst();
         }
      }
   }
}

void CommandLineParser::addParserOptions(QStringList& opts)
{
   QStringList::const_iterator itr = opts.constBegin();
   while (itr != opts.constEnd())
   {
      if(!optionList->contains(*itr))
         optionList->append(*itr);
      itr++;
   }
}

QString CommandLineParser::optionValue(QString opt)
{
   if (tokenDict->count(opt))
      return tokenDict->operator[](opt);
   return "";
}

/*
 * 1 6   bj
 * 2 112 rng d
 * 3 54  rng d 2
 * 4 43  bj
 * 5 37  bjf
 * 6 cp  1
 * 7 112 bng d
 * 8 10  noth
 * 9 48  bj
 * 10 lg rng d 3
 * 11 lg vbj
 * 12 cp 5
 * 13 lg vbj 2
 * 14 lg bng
 * */
