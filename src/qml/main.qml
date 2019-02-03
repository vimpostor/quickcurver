import QtQuick 2.12
import QtQuick.Window 2.12
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
		appBar.maxActionCount: 4
		actions: [
			Action {
				icon.source: Utils.iconUrl("content/undo")
				text: "Reset game"
				enabled: !c_settings.connectedToServer
				onTriggered: game.resetGame();
			},
			Action {
				icon.source: Utils.iconUrl("av/hearing")
				text: "Server listen"
				enabled: !c_settings.connectedToServer
				onTriggered: listenDialog.open();
			},
			Action {
				icon.source: Utils.iconUrl("file/cloud_upload")
				text: "Join game"
				onTriggered: clientDialog.open();
				shortcut: "Ctrl+J"
			},
			Action {
				icon.source: Utils.iconUrl("action/settings")
				text: "Settings"
				enabled: !c_settings.connectedToServer
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
		Wave {
			id: gameWave
			anchors {top: parent.top; left: parent.left; right: gameSeparator.left; bottom: parent.bottom; margins: Units.smallSpacing}
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
				property int realWidth: c_settings.width
				property int realHeight: c_settings.height
				function checkDimension() {
					if (!c_settings.connectedToServer) {
						// we are manually resizing anyway
						return;
					}
					if (realWidth > width || realHeight > height || width > root.width) {
						resizeSnackbar.open("The server has set a larger game size.", "Resize automatically");
					} else {
						resizeSnackbar.close();
					}
				}
				onWidthChanged: {
					if (!c_settings.connectedToServer) {
						c_settings.width = width;
					}
					checkDimension();
				}
				onHeightChanged: {
					if (!c_settings.connectedToServer) {
						c_settings.height = height;
					}
					checkDimension();
				}
				onRealWidthChanged: checkDimension();
				onRealHeightChanged: checkDimension();
				onPostInfoBar: infoBar.open(msg);
				Keys.onPressed: {
					game.processKey(event.key, false);
				}
				Keys.onReleased: {
					game.processKey(event.key, true);
				}
				onGameStarted: {
					players.startButton.visible = false;
					game.forceActiveFocus();
					gameWave.openWave();
					if (Device.isMobile) {
						appBar.visible = false;
					}
				}
			}
			Ripple {
				anchors.fill: parent
				onClicked: game.forceActiveFocus();
			}
			MouseArea {
				enabled: Device.isMobile
				anchors.fill: parent
				anchors.rightMargin: parent.width / 2
				onPressedChanged: game.processKey(Qt.Key_Left, !pressed);
			}
			MouseArea {
				enabled: Device.isMobile
				anchors.fill: parent
				anchors.leftMargin: parent.width / 2
				onPressedChanged: game.processKey(Qt.Key_Right, !pressed);
			}
		}
		Button {
			id: gameSeparator
			x: Device.isMobile ? 200 : 720
			width: Units.smallSpacing
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.margins: Units.smallSpacing
			onClicked: {
				if (Device.isMobile) {
					appBar.visible = !appBar.visible;
				}
			}
			DragHandler {
				yAxis.enabled: false
			}
		}
		Chat {
			id: chat
			anchors {top: parent.top; left: gameSeparator.right; right: parent.right; margins: Units.smallSpacing}
			height: parent.height / 3
		}
		Players {
			id: players
			anchors {top: chat.bottom; left: gameSeparator.right; right: parent.right; bottom: parent.bottom; margins: Units.smallSpacing}
		}
		SnackBar {
			id: infoBar
		}
		SnackBar {
			id: resizeSnackbar
			duration: 10000
			onClicked: {
				gameSeparator.x =  Math.min(c_settings.width + Units.largeSpacing, Screen.width - Units.largeSpacing);
				root.height = Math.min(c_settings.height + 3 * Units.largeSpacing, Screen.height - Units.largeSpacing);
				if (root.width < gameSeparator.x) {
					root.width = gameSeparator.x + 14 * Units.largeSpacing;
				}
				close();
			}
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
