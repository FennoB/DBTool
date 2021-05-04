#include "managerlogic.h"
#include <QDesktopServices>
#include <QQmlApplicationEngine>
#include "columnsorter.h"
#include <qmessagebox.h>
#include "csvhandler.h"


ManagerLogic::ManagerLogic(QObject *parent)
{
}

ManagerLogic::ManagerLogic(QQmlApplicationEngine* v)
{
  view = v;
  view->rootContext()->setContextProperty("name", "");
  view->rootContext()->setContextProperty("fileModel", fileModel);
  view->rootContext()->setContextProperty("columnFlags", columnFlags);
  view->rootContext()->setContextProperty("tableColumnFlags", tableColumnFlags);
  view->rootContext()->setContextProperty("entryDropdowns", entryDropdowns);
  view->rootContext()->setContextProperty("tableDropdowns", tableDropdowns);

  setLoggedIn(false);
  setAdmin(false);
  databaseHandler.initDatabase();
  menuEntryOfTable["location"] = "Standort";
  menuEntryOfTable["building"] = "Halle";
  menuEntryOfTable["system"] = "Hauptanlage";
  menuEntryOfTable["subsystem"] = "Teilanlage";
  menuEntryOfTable["container"] = QLatin1String("Behälter");

  view->rootContext()->setContextProperty("logic", QVariant::fromValue(this));
  view->rootContext()->setContextProperty("explorerList", QVariant::fromValue(m_explorerList));

  setFormtype("");
  form["entry_id"] = 0;
  view->rootContext()->setContextProperty("entryForm", form);
  tableForm.append(QVariantMap());
  view->rootContext()->setContextProperty("tableForm", tableForm);
}

bool ManagerLogic::load()
{
  m_explorerList.clear();
  QList<int> depth;
  QList<int> hids;
  databaseHandler.getFullHierarchy(user(), depth, hids);

  int count = 0;
  int lastDepth = -1;
  std::vector<int> context;
  for(int i = 0; i < hids.length(); ++i)
  {
    if (depth[i] == 0)
    {
      count++;
    }

    if (depth[i] != lastDepth)
    {
      QString table;
      QVariantMap base;
      databaseHandler.getSubEntryBase(hids[i], base);
      table = base["entry_table"].toString();
      QStringList flags;
      databaseHandler.getFlags(table, "*", flags);

      if (depth[i] > lastDepth)
      {
        context.push_back(flags.contains("counter") ? 0 : -1);
        lastDepth = depth[i];
      }
      while (depth[i] < lastDepth)
      {
        lastDepth--;
        context.pop_back();
      }
    }

    if (context.back() != -1)
    {
      context.back()++;
    }

    QString lfnr;
    for (size_t j = 0; j < context.size(); ++j)
    {
      if (context[j] != -1)
      {
        if (lfnr != "")
        {
          lfnr += ".";
        }
        lfnr += QString::number(context[j]);
      }
    }

    QVariantMap entry = databaseHandler.getEmptyTable("hierarchy");
    databaseHandler.getEntry("hierarchy", hids[i], entry);
    QString symbol = entry["entry_table"] == "client"
        ? "Dir" :
          entry["entry_table"] == "container"
          ? "Container" :
            "Dir2";
    m_explorerList.append(
          new ExplorerEntry(
            entry["name"].toString(),
            symbol,
            depth[i],
            true,
            true,
            hids[i],
            lfnr));
  }
  setClientCount(count);
  view->rootContext()->setContextProperty("explorerList", QVariant::fromValue(m_explorerList));

  return true;
}

void ManagerLogic::reloadForm()
{
  QVariantMap base;
  databaseHandler.getSubEntryBase(form["entry_hid"].toInt(), base);
  forcedDirection.clear();
  databaseHandler.getForcedDirection(base["entry_table"].toString(), forcedDirection);
  setClientSelected(base["entry_table"] == "client");

  fileModel = QVariantList();
  databaseHandler.getFileList(form["entry_hid"].toInt(), fileModel);

  reloadMenus();

  // Set filter
  QVariantMap base_filter;
  QVariantMap filter;
  QList<QVariant> searchResults;

  if (this->filter() != "")
  {
    if (searchColumn == "__000_name")
    {
      base_filter["name"] = this->filter();
    }
    else if (searchColumn != "")
    {
      filter[searchColumn] = this->filter();
    }
  }

  base_filter["client_id"] = base["client_id"].toInt();
  base_filter["child_of"] = base["id"].toInt();

  // Reload table
  setSortingColumn("");
  tableForm.clear();
  databaseHandler.search(table, base_filter, filter, searchResults);
  for (int i = 0; i < searchResults.length(); ++i)
  {
    QVariantMap row = searchResults[i].toMap();
    QVariantMap hrow;
    databaseHandler.getSubEntryBase(row["entry_hid"].toInt(), hrow);
    row["__000_name"] = hrow["name"];
    tableForm.append(row);
  }

  if (tableForm.empty())
  {
    tableForm.append(QVariantMap());
  }
  else
  {
    // Load table flags
    tableColumnFlags.clear();
    QVariantMap first = tableForm.front().toMap();
    for (auto i = first.begin(); i != first.end(); ++i)
    {
      QStringList flags;
      databaseHandler.getFlags(table, i.key(), flags);
      tableColumnFlags[i.key()] = flags;
    }
  }

  // Load main entry flags
  columnFlags.clear();
  for (auto i = form.begin(); i != form.end(); ++i)
  {
    QStringList flags;
    databaseHandler.getFlags(base["entry_table"].toString(), i.key(), flags);
    columnFlags[i.key()] = flags;
  }

  flags2Dropdown(columnFlags, entryDropdowns);
  flags2Dropdown(tableColumnFlags, tableDropdowns);

  form["entry_hid"] = base["id"].toInt();
  view->rootContext()->setContextProperty("name", base["name"]);
  view->rootContext()->setContextProperty("columnFlags", columnFlags);
  view->rootContext()->setContextProperty("tableColumnFlags", tableColumnFlags);
  view->rootContext()->setContextProperty("entryForm", form);
  view->rootContext()->setContextProperty("tableForm", tableForm);
  view->rootContext()->setContextProperty("fileModel", fileModel);
  view->rootContext()->setContextProperty("entryDropdowns", entryDropdowns);
  view->rootContext()->setContextProperty("tableDropdowns", tableDropdowns);
}

void ManagerLogic::reloadMenus()
{
  QStringList model;
  QStringList tables;

  for (auto i = menuEntryOfTable.begin(); i != menuEntryOfTable.end(); ++i)
  {
    tables.append(i.value());
    if (i.key() == table)
    {
      setTableListIndex(tables.count()-1);
    }

    if (forcedDirection.count() == 0 || forcedDirection.contains(i.key()))
    {
      if(model.empty() && table == "")
      {
        table = i.key();
        setTableListIndex(tables.count()-1);
      }
      model.append(i.value());
    }
  }

  setMenuModel(model);

  int buf = tableListIndex();
  setTableListIndex(0);
  setTableList(tables);
  setTableListIndex(buf);
}

void ManagerLogic::flags2Dropdown(QVariantMap &columnFlags, QVariantMap &dropdowns)
{
  dropdowns.clear();

  for(auto i = columnFlags.begin(); i != columnFlags.end(); ++i)
  {
    QVariantList flags = i.value().toList();
    for(auto f = flags.begin(); f < flags.end(); ++f)
    {
      QString s = f->toString();
      if (s.contains("dropdown"))
      {
        QStringList l = s.split(".");
        s = l[1];
        l.clear();
        databaseHandler.getDropDown(s, l);
        dropdowns[i.key()] = l;
      }
    }
  }
}

void ManagerLogic::addClient()
{
  int pid;
  if(!databaseHandler.clientCreate("Neuer Kunde", {user()}, pid))
  {
    return;
  }

  load();
}

void ManagerLogic::addFile()
{
  emit dialog("addfile", QStringList());
}

void ManagerLogic::addFile(QString url)
{
  QVariantMap entry;
  entry["url"] = url;
  entry["name"] = "Neuer Anhang";
  entry["hid"] = form["entry_hid"];
  databaseHandler.addEntry("files", entry);
  reloadForm();
}

void ManagerLogic::setFileName(int index, QString text)
{
  QVariantMap entry = fileModel[index].toMap();
  if (entry["name"].toString() == text)
  {
    return;
  }
  entry["name"] = text;
  int id = entry["id"].toInt();
  entry.remove("id");
  databaseHandler.setEntry("files", id, entry);
}

void ManagerLogic::removeFile(int index)
{
  QVariantMap entry = fileModel[index].toMap();
  int id = entry["id"].toInt();
  databaseHandler.removeEntry("files", id);
  reloadForm();
}

void ManagerLogic::openFile(int index)
{
  QVariantMap entry = fileModel[index].toMap();
  QString url = entry["url"].toString();
  QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void ManagerLogic::removeEntry()
{
  QVariantMap base;
  databaseHandler.getSubEntryBase(form["entry_hid"].toInt(), base);

  if (base["entry_table"].toString() == "client")
  {
    databaseHandler.clientRemove(form["id"].toInt());
  }
  else
  {
    databaseHandler.removeSubEntry(form["entry_hid"].toInt());
  }
  load();
  open(static_cast<ExplorerEntry*>( m_explorerList[0] )->_hid());
}

void ManagerLogic::menuClicked(int menu_index, QString elem)
{
  if (menu_index == 0)
  {
    QString table = menuEntryOfTable.key(elem);
    int parent_hid = form.isEmpty() ? 0 : form["entry_hid"].toInt();
    form = databaseHandler.getEmptyTable(table);

    int pid = 0;
    if (parent_hid != 0)
    {
      QVariantMap base;
      databaseHandler.getSubEntryBase(parent_hid, base);
      pid = base["client_id"].toInt();
    }

    if(!databaseHandler.addSubEntry(table, form, elem + " Neu", parent_hid, pid))
    {
      qDebug() << "Failed to add subentry";
    }

    load();
    open(form["entry_hid"].toInt());
  }
}

void ManagerLogic::setValue(QString name, QVariant value)
{
  if (name == "name")
  {
    entryName = value.toString();
    return;
  }

  value.convert(form[name].type());
  form[name] = value;
}

void ManagerLogic::saveEntry()
{
  QVariantMap base;
  databaseHandler.getSubEntryBase(form["entry_hid"].toInt(), base);
  base["name"] = entryName;
  databaseHandler.setSubEntryBase(form["entry_hid"].toInt(), base);
  view->rootContext()->setContextProperty("name", entryName);
  databaseHandler.setSubEntry(form["entry_hid"].toInt(), form);
  load();
  reloadForm();
}

void ManagerLogic::setTableValue(int rowIndex, QString key, QVariant value)
{
  QVariantMap row = tableForm[rowIndex].toMap();

  if (key == "__000_name")
  {
    QVariantMap base;
    databaseHandler.getSubEntryBase(row["entry_hid"].toInt(), base);
    if(base["name"].toString() == value.toString())
    {
      return;
    }

    base["name"] = value.toString();
    databaseHandler.setSubEntryBase(row["entry_hid"].toInt(), base);
    databaseHandler.getSubEntry(form["entry_hid"].toInt(), form);
    reloadForm();
    load();
    return;
  }

  QVariantMap buf = row;
  //databaseHandler.getSubEntry(row["entry_hid"].toInt(), buf);

  if (buf[key] == value)
  {
    return;
  }

  row[key] = value;
  buf = row;
  buf.remove("__000_name");

  databaseHandler.setSubEntry(row["entry_hid"].toInt(), buf);
  databaseHandler.getSubEntry(form["entry_hid"].toInt(), form);
  tableForm[rowIndex] = row;

  view->rootContext()->setContextProperty("tableForm", tableForm);
  view->rootContext()->setContextProperty("entryForm", form);

  //reloadForm();
}

bool ManagerLogic::login(QString email, QString password)
{
  if((email == "" && password == "") || databaseHandler.userLogin(email, password))
  {
    setUser(email);
    setAdmin(true);
    setLoggedIn(true);
    load();
    open(static_cast<ExplorerEntry*>( m_explorerList[0] )->_hid());
    return true;
  }
  else
  {
    return false;
  }
}

bool ManagerLogic::registration(QString email, QString password)
{
  return databaseHandler.userRegister(email, password);
}

void ManagerLogic::toggleDir(int hid)
{
  bool found = false;
  ExplorerEntry* target = nullptr;

  for(QObject* elem : m_explorerList)
  {
    ExplorerEntry* e = static_cast<ExplorerEntry*>(elem);

    if(found)
    {
      if(e->_layer() > target->_layer())
      {
        // this is child
        if(target->_open())
        {
          // target open now
          e->setVisible(e->lastVisibility);
          e->lastVisibility = true;
        }
        else
        {
          // target closes now
          e->lastVisibility = e->_visible();
          e->setVisible(false);
        }
      }
      else
      {
        found = false;
      }
    }

    if(e->_hid() == hid)
    {
      e->setOpen(!e->_open());
      found = true;
      target = e;
    }
  }
}

void ManagerLogic::open(int hid)
{
  table = "";
  databaseHandler.getSubEntry(hid, form);
  reloadForm();
}

void ManagerLogic::csvEntryImport(QString path)
{
  // Get table
  QVariantMap base;
  databaseHandler.getSubEntryBase(form["entry_hid"].toInt(), base);
  QString table = base["entry_table"].toString();
  QVariantMap entry = form;
  entry["name"] = "";
  path = path.right(path.size()-8);
  csvHandler h;
  h.beginReadFile(path, ',');

  while (h.readNextLine(entry)) {

    if (entry["name"] == base["name"])
    {
      // Found
      entry.remove("name");
      form = entry;
      reloadForm();
      break;
    }
  }

  h.endReadFile();
}

void ManagerLogic::setTable(int index)
{
  setTableListIndex(index);
  table = menuEntryOfTable.key(m_tableList[index]);
  search("", "");
}

void ManagerLogic::csvSubentriesImport(QString path)
{
  path = path.right(path.size()-8);

  csvHandler h;
  if (!h.beginReadFile(path, ','))
  {
    emit dialog("error", {"Error!", "Datei konnte nicht geladen werden."});
    return;
  }

  QString table = databaseHandler.detectTable( h.getColumnNames() );

  if (forcedDirection.contains(table) || forcedDirection.empty())
  {
    int parent_hid = form.isEmpty() ? 0 : form["entry_hid"].toInt();

    int pid = 0;
    if (parent_hid != 0)
    {
      QVariantMap base;
      databaseHandler.getSubEntryBase(parent_hid, base);
      pid = base["client_id"].toInt();
    }

    QVariantMap entry = databaseHandler.getEmptyTable(table);
    entry["name"] = "";
    while (h.readNextLine(entry))
    {
      QString name = entry["name"].toString();
      entry.remove("name");
      if(!databaseHandler.addSubEntry(table, entry, name == "" ? "[importiert]" : name , parent_hid, pid))
      {
        emit dialog("error", {"Error!", "Einige Einträge konnten nicht importiert werden. "});
      }
      entry = databaseHandler.getEmptyTable(table);
      entry["name"] = "";
    }
  }
  else
  {
    QString message = "Erkannte Tabelle: '" + table + "'.\n Erlaubte Tabellen: {";

    for (int i = 0; i < forcedDirection.length(); ++i)
    {
      message += forcedDirection[i];
      if ( i != forcedDirection.length() -1)
      {
        message += ", ";
      }
      else
      {
        message += "}";
      }
    }

    emit dialog("error", {"Error!", message});
  }

  h.endReadFile();
  load();
  reloadForm();
}

QString ManagerLogic::toGermanDateTime(QDateTime dt, QString param)
{
  bool p1 = param.contains("date");
  bool p2 = param.contains("time");
  QString format = (p1 ? "dd.MM.yyyy" : "") + QString(p1 && p2 ? " " : "") + QString(p2 ? "hh:mm:ss" : "");
  return dt.toString(format);
}

QDateTime ManagerLogic::fromGermanDateTime(QString text, QString param)
{
  text = text.trimmed();
  bool p1 = param.contains("date");
  bool p2 = param.contains("time");
  QString format = (p1 ? "dd.MM.yyyy" : "") + QString(p1 && p2 ? " " : "") + QString(p2 ? "hh:mm:ss" : "");
  return QDateTime::fromString(text, format);
}

QString ManagerLogic::beautifyColumnName(QString column)
{
  QString res = column.mid(6, column.length());
  QStringList parts = res.split("_");
  res = "";
  for (auto i = parts.begin(); i != parts.end(); ++i)
  {
    if (i->size())
    {
      res += i->at(0).toUpper() + i->mid(1) + " ";
    }
  }
  //res.replace("Ae", "Ä")
  //    .replace("Oe", "Ö")
  //    .replace("Ue", "Ü")
  //    .replace("ae", "ä")
  //    .replace("oe", "ö")
  //    .replace("ue", "ü");
  return res;
}

void ManagerLogic::search(QString text, QString column)
{
  if (text != filter() || column != searchColumn || (text == "" && column == ""))
  {
    setFilter(text);
    searchColumn = column;
    reloadForm();
  }
}

void ManagerLogic::sort(QString column)
{
  if (sortingColumn() == "")
  {
    setSortingDescending(true);
  }
  else if (sortingColumn() == column)
  {
    if (!sortingDescending())
    {
      reloadForm();
      return;
    }
    else
    {
      setSortingDescending(false);
    }
  }
  setSortingColumn(column);
  ColumnSorter::sort(tableForm, sortingColumn(), sortingDescending());
  view->rootContext()->setContextProperty("tableForm", tableForm);
}
