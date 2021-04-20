import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.1

Item
{
  id: root

  function find(model, criteria) {
    for(var i = 0; i < model.count; ++i)
    {
      if (criteria(model.get(i)))
      {
        return model.get(i)
      }
    }
    return null
  }

  function contains(model, value)
  {
    return find(model, function (item) {return item === value}) !== null
  }

  Row
  {
    id: menuBar
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.margins: 3

    Button
    {
      id: b_new
      height: root.height / 30
      width: 100
      background:
      Rectangle
      {
        color: !parent.open ? "#555555" : "#333333"
      }

      text: "<font color='#ffffff'>" + "Import/Export" + "</font>"

      onClicked:
      {
        open ? open=false : open=true
      }
      property bool open: false
      onOpenChanged:
      {
        open ? m_new.open() : m_new.close()
      }

      Menu
      {
        id: m_new
        title: "Import/Export"
        y: parent.height

        onClosed: parent.clicked()
        MenuItem
        {
          text: "Werte aus Excel übernehmen "
          onTriggered:
          {
            fileDialog.method = "entry"
            fileDialog.open()
            b_new.open = false
          }
        }

        MenuItem
        {
          text: "Untereinträge aus Excel übernehmen"
          onTriggered:
          {
            fileDialog.method = "subentries"
            fileDialog.open()
            b_new.open = false
          }
        }
      }
    }

    Button
    {
      id: b_entry
      background:
      Rectangle
      {
        color: !parent.open ? "#555555" : "#333333"
      }

      text: "<font color='#ffffff'>" + "Eintrag" + "</font>"

      height: root.height / 30
      width: 100

      onClicked: open = !open
      property bool open: false
      onOpenChanged:
      {
        open ? m_entry.open() : m_entry.close()
      }

      Menu
      {
        y: parent.height
        id: m_entry
        title: "Eintrag"

        onClosed: parent.clicked()

        Repeater
        {
          model: logic.menuModel
          MenuItem
          {
            text: modelData + " hinzufügen"
            onTriggered:
            {
              b_entry.open = false
              logic.menuClicked(0, modelData)
            }
          }
        }

        MenuItem
        {
          text: "Diesen Eintrag löschen"
          enabled: logic.clientCount > 1 || !logic.clientSelected
          onTriggered:
          {
            messagebox.icon = StandardIcon.Critical
            messagebox.standardButtons = StandardButton.Yes | StandardButton.No
            messagebox.title = "Löschen bestätigen"
            messagebox.text = "Diesen Eintrag wirklich löschen? \nAlle Untereinträge werden ebenfalls gelöscht."
            messagebox.visible = true
            messagebox.method = "removeEntry"
            b_entry.open = false
          }
        }
      }
    }

    Button
    {
      id: b_manage
      background:
      Rectangle
      {
        color: !parent.open ? "#555555" : "#333333"
      }

      text: "<font color='#ffffff'>" + "Verwalten" + "</font>"
      height: root.height / 30
      width: 100

      onClicked: open ? open=false : open=true
      property bool open: false
      onOpenChanged:
      {
        open ? m_manage.open() : m_manage.close()
      }

      Menu
      {
        y: parent.height
        id: m_manage
        title: "Verwalten"

        onClosed: parent.clicked()
        MenuItem
        {
          text: "Neuer Kunde"
          onTriggered:
          {
            b_manage.open = false
            logic.addClient()
          }
        }

        MenuItem
        {
          text: "Neuer Benutzer"
          onTriggered:
          {
            b_manage.open = false
            regView.visible = true
            mainContent.visible = false
          }
        }
      }
    }
  }


  Item
  {
    id: mainContent
    anchors.top: menuBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: 3

    ProjectExplorer
    {
      y: 20
      anchors.left: parent.left
      id: expl
    }

    Item
    {
      id: tabbar
      anchors.top: parent.top
      anchors.left: expl.right
      anchors.right: parent.right
      anchors.leftMargin: 3
      height: 20

      Row
      {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Rectangle
        {
          id: entryTab
          color: !entryEditor.visible ? "#555555" : "#333333"
          width: 100
          height: 20
          radius: 3

          MouseArea
          {
            anchors.fill: parent
            onClicked:
            {
              tableEditor.visible = false
              entryEditor.visible = true
            }
          }

          Text
          {
            anchors.leftMargin: 20
            anchors.left: parent.left
            color: "white"
            text: "Eintrag"
            font.family: "Roboto"
            font.pixelSize: 14
          }
        }

        Rectangle
        {
          id: tableTab
          color: !tableEditor.visible ? "#555555" : "#333333"
          width: 100
          height: 20
          radius: 3

          MouseArea
          {
            anchors.fill: parent
            onClicked:
            {
              tableEditor.visible = true
              entryEditor.visible = false
            }
          }

          Text
          {
            anchors.leftMargin: 20
            anchors.left: parent.left
            color: "white"
            text: "Tabelle"
            font.family: "Roboto"
            font.pixelSize: 14
          }
        }

      }

    }


    EntryEditor
    {
      id: entryEditor
      model: entryForm
      visible: true
      anchors.top: tabbar.bottom
      anchors.left: expl.right
      anchors.bottom: parent.bottom
      anchors.right: parent.right
      anchors.leftMargin: 3
    }

    TableEditor
    {
      id: tableEditor
      model: tableForm
      visible: false
      anchors.top: tabbar.bottom
      anchors.left: expl.right
      anchors.bottom: parent.bottom
      anchors.right: parent.right
    }
  }

  RegisterView
  {
    id: regView
    anchors.fill: parent
    visible: false

    onClose:
    {
      regView.visible = false
      mainContent.visible = true
    }
  }

  FileDialog
  {
    id: fileDialog
    property string method: ""
    title: "Datei wählen"
    nameFilters: method === "entry" || method === "subentries" ? [ "CSV Tabelle (*.csv)", "Andere Datei (*)" ] : [ "PDF-Dokument (*.pdf)", "Andere Datei (*)"]
    folder: shortcuts.home
    onAccepted:
    {
      if (method == "entry")
      {
        logic.csvEntryImport(fileUrl)
      }
      else if (method == "subentries")
      {
        logic.csvSubentriesImport(fileUrl)
      }
      else if (method == "addfile")
      {
        logic.addFile(fileUrl)
      }
    }

    onRejected:
    {
    }
  }

  MessageDialog
  {
    id: messagebox
    title: ""
    width: 500
    height: 200
    icon: StandardIcon.Critical
    text: ""
    property string method: "info"

    onAccepted:
    {
      if (method == "removeEntry")
      {
        logic.removeEntry()
      }
    }

    onYes:
    {
      if (method == "removeEntry")
      {
        logic.removeEntry()
      }
    }

    onNo:
    {
    }

    onRejected:
    {
    }
  }

  Connections
  {
    target: logic
    onDialog:
    {
      if (mode == "error")
      {
        messagebox.icon = StandardIcon.Critical
        messagebox.standardButtons = StandardButton.Ok
        messagebox.title = params[0]
        messagebox.text = params[1]
        messagebox.visible = true
      }

      if (mode == "addfile")
      {
        fileDialog.method = mode
        fileDialog.open()
      }
    }
  }

}
