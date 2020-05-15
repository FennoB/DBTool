#ifndef CSVHANDLER_H
#define CSVHANDLER_H

#include <QFile>
#include <QStringList>
#include <QDebug>

class csvHandler
{
  QFile* currentfile;
  QList<QString> columnNames;
  char seperator;
public:
  csvHandler();

  bool beginReadFile(QString file, char seperator);
  bool readNextLine(QVariantMap& entry);
  void endReadFile();

  bool beginWriteFile(QString file, char seperator);
  bool writeNextLine(QVariantMap& entry);
  void endWriteFile();

  QStringList getColumnNames();
};

#endif // CSVHANDLER_H
