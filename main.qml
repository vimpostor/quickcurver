import QtQuick 2.7
import QtQuick.Window 2.2
//import QCurver 1.0
import Game 1.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: root
    width: 1000
    height: 1000
    border.color: "cyan"
    color: "steelblue"

    signal addnewPlayer();
    signal controlsChanged(int index, Keys k, bool isRight);
    signal colorChanged(int index, color color);

    Behavior on color {
        ColorAnimation {
            duration: 500
        }
    }

    Rectangle {
        id: rect
        anchors.fill: root
        border.color: "black"
        border.width: 5
        SequentialAnimation on radius {
            NumberAnimation {to: 100; duration: 1000; easing.type: Easing.OutCubic }
        }
        gradient: Gradient {
            GradientStop {position: 0.0; color: "lightsteelblue" }
            GradientStop {position: 1.0; color: "steelblue" }
        }
        Behavior on opacity {
            NumberAnimation {
                duration: 500
            }
        }
        Behavior on radius {
            NumberAnimation {
                duration: 500
            }
        }
        PlayerSelector {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.height / 30
        }

        Button {
            id: startbtn
            onClicked: {
                anchors.bottom = undefined
                startbtn.state = "clicked";
                rect.opacity = 0;
                rect.radius = 0;
                root.color = "black";
                game.start();
                game.forceActiveFocus();
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height / 40
            style: ButtonStyle {
                label: Text {
                    color: "black"
                    text: "Start"
                    SequentialAnimation on font.pointSize {
                        NumberAnimation {
                            to: 25
                            duration: 500
                            easing.type: Easing.InOutQuad
                        }
                    }
                }
                background: Rectangle {
                    border.width: control.activeFocus ? 2 : 1
                    radius: 10
                    gradient: Gradient {
                        GradientStop { position: 0 ; color: control.pressed ? "springgreen" : "springgreen" }
                        GradientStop { position: 1 ; color: control.pressed ? "lawngreen" : "forestgreen" }
                    }
                }
            }
            states: [
                State {
                    name: "clicked"
                    PropertyChanges {
                        target: startbtn
                        y: root.height+10
                    }
                }

            ]
            transitions: [
                Transition {
                    NumberAnimation {
                        duration: 50
                        properties: "y"
                    }
                }
            ]
//
        }
    }
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
