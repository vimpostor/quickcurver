import QtQuick 2.7
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

import Game 1.0
import QtQuick.Layouts 1.1

ApplicationWindow {
    onClosing: game.close()
    property bool clientInGame: false;
    function sendMessage(username, message) {
        pageStack.currentItem.sendMessage(username, message);
    }
    function changeScore(index, newScore, roundScore) {
        playerListModel.setProperty(index, "escore", newScore);
        playerListModel.setProperty(index, "eroundScore", roundScore);
    }
    function setJoinStatus(s) {
        if (s === "TCPACK") {
            joinButton.text = "Testing UDP connection...";
        } else if (s === "JOINED") {
            joinButton.text = "Joined, waiting for host to start...";
        } else if (s === "REJECTED") {
            clientDialog.close();
            playerselector.mysnackbar.open("Join request was rejected :(");
        } else if (s === "TIMEOUT") {
            clientDialog.close();
            playerselector.mysnackbar.open("Join request timed out! :(");
        } else if (s === "STARTED") {
            clientInGame = true;
            clientDialog.close();
            pageStack.push(Qt.resolvedUrl("gamePage.qml"));
            game.focus = true;
        } else if (s === "KICKED") {
            clientDialog.close();
            playerselector.mysnackbar.open("You got kicked from the game!");
        }
    }
    function setPlayerStatus(index,s) {
        if (s === "JOINED") {
            playerListModel.setProperty(index, "eJoined", true);
        } else if (s === "LEFT") {
            playerListModel.setProperty(index, "eJoined", false);
            playerListModel.setProperty(index, "eReady", false);
        } else if (s === "READY") {
            playerListModel.setProperty(index, "eReady", true);
        } else if (s === "UNREADY") {
            playerListModel.setProperty(index, "eReady", false);
        } else if (s.substring(0, 8) === "USERNAME") {
            playerListModel.setProperty(index, "ename", s.substring(8));
        }
    }
    function notifyGUI(s, mode) {
        if (mode === "SNACKBAR") {
            playerselector.mysnackbar.open(s);
        }
    }

    id: root
    width: 1400
    height: 1150
    minimumWidth: 1400
    minimumHeight: 997
    title: "Quick Curver"
    visible: true
    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
    }

    initialPage: Page {
        title: "Quick Curver"
        actionBar.maxActionCount: 4
        actions: [
            Action {
                iconName: "action/search_web"
                name: "Join an online game"
                onTriggered: clientDialog.show()
                shortcut: "Ctrl+J"
            },
            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },
            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: pageStack.push(Qt.resolvedUrl("settings.qml"))
            },
            Action {
                name: "Licenses"
                onTriggered: licensesDialog.open()
            },
            Action {
                name: "Quit"
                onTriggered: close()
            }

        ]
        PlayerSelector {
            id: playerselector
            anchors.fill: parent
        }
    }
    Dialog {
        id: colorPicker
        title: "Pick color"
        positiveButtonText: "Done"
        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color"]
            width: dp(160)
        }
        Grid {
            columns: 7
            spacing: dp(8)
            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]
                Rectangle {
                    width: dp(30)
                    height: dp(30)
                    radius: dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)
                    Ink {
                        anchors.fill: parent
                        onPressed: {
                            switch(selection.selectedIndex) {
                            case 0:
                                theme.primaryColor = parent.color
                                break;
                            case 1:
                                theme.accentColor = parent.color
                                break;
                            case 2:
                                theme.backgroundColor = parent.color
                                break;
                            }
                        }
                    }
                }
            }
        }

    }
    Item {
        y: 55
        x: parent.width - 1000
        Game {
            id: game
            objectName: "game"
            anchors.fill: parent

            Keys.onPressed:  {
                game.sendKey(event.key);
            }
            Keys.onReleased: {
                game.releaseKey(event.key);
            }
        }
    }
    ListModel {
        id: playerListModel
        ListElement {
            ename: "Player 0"
            escore: 0
            eroundScore: 0
            eBot: false
            eJoined: false
            eReady: false
        }
        ListElement {
            ename: "Player 1"
            escore: 0
            eroundScore: 0
            eBot: true
            eJoined: false
            eReady: false
        }
    }
    Dialog {
        id: licensesDialog
        hasActions: false
        anchors.fill: parent
        anchors.margins: dp(80)
        Licenses {
        }
    }
    Dialog {
        id: clientDialog
        title: "Join Online Game"
        hasActions: false
        ColumnLayout {
            width: dp(360)
            spacing: dp(16)
            Item { // dummy element to create an height offset at the beginning
                height: dp(0)
                Layout.fillHeight: true
            }
            RowLayout {
                Layout.fillWidth: true
                IconButton {
                    iconName: "content/paste"
                    onClicked: {
                        var c = game.getClipboardContent();
                        if (c === "") {
                            playerselector.mysnackbar.open("No text in clipboard!");
                        } else {
                            serverIp.text = c;
                        }
                    }
                }
                TextField {
                    id: serverIp
                    Layout.fillWidth: true
                    placeholderText: "Server IP Adress"
                    floatingLabel: true
                    Keys.onReturnPressed: {
                        joinButton.clicked();
                    }
                }
                Label {
                    text: ":"
                }

                TextField {
                    id: serverPort
                    Layout.fillWidth: true
                    placeholderText: "Port"
                    floatingLabel: true
                    text: "52552"
                }
            }
            Card {
                height: joinDialogDataLayout.height + dp(32)
//                width: joinDialogDataLayout.width + dp(64)
//                Layout.fillHeight: true
                Layout.fillWidth: true
                anchors.margins: dp(16)
                ColumnLayout {
                    id: joinDialogDataLayout
                    spacing: dp(16)
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: dp(16)
                    ListItem.Standard {
                        text: "Username"
                        iconName: "action/account_circle"
                        secondaryItem: TextField {
                            id: clientUserNameTextField
                            anchors.centerIn: parent
                            placeholderText: "Username"
                            floatingLabel: true
                            onTextChanged: game.changeClientSettings(text, clientReadyCheckBox.checked)
                        }
                    }
                    ListItem.Standard {
                        text: "Ready"
                        iconName: "action/check_circle"
                        secondaryItem: CheckBox {
                            id: clientReadyCheckBox
                            anchors.centerIn: parent
                            checked: false
                            onCheckedChanged: game.changeClientSettings(clientUserNameTextField.text, checked)
                        }
                        onClicked: clientReadyCheckBox.checked = !clientReadyCheckBox.checked
                    }
                }
            }
            RowLayout {
                ProgressCircle {
                    id: cyclicColorProgress
                    visible: false
                    SequentialAnimation {
                        id: animation
                        running: true
                        loops: Animation.Infinite
                        ColorAnimation {
                            from: "red"
                            to: "blue"
                            target: cyclicColorProgress
                            properties: "color"
                            easing.type: Easing.InOutQuad
                            duration: 2400
                        }

                        ColorAnimation {
                            from: "blue"
                            to: "green"
                            target: cyclicColorProgress
                            properties: "color"
                            easing.type: Easing.InOutQuad
                            duration: 1560
                        }

                        ColorAnimation {
                            from: "green"
                            to: "#FFCC00"
                            target: cyclicColorProgress
                            properties: "color"
                            easing.type: Easing.InOutQuad
                            duration:  840
                        }

                        ColorAnimation {
                            from: "#FFCC00"
                            to: "red"
                            target: cyclicColorProgress
                            properties: "color"
                            easing.type: Easing.InOutQuad
                            duration:  1200
                        }
                    }
                }
                Button {
                    id: joinButton
                    Layout.fillWidth: true
                    elevation: 1
                    text: "Join"
                    backgroundColor: Theme.accentColor
                    onClicked: {
                        enabled = false;
                        cyclicColorProgress.visible = true;
                        text = "Waiting for server response...";
                        game.clientStart(serverIp.text, serverPort.text);
                    }
                }
            }
        }
        onOpened: {
            serverIp.focus = true;
            cyclicColorProgress.visible = false;
            joinButton.enabled = true;
            joinButton.text = "Join";
        }
        onClosed: {
            if (!clientInGame) {
                game.leaveGame();
            }
        }
    }
}
