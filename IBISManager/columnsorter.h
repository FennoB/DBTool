#ifndef COLUMNSORTER_H
#define COLUMNSORTER_H

#include <QVariantList>

class ColumnSorter
{
  QString column;
  bool descending;

public:
  bool operator()(QVariant first, QVariant second);
  ColumnSorter(QString column, bool descending);
  static void sort(QVariantList &tableModel, QString column, bool descending);
};

#endif // COLUMNSORTER_H
