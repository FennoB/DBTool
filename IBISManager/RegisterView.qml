import QtQuick 2.0
import QtQuick.Controls 2.2

Item
{
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: parent.width / 5
    anchors.rightMargin: parent.width / 5

    signal register()

    property bool success: false

    onVisibleChanged:
    {
        if(visible)
        {
            success = false
        }
    }

    Label
    {
        id: regLabelHeadline

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 100

        text: "Neuen Benutzer registrieren"
        font.pixelSize: 20
    }

    Label
    {
        id: regLabel

        anchors.left: parent.left
        anchors.top: regLabelHeadline.bottom
        anchors.topMargin: 20

        text: "Benutzer bereits vorhanden"
        color: success ? "green" :"red"
        visible: false
        font.pixelSize: 12
    }

    TextField
    {
        id: regFieldUsername

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: regLabelHeadline.bottom
        anchors.topMargin: 50

        font.pixelSize: 15
        placeholderText: "E-Mail"
        validator: RegExpValidator { regExp:/\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/ }
    }

    TextField
    {
        id: regFieldPassword

        anchors.left: parent.left
        anchors.right: parent.right

        anchors.top: regFieldUsername.bottom
        anchors.topMargin: 10

        font.pixelSize: 15
        placeholderText: "Passwort"
        echoMode: TextInput.Password
    }

    Button
    {
        id: regButton

        text: success ? "Schließen" : "Registrieren"

        anchors.top: regFieldPassword.bottom
        anchors.topMargin: 30
        anchors.left: parent.left

        onClicked:
        {
            if(success)
            {
                register()
            }

            if(!regFieldUsername.acceptableInput || regFieldUsername.text == "")
            {
                regLabel.text = "Bitte geben sie eine korrekte E-Mail Adresse ein."
                regLabel.visible = true
                return
            }

            if(regFieldPassword.text == "")
            {
                regLabel.text = "Bitte geben sie ein Passwort ein."
                regLabel.visible = true
                return
            }

            if(logic.registration(regFieldUsername.text, regFieldPassword.text))
            {
                regLabel.text = "Benutzer erfolgreich registriert!"
                regLabel.visible = true
                success = true
            }
            else
            {
                regLabel.text = "Benutzer bereits vorhanden."
                regLabel.visible = true
            }
        }
    }
}
