import QtQuick 2.7
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Layouts 1.1

Page {
    function sendMessage(username, message) { // called by cpp when a message was sent
        chatModel.append({username: username, message: message});
        chatListView.positionViewAtEnd();
    }
    property int fieldsize: 800
    backgroundColor: "#31363b"
    focus: false
    canGoBack: false
//    onGoBack: pageStack.push(this) //as of now, canGoBack = false does not work, this disables it manually
    onGoBack: close()
    onParentChanged: fieldsize = game.getFieldSize()
    PageSidebar {
//        mode: "right"
        width: parent.width - fieldsize
        backgroundColor: Theme.backgroundColor
        Card {
            id: statsCard
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: dp(32)
            ListItem.Subheader {
                id: scoreListHeader
                text: "Players"
                anchors.top: parent.top
            }
            View {
                anchors.top: scoreListHeader.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                ListView {
                    anchors.fill: parent
                    model: playerListModel
                    delegate: scoreListDelegate
                }
            }
            Component {
                id: scoreListDelegate
                ListItem.Standard {
                    text: ename
                    secondaryItem: Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: escore + " (+" +  eroundScore + ")"
                    }
                }
            }
        }
        Card {
            id: chatCard
            anchors.top: statsCard.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: dp(32)
            ListItem.Subheader {
                id: chatHeader
                text: "Chat"
                anchors.top: parent.top
            }
            View {
                anchors.top: chatHeader.bottom
                anchors.bottom: messageRowLayout.top
                anchors.left: parent.left
                anchors.right: parent.right
                ListModel {
                    id: chatModel
//                    ListElement {
//                        username: "Alice"
//                        message: "Test message"
//                    }
                }
                ListView {
                    id: chatListView
                    anchors.fill: parent
                    model: chatModel
                    delegate: chatDelegate
                    add: Transition {
                        NumberAnimation { properties: "y"; from: messageRowLayout.y; duration: 150}
                    }
                }
                Component {
                    id: chatDelegate
                    ListItem.Subtitled {
                        text: username
                        subText: message
                        valueText: new Date().toLocaleTimeString("hh:mm");
                        iconName: username == "Chat Bot"? "communication/robot" : "action/account_circle"
                    }
                }
            }
            RowLayout {
                id: messageRowLayout
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: dp(16)
                TextField {
                    id: chatTextField
                    characterLimit: 32
                    Layout.fillWidth: true
                    placeholderText: "Send messages..."
                    floatingLabel: true
                    onAccepted: sendMessageAction.trigger()
                }
                ActionButton {
                    iconName: "content/send"
                    action: Action {
                        id: sendMessageAction
                        onTriggered: {
                            game.requestSendMessage(chatTextField.text);
                            chatTextField.text = "";
                            game.focus = true;
                        }
                    }
                }
            }
        }
    }
}
