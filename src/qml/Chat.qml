import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import Fluid.Controls 1.1
import Fluid.Core 1.0

Card {
	ColumnLayout {
		anchors.fill: parent
		anchors.margins: Units.smallSpacing
		ListView {
			header: Subheader {
				text: "Chat"
			}
			clip: true
			model: c_chatModel
			delegate: ListItem {
				text: model.message
				valueText: new Date().toLocaleTimeString("hh:mm");
				leftItem: Chip {
					text: username
					icon.source: "qrc:///action/account_circle"
				}
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
