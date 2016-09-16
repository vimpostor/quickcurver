import QtQuick 2.7
//import QtQuick.Window 2.2
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

import Game 1.0
//import QtQuick.Controls 1.4
//import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: root
    width: 997
    height: 1150
    minimumWidth: 997
    minimumHeight: 997
    title: "Quick Curver"
    visible: true
    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
    }

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
}


//ApplicationWindow {
//    id: root
//    title: "test"
//    width: 1000
//    height: 1000
////    border.color: "cyan"
//    color: "steelblue"
//    visible: true

//    signal addnewPlayer();
//    signal controlsChanged(int index, Keys k, bool isRight);
//    signal colorChanged(int index, color color);

//    Behavior on color {
//        ColorAnimation {
//            duration: 500
//        }
//    }

//    Rectangle {
//        id: rect
//        anchors.fill: root
//        border.color: "black"
//        border.width: 5
//        SequentialAnimation on radius {
//            NumberAnimation {to: 100; duration: 1000; easing.type: Easing.OutCubic }
//        }
//        gradient: Gradient {
//            GradientStop {position: 0.0; color: "lightsteelblue" }
//            GradientStop {position: 1.0; color: "steelblue" }
//        }
//        Behavior on opacity {
//            NumberAnimation {
//                duration: 500
//            }
//        }
//        Behavior on radius {
//            NumberAnimation {
//                duration: 500
//            }
//        }
//        PlayerSelector {
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.top: parent.top
//            anchors.topMargin: parent.height / 30
//        }

//        Button {
//            id: startbtn
//            onClicked: {
//                anchors.bottom = undefined
//                startbtn.state = "clicked";
//                rect.opacity = 0;
//                rect.radius = 0;
//                root.color = "black";
//                game.start();
//                game.forceActiveFocus();
//            }
//            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.bottom: parent.bottom
//            anchors.bottomMargin: parent.height / 40
//            style: ButtonStyle {
//                label: Text {
//                    color: "black"
//                    text: "Start"
//                    SequentialAnimation on font.pointSize {
//                        NumberAnimation {
//                            to: 25
//                            duration: 500
//                            easing.type: Easing.InOutQuad
//                        }
//                    }
//                }
//                background: Rectangle {
//                    border.width: control.activeFocus ? 2 : 1
//                    radius: 10
//                    gradient: Gradient {
//                        GradientStop { position: 0 ; color: control.pressed ? "springgreen" : "springgreen" }
//                        GradientStop { position: 1 ; color: control.pressed ? "lawngreen" : "forestgreen" }
//                    }
//                }
//            }
//            states: [
//                State {
//                    name: "clicked"
//                    PropertyChanges {
//                        target: startbtn
//                        y: root.height+10
//                    }
//                }

//            ]
//            transitions: [
//                Transition {
//                    NumberAnimation {
//                        duration: 50
//                        properties: "y"
//                    }
//                }
//            ]
////
//        }
//    }
//    Game {
//        id: game
//        objectName: "game"
//        anchors.fill: parent

//        Keys.onPressed:  {
//            game.sendKey(event.key);
//        }
//        Keys.onReleased: {
//            game.releaseKey(event.key);
//        }
//    }
//}
