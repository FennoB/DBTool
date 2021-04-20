import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle
{
  id: editor
  implicitHeight: 100
  implicitWidth: 100
  border.color: "#333333"
  border.width: 3
  color: "#FFFFFF" //"#88FFAA"

  function hiddenField(field)
  {
    return field === "entry_hid" || field === "id" || field.includes("color")
  }

  property var model: null
  property bool editing: false
  onModelChanged:
  {
    list.model = 0
    list.model = Object.keys(editor.model).length+1
    editing = false
  }

  Button
  {
    id: b_edit
    text: editing ? "Speichern" : "Bearbeiten"
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.margins: 20
    height: 30
    font.pixelSize: 16

    onClicked:
    {
      if (editing)
      {
        logic.saveEntry()
      }
      else
      {
        editing = !editing
      }
    }
  }

  Button
  {
    id: b_abort
    text: "Verwerfen"
    visible: editing
    anchors.left: b_edit.right
    anchors.top: parent.top
    anchors.margins: 20
    height: 30
    font.pixelSize: 16

    onClicked:
    {
      if (editor.editing)
      {
        editor.editing = false
        logic.open(editor.model["entry_hid"])
      }
    }
  }


  ListView
  {
    id: list
    model: Object.keys(editor.model).length+1

    anchors.top: b_edit.bottom
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    width: parent.width / 2.5
    anchors.margins: 20

    spacing: 5

    clip: true

    delegate: Item
    {
      height: 32
      Row
      {
        anchors.verticalCenter: parent.verticalCenter
        id: rw
        property string key: index == 0 ? "__000_Name" : Object.keys(editor.model)[index-1]
        property var content: index == 0 ? name : editor.model[key]
        property bool isDate: Object.prototype.toString.call(content) === '[object Date]'
        property string text: isDate ? logic.toGermanDateTime(content, "date") : content
        property string type: isDate ? "date" : typeof(content)
        property bool changeable: editor.editing && !(key in columnFlags && columnFlags[key].indexOf("function") > -1)

        Text
        {
          id: txt
          text: logic.beautifyColumnName(rw.key)
          font.family: "Roboto"
          font.pixelSize: 14
          width: 100
          visible: !hiddenField(rw.key)
        }

        Rectangle
        {
          width: 300
          height: 30
          border.color: "darkgrey"
          color: "lightgrey"
          Text
          {
            id: greytext
            font.family: "Roboto"
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins: 10
            text: rw.text
          }
          visible: txt.visible && !parent.changeable
        }

        TextField
        {
          font.family: "Roboto"
          font.pixelSize: 12
          width: 300
          height: 30
          property string content: rw.text
          text: content
          onContentChanged: text = content
          onTextChanged:
          {
            if (visible)
            {
              logic.setValue(index == 0 ? "name" : rw.key, parent.isDate ? logic.fromGermanDateTime(text, "date") : text)
            }
          }
          onVisibleChanged:
          {
              logic.setValue(index == 0 ? "name" : rw.key, parent.isDate ? logic.fromGermanDateTime(text, "date") : text)
          }

          property var dval: DoubleValidator
          {

          }

          property var dtval: RegExpValidator
          {
            regExp: /[0-9.]+/
          }

          validator: parent.type === "number" ? dval : parent.isDate ? dtval : null

          visible: txt.visible && !combo.visible && parent.changeable
        }

        ComboBox
        {
          id: combo
          font.family: "Roboto"
          font.pixelSize: 12
          width: 300
          height: 30

          visible: entryDropdowns[rw.key] !== undefined && entryDropdowns[rw.key] !== null && parent.changeable

          model: !visible ? 0 : entryDropdowns[rw.key]
          property int content: !visible ? 0 : model.indexOf(rw.content)

          onCurrentIndexChanged:
          {
            if (visible)
            {
              logic.setValue(rw.key, model[currentIndex])
            }
          }

          onContentChanged:
          {
            if (visible && currentIndex !== content)
            {
              currentIndex = content
            }
          }
        }
      }
    }
  }

  Row
  {
    id: fileListHeader

    anchors.margins: 30
    anchors.top: parent.top
    anchors.left: list.right

    spacing: 50

    Text
    {
      id: fileListDesc
      font.pixelSize: 16
      text: "Hinterlegte Dateien: "
    }

    Button
    {
      anchors.verticalCenter: fileListDesc.verticalCenter
      text: "Hinzufügen"
      font.pixelSize: 16
      height: 30

      onClicked: logic.addFile()
    }
  }

  ListView
  {
    id: fileList
    anchors.left: list.right
    anchors.top: fileListHeader.bottom
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.margins: 20
    model: fileModel
    spacing: 10

    delegate: Row
    {
      spacing: 10

      Button
      {
        text: "Öffnen"
        height: 30
        onClicked: logic.openFile(index)
      }

      Button
      {
        text: "Entfernen"
        height: 30
        onClicked: logic.removeFile(index)
      }

      TextField
      {
        text: modelData.name
        height: 30
        onTextChanged: logic.setFileName(index, text)
        font.pixelSize: 16
      }
    }
  }
}
