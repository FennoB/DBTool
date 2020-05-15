#include "databasehandler.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QThread>
#include <qvariant.h>
#include <iostream>
#include <qdatetime.h>
#include <QSet>
#include <qdebug.h>

DatabaseHandler::DatabaseHandler()
{
  initMetaDatabase();

  hierarchyBlueprint["id"         ] = QVariant::Int;
  hierarchyBlueprint["name"       ] = QVariant::String;
  hierarchyBlueprint["entry_table"] = QVariant::String;
  hierarchyBlueprint["entry_id"   ] = QVariant::Int;
  hierarchyBlueprint["parent_hid" ] = QVariant::Int;
  hierarchyBlueprint["client_id" ] = QVariant::Int;
  hierarchyBlueprint["depth"      ] = QVariant::Int;
  tableBlueprints["hierarchy"] = hierarchyBlueprint;

  tableBlueprints["forced_direction"] = QVariantMap();
  tableBlueprints["forced_direction"]["id"] = QVariant::Int;
  tableBlueprints["forced_direction"]["parent"] = QVariant::String;
  tableBlueprints["forced_direction"]["child"] = QVariant::String;

  tableBlueprints["files"] = QVariantMap();
  tableBlueprints["files"]["id"] = QVariant::Int;
  tableBlueprints["files"]["hid"] = QVariant::Int;
  tableBlueprints["files"]["url"] = QVariant::String;

  tableBlueprints["column_flag"] = QVariantMap();
  tableBlueprints["column_flag"]["id"] = QVariant::Int;
  tableBlueprints["column_flag"]["entry_table"] = QVariant::String;
  tableBlueprints["column_flag"]["entry_column"] = QVariant::String;
  tableBlueprints["column_flag"]["flag"] = QVariant::String;

  tableBlueprints["client"] = QVariantMap();
  tableBlueprints["client"]["id"         ] = QVariant::Int;
  tableBlueprints["client"]["entry_hid"  ] = QVariant::Int;

  metaQuery.exec("SELECT * FROM blueprint");
  if (metaQuery.first())
  {
    do
    {
      QString table = metaQuery.value("table_name").toString();
      QString column = metaQuery.value("column_name").toString();
      int type = metaQuery.value("variant_type").toInt();
      QVariant def = metaQuery.value("def_value");
      def.convert(type);

      if (!tableBlueprints.contains(table))
      {
        tableBlueprints[table] = QVariantMap();
      }

      tableBlueprints[table][column] = def;
    } while (metaQuery.next());
  }

  //tableBlueprints["seperator"] = QVariantMap();
  //tableBlueprints["seperator"]["id"                                   ] = QVariant::Int;
  //tableBlueprints["seperator"]["entry_hid"                            ] = QVariant::Int;
  //tableBlueprints["seperator"]["Auftraggeber"                         ] = QVariant::String;
  //tableBlueprints["seperator"]["NameDerFirma"                         ] = QVariant::String;
  //tableBlueprints["seperator"]["Strasse"                              ] = QVariant::String;
  //tableBlueprints["seperator"]["PLZ"                                  ] = QVariant::String;
  //tableBlueprints["seperator"]["Ort"                                  ] = QVariant::String;
  //tableBlueprints["seperator"]["Sachkundiger"                         ] = QVariant::String;
  //tableBlueprints["seperator"]["Tel"                                  ] = QVariant::String;
  //tableBlueprints["seperator"]["Fax"                                  ] = QVariant::String;
  //tableBlueprints["seperator"]["StandortAbscheider"                   ] = QVariant::String;
  //tableBlueprints["seperator"]["Datum"                                ] = QVariant::String;
  //tableBlueprints["seperator"]["Bearbeiter"                           ] = QVariant::String;
  //tableBlueprints["seperator"]["ArtEinleitung"                        ] = QVariant::String;
  //tableBlueprints["seperator"]["S"                                    ] = QVariant::String;
  //tableBlueprints["seperator"]["L"                                    ] = QVariant::String;
  //tableBlueprints["seperator"]["K"                                    ] = QVariant::String;
  //tableBlueprints["seperator"]["KO"                                   ] = QVariant::String;
  //tableBlueprints["seperator"]["Betriebstagebuch"                     ] = QVariant::String;
  //tableBlueprints["seperator"]["Bemerkung"                            ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_Hersteller"                        ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_Typ"                               ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_NI"                                ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_WT"                                ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_Max"                               ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_SchlammGemessen"                   ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_Entsorgen"                         ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_Maengel"                           ] = QVariant::String;
  //tableBlueprints["seperator"]["SF_Bemerkungen"                       ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_Hersteller"                       ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_Typ"                              ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_NG1"                              ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_OelMax"                           ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_OelschichtGemessen"               ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_EntleerungErforderlich"           ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_AbschlussGaengigLeichtBeweglich"  ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_Maengel"                          ] = QVariant::String;
  //tableBlueprints["seperator"]["OeF_Bemerkung"                        ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_Hersteller"                        ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_Typ"                               ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_NG2"                               ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_OelMax"                            ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_AufstauGemessen"                   ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_OelschichtGemessen"                ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_EntleerungErforderlich"            ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_AbschlussGaengigLeichtBeweglich"   ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_Maengel"                           ] = QVariant::String;
  //tableBlueprints["seperator"]["KA_Bemerkung"                         ] = QVariant::String;
  //tableBlueprints["seperator"]["AlarmanlageVorhanden"                 ] = QVariant::String;
  //tableBlueprints["seperator"]["AlarmanlageFunktionsfaehig"           ] = QVariant::String;
  //tableBlueprints["seperator"]["ProbenahmeMoeglichkeit"               ] = QVariant::String;
  //tableBlueprints["seperator"]["ArtDerProbenahmeMoeglichkeit"         ] = QVariant::String;
  //tableBlueprints["seperator"]["ProbeEntnommen"                       ] = QVariant::String;
  //tableBlueprints["seperator"]["Hinweis"                              ] = QVariant::String;
  //tableBlueprints["seperator"]["AbschlussBemerkungen"                 ] = QVariant::String;
}

bool DatabaseHandler::initDatabase()
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("ibis_db.sqlite3");

  if(!db.open())
  {
    QMessageBox msgBox;
    msgBox.setText("Fehler: Die Datenbank ist nicht verfügbar!");
    msgBox.exec();

    QThread::currentThread()->exit(0);
  }

  query = QSqlQuery(db);

  // Check if Database empty
  if(query.exec("SELECT * FROM USER"))
  {
    // No, database is already set up, thats okay
    return true;
  }

  // Yes so we have to create the structures
  bool result = true;

  // Create tables
  result = result && query.exec("CREATE TABLE user (username TEXT PRIMARY KEY NOT NULL, password TEXT, admin INTEGER)");
  result = result && query.exec("CREATE TABLE userclient (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, client INTEGER)");

  for(QMap<QString, QVariantMap>::iterator i = tableBlueprints.begin(); i != tableBlueprints.end(); ++i)
  {
    result = result && createTable(i.key(), i.value());
  }

  if(result)
  {
    // Add Dummy data
    userRegister("frodo", "frodo");
    int pid;
    clientCreate("Ein Kunde", {"frodo"}, pid);

    //QVariantMap system = tableBlueprints["system"];
    //addSubEntry("system", system, "System1", 0, pid);
    //
    //QVariantMap subsystem = tableBlueprints["subsystem"];
    //addSubEntry("subsystem", subsystem, "JENES subsystem", system["entry_hid"].toInt(), pid);
    //
    //QVariantMap container = tableBlueprints["container"];
    //addSubEntry("container", container, "SUBENtRy", subsystem["entry_hid"].toInt(), pid);
    //
    //QVariantMap sep = tableBlueprints["seperator"];
    //addSubEntry("seperator", sep, "SEPeratus!", 0, pid);

    setForcedDirection("client", "location");
    setForcedDirection("location", "building");
    setForcedDirection("building", "system");
    setForcedDirection("system", "subsystem");
    setForcedDirection("subsystem", "container");
    setForcedDirection("container", "none");
  }
  else
  {
    qDebug("Database Error!");
  }
  return result;
}

bool DatabaseHandler::initMetaDatabase()
{
  QSqlDatabase metadb = QSqlDatabase::addDatabase("QSQLITE", "ibis_meta");
  metadb.setDatabaseName("ibis_meta.sqlite3");
  metadb.setHostName("localhost");

  if(!metadb.open())
  {
    QMessageBox msgBox;
    msgBox.setText("Fehler: Metacode-Datenbank ist nicht verfügbar!");
    msgBox.exec();
    std::cout << "Fehler: Metacode-Datenbank ist nicht verfügbar!";
    return false;
  }

  metaQuery = QSqlQuery(metadb);

  // Check if Meta-Database empty
  if(!metaQuery.exec("SELECT * FROM metacode"))
  {
    metaQuery.exec("CREATE TABLE metacode (name TEXT PRIMARY KEY, getter TEXT, setter TEXT)");
    metaQuery.exec("CREATE TABLE blueprint (table_name TEXT, column_name TEXT, \
                variant_type INT, def_value TEXT, PRIMARY KEY(table_name, column_name))");
  }
}

bool DatabaseHandler::blueprintAdd(QString table, QString column, QVariant::Type type, QVariant def)
{
  metaQuery.prepare("INSERT INTO blueprint(table_name, column_name, variant_type, def_value) VALUES(?, ?, ?, ?)");
  metaQuery.addBindValue(table);
  metaQuery.addBindValue(column);
  metaQuery.addBindValue((int)type);
  metaQuery.addBindValue(def);
  return metaQuery.exec();
}

bool DatabaseHandler::flagAdd(QString table, QString column, QStringList flags)
{
  for (int i = 0; i < flags.count(); ++i)
  {
    QVariantMap entry = getEmptyTable("column_flag");
    entry["entry_table"] = table;
    entry["entry_column"] = column;
    entry["flag"] = flags[i];
    addEntry("column_flag", entry);
  }
}

bool DatabaseHandler::blueprintRemove(QString table, QString column)
{
  metaQuery.prepare("DELETE FROM blueprint WHERE table_name = ? AND column_name = ?");
  metaQuery.addBindValue(table);
  metaQuery.addBindValue(column);
  return metaQuery.exec();
}

bool DatabaseHandler::blueprintSetDefault(QString table, QString column, QVariant def)
{
  metaQuery.prepare("UPDATE blueprint SET def_value = ? WHERE table_name = ? AND column_name = ?");
  metaQuery.addBindValue(def);
  metaQuery.addBindValue(table);
  metaQuery.addBindValue(column);
  return metaQuery.exec();
}

bool DatabaseHandler::metaAdd(QString meta, QString getter_sql, QString setter_sql)
{
  metaQuery.prepare("INSERT INTO metacode(name, getter, setter) VALUES(?, ?, ?)");
  metaQuery.addBindValue(meta);
  metaQuery.addBindValue(getter_sql);
  metaQuery.addBindValue(setter_sql);
  return metaQuery.exec();
}

bool DatabaseHandler::metaSet(QString meta, QString getter_sql, QString setter_sql)
{
  metaQuery.prepare("UPDATE metacode SET getter = ?, setter = ? WHERE name = ?");
  metaQuery.addBindValue(getter_sql);
  metaQuery.addBindValue(setter_sql);
  metaQuery.addBindValue(meta);
  return metaQuery.exec();
}

bool DatabaseHandler::metaRemove(QString meta)
{
  metaQuery.prepare("DELETE FROM metacode WHERE name = ?");
  metaQuery.addBindValue(meta);
  return metaQuery.exec();
}

QVariant DatabaseHandler::callMetaGetter(QString meta, QString column, QVariantMap makros)
{
  metaQuery.prepare("SELECT * FROM metacode WHERE name = ?");
  metaQuery.addBindValue(meta);
  if(metaQuery.exec() && metaQuery.first())
  {
    QString sql = metaQuery.value("getter").toString();
    query.prepare(sql);
    for (QVariantMap::iterator i = makros.begin(); i != makros.end(); ++i)
    {
      query.bindValue(i.key(), i.value());
    }

    query.exec();
    if(query.first())
    {
      QVariant result = query.value(0);
      return result;
    }
  }

  return "";
}

bool DatabaseHandler::callMetaSetter(QString meta, QString column, QVariant value, QVariantMap makros)
{
  metaQuery.prepare("SELECT * FROM metacode WHERE name = ?");
  metaQuery.addBindValue(meta);
  if(metaQuery.exec() && metaQuery.first())
  {
    QString sql = query.value("setter").toString();
    metaQuery.prepare(sql);
    for (QVariantMap::iterator i = makros.begin(); i != makros.end(); ++i)
    {
      metaQuery.bindValue(i.key(), i.value());
    }

    return metaQuery.exec();
  }
}

bool DatabaseHandler::getColumnFlags(QString table, QString column, QStringList &flags)
{
  QString sql = "SELECT flag FROM column_flag WHERE entry_table = ? AND entry_column = ?";
  query.prepare(sql);
  query.addBindValue(table);
  query.addBindValue(column);
  bool result;
  if ((result = query.exec()))
  {
    if(query.first())
    {
      do
      {
        flags.append(query.value(0).toString());
      } while(query.next());
    }
  }

  return result;
}

bool DatabaseHandler::userLogin(QString username, QString password)
{
  query.prepare("SELECT password FROM user WHERE username = ?");
  query.addBindValue(username);
  query.exec();
  if(!query.first())
  {
    return false;
  }

  if(query.value(0).toString() == password)
  {
    return true;
  }

  return false;
}

bool DatabaseHandler::userRegister(QString username, QString password)
{
  query.prepare("INSERT INTO user(username, password, admin) VALUES(?, ?, 0)");
  query.addBindValue(username);
  query.addBindValue(password);
  return query.exec();
}

bool DatabaseHandler::userSetAdminState(QString username, bool admin)
{
  query.prepare("UPDATE username SET admin = ? WHERE username = ?");
  query.addBindValue(admin);
  query.addBindValue(username);
  return query.exec();
}

bool DatabaseHandler::getUserList(QList<QString> &user_list)
{
  query.prepare("SELECT username FROM user");
  query.exec();

  if(query.first())
  {
    do
    {
      user_list.append(query.value(0).toString());
    } while(query.next());
    return true;
  }

  return false;
}

bool DatabaseHandler::clientCreate(QString name, QList<QString> user_list, int &id)
{
  QVariantMap client = getEmptyTable("client");
  addSubEntry("client", client, name, 0, 0);
  id = client["id"].toInt();

  QVariantMap base;
  getSubEntryBase(client["entry_hid"].toInt(), base);
  base["client_id"] = id;
  setSubEntryBase(client["entry_hid"].toInt(), base);

  query.prepare("INSERT INTO userclient(username, client) VALUES(?, ?)");

  bool result = true;

  for(int i = 0; i < user_list.length(); ++i)
  {
    query.addBindValue(user_list[i]);
    query.addBindValue(id);
    result = result && query.exec();
  }

  return result;
}

bool DatabaseHandler::clientRemove(int id)
{
  QVariantMap p = getEmptyTable("client");
  getEntry("client", id, p);
  bool result = removeSubEntry(p["entry_hid"].toInt());

  query.prepare("DELETE FROM userclient WHERE client = ?");
  query.addBindValue(id);
  return result && query.exec();
}

bool DatabaseHandler::clientRemoveUser(int client_id, QString username)
{
  query.prepare("DELETE FROM userclient WHERE client = ? AND username = ?");
  query.addBindValue(client_id);
  query.addBindValue(username);
  return query.exec();
}

bool DatabaseHandler::clientAddUser(int client_id, QString username)
{
  query.prepare("INSERT INTO userclient(username, client) VALUES(?, ?)");
  query.addBindValue(username);
  query.addBindValue(client_id);
  return query.exec();
}

bool DatabaseHandler::getUserClients(QString username, QList<int> &clients)
{
  query.prepare("SELECT client FROM userclient WHERE username = ?");
  query.addBindValue(username);
  query.exec();

  if(query.first())
  {
    do
    {
      clients.append(query.value(0).toInt());
    } while(query.next());

    return true;
  }

  return false;
}


bool DatabaseHandler::createTable(QString table, QVariantMap blueprint)
{
  if (!blueprint.contains("id"))
  {
    return false;
  }

  QString sql = "CREATE TABLE " + table + " (id INTEGER PRIMARY KEY AUTOINCREMENT, ";

  for (QVariantMap::iterator i = blueprint.begin(); i != blueprint.end(); ++i)
  {
    if (i.key() != "id")
    {
      sql += i.key();

      QString tp = "TEXT";

      switch (i.value().type())
      {
      case QVariant::Int:
        tp = "INTEGER";
        break;
      case QVariant::Date:
        tp = "DATE";
        break;
      default:
        tp = "TEXT";
      }

      sql += " " + tp;
      if(i+1 != blueprint.end() && (i+2 != blueprint.end() || blueprint.lastKey() != "id"))
      {
        sql += ", ";
      }
    }
  }

  sql += ")";

  qDebug(sql.toLocal8Bit());

  return query.exec(sql);
}

bool DatabaseHandler::addEntry(QString table, QVariantMap &entry)
{
  QString sql = "INSERT INTO " + table + "(";
  QString sql2 = " VALUES(";

  for(QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    if (i.key() != "id")
    {
      sql += i.key();
      sql2 += "?";
      if(i+1 != entry.end() && (i+2 != entry.end() || entry.lastKey() != "id"))
      {
        sql += ", ";
        sql2 += ", ";
      }
    }
  }

  sql += ")";
  sql2 += ")";

  sql += sql2;
  query.prepare(sql);
  qDebug(sql.toLocal8Bit());

  for(QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    if (i.key() != "id")
    {
      query.addBindValue(i.value());
    }
  }

  bool result = query.exec();
  entry["id"] = query.lastInsertId().toInt();
  return result;
}

bool DatabaseHandler::getEntry(QString table, int id, QVariantMap &entry)
{
  QString sql = "SELECT ";

  for(QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    sql += i.key();
    if(i+1 != entry.end())
    {
      sql += ", ";
    }
  }

  sql += " FROM " + table + " WHERE id = ? ";

  query.prepare(sql);
  query.addBindValue(id);

  query.exec();

  if(query.first())
  {
    for(QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
    {
      QVariant buf = query.value(i.key());
      buf.convert(i.value().type());
      i->setValue(buf);
    }
    return true;
  }

  return false;
}

bool DatabaseHandler::removeEntry(QString table, int id)
{
  query.prepare("DELETE FROM " + table + " WHERE id=?");
  query.addBindValue(id);
  return query.exec();
}

bool DatabaseHandler::setEntry(QString table, int id, QVariantMap entry)
{
  QString sql = "UPDATE " + table + " SET ";

  for(QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    if (i.key() != "id")
    {
      sql += i.key() + "=" + "?";
      if(i+1 != entry.end()  && (i+2 != entry.end() || entry.lastKey() != "id"))
      {
        sql += ", ";
      }
    }
  }

  sql += " WHERE id=?";

  query.prepare(sql);

  for(QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    if (i.key() != "id")
    {
      query.addBindValue(i.value());
    }
  }

  query.addBindValue(id);

  return query.exec();
}

bool DatabaseHandler::getFullHierarchy(QString username, QList<int>& depth, QList<int>& ids)
{
  QList<int> clients;

  getUserClients(username, clients);

  bool r = true;

  for(int i = 0; i < clients.length(); ++i)
  {
    depth.append(0);
    QVariantMap p = getEmptyTable("client");
    getEntry("client", clients[i], p);
    ids.append(p["entry_hid"].toInt());

    query.prepare("SELECT id FROM hierarchy WHERE entry_table != 'client' AND client_id = ? AND depth = 1");
    query.addBindValue(clients[i]);
    if(!query.exec())
    {
      r = false;
      continue;
    }

    QList<int> results;
    if(query.first())
    {
      do
      {
        results.append(query.value(0).toInt());
      } while(query.next());
    }

    for(QList<int>::iterator j = results.begin(); j != results.end(); ++j)
    {
      r = r && getHierarchy(*j, depth, ids);
    }
  }

  return r;
}

bool DatabaseHandler::getHierarchy(int hid, QList<int> &depth, QList<int> &hids)
{
  QVariantMap h_entry = getEmptyTable("hierarchy");
  getSubEntryBase(hid, h_entry);

  depth.append(h_entry["depth"].toInt());
  hids.append(hid);

  query.prepare("SELECT id FROM hierarchy WHERE parent_hid = ?");
  query.addBindValue(hid);
  if(!query.exec())
  {
    return false;
  }

  QList<int> results;
  if(query.first())
  {
    do
    {
      results.append(query.value(0).toInt());
    } while(query.next());
  }

  bool r = true;

  for(QList<int>::iterator i = results.begin(); i != results.end(); ++i)
  {
    r = r && getHierarchy(*i, depth, hids);
  }

  return r;
}

bool DatabaseHandler::addSubEntry(QString table, QVariantMap& data, QString name_in_hierarchy, int parent_hid, int client_id)
{
  bool result = true;
  result = result && addEntry(table, data);
  int id = data["id"].toInt();

  QVariantMap entryHierarchy = hierarchyBlueprint;
  entryHierarchy["name"] = name_in_hierarchy;
  entryHierarchy["entry_table"] = table;
  entryHierarchy["entry_id"] = id;
  entryHierarchy["parent_hid"] = parent_hid;
  entryHierarchy["client_id"] = client_id;

  int depth = table == "client" ? 0 : 1;
  if (parent_hid > 0)
  {
    QVariantMap parent = getEmptyTable("hierarchy");
    if(getEntry("hierarchy", parent_hid, parent))
    {
      depth = parent["depth"].toInt() + 1;
    }
  }

  entryHierarchy["depth"] = depth;
  result = result && addEntry("hierarchy", entryHierarchy);
  int hid = entryHierarchy["id"].toInt();

  data["entry_hid"] = hid;
  return result && setEntry(table, id, data);
}

bool DatabaseHandler::removeSubEntry(int hid)
{
  bool result = true;
  QString sql = "SELECT id FROM hierarchy WHERE parent_hid=?";
  query.prepare(sql);
  query.addBindValue(hid);

  result = result && query.exec();

  QList<int> hids;

  if(query.first())
  {
    do
    {
      hids.append(query.value(0).toInt());
    } while(query.next());
  }

  for(QList<int>::iterator i = hids.begin(); i != hids.end(); ++i)
  {
    result = result && removeSubEntry(*i);
  }

  QVariantMap h_entry;
  result = result && getSubEntryBase(hid, h_entry);
  result = result && removeEntry(h_entry["entry_table"].toString(), h_entry["entry_id"].toInt());
  result = result && removeEntry("hierarchy", hid);

  return result;
}

bool DatabaseHandler::getSubEntry(int hid, QVariantMap &entry)
{
  QVariantMap h_entry;
  getSubEntryBase(hid, h_entry);
  QString table = h_entry["entry_table"].toString();
  entry = getEmptyTable(table);

  bool res = getEntry(table, h_entry["entry_id"].toInt(), entry);
  for (QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    QStringList flags;
    getColumnFlags(table, i.key(), flags);
    for(int f = 0; f < flags.count(); ++f)
    {
      if (flags[f] == "function")
      {
        QVariantMap makros = QVariantMap();
        makros[":hid"] = hid;
        entry[i.key()] = callMetaGetter(i.value().toString(), i.key(), makros);
      }
    }
  }

  return res;
}

bool DatabaseHandler::setSubEntry(int hid, QVariantMap entry)
{
  QVariantMap h_entry;
  getSubEntryBase(hid, h_entry);
  QString table = h_entry["entry_table"].toString();

  for (QVariantMap::iterator i = entry.begin(); i != entry.end(); ++i)
  {
    QStringList flags;
    getColumnFlags(table, i.key(), flags);
    for(int f = 0; f < flags.count(); ++f)
    {
      if (flags[f] == "function")
      {
        //QVariantMap makros = QVariantMap();
        //makros[":hid"] = hid;
        //entry[i.key()] = callMetaSetter(i.value().toString(), i.key(), makros);
        entry.remove(i.key());
      }
    }
  }

  return setEntry(table, h_entry["entry_id"].toInt(), entry);
}

bool DatabaseHandler::getSubEntryBase(int hid, QVariantMap &base)
{
  base = getEmptyTable("hierarchy");
  return getEntry("hierarchy", hid, base);
}

bool DatabaseHandler::setSubEntryBase(int hid, QVariantMap base)
{
  return setEntry("hierarchy", hid, base);
}

bool DatabaseHandler::getForcedDirection(QString table, QList<QString> &result)
{
  query.prepare("SELECT child FROM forced_direction WHERE parent = ?");
  query.addBindValue(table);
  query.exec();

  if(query.first())
  {
    do
    {
      result.append(query.value(0).toString());
    } while(query.next());

    return true;
  }

  return false;
}

bool DatabaseHandler::search(QString table, QVariantMap base_filter, QVariantMap filter, QVariantList &result)
{
  // Monster of searching
  // Can filter both on base columns and table columns

  QSet<int> childrenOfRequired;
  if (base_filter.contains("child_of"))
  {
    int requiredParent = base_filter["child_of"].toInt();
    base_filter.remove("child_of");
    QList<int> depth;
    QList<int> hids;
    getHierarchy(requiredParent, depth, hids);

    // memorize all this children
    for (int i = 0; i < hids.count(); ++i)
    {
      childrenOfRequired.insert(hids[i]);
    }
  }

  QString sql = "SELECT entry_hid FROM " + table + " AS tbl";

  if (!base_filter.empty() || !filter.empty())
  {
    sql += " WHERE ";
    for (int i = 0; i < filter.count(); ++i)
    {
      if (filter[filter.keys()[i]].type() == QVariant::Int)
      {
        sql += filter.keys()[i] + " = ?";
      }
      else
      {
        sql += filter.keys()[i] + " LIKE ?";
      }

      if(i+1 < filter.count() || !base_filter.empty())
      {
        sql += " AND ";
      }
    }

    if (!base_filter.empty())
    {
      sql += "EXISTS ( SELECT * FROM hierarchy WHERE tbl.entry_hid = id AND ";

      for (int i = 0; i < base_filter.count(); ++i)
      {
        if (base_filter[base_filter.keys()[i]].type() == QVariant::Int)
        {
          sql += base_filter.keys()[i] + " = ?";
        }
        else
        {
          sql += base_filter.keys()[i] + " LIKE ?";
        }

        if(i+1 < base_filter.count())
        {
          sql += " AND ";
        }
      }
      sql += ")";
    }
  }

  query.prepare(sql);
  qDebug() << sql;

  for (int i = 0; i < filter.count(); ++i)
  {
    QVariant val = filter[filter.keys()[i]];
    if (filter[filter.keys()[i]].type() == QVariant::Int)
    {
      query.addBindValue(val);
    }
    else
    {
      val = val.toString()
          .replace("!", "!!")
          .replace("%", "!%")
          .replace("_", "!_")
          .replace("[", "![");
      query.addBindValue("%" + val.toString() + "%");
    }
  }

  for (int i = 0; i < base_filter.count(); ++i)
  {
    QVariant val = base_filter[base_filter.keys()[i]];
    if (base_filter[base_filter.keys()[i]].type() == QVariant::Int)
    {
      query.addBindValue(val);
    }
    else
    {
      val = val.toString()
          .replace("!", "!!")
          .replace("%", "!%")
          .replace("_", "!_")
          .replace("[", "![");
      query.addBindValue("%" + val.toString() + "%");
    }
  }

  if (!query.exec())
  {
    return false;
  }

  QList<int> result_hids;

  if (query.first())
  {
    do
    {
      if (childrenOfRequired.count() == 0 || childrenOfRequired.contains(query.value("entry_hid").toInt()))
      {
        result_hids.append(query.value("entry_hid").toInt());
      }
    } while (query.next());
  }

  if (result_hids.empty())
  {
    result.append(getEmptyTable(table));
  }
  else
  {
    for(int i = 0; i < result_hids.count(); ++i)
    {
      QVariantMap entry = getEmptyTable(table);
      getSubEntry(result_hids[i], entry);
      result.append(entry);
    }
  }

  return true;
}

QString DatabaseHandler::detectTable(QStringList columns)
{
  int max = 0;
  QString result = "";
  for (QMap<QString, QVariantMap>::iterator i = tableBlueprints.begin();
       i != tableBlueprints.end(); ++i)
  {
    QString table = i.key();
    int counter = 0;
    for (QVariantMap::iterator j = i.value().begin();
         j != i.value().end(); ++j)
    {
      if (columns.contains(j.key()))
      {
        counter++;
      }
    }

    if (counter > max)
    {
      result = table;
      max = counter;
    }
  }

  return result;
}

bool DatabaseHandler::setForcedDirection(QString parent, QString child)
{
    QVariantMap forcedDirection = tableBlueprints["forced_direction"];
    forcedDirection["parent"] = parent;
    forcedDirection["child"] = child;
    addEntry("forced_direction", forcedDirection);
}

  // Old System
  // name
  // client
  // volumen
  // wgk
  // wsgeb
  // spec
  // aev
  // gefstufe
  // awsvlast
  // awsvnext
  // awsvbeg
  // awsvstill
  // awsvwdk
  // awsvintvl
  // fbpflicht
  // anzerf
  // bemerkung
  // empfehlung

  // Old subsystem
  // name
  // system
  // volumen
  // ort
  // wgk
  // art

  // Old container
  // name
  // subsystem
  // volumen
  // ort
  // stoff
  // wgk
  // art
  // auffangraum
  // erfrvol
  // vorrvol
