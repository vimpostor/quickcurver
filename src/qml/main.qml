import QtQuick 2.7
import Game 1.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import Fluid.Controls 1.0
import Fluid.Material 1.0
import QtQuick.Layouts 1.1

ApplicationWindow {
	onClosing: game.close()
	property bool clientInGame: false;
	property bool serverStarted: false;
	function sendMessage(username, message) {
		pageStack.currentItem.sendMessage(username, message);
	}
	function changeScore(index, newScore, roundScore) {
		playerListModel.setProperty(index, "escore", newScore);
		playerListModel.setProperty(index, "eroundScore", roundScore);
	}
	function setJoinStatus(s) {
		if (s === "TCPACK") {
			joinButton.text = "Testing UDP connection...";
		} else if (s === "JOINED") {
			joinButton.text = "Joined, waiting for host to start...";
		} else if (s === "REJECTED") {
			clientDialog.close();
			playerselector.mysnackbar.open("Join request was rejected :(");
		} else if (s === "TIMEOUT") {
			clientDialog.close();
			playerselector.mysnackbar.open("Join request timed out! :(");
		} else if (s === "STARTED") {
			clientInGame = true;
			clientDialog.close();
			pageStack.push(Qt.resolvedUrl("gamePage.qml"));
			game.focus = true;
		} else if (s === "KICKED") {
			clientDialog.close();
			playerselector.mysnackbar.open("You got kicked from the game!");
		} else if (s === "TERMINATE") {
			close();
		}
	}
	function setPlayerStatus(index,s) {
		if (s === "JOINED") {
			playerListModel.setProperty(index, "eJoined", true);
		} else if (s === "LEFT") {
			playerListModel.setProperty(index, "eJoined", false);
			playerListModel.setProperty(index, "eReady", false);
		} else if (s === "READY") {
			playerListModel.setProperty(index, "eReady", true);
		} else if (s === "UNREADY") {
			playerListModel.setProperty(index, "eReady", false);
		} else if (s.substring(0, 8) === "USERNAME") {
			playerListModel.setProperty(index, "ename", s.substring(8));
		}
	}
	function notifyGUI(s, mode) {
		if (mode === "SNACKBAR") {
			playerselector.mysnackbar.open(s);
		}
	}
	function clientEditPlayer(index,name) {
		if (index > playerListModel.count - 1) {
			// add player
			playerListModel.append({ename: name})
		} else {
			playerListModel.setProperty(index, "ename", name);
		}
	}

	id: root
	width: 1200
	height: 900
	title: "Quick Curver"
	Material.primary: Material.Yellow
	Material.accent: Material.Blue
	visible: true
	appBar.maxActionCount: 3
	initialPage: Page {
		id: initialPage
		title: "Quick Curver"
		actions: [
			Action {
				iconName: "action/search"
				text: "Join an online game"
				onTriggered: clientDialog.open()
				shortcut: "Ctrl+J"
			},
			Action {
				id: startServerAction
				iconName: serverStarted? "content/content_copy" : "file/cloud_upload"
				text: serverStarted? "Copy IP address" : "Host an online game"
				onTriggered: {
					if (serverStarted) {
						playerselector.mysnackbar.open("Copied IP address " + game.copyIp())
					} else {
						serverDialog.open();
					}
				}
				shortcut: "Ctrl+C"
			},
			Action {
				iconName: "action/settings"
				text: "Settings"
				onTriggered: pageStack.push(Qt.resolvedUrl("settings.qml"))
				shortcut: "Ctrl+I"
			},
			Action {
				iconName: "action/info"
				text: "About"
				onTriggered: licensesOverlay.open(initialPage.actionBar)
			},
			Action {
				iconName: "action/help"
				text: "Help"
				onTriggered: pageStack.push(Qt.resolvedUrl("help.qml"))
			},
			Action {
				iconName: "navigation/close"
				text: "Quit"
				onTriggered: close()
				shortcut: "Ctrl+Q"
			}

		]
		PlayerSelector {
			id: playerselector
			anchors.fill: parent
		}
	}
	Item {
		anchors.top: parent.top
		anchors.right: parent.right
		anchors.margins: 16
		width: 800
		Game {
			id: game
			objectName: "game"
			anchors.fill: parent
			Keys.onPressed:  {
				game.sendKey(event.key);
			}
			Keys.onReleased: {
				game.releaseKey(event.key);
			}
		}
	}
	ListModel {
		id: playerListModel
		ListElement {
			ename: "Player 0"
			escore: 0
			eroundScore: 0
			eBot: false
			eOnline: false
			eJoined: false
			eReady: false
		}
		ListElement {
			ename: "Player 1"
			escore: 0
			eroundScore: 0
			eBot: true
			eOnline: false
			eJoined: false
			eReady: false
		}
	}
	Dialog {
		id: licensesOverlay
		x: (parent.width-width) / 2
		y: (parent.height-height) / 2
		width: parent.width/2
		height: parent.height/2
		Licenses {
		}
	}
	Dialog {
		id: clientDialog
		title: "Join Online Game"
		x: (parent.width - width) / 2;
		y: (parent.height - height) / 2;
		ColumnLayout {
			anchors.left: parent.left
			anchors.right: parent.right
			RowLayout {
				anchors.left: parent.left
				anchors.right: parent.right
				Layout.fillWidth: true
				IconButton {
					iconName: "content/content_paste"
					onClicked: {
						var c = game.getClipboardContent();
						if (c === "") {
							playerselector.mysnackbar.open("No text in clipboard!");
						} else {
							serverIp.text = c;
						}
					}
				}
				TextField {
					id: serverIp
					Layout.fillWidth: true
					placeholderText: "Server IP Adress"
					Keys.onReturnPressed: {
						joinButton.clicked();
					}
				}
				Label {
					text: ":"
				}
				TextField {
					id: serverPort
					Layout.fillWidth: true
					placeholderText: "Port"
					text: "52552"
				}
				BusyIndicator {
					id: cyclicColorProgress
					visible: running
				}
			}
			ListItem {
				text: "Username"
				iconName: "action/account_circle"
				rightItem: TextField {
					id: clientUserNameTextField
					placeholderText: "Username"
					onTextChanged: game.changeClientSettings(text, clientReadyCheckBox.checked)
				}
			}
			ListItem {
				text: "Ready"
				iconName: "action/check_circle"
				rightItem: CheckBox {
					id: clientReadyCheckBox
					checked: false
					onCheckedChanged: game.changeClientSettings(clientUserNameTextField.text, checked)
				}
				onClicked: clientReadyCheckBox.checked = !clientReadyCheckBox.checked
			}
			Button {
				id: joinButton
				Layout.fillWidth: true
				text: "Join"
				highlighted: true
				onClicked: {
					enabled = false;
					cyclicColorProgress.running = true;
					text = "Waiting for server response...";
					game.clientStart(serverIp.text, serverPort.text);
				}
			}
		}
		onOpened: {
			serverIp.forceActiveFocus();
			cyclicColorProgress.running = false;
			joinButton.enabled = true;
			joinButton.text = "Join";
		}
		onClosed: {
			if (!clientInGame) {
//				game.leaveGame();
			}
		}
	}
	InputDialog {
		id: serverDialog
		title: "Host an online game"
		x: (parent.width - width) / 2;
		y: (parent.height - height) / 2;
		text: "On what port do you want the server to run?"
		textField.text: "52552"
		textField.inputMask: "99999"
		onAccepted: {
			game.startServer(textField.text)
			serverStarted = true;
		}
	}
}
