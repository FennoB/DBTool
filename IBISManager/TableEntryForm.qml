import QtQuick 2.0
import QtQuick.Controls 2.0

Item
{
  property string key: ""
  property int row: 0
  property string type: ""
  property bool isDate: false
  anchors.fill: parent
  onActiveFocusChanged: child.forceActiveFocus()

  TextField
  {
    id: child
    anchors.fill: parent
    font.family: "Roboto"
    text: parent.isDate ? logic.toGermanDateTime(editor.model[row][key], "date") :
                          editor.model[row][key]

    font.pointSize: 12
    color: "#333"

    onTextChanged:
    {
      if(parent.isDate)
      {
        if (text != "")
        {
          logic.setTableValue(row, key, logic.fromGermanDateTime(text, "date"))
        }
      }
      else
      {
        logic.setTableValue(row, key, text)
      }
    }

    // Add validators here
    property var doubleval:
    DoubleValidator
    {

    }

    property var dateval:
    RegExpValidator
    {
      regExp: /[0-9.]+/
    }

    validator: parent.type === "number" ? doubleval : parent.type === "date" ? dateval : null

    visible: !combo.visible
    enabled: visible
  }

  ComboBox
  {
    id: combo
    font.family: "Roboto"
    font.pixelSize: 12
    width: 300
    height: 30

    visible: tableDropdowns[key] !== undefined && tableDropdowns[key] !== null

    model: !visible ? 0 : tableDropdowns[key]
    currentIndex: content
    property int content: !visible ? 0 : model.indexOf(editor.model[row][key])

    onCurrentIndexChanged:
    {
      if (visible)
      {
        logic.setTableValue(row, key, model[currentIndex])
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
