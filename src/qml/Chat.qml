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
			delegate: Label {
				text: model.message
				/* valueText: new Date().toLocaleTimeString("hh:mm"); */
				/* leftItem: Label { */
					/* text: username */
					/* icon.source: "qrc:///action/account_circle" */
				/* } */
			}
			Layout.fillHeight: true
			Layout.fillWidth: true
			onCountChanged: currentIndex = count - 1;
		}
		RowLayout {
			TextField {
				id: messageTextField
				background.width: width
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
