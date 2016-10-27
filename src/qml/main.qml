import QtQuick 2.7
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

import Game 1.0
import QtQuick.Layouts 1.1

ApplicationWindow {
    onClosing: game.close()
    function changeScore(index, newScore, roundScore) {
        playerListModel.setProperty(index, "escore", newScore);
        playerListModel.setProperty(index, "eroundScore", roundScore);
    }
    function setJoinStatus(s) {
        if (s === "JOINED") {
            joinButton.text = "Joined, waiting for host to start...";
        } else if (s === "REJECTED") {
            clientDialog.close();
            playerselector.mysnackbar.open("Join request was rejected :(");
        } else if (s === "TIMEOUT") {
            clientDialog.close();
            playerselector.mysnackbar.open("Join request timed out! :(");
        }  else if (s === "STARTED") {
            clientDialog.close();
            pageStack.push(Qt.resolvedUrl("gamePage.qml"));
            game.focus = true;
        } else if (s === "KICKED") {
            clientDialog.close();
            playerselector.mysnackbar.open("You got kicked from the game!");
        }
    }
    function setPlayerStatus(index,s) {
        if (s==="JOINED") {
            playerListModel.setProperty(index, "eJoined", true);
        } else if (s==="KICKED") {
            playerListModel.setProperty(index, "eJoined", false);
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
                iconName: "file/cloud_upload"
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
//                iconName: "navigation/refresh"
                name: "Update"
                onTriggered: playerselector.mysnackbar.open("Looks like the developer was too lazy to implement an Auto-Update feature yet. Stay tuned!")
            },
            Action {
                name: "Licenses"
                onTriggered: licensesDialog.open()
            }

        ]
//        backAction: navDrawer.action

//        NavigationDrawer {
//            id: navDrawer
//            enabled: root.width < dp(500)
////            onEnabledChanged: smallLoader.active = enabled
//        }
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
        }
        ListElement {
            ename: "Player 1"
            escore: 0
            eroundScore: 0
            eBot: true
            eJoined: false
        }
    }
    Dialog {
        id: licensesDialog
        anchors.fill: parent
        anchors.margins: dp(80)
        Licenses {
        }
    }
    Dialog {
        id: clientDialog
        title: "Join Online Game"
        hasActions: false
        RowLayout {
            TextField {
                id: serverIp
                placeholderText: "Server IP Adress"
                floatingLabel: true
            }
            Label {
                text: ":"
            }

            TextField {
                id: serverPort
                placeholderText: "Port"
                floatingLabel: true
                text: "52552"
            }
        }
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
        onOpened: {
            cyclicColorProgress.visible = false;
            joinButton.enabled = true;
            joinButton.text = "Join";
        }
    }
}
