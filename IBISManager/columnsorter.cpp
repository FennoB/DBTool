#include "columnsorter.h"
#include <QDateTime>

bool ColumnSorter::operator()(QVariant first, QVariant second)
{
  QVariantMap left = first.toMap();
  QVariantMap right = second.toMap();
  QVariant col = left[column];
  bool dif = false;

  if (col.type() == QVariant::Double)
  {
    dif = left[column].toDouble() > right[column].toDouble();
  }
  else if(col.type() == QVariant::Int)
  {
    dif = left[column].toInt() > right[column].toInt();
  }
  else if(col.type() == QVariant::String)
  {
    dif = left[column].toString() > right[column].toString();
  }
  else if(col.type() == QVariant::DateTime)
  {
    dif = left[column].toDateTime() > right[column].toDateTime();
  }
  else if(col.type() == QVariant::Date)
  {
    dif = left[column].toDate() > right[column].toDate();
  }
  return descending ? dif : !dif;
}

ColumnSorter::ColumnSorter(QString column, bool descending)
  : column(column), descending(descending)
{
}

void ColumnSorter::sort(QVariantList &tableModel, QString column, bool descending)
{
  ColumnSorter sorter(column, descending);
  std::sort(tableModel.begin(), tableModel.end(), sorter);
}
