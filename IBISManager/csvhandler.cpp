#include "csvhandler.h"

csvHandler::csvHandler()
{
  currentfile = nullptr;
}

bool csvHandler::beginReadFile(QString file, char seperator)
{
  if (currentfile != nullptr)
  {
    qDebug() << "File already open!";
    return false;
  }

  this->seperator = seperator;
  currentfile = new QFile(file);
  if (!currentfile->open(QIODevice::ReadOnly))
  {
    qDebug() << currentfile->errorString();
    currentfile = nullptr;
    return false;
  }

  columnNames.clear();
  QVariantMap map;
  readNextLine(map);

  return true;
}

bool csvHandler::readNextLine(QVariantMap &entry)
{
  if (currentfile == nullptr)
  {
    return false;
  }

  if(currentfile->atEnd())
  {
    return false;
  }

  QByteArray line = currentfile->readLine();

  line = line.remove(line.length()-2, 2);

  QList<QByteArray> data = line.split(seperator);

  if(columnNames.length() > 0)
  {
    for (int i = 0; i < columnNames.length(); ++i)
    {
      QString buf = QString(data[i]);
      if(entry.contains(columnNames[i]))
      {
        entry[columnNames[i]] = buf.mid(1, buf.length()-2);
      }
    }
  }
  else
  {
    for (int i = 0; i < data.length(); ++i)
    {
      columnNames.append( QString(data[i]) );
    }
  }
}

void csvHandler::endReadFile()
{
  if (currentfile == nullptr)
  {
    return;
  }
  currentfile->close();
  delete currentfile;
  currentfile = nullptr;
}

bool csvHandler::beginWriteFile(QString file, char seperator)
{
  if (currentfile != nullptr)
  {
    qDebug() << "File already open!";
    return false;
  }

  this->seperator = seperator;
  currentfile = new QFile(file);
  if (!currentfile->open(QIODevice::WriteOnly))
  {
    qDebug() << currentfile->errorString();
    currentfile = nullptr;
    return false;
  }

  columnNames.clear();

  return true;
}

bool csvHandler::writeNextLine(QVariantMap &entry)
{
  if (columnNames.empty())
  {
    columnNames = entry.keys();
    for (QList<QString>::iterator i = columnNames.begin(); i != columnNames.end(); ++i)
    {
      currentfile->write((*i).toUtf8());
      if(i+1 != columnNames.end())
      {
        char str[] = {seperator, ' ', 0};
        currentfile->write(str);
      }
    }

    currentfile->write("\r\n");
  }

  for (QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    currentfile->write("\"");
    currentfile->write(i.value().toString().toUtf8());
    currentfile->write("\"");
    if(i+1 != entry.end())
    {
      char str[] = {seperator, ' ', 0};
      currentfile->write(str);
    }
  }

  currentfile->write("\r\n");
}

void csvHandler::endWriteFile()
{
  if (currentfile == nullptr)
  {
    return;
  }
  currentfile->close();
  delete currentfile;
  currentfile = nullptr;
}

QStringList csvHandler::getColumnNames()
{
  return columnNames;
}
