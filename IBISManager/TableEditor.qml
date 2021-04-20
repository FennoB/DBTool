import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle
{
  id: editor
  implicitHeight: 100
  implicitWidth: 100
  border.color: "#333333"
  border.width: 3
  color: "white"

  property var model: null

  property int cellWidth: 150
  property int cellHeight: 30

  function hiddenField(field)
  {
    return field === "entry_hid" || field === "id" || field.includes("color")
  }

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
      if (!hiddenField(raw))
      {
        result.push(logic.beautifyColumnName(raw))
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

  Rectangle
  {
    anchors.top: search.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: 20

    border.color: "#aaaaaa"
    border.width: 1
    color: "#00000000"

    Flickable
    {
      id: flicky
      anchors.fill: parent
      anchors.margins: 5

      contentWidth: col.width
      contentHeight: col.height
      clip: true

      property int editRow: -1
      property string editKey: ""


      //MouseArea
      //{
      //  anchors.fill: parent
      //  onPressed:
      //  {
      //    var keys = 0
      //    for (var i = 0; i < Object.keys(editor.model[0]).length; ++i)
      //    {
      //      if (!hiddenField(Object.keys(editor.model[0])[i]))
      //      {
      //        ++keys;
      //      }
      //    }
      //
      //    var clickrow = Math.floor(((mouseY - textrow.height) / (height - textrow.height)) * editor.model.length)
      //    var clickcol = Math.floor((mouseX / width) * keys)
      //
      //    flicky.editKey = Object.keys(editor.model[0])[clickcol]
      //    while (hiddenField(flicky.editKey))
      //    {
      //      clickcol++
      //      if (clickcol >= Object.keys(editor.model[0]).length)
      //      {
      //        flicky.editKey = ""
      //        flicky.editRow = -1
      //        return
      //      }
      //
      //      flicky.editKey = Object.keys(editor.model[0])[clickcol]
      //    }
      //
      //    flicky.editRow = clickrow
      //  }
      //}

      Column
      {
        id: col
        spacing: 0
        Row
        {
          id: textrow
          spacing: 2
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
                text: logic.beautifyColumnName(raw)
                font.family: "Roboto"
                font.pixelSize: 18
                width: editor.cellWidth
                height: editor.cellHeight
                fontSizeMode: Text.Fit
                //color: "white"

                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
                MouseArea
                {
                  anchors.fill: parent
                  onClicked:
                  {
                    logic.sort(parent.raw)
                  }
                  Image
                  {
                    visible: logic.sortingColumn !== txt_name.raw ||
                             (logic.sortingColumn === txt_name.raw &&
                              logic.sortingDescending)
                    anchors.right: parent.right
                    anchors.top: parent.top
                    source: "sel_closed.png"
                    rotation: -90
                    anchors.margins: 5
                  }
                  Image
                  {
                    visible: logic.sortingColumn !== txt_name.raw ||
                             (logic.sortingColumn === txt_name.raw &&
                              !logic.sortingDescending)
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    source: "sel_closed.png"
                    rotation: 90
                    anchors.margins: 5
                  }
                }
              }
              visible: !hiddenField(txt_name.raw)
            }
          }
        }

        Repeater
        {
          id: rows_rep
          model: editor.model.length
          onModelChanged:
          {
            flicky.editKey = ""
            flicky.editRow = -1
          }

          Rectangle
          {
            id: background
            color: "lightgrey"
            property string table: box.tableListIndex
            onTableChanged: color = "lightgrey"
            height: row.height+2
            width: row.width+4

            Row
            {
              id: row
              property int rowIndex: index
              spacing: 2
              anchors.centerIn: parent
              Repeater
              {
                id: cols_rep
                model: editor.model[0]["entry_hid"] > 0 ? Object.keys(editor.model[0]).length : 0

                delegate: Rectangle
                {
                  property string key: Object.keys(editor.model[0])[index]
                  property bool isDate: (Object.prototype.toString.call(editor.model[row.rowIndex][key]) === '[object Date]')
                  property string type: isDate ? "date" : typeof( editor.model[row.rowIndex][key] )
                  property bool changeable: false //!(key in tableColumnFlags && tableColumnFlags[key].indexOf("function") > -1)
                  property bool editMode: changeable && flicky.editKey === key && flicky.editRow == row.rowIndex
                  visible: !hiddenField(key)
                  width: editor.cellWidth
                  height: editor.cellHeight
                  color: "#FFF" //changeable ? "#FFF" : "lightgrey"
                  border.color: "#AAA"
                  border.width: 1
                  clip: true
                  onKeyChanged:
                  {
                    if (key.includes("color"))
                    {
                      background.color = editor.model[row.rowIndex][key]
                    }
                  }

                  Text
                  {
                    id: t_content
                    anchors.fill: parent
                    anchors.margins: 5
                    font.pointSize: 12
                    color: "#333"
                    property string content: parent.isDate ? logic.toGermanDateTime(editor.model[row.rowIndex][key], "date") :
                          editor.model[row.rowIndex][key]
                    property string linkContent: "<a href=\"" +
                                                 editor.model[row.rowIndex]["entry_hid"] +
                                                 "\">" + content + "</a>"
                    text: key == "__000_name" ? linkContent : content
                    onLinkActivated:
                    {
                      logic.open(link)
                    }
                    visible: !parent.editMode
                  }

                  Loader
                  {
                    source: parent.editMode ? "TableEntryForm.qml" : ""
                    anchors.fill: parent
                    onLoaded:
                    {
                      if (parent.editMode)
                      {
                        item.key = parent.key
                        item.row = row.rowIndex
                        item.isDate = parent.isDate
                        item.type = parent.type
                        item.forceActiveFocus()
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
