import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0
import Fluid.Controls 1.0
import Fluid.Material 1.0
import Fluid.Core 1.0
import QtQuick.Controls.Material 2.0

Item {
	id: playerSelectorRoot
	anchors.fill: parent
	property var mysnackbar: snackbar
	Flickable {
		anchors {
			top: parent.top
			left: parent.left
			right: parent.right
			bottom: startButton.top
			margins: Units.smallSpacing
		}
		contentHeight: playerListView.implicitHeight
		clip: true
		AutomaticGrid {
			id: playerListView
			model: playerListModel
			cellWidth: Device.isMobile ? playerSelectorRoot.width - 2*Units.smallSpacing : 356
			cellHeight: 300
			delegate: Item {
				property string name: (eBot ? "Bot " : "Player ") + index
				onNameChanged: {
					game.setName(index, name);
					ename = name;
				}
				property color mycolor:  Material.color(Math.random()*19)
				onMycolorChanged: game.setColor(index, mycolor);
				width: playerListView.cellWidth
				height: playerListView.cellHeight
				Card {
					anchors.fill: parent
					anchors.margins: Units.smallSpacing
					Column {
						anchors.fill: parent
						anchors.margins: Units.smallSpacing
						TextField {
							id: nameTextField
							enabled: playerComboBox.currentIndex !== 2
							text: ename
							width: parent.width
							placeholderText: "Name"
							onTextChanged: name = this.text
						}
						ListItem {
							text: "Player"
							iconName: playerComboBox.currentIndex === 0 ? "av/games" : (playerComboBox.currentIndex === 1 ? "action/android" : (!eJoined ? "action/language" : (eReady ? "action/check_circle" : "action/account_circle")))
							rightItem: ComboBox {
								id: playerComboBox
								currentIndex: eBot ? 1 : eOnline ? 2 : 0
								model: ListModel {
									ListElement { text: "Local" }
									ListElement { text: "Bot" }
									ListElement { text: "Online" }
								}
								onCurrentIndexChanged: game.setController(index, currentIndex)
							}
							secondaryItem: BusyIndicator {
								running: playerComboBox.currentIndex === 2 && !eJoined
								visible: running
							}
						}
						RowLayout {
							anchors.left: parent.left
							anchors.right: parent.right
							Repeater {
								model: 2
								Button {
									text: index ? "Right" : "Left"
									Layout.fillWidth: true
									enabled: playerComboBox.currentIndex === 0
									Keys.onPressed: {
										if (event.text === "") {
											this.text = "No key description";
										} else {
											this.text = event.text;
										}
										game.setControls(nameTextField.index, event.key, index);
									}
								}
							}
						}
						Button {
							text: "Remove player"
							width: parent.width
							Layout.fillWidth: true
							highlighted: true
							onClicked: {
								if (playerComboBox.currentIndex === 2) {
									alertDialogKick.open();
								} else {
									removePlayer(index);
								}
							}
							AlertDialog {
								id: alertDialogKick
								width: Device.isMobile ? playerSelectorRoot.width : 300
								title: "Remove player"
								text: "Are you sure, you want to remove this player?"
								standardButtons: Dialog.Ok | Dialog.Cancel
								onAccepted: removePlayer(index);
							}
						}
					}
				}
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
			if (game.isReady()) {
				pageStack.push(Qt.resolvedUrl("GamePage.qml"));
				game.parent.x = game.parent.parent.width - game.getFieldSize();
				game.start();
				game.focus = true;
			}
		}
	}
	ActionButton {
		id: addPlayerButton
		anchors {
			right: snackbar.right
			bottom: snackbar.top
			bottomMargin: startButton.height/2
		}
		iconName: "content/add"
		Material.background: Material.primary
		onClicked: {
			if (playerListModel.count >= 16) {
				snackbar.open("Sorry, you have reached maximum player capacity!");
			} else {
				game.addPlayer();
				playerListModel.append({});
			}
		}
	}
	InfoBar {
		id: snackbar
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.margins: Units.mediumSpacing
	}
}
