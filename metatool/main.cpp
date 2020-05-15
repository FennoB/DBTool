#include "../IBISManager/databasehandler.h"
#include <QCoreApplication>
#include <QDateTime>
#include <qdatetime.h>
#include <iostream>
#include <qdebug.h>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  DatabaseHandler db;

  db.blueprintAdd("location", "id",         QVariant::Int,    "");
  db.blueprintAdd("location", "entry_hid",  QVariant::Int,    "");
  db.blueprintAdd("location", "__001_adresse",    QVariant::String, "");
  db.blueprintAdd("location", "__002_chef",       QVariant::String, "");

  db.blueprintAdd("building", "id",         QVariant::Int,    "");
  db.blueprintAdd("building", "entry_hid",  QVariant::Int,    "");
  db.blueprintAdd("building", "__001_art",        QVariant::String, "");

  db.blueprintAdd("system", "id"        ,   QVariant::Int,    0);
  db.blueprintAdd("system", "entry_hid" ,   QVariant::Int,    0);
  db.blueprintAdd("system", "__001_halle"   ,   QVariant::String, "building");
  db.blueprintAdd("system", "__002_volumen"   ,   QVariant::String, "");
  db.blueprintAdd("system", "__003_wgk"       ,   QVariant::Int,    0);
  db.blueprintAdd("system", "__004_gefstufe"  ,   QVariant::String, "");
  db.blueprintAdd("system", "__005_wsgeb"     ,   QVariant::String, "");
  db.blueprintAdd("system", "__006_unterird"  ,   QVariant::String, "");
  db.blueprintAdd("system", "__007_spec"      ,   QVariant::String, "");
  db.blueprintAdd("system", "__008_art"       ,   QVariant::String, "");
  QString sql = "SELECT name from hierarchy AS hr WHERE EXISTS (SELECT 1 FROM hierarchy WHERE id = :hid AND parent_hid = hr.hid)";
  db.metaAdd("building", sql, "");

  db.blueprintAdd("subsystem", "id"       , QVariant::Int   , 0 );
  db.blueprintAdd("subsystem", "entry_hid", QVariant::Int   , 0 );
  db.blueprintAdd("subsystem", "__001_volumen"  , QVariant::Double, 0 );
  db.blueprintAdd("subsystem", "__002_art"      , QVariant::String, "" );
  db.blueprintAdd("subsystem", "__003_datum"    , QVariant::Date  , QDate::currentDate());

  db.blueprintAdd("container", "id"       , QVariant::Int   , 0 );
  db.blueprintAdd("container", "entry_hid", QVariant::Int   , 0 );
  db.blueprintAdd("container", "__001_volumen"  , QVariant::String, "");
  db.blueprintAdd("container", "__002_volaktiv" , QVariant::String, "");
  db.blueprintAdd("container", "__003_wgk"      , QVariant::Int   , 0 );
  db.blueprintAdd("container", "__004_spec"     , QVariant::String, "");

  db = DatabaseHandler();

  db.initDatabase();

  db.flagAdd("system", "__001_halle", {"function"});

  qDebug() << "Done.";

  return a.exec();
}
