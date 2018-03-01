import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import Fluid.Controls 1.0
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
				text: model.username
				subText: model.message
				valueText: new Date().toLocaleTimeString("hh:mm");
				icon.source: Utils.iconUrl("action/account_circle")
			}
			Layout.fillHeight: true
			Layout.fillWidth: true
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
					game.forceActiveFocus();
				}
				icon.source: Utils.iconUrl("content/send")
				icon.color: Material.accent
				onClicked: send();
			}
		}
	}
}
