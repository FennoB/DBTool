#include "managerlogic.h"
#include <qmessagebox.h>
#include "csvhandler.h"


ManagerLogic::ManagerLogic(QObject *parent)
{
}

ManagerLogic::ManagerLogic(QQuickView* v)
{
  setLoggedIn(false);
  setAdmin(false);
  databaseHandler.initDatabase();
  menuEntryOfTable["location"] = "Standort";
  menuEntryOfTable["building"] = "Halle";
  menuEntryOfTable["system"] = "Hauptanlage";
  menuEntryOfTable["subsystem"] = "Teilanlage";
  menuEntryOfTable["container"] = QLatin1String("Behälter");

  view = v;
  view->rootContext()->setContextProperty("logic", QVariant::fromValue(this));
  view->rootContext()->setContextProperty("explorerList", QVariant::fromValue(m_explorerList));

  setFormtype("");
  form["name"] = "None";
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

  for(int i = 0; i < hids.length(); ++i)
  {
    QVariantMap entry = databaseHandler.getEmptyTable("hierarchy");
    databaseHandler.getEntry("hierarchy", hids[i], entry);
    QString symbol = entry["entry_table"] == "client"
        ? "Dir" :
          entry["entry_table"] == "container"
          ? "Container" :
            "Dir2";
    m_explorerList.append(new ExplorerEntry(entry["name"].toString(), symbol, depth[i], true, true, hids[i]));
  }
  view->rootContext()->setContextProperty("explorerList", QVariant::fromValue(m_explorerList));

  return true;
}

void ManagerLogic::reloadForm()
{
  QVariantMap base;
  databaseHandler.getSubEntryBase(form["entry_hid"].toInt(), base);
  forcedDirection.clear();
  databaseHandler.getForcedDirection(base["entry_table"].toString(), forcedDirection);

  QStringList model;
  QStringList tables;

  for (QHash<QString, QString>::iterator i = menuEntryOfTable.begin(); i != menuEntryOfTable.end(); ++i)
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

  view->rootContext()->setContextProperty("name", base["name"]);
  form["entry_hid"] = base["id"];
  view->rootContext()->setContextProperty("entryForm", form);
  view->rootContext()->setContextProperty("tableForm", tableForm);
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
    table = "";
    reloadForm();
  }

}

void ManagerLogic::setValue(QString name, QVariant value)
{
  if (name == "name")
  {
    QVariantMap base;
    databaseHandler.getSubEntryBase(form["entry_hid"].toInt(), base);
    if(base["name"].toString() == value.toString())
    {
      return;
    }

    base["name"] = value.toString();
    databaseHandler.setSubEntryBase(form["entry_hid"].toInt(), base);
    //reloadForm();
    load();
    return;
  }

  form[name] = value;

  databaseHandler.setSubEntry(form["entry_hid"].toInt(), form);
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

  QVariantMap old;
  databaseHandler.getSubEntry(row["entry_hid"].toInt(), old);

  if (old[key] == value)
  {
    return;
  }

  row.remove("__000_name");
  row[key] = value;

  databaseHandler.setSubEntry(row["entry_hid"].toInt(), row);
  databaseHandler.getSubEntry(form["entry_hid"].toInt(), form);
  reloadForm();
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
    emit messageBox("error", {"Error!", "Datei konnte nicht geladen werden."});
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
        emit messageBox("error", {"Error!", "Einige Einträge konnten nicht importiert werden. "});
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

    emit messageBox("error", {"Error!", message});
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

void ManagerLogic::search(QString text, QString column)
{
  if (text != filter() || column != searchColumn || (text == "" && column == ""))
  {
    setFilter(text);
    searchColumn = column;
    reloadForm();
  }
}
