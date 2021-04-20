import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 2.1

ApplicationWindow
{
  id: root
  visible: true
  width: 640
  height: 450

  minimumWidth: 640
  minimumHeight: 450

  title: qsTr("IBIS Manager")
  color: "#444444"

  Loader
  {
    source: logic.loggedIn ? "MainMenuView.qml" : ""
    anchors.fill: parent
    visible: logic.loggedIn
  }

  LoginView
  {
    z: 2
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: parent.width / 5
    anchors.rightMargin: parent.width / 5
    visible: !logic.loggedIn
    onLogin:
    {
      root.showMaximized()
    }
  }
}
