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

    Button
    {
      id: b_new
      text: "Neu..."
      height: root.height / 30

      onClicked: open ? open=false : open=true
      property bool open: false
      onOpenChanged:
      {
        open ? m_new.open() : m_new.close()
      }

      Menu
      {
        id: m_new
        title: "Neu"
        y: parent.height

        MenuItem
        {
          text: "Kunde"
          onTriggered:
          {
            b_new.open = false
            logic.addClient()
          }
        }

        Repeater
        {
          model: logic.menuModel
          MenuItem
          {
            text: modelData
            onTriggered:
            {
              b_new.open = false
              logic.menuClicked(0, modelData)
            }
          }
        }

        MenuItem
        {
          text: "Benutzer"
          onTriggered:
          {
            regView.visible = true
            mainContent.visible = false
          }
        }

        MenuSeparator { }
        MenuItem
        {
          text: qsTr("Schließen")
          onTriggered: Qt.quit()
        }
      }
    }

    Button
    {
      id: b_entry

      text: "Eintrag"
      height: root.height / 30

      onClicked: m_entry.open()

      Menu
      {
        y: parent.height
        id: m_entry
        title: "Eintrag"
        MenuItem
        {
          text: "Werte importieren"
          onTriggered:
          {
            fileDialog.method = "entry"
            fileDialog.open()
          }
        }

        MenuItem
        {
          text: "Untereinträge importieren"
          onTriggered:
          {
            fileDialog.method = "subentries"
            fileDialog.open()
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

    ProjectExplorer
    {
      y: 20
      anchors.left: parent.left
      id: expl
    }

    Rectangle
    {
      id: tabbar
      anchors.top: parent.top
      anchors.left: expl.right
      anchors.right: parent.right
      height: 20
      radius: 3
      color: "#888888"

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

    onRegister:
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
    nameFilters: [ "CSV Tabelle (*.csv)", "Andere Datei (*)" ]
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
    //detailedText: "To replace a file means that its existing contents will be lost. " +
    //              "The file that you are copying now will be copied over it instead."

    //onAccepted: dialog.open()
    //onNo: console.log("didn't copy")
    //onRejected: console.log("aborted")
  }

  Connections
  {
    target: logic
    onMessageBox:
    {
      if (mode == "error")
      {
        messagebox.icon = StandardIcon.Critical
        messagebox.standardButtons = StandardButton.Ok
        messagebox.title = params[0]
        messagebox.text = params[1]
        messagebox.visible = true
      }
    }
  }

}
