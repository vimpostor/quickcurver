import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import Quartz

import Backend

Item {
	ColumnLayout {
		anchors.fill: parent
		anchors.margins: 8
		ListView {
			header: Label {
				text: "Chat"
			}
			clip: true
			model: ChatModel
			delegate: RowLayout {
				width: parent.width
				Label {
					id: usernameLabel
					text: model.username + ":"
				}
				Label {
					text: model.message
				}
				Item {
					Layout.fillWidth: true
				}
				Label {
					text: Qt.formatDateTime(model.timestamp, "HH:mm")
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
			IconButton {
				id: sendButton
				function send() {
					game.sendChatMessage(messageTextField.text);
					messageTextField.text = "";
					if (game.isStarted) {
						game.forceActiveFocus();
					}
				}
				ico.name: "send"
				ico.color: Material.accent
				onClicked: send();
			}
		}
	}
}
