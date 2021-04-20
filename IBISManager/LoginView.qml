import QtQuick 2.1
import QtQuick.Controls 2.1

// The LoginView Item

Item
{
  signal login()

  Label
  {
    id: loginLabelHeadline

    anchors.left: parent.left
    anchors.top: parent.top
    anchors.topMargin: 100
    color: "#FFFFFF"

    text: "Willkommen."
    font.pixelSize: 20
  }

  Label
  {
    id: loginLabelError

    anchors.left: parent.left
    anchors.top: loginLabelHeadline.bottom
    anchors.topMargin: 20

    text: "Email oder Passwort falsch"
    color: "red"
    visible: false
    font.pixelSize: 12
  }

  TextField
  {
    id: loginFieldUsername

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: loginLabelHeadline.bottom
    anchors.topMargin: 50

    font.pixelSize: 15
    placeholderText: "Email"
  }

  TextField
  {
    id: loginFieldPassword

    anchors.left: parent.left
    anchors.right: parent.right

    anchors.top: loginFieldUsername.bottom
    anchors.topMargin: 10

    font.pixelSize: 15
    placeholderText: "Passwort"
    echoMode: TextInput.Password
  }

  Button
  {
    id: loginButton

    text: "Anmelden"

    anchors.top: loginFieldPassword.bottom
    anchors.topMargin: 30
    anchors.left: parent.left

    onClicked:
    {
      if(logic.login(loginFieldUsername.text, loginFieldPassword.text))
      {
        login()
      }
      else
      {
        loginLabelError.visible = true
      }
    }

    Keys.onReturnPressed: clicked()
  }

  Component.onCompleted:
  {
    //logic.login("frodo", "frodo")
    //login()
  }
}
