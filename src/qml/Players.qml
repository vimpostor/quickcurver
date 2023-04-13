import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
	id: playersRoot
	ListView {
		id: playerListView
		header: Label {
			property real pingFactor: Math.min(500, c_settings.ping) / 500
			text: "Players (Ping: " + c_settings.ping + ")"
			color: c_settings.ping ? Qt.rgba(pingFactor, (1 - pingFactor), 0, 1) : Material.secondaryTextColor
			Behavior on color {
				ColorAnimation {
					duration: 800
					easing.type: Easing.OutCubic
				}
			}
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
			ToolButton {
				id: playerIcon
				icon.source: model.controller === 0 ? "qrc:///user" : model.controller === 2 ? "qrc:///hard-drive" : "qrc:///users"
				anchors.left: parent.left
				onClicked: playerListView.open(index, model.controller);
			}
			Label {
				text: model.name + "   " + model.totalScore + "(+" +  model.roundScore + ")"
				anchors.verticalCenter: parent.verticalCenter
				anchors.left: playerIcon.right
			}
		}
		Dialog {
			id: bottomSheet
			property bool playerEditable: true
			title: qsTr("Edit Player")
			ColumnLayout {
				Button {
					text: "Edit name"
					enabled: bottomSheet.playerEditable
					icon.source: "qrc:///user"
					onClicked: inputDialog.open();
				}
				Button {
					text: "Edit color"
					enabled: bottomSheet.playerEditable
					icon.source: "qrc:///droplet"
					onClicked: colorDialog.open();
				}
				Button {
					text: "Set counterclockwise key"
					enabled: bottomSheet.playerEditable
					icon.source: "qrc:///rotate-ccw"
					onClicked: {
						infoBar.open("Press a key!");
						leftKeyItem.forceActiveFocus();
					}
				}
				Button {
					text: "Set clockwise key"
					enabled: bottomSheet.playerEditable
					icon.source: "qrc:///rotate-cw"
					onClicked: {
						infoBar.open("Press a key!");
						rightKeyItem.forceActiveFocus();
					}
				}
				Button {
					text: "Bot Settings"
					enabled: bottomSheet.playerEditable
					icon.source: "qrc:///hard-drive"
					onClicked: botDialog.open();
				}
				Button {
					text: "Delete"
					enabled: !root.connectedToServer
					icon.source: "qrc:///trash-2"
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
	Button {
		id: addPlayerButton
		enabled: !root.connectedToServer
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.margins: 16
		icon.source: "qrc:///user-plus"
		highlighted: true
		onClicked: {
			c_playerModel.appendPlayer();
			/* c_playerModel.appendBot(); */
			game.forceActiveFocus();
		}
	}
}
