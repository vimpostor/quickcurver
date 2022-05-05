import Qt.labs.platform 1.0
import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

Item {
	id: playersRoot
	property alias startButton: startButton
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
		delegate: Label {
			/* icon.source: model.controller === 0 ? "qrc:///hardware/gamepad" : model.controller === 2 ? "qrc:///action/android" : "action/account_circle" */
			height: 65
			text: model.name + "   " + model.totalScore + "(+" +  model.roundScore + ")"
			/* Ripple { */
				/* anchors.fill: parent */
				/* acceptedButtons: Qt.LeftButton | Qt.RightButton */
				/* onClicked: playerListView.open(index, model.controller); */
			/* } */
			/* rightItem: ToolButton { */
				/* anchors.verticalCenter: parent.verticalCenter */
				/* icon.source: "qrc:///editor/mode_edit" */
			/* } */
		}
		/* BottomSheetList { */
			/* id: bottomSheet */
			/* property bool playerEditable: true */
			/* title: qsTr("Edit Player") */
			/* actions: [ */
				/* Action { */
					/* text: "Edit name" */
					/* enabled: bottomSheet.playerEditable */
					/* icon.source: "qrc:///action/account_circle" */
					/* onTriggered: inputDialog.open(); */
				/* }, */
				/* Action { */
					/* text: "Edit color" */
					/* enabled: bottomSheet.playerEditable */
					/* icon.source: "qrc:///editor/format_color_fill" */
					/* onTriggered: colorDialog.open(); */
				/* }, */
				/* Action { */
					/* text: "Set counterclockwise key" */
					/* enabled: bottomSheet.playerEditable */
					/* icon.source: "qrc:///hardware/keyboard_arrow_left" */
					/* onTriggered: { */
						/* infoBar.open("Press a key!"); */
						/* leftKeyItem.forceActiveFocus(); */
					/* } */
				/* }, */
				/* Action { */
					/* text: "Set clockwise key" */
					/* enabled: bottomSheet.playerEditable */
					/* icon.source: "qrc:///hardware/keyboard_arrow_right" */
					/* onTriggered: { */
						/* infoBar.open("Press a key!"); */
						/* rightKeyItem.forceActiveFocus(); */
					/* } */
				/* }, */
				/* Action { */
					/* text: "Bot Settings" */
					/* enabled: bottomSheet.playerEditable */
					/* icon.source: "qrc:///action/android" */
					/* onTriggered: botDialog.open(); */
				/* }, */
				/* Action { */
					/* text: "Delete" */
					/* enabled: !root.connectedToServer */
					/* icon.source: "qrc:///action/delete" */
					/* onTriggered: c_playerModel.removePlayer(playerListView.modelIndex); */
				/* } */
			/* ] */
		/* } */
		/* InputDialog { */
			/* id: inputDialog */
			/* x: (parent.width - width) / 2 */
			/* y: (parent.height - height) / 2 */
			/* title: "Player name" */
			/* onAccepted: c_playerModel.setUserName(playerListView.modelIndex, textField.text); */
		/* } */
		ColorDialog {
			id: colorDialog
			onAccepted: c_playerModel.setColor(playerListView.modelIndex, color);
		}
		/* Dialog { */
			/* id: botDialog */
			/* x: (parent.width - width) / 2 */
			/* y: (parent.height - height) / 2 */
			/* Label { */
				/* text: qsTr("Controlled by AI") */
				/* height: 65 */
				/* rightItem: CheckBox { */
					/* id: botCheckbox */
					/* anchors.verticalCenter: parent.verticalCenter */
					/* onCheckedChanged: c_playerModel.setController(playerListView.modelIndex, 2 * botCheckbox.checked); */
				/* } */
				/* onClicked: botCheckbox.checked = !botCheckbox.checked; */
			/* } */
		/* } */
		Item {
			id: leftKeyItem
			visible: false
			Keys.onPressed: {
				c_playerModel.setLeftKey(playerListView.modelIndex, event.key);
				game.forceActiveFocus();
			}
		}
		Item {
			id: rightKeyItem
			visible: false
			Keys.onPressed: {
				c_playerModel.setRightKey(playerListView.modelIndex, event.key);
				game.forceActiveFocus();
			}
		}
	}
	Button {
		id: startButton
		enabled: !root.connectedToServer
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.margins: 8
		height: 64
		text: "Start!"
		highlighted: true
		onClicked: {
			game.startGame();
		}
	}
	ToolButton {
		id: addPlayerButton
		enabled: !root.connectedToServer
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.margins: 16
		icon.source: "qrc:///content/add"
		Material.background: Material.primary
		onClicked: {
			c_playerModel.appendPlayer();
			/* c_playerModel.appendBot(); */
			game.forceActiveFocus();
		}
	}
}
