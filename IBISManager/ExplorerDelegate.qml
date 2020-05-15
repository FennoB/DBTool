import QtQuick 2.0

MouseArea
{
  height: visible ? 20 : 0
  width: 200

  property bool toggled: false
  property string iconSource: "Dir"
  property string descText: "[none]"
  property int treeLayer: 0
  property bool leaf: false
  property int hid: 0

  id: area

  hoverEnabled: true

  onClicked:
  {
    logic.open(hid)
  }

  onDoubleClicked:
  {
    logic.toggleDir(hid)
  }

  Rectangle
  {
    id: rect
    z: -1
    color: area.pressed || (entryForm["entry_hid"] === hid) ? "lightblue" :
                                                              area.containsMouse ? "#DDEEFF" : "white"

    anchors.left: semanticIcon.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.right
  }

  MouseArea
  {
    id: area2
    anchors.fill: toggleIcon

    hoverEnabled: true

    onClicked:
    {
      if(toggleIcon.visible)
      {
        logic.toggleDir(hid)
      }
    }
  }

  Image
  {
    Rectangle
    {
      z: -1
      anchors.fill: parent
      color: area2.pressed ? "lightblue" :
                             area2.containsMouse ? "#DDEEFF" : "white"

    }

    visible: !leaf
    id: toggleIcon
    opacity: toggled ? 1 : 0.5
    x: treeLayer * 15
    source: "sel_closed.png"
    anchors.verticalCenter: parent.verticalCenter
    rotation: toggled ? 90 : 0
  }

  Image
  {
    id: semanticIcon

    source: iconSource + ".png"

    anchors.leftMargin: 5
    anchors.left: toggleIcon.right
    anchors.verticalCenter: parent.verticalCenter
  }

  Text
  {
    id: content
    text: descText

    anchors.left: semanticIcon.right
    anchors.leftMargin: 10
    anchors.verticalCenter: parent.verticalCenter
  }
}
