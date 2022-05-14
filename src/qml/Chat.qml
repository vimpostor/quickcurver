import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
	ColumnLayout {
		anchors.fill: parent
		anchors.margins: 8
		ListView {
			header: Label {
				text: "Chat"
			}
			clip: true
			model: c_chatModel
			delegate: Item {
				height: usernameLabel.implicitHeight
				Text {
					id: usernameLabel
					anchors.left: parent.left
					text: model.username + ":"
				}
				Text {
					anchors.left: usernameLabel.right
					anchors.right: parent.right
					anchors.margins: 8
					text: model.message
				}
			}
			Layout.fillHeight: true
			Layout.fillWidth: true
			onCountChanged: currentIndex = count - 1;
		}
		RowLayout {
			TextField {
				id: messageTextField
				Layout.fillWidth: true
				onAccepted: sendButton.send();
			}
			ToolButton {
				id: sendButton
				function send() {
					game.sendChatMessage(messageTextField.text);
					messageTextField.text = "";
					if (game.isStarted) {
						game.forceActiveFocus();
					}
				}
				icon.source: "qrc:///content/send"
				icon.color: Material.accent
				onClicked: send();
			}
		}
	}
}
