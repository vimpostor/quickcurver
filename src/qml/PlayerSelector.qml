import QtQuick 2.7
import QtQuick.Layouts 1.1
//import QtQuick.Controls 1.3 as QuickControls
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Controls.Material 2.0

Item {
    property var mysnackbar: snackbar
    View {
        anchors.fill: parent
        anchors.margins: dp(32)
        elevation: 1
        ListItem.Subheader {
            id: playerListHeader
            text: "Players"
            anchors.bottom: playerList.top
        }

        View {
            id: playerList
            anchors.top: playerListHeader.bottom
            anchors.bottom: parent.bottom
            anchors.bottomMargin: dp(50)
            anchors.left: parent.left
            anchors.right: parent.right
            ListView {
                id: playerListView
//                spacing: dp(8)
                anchors.fill: parent
                model: playerListModel
                delegate: playerDelegate
            }
        }
        Button {
            id: startButton
            focus: true
            anchors.top: playerList.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            text: "Start!"
            elevation: 1
            backgroundColor: Theme.primaryColor
            onClicked: {
                pageStack.push(Qt.resolvedUrl("gamePage.qml"));
                game.start();
                game.focus = true;
            }
        }

//        ListModel { //this got moved to main.qml
//            id: playerListModel
//            ListElement {
//                name: "Player 0"
//            }
//            ListElement {
//                name: "Player 1"
//            }
//        }

        Component {
            id: playerDelegate
            ListItem.Subtitled {
                property string name: (eBot ? "Bot " : "Player ") +index
                property color mycolor:  Material.color(Math.random()*19)
                onNameChanged: {
                    game.setName(index, name);
                    ename = name;
                }
                onMycolorChanged: {
                    game.setColor(index, mycolor);
                }
                onClicked: playerEditDialog.show()

                text: name
                subText: playerController.selectedIndex == 2  ? (eJoined ? "Joined" : "Waiting for player to join...") : "Change color, controls etc..."

                secondaryItem: IconButton {
                    iconName: "editor/mode_edit"
                    anchors.verticalCenter: parent.verticalCenter
                    color: mycolor
                    onClicked: playerEditDialog.show()
                }
                Dialog {
                    id: playerEditDialog
                    title: "Edit Player"
                    hasActions: true
                    TextField {
                        id: nameTextField
                        focus: true
                        width: parent.width
                        placeholderText: "Name"
                        floatingLabel: true
                        onTextChanged: name = this.text
                    }

                MenuField {
                    id: playerController
                    model: ["Local Player", "Bot", "Online Player"]
                    width: dp(160)
                    selectedIndex: eBot ? 1 : 0
                    onSelectedIndexChanged: game.setController(index, selectedIndex)
                }

                    GridLayout {
                        id: editPlayerGrid
                        rowSpacing: dp(20)
                        columnSpacing: dp(10)
                        columns: 2

                        Button {
                            id: buttonLeft
                            enabled: playerController.selectedIndex == 0
                            text: "Left"
                            elevation: 1
                            activeFocusOnPress: true
                            Keys.onPressed: {
                                if (event.text === "") {
                                    this.text = "No key description available";
                                } else {
                                    this.text = event.text;
                                }
                                game.setControls(index, event.key, false);
                            }
                        }
                        Button {
                            id: buttonRight
                            enabled: buttonLeft.enabled
                            text: "Right"
                            elevation: 1
                            activeFocusOnPress: true
                            Keys.onPressed: {
                                if (event.text === "") {
                                    this.text = "No key description available";
                                } else {
                                    this.text = event.text;
                                }
                                game.setControls(index, event.key, true);
                            }
                        }
                        Text {
                            text: "Color:"
                        }
                        Button {
                            elevation: 1
                            backgroundColor: mycolor
                            onClicked: {
                                mycolorPicker.show();
                            }
                        }
                        Dialog {
                            id: mycolorPicker
                            title: "Pick color"
                            positiveButtonText: "Done"
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
                                            onPressed: mycolor = parent.color
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
    ActionButton {
        anchors {
            right: parent.right
            bottom: snackbar.top
            margins: dp(64)
        }
        action: Action {
            id: addPlayer
            text: "&Add"
            shortcut: "Ctrl+N"
            onTriggered: {
                if (playerListModel.count >= 16) {
                    snackbar.open("Sorry, you have reached maximum player capacity!");
                } else {
                    game.addPlayer();
                    playerListModel.append({});
                }
            }
        }
        iconName: "content/add"
    }
    Action {
        shortcut: "Ctrl+Shift+N"
        onTriggered: {
            if (playerListModel.count >= 16) {
                snackbar.open("Sorry, you have reached maximum player capacity!");
            } else {
                game.addPlayer();
                playerListModel.append({eBot: true});
            }
        }
    }

    Snackbar {
        id: snackbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: dp(70)
    }
}
