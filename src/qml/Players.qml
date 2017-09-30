import Qt.labs.platform 1.0
import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0
import Fluid.Controls 1.0
import Fluid.Core 1.0
import QtQuick.Controls.Material 2.0

Card {
	id: playersRoot
	property alias startButton: startButton
	ListView {
		id: playerListView
		header: Subheader {
			text: "Players"
		}
		clip: true
		property int modelIndex: 0
		function open(index, controller) {
			modelIndex = index;
			bottomSheet.playerEditable = controller !== 1;
			if (controller !== 1) {
				botCheckbox.checked = controller === 2;
			}
			bottomSheet.open();
		}
		anchors.fill: parent
		model: c_playerModel
		delegate: ListItem {
			iconName: model.controller === 0 ? "hardware/gamepad" : model.controller === 2 ? "action/android" : "action/account_circle"
			text: model.name + "   " + model.totalScore + "(+" +  model.roundScore + ")"
			rightItem: Row {
				IconButton {
					iconName: "hardware/keyboard_arrow_left"
					Keys.onPressed: {
						c_playerModel.setLeftKey(index, event.key);
						game.forceActiveFocus();
					}
				}
				IconButton {
					iconName: "hardware/keyboard_arrow_right"
					Keys.onPressed: {
						c_playerModel.setRightKey(index, event.key);
						game.forceActiveFocus();
					}
				}
			}
			onClicked: playerListView.open(index, model.controller);
		}
		BottomSheetList {
			id: bottomSheet
			property bool playerEditable: true
			title: qsTr("Edit Player")
			actions: [
				Action {
					text: "Edit name"
					enabled: bottomSheet.playerEditable
					iconName: "action/account_circle"
					onTriggered: inputDialog.open();
				},
				Action {
					text: "Edit color"
					enabled: bottomSheet.playerEditable
					iconName: "editor/format_color_fill"
					onTriggered: colorDialog.open();
				},
				Action {
					text: "Bot Settings"
					enabled: bottomSheet.playerEditable
					iconName: "action/android"
					onTriggered: botDialog.open();
				},
				Action {
					text: "Delete"
					iconName: "action/delete"
					onTriggered: c_playerModel.removePlayer(playerListView.modelIndex);
				}
			]
		}
		InputDialog {
			id: inputDialog
			x: (parent.width - width) / 2
			y: (parent.height - height) / 2
			title: "Player name"
			onAccepted: c_playerModel.setUserName(playerListView.modelIndex, textField.text);
		}
		ColorDialog {
			id: colorDialog
			onAccepted: c_playerModel.setColor(playerListView.modelIndex, color);
		}
		Dialog {
			id: botDialog
			x: (parent.width - width) / 2
			y: (parent.height - height) / 2
			ListItem {
				text: "Controlled by Computer"
				rightItem: CheckBox {
					id: botCheckbox
					anchors.verticalCenter: parent.verticalCenter
					onCheckedChanged: c_playerModel.setController(playerListView.modelIndex, 2 * botCheckbox.checked);
				}
				onClicked: botCheckbox.checked = !botCheckbox.checked;
			}
		}
	}
	Button {
		id: startButton
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: Units.smallSpacing
		height: 64
		text: "Start!"
		highlighted: true
		onClicked: {
			game.startGame();
		}
	}
	FloatingActionButton {
		id: addPlayerButton
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.margins: Units.largeSpacing
		iconName: "content/add"
		Material.background: Material.primary
		onClicked: {
			c_playerModel.appendPlayer();
			game.forceActiveFocus();
		}
	}
}
