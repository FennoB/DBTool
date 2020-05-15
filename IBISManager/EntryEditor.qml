import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle
{
  id: editor
  implicitHeight: 100
  implicitWidth: 100
  border.color: "#333333"
  border.width: 3
  radius: 5
  color: "white"

  property var model: null

  ListView
  {
    id: list
    model: Object.keys(editor.model).length+1

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    width: parent.width / 2
    anchors.margins: 30

    spacing: 10
    clip: true

    delegate: Row
    {
      property bool isDate: index == 0 ? false : (Object.prototype.toString.call(editor.model[Object.keys(editor.model)[index-1]]) === '[object Date]')
      property string type: index == 0 ? "string" : isDate ? "date" : typeof( editor.model[Object.keys(editor.model)[index-1]])

      Text
      {
        id: txt
        property string raw: index == 0 ? "__000_Name" : Object.keys(editor.model)[index-1]
        text: raw.substring(6, raw.length)
        font.family: "Roboto"
        font.pixelSize: 18
        width: 200
        visible: raw != "id" && raw != "entry_hid"
      }

      TextField
      {
        font.family: "Roboto"
        font.pixelSize: 16
        text: index == 0 ? name : editor.model[Object.keys(editor.model)[index-1]]
        onTextChanged:
        {
          logic.setValue(index == 0 ? "name" : Object.keys(editor.model)[index-1], text)
        }

        visible: txt.visible && parent.type === "string"
      }

      TextField
      {
        font.family: "Roboto"
        font.pixelSize: 16
        text: index == 0 ? name : editor.model[Object.keys(editor.model)[index-1]]
        onTextChanged:
        {
          logic.setValue(index == 0 ? "name" : Object.keys(editor.model)[index-1], text)
        }

        validator: DoubleValidator
        {

        }

        visible: txt.visible && parent.type === "number"
      }

      TextField
      {
        font.family: "Roboto"
        font.pixelSize: 16
        text: !visible ? "" : index == 0 ? name : logic.toGermanDateTime(editor.model[Object.keys(editor.model)[index-1]], "date")
        onTextChanged:
        {
          logic.setValue(index == 0 ? "name" : Object.keys(editor.model)[index-1], logic.fromGermanDateTime(text, "date"))
        }

        validator: RegExpValidator {
          regExp: /^\s*(3[01]|[12][0-9]|0?[1-9])\.(1[012]|0?[1-9])\.((?:19|20)\d{2})\s*$/
        }

        visible: txt.visible && parent.type === "date"
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
      font.pixelSize: 25
      text: "Hinterlegte Dateien: "
    }

    Button
    {
      anchors.verticalCenter: fileListDesc.verticalCenter
      text: "Hinzufügen"
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
    //model: logic.fileModel
    spacing: 10

    model: 5

    delegate: Row
    {
      spacing: 10

      Button
      {
        text: "Öffnen"
      }

      Button
      {
        text: "Entfernen"
      }

      Text
      {
        text: "Datei" + index + ".pdf" //model[index]
        font.pixelSize: 20
      }
    }
  }
}
