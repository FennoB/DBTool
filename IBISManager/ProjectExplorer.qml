import QtQuick 2.0

Rectangle
{
  width: parent.width / 4
  height: parent.height - y

  border.color: "#333333"
  border.width: 3
  radius: 5
  color: "white"

  Rectangle
  {
    id: caption
    color: "#888888"
    width: parent.width
    height: 20
    y: -20
    radius: 3

    Rectangle
    {
      color: "#333333"
      width: 100
      height: 20
      radius: 3
      Text
      {
        anchors.leftMargin: 20
        anchors.left: parent.left
        color: "white"
        text: "Projekte"
        font.family: "Roboto"
        font.pixelSize: 14
      }
    }
  }


  ListView
  {
    anchors.margins: 5
    anchors.fill: parent
    clip: true
    model: explorerList
    delegate: ExplorerDelegate
    {
      anchors.left: parent.left
      anchors.right: parent.right

      iconSource: _source
      descText: _text
      treeLayer: _layer
      visible: _visible
      toggled: _open
      hid: _hid
      leaf: index + 1 === explorerList.length || explorerList[index+1]._layer <= _layer
    }
  }

}
