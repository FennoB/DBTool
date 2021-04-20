#include "../IBISManager/databasehandler.h"
#include <QCoreApplication>
#include <QDateTime>
#include <qdatetime.h>
#include <iostream>
#include <fstream>
#include <qdebug.h>
#include <qfile.h>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  DatabaseHandler::removeAllDatabases();
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
  db.blueprintAdd("system", "__001_halle"   ,   QVariant::String, "");
  db.blueprintAdd("system", "__002_volumen"   ,   QVariant::Double, "");
  db.blueprintAdd("system", "__003_wgk"       ,   QVariant::String, "");
  db.blueprintAdd("system", "__004_gefstufe"  ,   QVariant::String, "");
  db.blueprintAdd("system", "__005_wsgeb"     ,   QVariant::String, "");
  db.blueprintAdd("system", "__006_erste_prf"  ,  QVariant::Date  , QDate::currentDate());
  db.blueprintAdd("system", "__007_letzte_prf" ,  QVariant::Date  , QDate::currentDate());
  db.blueprintAdd("system", "__008_naechste_prf", QVariant::Date  , QDate::currentDate());
  db.blueprintAdd("system", "__009_color"     ,   QVariant::String, "");

  QString sql = "UPDATE subsystem SET __001_volumen = (\
  SELECT SUM(c.__001_volumen) FROM container c \
  LEFT JOIN hierarchy hr ON c.entry_hid = hr.id \
  WHERE hr.parent_hid = :hid LIMIT 1)\
  WHERE entry_hid = :hid";
  db.metaAdd("subsystem", sql);

  QFile file("sysrefresh.sql");
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream filestream(&file);
  db.metaAdd("system", filestream.readAll());

  db.blueprintAdd("subsystem", "id"       , QVariant::Int   , 0 );
  db.blueprintAdd("subsystem", "entry_hid", QVariant::Int   , 0 );
  db.blueprintAdd("subsystem", "__001_volumen"  , QVariant::Double, 0.0);
  db.blueprintAdd("subsystem", "__002_art"      , QVariant::String, "" );
  db.blueprintAdd("subsystem", "__003_datum"    , QVariant::Date  , QDate::currentDate());

  db.blueprintAdd("container", "id"       , QVariant::Int   , 0 );
  db.blueprintAdd("container", "entry_hid", QVariant::Int   , 0 );
  db.blueprintAdd("container", "__001_volumen"  , QVariant::Double, 0);
  db.blueprintAdd("container", "__002_volaktiv" , QVariant::String, "");
  db.blueprintAdd("container", "__003_wgk"      , QVariant::String   , "nwg" );
  db.blueprintAdd("container", "__004_unterirdisch", QVariant::String, "");

  db = DatabaseHandler();

  db.initDatabase();

  db.addDropDown("wgk", {"nwg", "1", "2", "3", "awg"});
  db.addDropDown("yesno", {"Ja", "Nein"});

  db.flagAdd("container", "__003_wgk", {"dropdown.wgk"});
  db.flagAdd("container", "__004_unterirdisch", {"dropdown.yesno"});

  db.flagAdd("system", "__001_halle", {"function"});
  db.flagAdd("system", "__002_volumen", {"function"});
  db.flagAdd("system", "__003_wgk", {"function"});
  db.flagAdd("system", "__004_gefstufe", {"function"});
  db.flagAdd("system", "__008_naechste_prf", {"function"});

  db.flagAdd("subsystem", "__001_volumen", {"function"});

  db.addBinding("container", "__001_volumen", "system");
  db.addBinding("container", "__001_volumen", "subsystem");
  db.addBinding("container", "__003_wgk", "system");
  db.addBinding("container", "__004_unterirdisch", "system");
  db.addBinding("system", "__007_letzte_prf", "system");

  db.flagAdd("system", "*", {"counter"});
  db.flagAdd("subsystem", "*", {"counter"});
  db.flagAdd("container", "*", {"counter"});

  qDebug() << "Done.";

  db.close();

  system("move /y ibis_db.sqlite3 ../build-IBISManager-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/ibis_db.sqlite3");
  system("move /y ibis_meta.sqlite3 ../build-IBISManager-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/ibis_meta.sqlite3");

  return a.exec();
}
