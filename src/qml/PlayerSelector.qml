import QtQuick 2.7
import QtQuick.Layouts 1.1
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Controls.Material 2.0

Item {
	property var mysnackbar: snackbar
	View {
		anchors.fill: parent
		anchors.margins: dp(32)
		elevation: 1
		ListItem.Subheader {
			id: playerListHeader
			text: "Players"
			anchors.bottom: playerList.top
		}

		View {
			id: playerList
			anchors.top: playerListHeader.bottom
			anchors.bottom: parent.bottom
			anchors.bottomMargin: dp(50)
			anchors.left: parent.left
			anchors.right: parent.right
			ListView {
				id: playerListView
				anchors.fill: parent
				model: playerListModel
				delegate: playerDelegate
				add: Transition {
					NumberAnimation { properties: "y"; from: startButton.y; duration: 100}
				}
			}
		}
		Button {
			id: startButton
			focus: true
			anchors.top: playerList.bottom
			anchors.right: parent.right
			anchors.left: parent.left
			anchors.bottom: parent.bottom
			text: "Start!"
			elevation: 1
			backgroundColor: Theme.primaryColor
			onClicked: {
				if (game.isReady()) {
					pageStack.push(Qt.resolvedUrl("gamePage.qml"));
					game.parent.x = game.parent.parent.width - game.getFieldSize();
					game.start();
					game.focus = true;
				}
			}
		}

		Component {
			id: playerDelegate
			ListItem.Subtitled {
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
					playerEditOverlay.open(playerListView.currentItem);
				}
				text: ename
				subText: playerController.selectedIndex == 2  ? (eJoined ? "Joined" : "Waiting for player to join...") : "Change color, controls etc..."
				iconName: playerController.selectedIndex == 0 ? "action/account_circle" : (playerController.selectedIndex == 1 ? "communication/robot" : (!eJoined ? "social/account_off" : (eReady ? "social/account_check" : "social/account")))

				secondaryItem: IconButton {
					iconName: "editor/mode_edit"
					anchors.verticalCenter: parent.verticalCenter
					color: mycolor
					onClicked: mycolorPicker.show()
					Dialog {
						id: mycolorPicker
						title: "Pick color"
						positiveButtonText: "Done"
						Grid {
							columns: 7
							spacing: dp(8)
							Repeater {
								model: [
									"red", "pink", "purple", "deepPurple", "indigo",
									"blue", "lightBlue", "cyan", "teal", "green",
									"lightGreen", "lime", "yellow", "amber", "orange",
									"deepOrange", "grey", "blueGrey", "brown", "black",
									"white"
								]
								Rectangle {
									width: dp(30)
									height: dp(30)
									radius: dp(2)
									color: Palette.colors[modelData]["500"]
									border.width: modelData === "white" ? dp(2) : 0
									border.color: Theme.alpha("#000", 0.26)
									Ink {
										anchors.fill: parent
										onPressed: mycolor = parent.color
									}
								}
							}
						}
					}
				}
				OverlayView {
					id: playerEditOverlay
					width: dp(300)
					height: dp(300)
					ColumnLayout {
						id: playerEditDialog
						anchors.fill: parent
						anchors.margins: dp(16)
						Label {
							id: editHeader
							text: "Edit player"
							width: parent.width
							font.pixelSize: dp(22)
							Layout.alignment: Qt.AlignCenter
						}

						Card {
							Layout.fillHeight: true
							Layout.fillWidth: true
							ColumnLayout {
								anchors.fill: parent
								anchors.margins: dp(32)
								TextField {
									id: nameTextField
									enabled: playerController.selectedIndex != 2
									focus: true
									width: parent.width
									placeholderText: "Name"
									floatingLabel: true
									onTextChanged: name = this.text
									Layout.fillWidth: true
								}

								MenuField {
									id: playerController
									model: ["Local Player", "Bot", "Online Player"]
									width: dp(160)
									selectedIndex: eBot ? 1 : 0
									Layout.fillWidth: true
									onSelectedIndexChanged: game.setController(index, selectedIndex)
								}

								GridLayout {
									id: editPlayerGrid
									rowSpacing: dp(20)
									columnSpacing: dp(10)
									columns: 2
									Layout.fillWidth: true
									Layout.alignment: Qt.AlignCenter

									Button {
										id: buttonLeft
										enabled: playerController.selectedIndex == 0
										text: "Left"
										elevation: 1
										activeFocusOnPress: true
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
										elevation: 1
										activeFocusOnPress: true
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
							}
						}
					}
				}
			}
		}
	}

	ActionButton {
		id: onlineButton
		iconName: "action/language"
		y: addPlayerButton.y
		visible: false
		anchors.left: addPlayerButton.left
		anchors.right: addPlayerButton.right
		anchors.margins: dp(6)
		height: addPlayerButton.height - dp(12)
		state: "hidden"
		states: [
			State {
				name: "hidden"
				PropertyChanges {
					target: onlineButton
					y: addPlayerButton.y
					visible: true
				}
			},
			State {
				name: "visible"
				PropertyChanges {
					target: onlineButton
					y: botButton.y - dp(52)
					visible: true
				}
			}
		]
		Behavior on y {
			PropertyAnimation {easing.type: Easing.InOutQuad; duration: 150}
		}
		action: Action {
			shortcut: "Ctrl+Shift+Alt+N"
			onTriggered: {
				if (playerListModel.count >= 16) {
					snackbar.open("Sorry, you have reached maximum player capacity!");
				} else {
					snackbar.open("Sry, this is not yet implemented, pls manually edit a player");
				}
			}
		}
	}
	ActionButton {
		id: botButton
		iconName: "communication/robot"
		y: addPlayerButton.y
		visible: false
		anchors.left: addPlayerButton.left
		anchors.right: addPlayerButton.right
		anchors.margins: dp(6)
		height: addPlayerButton.height - dp(12)
		state: "hidden"
		states: [
			State {
				name: "hidden"
				PropertyChanges {
					target: botButton
					y: addPlayerButton.y
					visible: true
				}
			},
			State {
				name: "visible"
				PropertyChanges {
					target: botButton
					y: playerButton.y - dp(52)
					visible: true
				}
			}
		]
		Behavior on y {
			PropertyAnimation {easing.type: Easing.InOutQuad; duration: 150}
		}
		action: Action {
			shortcut: "Ctrl+Shift+N"
			onTriggered: {
				if (playerListModel.count >= 16) {
					snackbar.open("Sorry, you have reached maximum player capacity!");
				} else {
					game.addPlayer();
					playerListModel.append({eBot: true});
				}
			}
		}
	}
	ActionButton {
		id: playerButton
		iconName: "device/controller"
		y: addPlayerButton.y
		visible: false
		anchors.left: addPlayerButton.left
		anchors.right: addPlayerButton.right
		anchors.margins: dp(6)
		height: addPlayerButton.height - dp(12)
		state: "hidden"
		states: [
			State {
				name: "hidden"
				PropertyChanges {
					target: playerButton
					y: addPlayerButton.y
					visible: true
				}
			},
			State {
				name: "visible"
				PropertyChanges {
					target: playerButton
					y: addPlayerButton.y - dp(52)
					visible: true
				}
			}
		]
		Behavior on y {
			PropertyAnimation {easing.type: Easing.InOutQuad; duration: 150}
		}
		action: Action {
			id: addPlayer
			hoverAnimation: true
			shortcut: "Ctrl+N"
			onTriggered: {
				if (playerListModel.count >= 16) {
					snackbar.open("Sorry, you have reached maximum player capacity!");
				} else {
					game.addPlayer();
					playerListModel.append({});
				}
			}
		}
	}
	ActionButton {
		id: addPlayerButton
		anchors {
			right: parent.right
			bottom: snackbar.top
			margins: dp(64)
		}
		IconButton {
			anchors.fill: parent
			action: Action {
				text: "Add Player"
				hoverAnimation: true
				iconName: "content/add"
				onTriggered: {
					if (playerButton.state === "visible") {
						playerButton.state = "hidden";
						botButton.state = "hidden";
						onlineButton.state = "hidden";
					} else {
						playerButton.state = "visible";
						botButton.state = "visible";
						onlineButton.state = "visible";
					}
				}
			}
		}
	}
	Snackbar {
		id: snackbar
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.margins: dp(70)
		duration: buttonText == "" ? 2000 : 3000 // duration is longer if the notification comes with a button to press
		onClicked: snackbar.open("Clicking the snackbar button is not implemented yet")
	}
}
