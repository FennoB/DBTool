#ifndef MANAGERDATAMODEL_H
#define MANAGERDATAMODEL_H

#include <QVariant>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class DatabaseHandler
{
    QSqlQuery query;
    QSqlQuery metaQuery;
    QSqlDatabase db;
    QSqlDatabase metadb;
    QVariantMap hierarchyBlueprint;
    QMap<QString, QVariantMap> tableBlueprints;

    bool setForcedDirection(QString parent, QString child);

public:

    inline void setTableBlueprint(QString table, QVariantMap bp)
    {
      tableBlueprints[table] = bp;
    }
    inline QVariantMap getEmptyTable(QString table)
    {
      return tableBlueprints[table];
    }

    DatabaseHandler();

    void close();

    bool initDatabase();
    bool initMetaDatabase();

    static bool removeAllDatabases();

    // Meta stuff
    bool addDropDown(QString name, QList<QString> entries);
    bool getDropDown(QString name, QList<QString> &entries);
    bool blueprintAdd(QString table, QString column, QVariant::Type type, QVariant def);
    bool flagAdd(QString table, QString column, QStringList flags);
    bool blueprintRemove(QString table, QString column);
    bool blueprintSetDefault(QString table, QString column, QVariant def);
    bool metaAdd(QString table, QString script_sql);
    bool metaSet(QString table, QString script_sql);
    bool metaRemove(QString table);
    bool callMeta(QString meta, QVariantMap makros);
    bool getFlags(QString table, QString column, QStringList &flags);
    bool addBinding(QString from_table, QString from_column, QString to_table);
    bool removeBinding(QString from_table, QString from_column, QString to_table);
    bool getBindings(QString from_table, QString from_column, QStringList& to_tables);
    bool getRelatedEntries(int hid, QMap<QString, QList<int>> &related);
    bool resolveBindings(int hid, QString table, QStringList changedColumns);

    // User and client management
    bool userLogin(QString username, QString password);
    bool userRegister(QString username, QString password);
    bool userSetAdminState(QString username, bool admin);
    bool getUserList(QList<QString>& user_list);
    bool clientCreate(QString name, QList<QString> user_list, int& id);
    bool clientRemove(int id);
    bool clientRemoveUser(int client_id, QString username);
    bool clientAddUser(int client_id, QString username);
    bool getUserClients(QString username, QList<int>& clients);

    // Basic entry management
    bool createTable(QString table, QVariantMap blueprint);
    bool addEntry(QString table, QVariantMap& entry);
    bool getEntry(QString table, int id, QVariantMap& entry);
    bool removeEntry(QString table, int id);
    bool setEntry(QString table, int id, QVariantMap entry);

    // Hierarchy
    bool getFullHierarchy(QString username, QList<int>& depth, QList<int>& hids);
    bool getHierarchy(int hid, QList<int>& depth, QList<int>& hids);
    bool addSubEntry(QString table, QVariantMap& data, QString name_in_hierarchy, int parent_hid, int client_id);
    bool removeSubEntry(int hid);
    bool getSubEntry(int hid, QVariantMap& entry);
    bool setSubEntry(int hid, QVariantMap entry);
    bool getSubEntryBase(int hid, QVariantMap& base);
    bool setSubEntryBase(int hid, QVariantMap base);

    bool getForcedDirection(QString table, QList<QString>& result);

    bool search(QString table, QVariantMap base_filter, QVariantMap filter, QVariantList &result);

    bool getFileList(int hid, QVariantList& fileList);

    QString detectTable(QStringList columns);
};

#endif // MANAGERDATAMODEL_H
