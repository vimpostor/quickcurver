import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.0

Rectangle {
      ColorDialog {
            id: colorDialog
            signal colorChosen(color newColor)
            onAccepted: {
                 console.log("You chose: " + colorDialog.color)
                 /*  HERE IS how you actually send that signal out */
                 colorChosen(colorDialog.currentColor);
            }

        }
    id: listViewContainer
    width: parent.width/10*9;
    height: 50
    Behavior on height {
        NumberAnimation {
            duration: 100;
        }
    }

    gradient: Gradient {
        GradientStop {position: 0.0; color: "white" }
        GradientStop {position: 1.0; color: "silver" }
    }
    radius: 5

    ListModel {
        id: playerListViewModel;
        ListElement {
            name: "Name"
        }
    }

    Component {
        id: playerDelegate
//        property ListView listViewModel: playerListViewModel
        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            function changeColor(newColor) {
                console.log("newColor="+newColor);
                playerListViewModel.get(index).playerColor = newColor;
                playerListViewModel.get(index).name = newColor;
                root.colorChanged(index, newColor);
                console.log("After setting playerColor = newColor, playerColor="+playerListViewModel.get(index).playerColor);
                colorDialog.colorChosen.disconnect(changeColor);
            }
            Column {
                TextInput { text: name }
                Item {
                    id: colorSelector
                    property color color: Material.color(Math.random()*19)
                    onColorChanged: {
                        game.changeColor(index, color);
                    }
                }
                ColorDialog {
                    id: colorDialog
                    modality: Qt.ApplicationModal
                    title: "Please choose a color"
                    onAccepted: colorSelector.color = currentColor
                }
                Row {
                    MouseArea {
                        width: 20
                        height: 20
                        onClicked: {
                            colorDialog.color = colorSelector.color;
                            colorDialog.open();
                            //open color dialog
                        }

                        Rectangle {
                            radius: 3
                            anchors.fill: parent
//                            color: Qt.binding(function() { return playerColor})
                            color: colorSelector.color
                        }
                    }
                    Button {
                        id: leftButton
                        width: 200
                        height: 20
                        text: "Left"
                        style: ButtonStyle {
                            label: Text {
                                text: leftButton.text
                                horizontalAlignment: Text.Center
                            }
                        }
                        onClicked: {
                            this.forceActiveFocus();
                        }
                        Keys.onPressed: {
                            if (event.text == "") {
                                this.text = "No key description available"
                            } else {
                                this.text = event.text;
                            }
                            game.changeControls(index, event.key, false);
                        }
                    }
                    Button {
                        id: rightButton
                        width: 200
                        height: 20
                        text: "Right"
                        style: ButtonStyle {
                            label: Text {
                                text: rightButton.text
                                horizontalAlignment: Text.Center
                            }
                        }
                        onClicked: {
                            this.forceActiveFocus();
                        }
                        Keys.onPressed: {
                            if (event.text == "") {
                                this.text = "No key description available"
                            } else {
                                this.text = event.text;
                            }
                            game.changeControls(index, event.key, true);
                        }
                    }
                }
            }
        }
    }

    ListView {
        id: playerListView
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        model: playerListViewModel

        delegate: playerDelegate
    }
    Button {
        id: addPlayerButton
        anchors.top: playerListView.bottom
        anchors.left: playerListView.left
        anchors.right: playerListView.right
        style: ButtonStyle {
            label: Text {
                text: "Add new player"
                horizontalAlignment: Text.Center
            }
        }
        onClicked: {
            root.addnewPlayer(playerListView);
            playerListViewModel.append({name: "Name"});
            listViewContainer.height += 50;
        }
    }
}
