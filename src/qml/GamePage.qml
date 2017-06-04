import QtQuick 2.7
import QtQuick.Controls 2.1
import Fluid.Controls 1.0
import Fluid.Material 1.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.1

Page {
	function sendMessage(username, message) { // called by cpp when a message was sent
		chatModel.append({username: username, message: message});
		chatListView.positionViewAtEnd();
	}
	onGoBack: close()
	Pane {
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.right: gameRectangle.left
		Card {
			id: statsCard
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: 32
			height: parent.height/3
			ListView {
				clip: true
				anchors.fill: parent
				header: Subheader {
					text: "Players"
				}
				model: playerListModel
				delegate: ListItem {
					text: ename
					rightItem: Label {
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
			anchors.margins: 32
			ListModel {
				id: chatModel
			}
			ListView {
				id: chatListView
				clip: true
				anchors.top: parent.top
				anchors.bottom: messageRowLayout.top
				anchors.left: parent.left
				anchors.right: parent.right
				header: Subheader {
					text: "Chat"
				}

				model: chatModel
				delegate: ListItem {
					text: username
					subText: message
					valueText: new Date().toLocaleTimeString("hh:mm");
					iconName: username == "Chat Bot"? "action/android" : "action/account_circle"
				}
				add: Transition {
					NumberAnimation { properties: "y"; from: messageRowLayout.y; duration: 150}
				}
			}
			RowLayout {
				id: messageRowLayout
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.bottom: parent.bottom
				anchors.margins: 16
				TextField {
					id: chatTextField
					Layout.fillWidth: true
					placeholderText: "Send messages..."
					Keys.onReturnPressed: {
						// TODO: wait for upstream to expose click function of actionbutton
					}
				}
				ActionButton {
					id: sendMessageAction
					iconName: "content/send"
					Material.background: Material.primary
					onClicked: {
						game.requestSendMessage(chatTextField.text);
						chatTextField.text = "";
						game.focus = true;
					}
				}
			}
		}
	}
	Rectangle {
		id: gameRectangle
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		width: gameWidth + 32
		color: Material.color(Material.BlueGrey, Material.Shade900);
	}
}
