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

  property int cellWidth: 150
  property int cellHeight: 30

  Text
  {
    id: boxtext
    text: "Tabelle der Untereinträge: "
    font.pixelSize: 15
    anchors.left: parent.left
    anchors.verticalCenter: box.verticalCenter
    anchors.margins: 20
  }

  ComboBox
  {
    id: box
    anchors.top: parent.top
    anchors.left: boxtext.right
    anchors.margins: 20
    width: 200

    property int tableListIndex: logic.tableListIndex
    model: logic.tableList

    onCurrentIndexChanged:
    {
      if(currentIndex != logic.tableListIndex)
      {
        logic.setTable(currentIndex)
      }
    }

    onTableListIndexChanged:
    {
      if (model.length > 0 && currentIndex != logic.tableListIndex)
        currentIndex = logic.tableListIndex
    }
  }

  Text
  {
    text: "Suche in Feld: "
    font.pixelSize: 15
    anchors.margins: 20
    anchors.right: columnBox.left
    anchors.verticalCenter: box.verticalCenter
  }

  function getColumns(model)
  {
    var result = []
    for (var i = 0; i < model.length; ++i)
    {
      var raw = model[i]
      if(raw !== "id" && raw !== "entry_hid")
      {
        result.push(raw.substring(6, raw.length))
      }
    }

    return result
  }

  ComboBox
  {
    id: columnBox
    anchors.verticalCenter: box.verticalCenter
    anchors.right: search.left
    anchors.margins: 20
    property var columns: Object.keys(editor.model[0])
    model: getColumns(columns)
    onCurrentIndexChanged:
    {
      if (currentText != "" && logic.filter != search.text)
      {
        logic.search(search.text, columns[currentIndex])
      }
    }
  }

  TextField
  {
    id: search
    text: logic.filter

    anchors.top: parent.top
    anchors.right: parent.right
    anchors.margins: 20

    placeholderText: "Suchen"
    onTextChanged:
    {
      if (columnBox.currentText != "" && logic.filter != text)
      {
        logic.search(text, columnBox.columns[columnBox.currentIndex])
      }
    }
  }

  Flickable
  {
    anchors.top: search.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: 30


    contentWidth: col.width
    contentHeight: col.height
    clip: true

    Column
    {
      id: col
      Row
      {
        Repeater
        {
          id: names_rep
          model: Object.keys(editor.model[0]).length

          delegate: Item
          {
            width: editor.cellWidth
            height: editor.cellHeight
            Text
            {
              id: txt_name
              property string raw: Object.keys(editor.model[0])[index]
              text: raw.substring(6, raw.length)
              font.family: "Roboto"
              font.pixelSize: 18
              width: editor.cellWidth
              height: editor.cellHeight
              fontSizeMode: Text.Fit

              anchors.centerIn: parent
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter
            }
            visible: txt_name.raw != "id" && txt_name.raw != "entry_hid"
          }
        }
      }

      Repeater
      {
        id: rows_rep
        model: editor.model.length

        Row
        {
          id: row
          property int rowIndex: index
          Repeater
          {
            id: cols_rep
            model: editor.model[0]["entry_hid"] > 0 ? Object.keys(editor.model[0]).length : 0

            delegate: Item
            {
              property string key: Object.keys(editor.model[0])[index]
              property bool isDate: (Object.prototype.toString.call(editor.model[row.rowIndex][key]) === '[object Date]')
              property string type: isDate ? "date" : typeof( editor.model[row.rowIndex][key] )
              visible: key != "id" && key != "entry_hid"
              width: editor.cellWidth
              height: editor.cellHeight

              TextField
              {
                anchors.fill: parent
                font.family: "Roboto"
                font.pixelSize: 16
                text: editor.model[row.rowIndex][key]
                onTextChanged:
                {
                  logic.setTableValue(row.rowIndex, key, text)
                }

                visible: parent.type === "string"
              }

              TextField
              {
                anchors.fill: parent
                font.family: "Roboto"
                font.pixelSize: 16
                text: editor.model[row.rowIndex][key]
                onTextChanged:
                {
                  logic.setTableValue(row.rowIndex, key, text)
                }

                validator: DoubleValidator
                {

                }

                visible: parent.type === "number"
              }

              TextField
              {
                anchors.fill: parent
                font.family: "Roboto"
                font.pixelSize: 16
                text: !visible ? "" : logic.toGermanDateTime(editor.model[row.rowIndex][key], "date")
                onTextChanged:
                {
                  logic.setTableValue(row.rowIndex, key, logic.fromGermanDateTime(text, "date"))
                }

                validator: RegExpValidator {
                  regExp: /^\s*(3[01]|[12][0-9]|0?[1-9])\.(1[012]|0?[1-9])\.((?:19|20)\d{2})\s*$/
                }

                visible: parent.type === "date"
              }
            }
          }
        }
      }
    }
  }
}
