/****************************************************************************
** Released under the DoD Community Source Usage Agreement Version 1.1
** by the U.S. Army Aviation Missile Research Development and Engineering Center (AMRDEC);
** you may not use this file except in compliance with the terms and conditions of
** DFARS 227.7103-7 (Use and non-disclosure agreement), 252.204-7000 (Disclosure
** of Information) and 252.227-7025 (Limitations on the Use or Disclosure of
** Government-Furnished Information Marked with Restrictive Legends). Other requests
** must be referred to AMRDEC. This Software is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.
**
** Distribution authorized to U.S. Government Agencies and their contractors.
** Other requests for this document shall be referred to AMRDEC.
** Disseminate in accordance with provisions of DoD Directive 5230.25.
**
** See the DCS Usage Agreement for the specific language governing permissions
** and limitations. You may obtain a copy of the Agreement at
** https://software.forge.mil/sf/go/doc1216
****************************************************************************/


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
