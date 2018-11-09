import QtQuick 2.7
import Qt.labs.platform 1.0 as Platform
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import Fluid.Controls 1.0
import Fluid.Core 1.0
import QtQuick.Layouts 1.1

import Game 1.0

ApplicationWindow {
	onClosing: {
		game.destroy();
	}
	id: root
	visible: true
	width: 1200
	height: 900
	title: "Quick Curver"
	Material.primary: Material.Yellow
	Material.accent: Material.Blue
	initialPage: Page {
		id: initialPage
		title: "Quick Curver"
		appBar.maxActionCount: 6
		actions: [
			Action {
				icon.source: Utils.iconUrl("content/undo")
				text: "Reset game"
				onTriggered: game.resetGame();
			},
			Action {
				icon.source: Utils.iconUrl("av/hearing")
				text: "Server listen"
				onTriggered: listenDialog.open();
			},
			Action {
				icon.source: Utils.iconUrl("file/cloud_upload")
				text: "Join game"
				onTriggered: clientDialog.open();
				shortcut: "Ctrl+J"
			},
			Action {
				icon.source: Utils.iconUrl("editor/border_left")
				text: "Left"
				onTriggered: gameWave.width += 20
				shortcut: "Ctrl+H"
			},
			Action {
				icon.source: Utils.iconUrl("editor/border_right")
				text: "Right"
				onTriggered: gameWave.width -= 20
				shortcut: "Ctrl+L"
			},
			Action {
				icon.source: Utils.iconUrl("action/settings")
				text: "Settings"
				onTriggered: pageStack.push(Qt.resolvedUrl("Settings.qml"))
				shortcut: "Ctrl+I"
			},
			Action {
				icon.source: Utils.iconUrl("action/info")
				text: "About"
				onTriggered: pageStack.push(Qt.resolvedUrl("About.qml"))
			},
			Action {
				icon.source: Utils.iconUrl("navigation/close")
				text: "Quit"
				onTriggered: close();
				shortcut: "Ctrl+Q"
			}
		]
		Chat {
			id: chat
			anchors {top: parent.top; left: parent.left; right: gameWave.left; margins: Units.smallSpacing}
			height: parent.height / 3
		}
		Players {
			id: players
			anchors {top: chat.bottom; left: parent.left; right: gameWave.left; bottom: parent.bottom; margins: Units.smallSpacing}
		}
		Wave {
			id: gameWave
			anchors {top: parent.top; right: parent.right; bottom: parent.bottom; margins: Units.smallSpacing}
			width: 0
			Behavior on width {
				NumberAnimation {
					easing.type: Easing.OutCubic
				}
			}
			Rectangle {
				anchors.fill: parent
				color: Material.color(Material.BlueGrey, Material.Shade900)
			}
			Game {
				id: game
				anchors.fill: parent
				onPostInfoBar: infoBar.open(msg);
				onWidthChanged: c_settings.setWidth(width);
				onHeightChanged: c_settings.setHeight(height);
				Keys.onPressed: {
					game.processKey(event.key, false);
				}
				Keys.onReleased: {
					game.processKey(event.key, true);
				}
				onGameStarted: {
					game.forceActiveFocus();
					gameWave.width = 700;
					gameWave.openWave();
					players.startButton.visible = false;
				}
				Ripple {
					anchors.fill: parent
					onClicked: game.forceActiveFocus();
				}
			}
		}
		SnackBar {
			id: infoBar
		}
		InputDialog {
			id: listenDialog
			x: (parent.width - width)/2
			y: (parent.height - height)/2
			title: "Listen on which port"
			onAccepted: game.serverReListen(textField.text);
		}
		Dialog {
			id: clientDialog
			title: "Join game"
			x: (parent.width - width)/2
			y: (parent.height - height)/2
			Column {
				TextField {
					id: nameTextField
					placeholderText: "Username"
					text: "Client"
				}
				ToolButton {
					icon.source: Utils.iconUrl("editor/format_color_fill")
					icon.color: clientColorDialog.color
					onClicked: clientColorDialog.open();
					Platform.ColorDialog {
						id: clientColorDialog
						color: Material.accent
					}
				}
				TextField {
					id: ipTextField
					placeholderText: "IP (IPv4 or IPv6)"
					text: "127.0.0.1"
				}
				TextField {
					id: portTextField
					placeholderText: "Port"
					onAccepted: clientDialog.accept();
				}
			}
			standardButtons: Dialog.Cancel | Dialog.Ok
			onAccepted: {
				c_settings.setClientName(nameTextField.text);
				c_settings.setClientColor(clientColorDialog.color);
				game.connectToHost(ipTextField.text, portTextField.text);
			}
			onOpened: portTextField.forceActiveFocus();
		}
	}
}
