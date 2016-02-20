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

#include <QString>
#include <QStringList>
#include <QMap>

#include "commandlineparser.h"

CommandLineParser::CommandLineParser(QStringList& options)
{
   m_tokenDict = new QMap<QString,QString>;
   m_optionList = new QStringList;
   this->addParserOptions(options);
}

CommandLineParser::~CommandLineParser()
{
   delete m_tokenDict;
   delete m_optionList;
}

void CommandLineParser::processArgvList(QStringList& argv)
{
   QStringList newArgs(argv);
   while (!newArgs.isEmpty())
   {
      QString token = newArgs.takeFirst();
      if (m_optionList->contains(token))
      {
         if (!newArgs.isEmpty() && !m_optionList->contains(newArgs.at(0)))
         {
            m_tokenDict->operator[](token) = newArgs.takeFirst();
         }
      }
   }
}

void CommandLineParser::addParserOptions(QStringList& opts)
{
   QStringList::const_iterator itr = opts.constBegin();
   while (itr != opts.constEnd())
   {
      if(!m_optionList->contains(*itr))
         m_optionList->append(*itr);
      itr++;
   }
}

QString CommandLineParser::optionValue(QString opt)
{
   if (m_tokenDict->count(opt))
      return m_tokenDict->operator[](opt);
   return "";
}
