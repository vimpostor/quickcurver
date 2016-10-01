import QtQuick 2.7
//import QtQuick.Window 2.2
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

import Game 1.0
//import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    function changeScore(index, newScore, roundScore) {
        playerListModel.setProperty(index, "escore", newScore);
        playerListModel.setProperty(index, "eroundScore", roundScore);
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
//    Wave {
//        id: wave
//    }

    initialPage: Page {
        actions: [
            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },
            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: {
                    pageStack.push(Qt.resolvedUrl("settings.qml"));
                }
            }

        ]
//        backAction: navDrawer.action

//        NavigationDrawer {
//            id: navDrawer
//            enabled: root.width < dp(500)
////            onEnabledChanged: smallLoader.active = enabled
//        }
        PlayerSelector {
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
        }
        ListElement {
            ename: "Player 1"
            escore: 0
            eroundScore: 0
            eBot: true
        }
    }
}
