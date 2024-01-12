import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls.Material
import Quartz

Item {
	id: playersRoot
	ListView {
		id: playerListView
		header: Label {
			text: "Players"
		}
		clip: true
		property int modelIndex: 0
		function open(index, controller) {
			modelIndex = index;
			bottomSheet.playerEditable = controller !== 1;
			if (root.connectedToServer) {
				bottomSheet.playerEditable = false;
			}
			if (controller !== 1) {
				botCheckbox.checked = controller === 2;
			}
			bottomSheet.open();
		}
		anchors.fill: parent
		model: c_playerModel
		delegate: Item {
			height: playerIcon.implicitHeight
			width: ListView.view.width
			IconButton {
				id: playerIcon
				ico.name: model.controller === 0 ? "supervised_user_circle" : model.controller === 2 ? "robot" : "cloud_sync"
				anchors.left: parent.left
				text: model.name + "   " + model.totalScore + "(+" +  model.roundScore + ")"
				onClicked: playerListView.open(index, model.controller);
			}
			Label {
				property real pingFactor: Math.min(500, model.ping) / 500
				text: model.ping
				visible: model.ping
				color: Qt.rgba(pingFactor, (1 - pingFactor), 0, 1)
				Behavior on color {
					ColorAnimation {
						duration: 800
						easing.type: Easing.OutCubic
					}
				}
				anchors.verticalCenter: parent.verticalCenter
				anchors.right: parent.right
			}
		}
		Dialog {
			id: bottomSheet
			property bool playerEditable: true
			title: qsTr("Edit Player")
			width: 300
			ColumnLayout {
				IconButton {
					text: "Edit name"
					enabled: bottomSheet.playerEditable
					ico.name: "user_attributes"
					onClicked: inputDialog.open();
				}
				IconButton {
					text: "Edit color"
					enabled: bottomSheet.playerEditable
					ico.name: "color_lens"
					onClicked: colorDialog.open();
				}
				IconButton {
					text: "Set counterclockwise key"
					enabled: bottomSheet.playerEditable
					ico.name: "rotate_left"
					onClicked: {
						infoBar.open("Press a key!");
						leftKeyItem.forceActiveFocus();
					}
				}
				IconButton {
					text: "Set clockwise key"
					enabled: bottomSheet.playerEditable
					ico.name: "rotate_right"
					onClicked: {
						infoBar.open("Press a key!");
						rightKeyItem.forceActiveFocus();
					}
				}
				IconButton {
					text: "Bot Settings"
					enabled: bottomSheet.playerEditable
					ico.name: "robot"
					onClicked: botDialog.open();
				}
				IconButton {
					text: "Delete"
					enabled: !root.connectedToServer
					ico.name: "delete"
					onClicked: {
						c_playerModel.removePlayer(playerListView.modelIndex);
						bottomSheet.close();
					}
				}
			}
		}
		Dialog {
			id: inputDialog
			title: "Player name"
			standardButtons: Dialog.Ok | Dialog.Cancel
			ColumnLayout {
				TextField {
					id: textField
				}
			}
			onAccepted: c_playerModel.setUserName(playerListView.modelIndex, textField.text);
		}
		ColorDialog {
			id: colorDialog
			onAccepted: c_playerModel.setColor(playerListView.modelIndex, selectedColor);
		}
		Dialog {
			id: botDialog
			RowLayout {
				Label {
					text: qsTr("Controlled by AI")
					height: 65
				}
				CheckBox {
					id: botCheckbox
					onCheckedChanged: c_playerModel.setController(playerListView.modelIndex, 2 * botCheckbox.checked);
				}
			}
		}
		Item {
			id: leftKeyItem
			visible: false
			Keys.onPressed: (event) => {
				c_playerModel.setLeftKey(playerListView.modelIndex, event.key);
				game.forceActiveFocus();
			}
		}
		Item {
			id: rightKeyItem
			visible: false
			Keys.onPressed: (event) => {
				c_playerModel.setRightKey(playerListView.modelIndex, event.key);
				game.forceActiveFocus();
			}
		}
	}
	FloatingActionButton {
		id: addPlayerButton
		enabled: !root.connectedToServer
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.margins: 16
		name: "person_add"
		onClicked: {
			c_playerModel.appendPlayer();
			game.forceActiveFocus();
		}
	}
	FloatingActionButton {
		enabled: !root.connectedToServer
		anchors.bottom: addPlayerButton.top
		anchors.horizontalCenter: addPlayerButton.horizontalCenter
		anchors.margins: 16
		size: FloatingActionButton.Size.Small
		name: "robot"
		onClicked: {
			c_playerModel.appendBot();
			game.forceActiveFocus();
		}
	}
}
