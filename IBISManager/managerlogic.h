#ifndef MANAGERLOGIC_H
#define MANAGERLOGIC_H

#include "databasehandler.h"
#include "explorerentry.h"

#include <QObject>
#include <QQmlListProperty>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <qdatetime.h>

class ManagerLogic : public QObject
{
  Q_OBJECT
  bool m_loggedIn;
  QString m_formtype;
  bool m_isAdmin;
  QString table;
  QString searchColumn;
  QQmlApplicationEngine* view;

  QString entryName;
  QVariantMap form;
  QVariantList tableForm;
  QHash<QString, QString> menuEntryOfTable;
  QVariantMap entryDropdowns;
  QVariantMap tableDropdowns;
  QVariantMap columnFlags;
  QVariantMap tableColumnFlags;

  DatabaseHandler databaseHandler;

  QString m_user;
  QString m_filter;

  QStringList forcedDirection;
  QStringList m_menuModel;

  QStringList m_tableList;

  int m_tableListIndex;

  QVariantList fileModel;

  int m_clientCount;

  bool m_clientSelected;

  QString m_sortingColumn;

  bool m_sortingDescending;

public:

  QList<QObject*> m_explorerList;

  explicit ManagerLogic(QObject *parent = nullptr);

  ManagerLogic(QQmlApplicationEngine *v);

  Q_PROPERTY(QStringList menuModel READ menuModel WRITE setMenuModel NOTIFY menuModelChanged)
  Q_PROPERTY(QStringList tableList READ tableList WRITE setTableList NOTIFY tableListChanged)
  Q_PROPERTY(int tableListIndex READ tableListIndex WRITE setTableListIndex NOTIFY tableListIndexChanged)
  Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
  Q_PROPERTY(bool loggedIn READ loggedIn WRITE setLoggedIn NOTIFY loggedInChanged)
  Q_PROPERTY(QString formtype READ formtype WRITE setFormtype NOTIFY formtypeChanged)
  Q_PROPERTY(bool isAdmin READ isAdmin WRITE setAdmin NOTIFY isAdminChanged)
  Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
  Q_PROPERTY(int clientCount READ clientCount WRITE setClientCount NOTIFY clientCountChanged)
  Q_PROPERTY(bool clientSelected READ clientSelected WRITE setClientSelected NOTIFY clientSelectedChanged)
  Q_PROPERTY(QString sortingColumn READ sortingColumn WRITE setSortingColumn NOTIFY sortingColumnChanged)
  Q_PROPERTY(bool sortingDescending READ sortingDescending WRITE setSortingDescending NOTIFY sortingDescendingChanged)

  bool loggedIn() const
  {
    return m_loggedIn;
  }
  QString formtype() const
  {
    return m_formtype;
  }
  bool isAdmin() const
  {
    return m_isAdmin;
  }

  bool load();
  void reloadForm();
  void reloadMenus();
  void flags2Dropdown(QVariantMap &columnFlags, QVariantMap &dropdowns);

  Q_INVOKABLE
  void addClient();

  Q_INVOKABLE
  void addFile();

  Q_INVOKABLE
  void addFile(QString url);

  Q_INVOKABLE
  void setFileName(int index, QString text);

  Q_INVOKABLE
  void removeFile(int index);

  Q_INVOKABLE
  void openFile(int index);

  Q_INVOKABLE
  void removeEntry();

  Q_INVOKABLE
  void menuClicked(int menu_index, QString elem);

  Q_INVOKABLE
  void setValue(QString name, QVariant value);

  Q_INVOKABLE
  void saveEntry();

  Q_INVOKABLE
  void setTableValue(int rowIndex, QString key, QVariant value);

  Q_INVOKABLE
  bool login(QString email, QString password);

  Q_INVOKABLE
  bool registration(QString email, QString password);

  Q_INVOKABLE
  void toggleDir(int hid);

  Q_INVOKABLE
  void open(int hid);

  Q_INVOKABLE
  void csvEntryImport(QString path);

  Q_INVOKABLE
  void setTable(int index);

  Q_INVOKABLE
  void csvSubentriesImport(QString path);

  Q_INVOKABLE
  QString toGermanDateTime(QDateTime dt, QString param);

  Q_INVOKABLE
  QDateTime fromGermanDateTime(QString text, QString param);

  Q_INVOKABLE
  QString beautifyColumnName(QString column);

  Q_INVOKABLE
  void search(QString text, QString column);

  Q_INVOKABLE
  void sort(QString column);

  QString user() const
  {
    return m_user;
  }

  QString filter() const
  {
    return m_filter;
  }

  QStringList menuModel() const
  {
    return m_menuModel;
  }

  QStringList tableList() const
  {
    return m_tableList;
  }

  int tableListIndex() const
  {
    return m_tableListIndex;
  }

  int clientCount() const
  {
    return m_clientCount;
  }

  bool clientSelected() const
  {
    return m_clientSelected;
  }

  QString sortingColumn() const
  {
    return m_sortingColumn;
  }

  bool sortingDescending() const
  {
    return m_sortingDescending;
  }

signals:

  void loggedInChanged(bool loggedIn);
  void formtypeChanged(QString formtype);
  void isAdminChanged(bool isAdmin);

  void userChanged(QString user);

  void dialog(QString mode, QStringList params);

  void filterChanged(QString filter);

  void menuModelChanged(QStringList menuModel);

  void tableListChanged(QStringList tableList);

  void tableListIndexChanged(int tableListIndex);

  void clientCountChanged(int clientCount);

  void clientSelectedChanged(bool clientSelected);

  void sortingColumnChanged(QString sortingColumn);

  void sortingDescendingChanged(bool sortingDescending);

public slots:
  void setLoggedIn(bool loggedIn)
  {
    if (m_loggedIn == loggedIn)
      return;

    m_loggedIn = loggedIn;
    emit loggedInChanged(m_loggedIn);
  }
  void setFormtype(QString menu)
  {
    if (m_formtype == menu)
      return;

    m_formtype = menu;
    emit formtypeChanged(m_formtype);
  }
  void setAdmin(bool isAdmin)
  {
    if (m_isAdmin == isAdmin)
      return;

    m_isAdmin = isAdmin;
    emit isAdminChanged(m_isAdmin);
  }
  void setUser(QString user)
  {
    if (m_user == user)
      return;

    m_user = user;
    emit userChanged(m_user);
  }
  void setFilter(QString filter)
  {
    if (m_filter == filter)
      return;

    m_filter = filter;
    emit filterChanged(m_filter);
  }
  void setMenuModel(QStringList menuModel)
  {
    if (m_menuModel == menuModel)
      return;

    m_menuModel = menuModel;
    emit menuModelChanged(m_menuModel);
  }
  void setTableList(QStringList tableList)
  {
    if (m_tableList == tableList)
      return;
    m_tableList = tableList;
    emit tableListChanged(m_tableList);
  }
  void setTableListIndex(int tableListIndex)
  {
    m_tableListIndex = tableListIndex;
    emit tableListIndexChanged(m_tableListIndex);
  }
  void setClientCount(int clientCount)
  {
    if (m_clientCount == clientCount)
      return;

    m_clientCount = clientCount;
    emit clientCountChanged(m_clientCount);
  }
  void setClientSelected(bool clientSelected)
  {
    if (m_clientSelected == clientSelected)
      return;

    m_clientSelected = clientSelected;
    emit clientSelectedChanged(m_clientSelected);
  }
  void setSortingColumn(QString sortingColumn)
  {
    if (m_sortingColumn == sortingColumn)
      return;

    m_sortingColumn = sortingColumn;
    emit sortingColumnChanged(m_sortingColumn);
  }
  void setSortingDescending(bool sortingDescending)
  {
    if (m_sortingDescending == sortingDescending)
      return;

    m_sortingDescending = sortingDescending;
    emit sortingDescendingChanged(m_sortingDescending);
  }
};

#endif // MANAGERLOGIC_H
