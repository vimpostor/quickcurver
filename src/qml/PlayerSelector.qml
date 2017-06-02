import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import Fluid.Controls 1.0
import Fluid.Material 1.0
import QtQuick.Controls.Material 2.0

Item {
	property var mysnackbar: snackbar
	Flickable {
		anchors.fill: parent
		anchors.margins: 16
		Card {
			id: playerList
			anchors.fill: parent
			anchors.margins: 32
			anchors.bottomMargin: 72
			ListView {
				id: playerListView
				clip: true
				anchors.fill: parent
				header: Subheader {
					text: "Players"
				}
				model: playerListModel
				delegate: playerDelegate
				add: Transition {
					NumberAnimation { properties: "y"; from: startButton.y; duration: 100}
				}
			}
		}
		Button {
			id: startButton
			anchors.top: playerList.bottom
			anchors.right: parent.right
			anchors.left: parent.left
			anchors.bottom: parent.bottom
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
		Component {
			id: playerDelegate
			ListItem {
				property string name: (eBot ? "Bot " : "Player ") + index
				property color mycolor:  Material.color(Math.random()*19)
				onNameChanged: {
					game.setName(index, name);
					ename = name;
				}
				onMycolorChanged: {
					game.setColor(index, mycolor);
				}
				onClicked: {
					playerListView.currentIndex = index;
					playerEditOverlay.open();
				}
				text: ename
				subText: radioButtonOnlinePlayer.checked ? (eJoined ? "Joined" : "Waiting for player to join...") : ""
				iconName: radioButtonLocalPlayer.checked ? "av/games" : (radioButtonBot.checked ? "action/android" : (!eJoined ? "action/language" : (eReady ? "action/check_circle" : "action/account_circle")))
				Dialog {
					id: playerEditOverlay
					x: (parent.width - width) / 2;
					width: 400
					height: playerColumn.implicitHeight + 128
					title: "Edit player"
					Card {
						anchors.fill: parent
						Column {
							id: playerColumn
							anchors.fill: parent
							anchors.margins: 32
							TextField {
								id: nameTextField
								enabled: radioButtonOnlinePlayer.checked === false
								focus: true
								width: parent.width
								placeholderText: "Name"
								onTextChanged: name = this.text
								Layout.fillWidth: true
							}
							RadioButton {
								id: radioButtonLocalPlayer
								checked: !eBot && !eOnline
								text: "Local Player"
								onCheckedChanged: {
									if (checked) {
										game.setController(index, 0);
									}
								}
							}
							RadioButton {
								id: radioButtonBot
								text: "Bot"
								checked: eBot
								onCheckedChanged: {
									if (checked) {
										game.setController(index, 1);
									}
								}
							}
							RadioButton {
								id: radioButtonOnlinePlayer
								text: "Online Player"
								checked: eOnline
								onCheckedChanged: {
									if (checked) {
										game.setController(index, 2);
									}
								}
							}
							RowLayout {
								id: editPlayerGrid
								spacing: 16
								anchors.left: parent.left
								anchors.right: parent.right
								Button {
									id: buttonLeft
									enabled: radioButtonLocalPlayer.checked
									text: "Left"
									Layout.fillWidth: true
									Keys.onPressed: {
										if (event.text === "") {
											this.text = "No key description available";
										} else {
											this.text = event.text;
										}
										game.setControls(index, event.key, false);
									}
								}
								Button {
									id: buttonRight
									enabled: buttonLeft.enabled
									text: "Right"
									Layout.fillWidth: true
									Keys.onPressed: {
										if (event.text === "") {
											this.text = "No key description available";
										} else {
											this.text = event.text;
										}
										game.setControls(index, event.key, true);
									}
								}
							}
							Button {
								anchors.left: parent.left
								anchors.right: parent.right
								text: "Remove player"
								highlighted: true
								onClicked: {
									if (radioButtonOnlinePlayer.checked) {
										alertDialogKick.open();
									} else {
										removePlayer(index);
									}
								}
								AlertDialog {
									id: alertDialogKick
									width: 300
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
	}

	ActionButton {
		id: addPlayerButton
		anchors {
			right: parent.right
			bottom: snackbar.top
			margins: 56
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
		anchors.margins: 70
		duration: buttonText == "" ? 2000 : 3000 // duration is longer if the notification comes with a button to press
		onClicked: snackbar.open("Clicking the snackbar button is not implemented yet")
	}
}
