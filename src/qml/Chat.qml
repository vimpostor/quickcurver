import QtQuick
import QtQuick.Layouts
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
			delegate: RowLayout {
				width: parent.width
				Text {
					id: usernameLabel
					text: model.username + ":"
				}
				Text {
					text: model.message
				}
				Item {
					Layout.fillWidth: true
				}
				Text {
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
			ToolButton {
				id: sendButton
				function send() {
					game.sendChatMessage(messageTextField.text);
					messageTextField.text = "";
					if (game.isStarted) {
						game.forceActiveFocus();
					}
				}
				icon.source: "qrc:///send"
				icon.color: Material.accent
				onClicked: send();
			}
		}
	}
}
