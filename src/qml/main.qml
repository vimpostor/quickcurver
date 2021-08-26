import QtQuick 2.12
import QtQuick.Window 2.12
import Qt.labs.platform 1.0 as Platform
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import Fluid.Controls 1.0
import Fluid.Core 1.0
import QtQuick.Layouts 1.1

import Game 1.0
import Client 1.0

ApplicationWindow {
	property int connectedToServer: game.client.joinStatus === Client.JOINED
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
	onWidthChanged: game.checkDimension();
	initialPage: Page {
		id: initialPage
		title: "Quick Curver"
		appBar.maxActionCount: 3
		actions: [
			Action {
				icon.source: "qrc:///content/undo"
				text: "Reset game"
				enabled: !root.connectedToServer
				onTriggered: game.resetGame();
			},
			Action {
				icon.source: "qrc:///file/cloud_upload"
				text: "Join game"
				onTriggered: clientDialog.open();
				shortcut: "Ctrl+J"
			},
			Action {
				icon.source: "qrc:///action/settings"
				text: "Settings"
				enabled: !root.connectedToServer
				onTriggered: pageStack.push(Qt.resolvedUrl("Settings.qml"))
				shortcut: "Ctrl+I"
			},
			Action {
				icon.source: "qrc:///action/info"
				text: "About"
				onTriggered: pageStack.push(Qt.resolvedUrl("About.qml"))
			},
			Action {
				icon.source: "qrc:///navigation/close"
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
					if (!root.connectedToServer) {
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
					if (!root.connectedToServer) {
						c_settings.width = width;
					}
					checkDimension();
				}
				onHeightChanged: {
					if (!root.connectedToServer) {
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
		Dialog {
			property int joinStatus: game.client.joinStatus

			id: clientDialog
			title: "Join game"
			x: (parent.width - width)/2
			y: (parent.height - height)/2
			onJoinStatusChanged: {
				if (joinStatus === Client.JOINED) {
					clientDialog.accept();
				}
			}
			Column {
				width: 150
				TextField {
					id: nameTextField
					anchors.left: parent.left
					anchors.right: parent.right
					placeholderText: "Username"
					text: "Client"
				}
				ToolButton {
					icon.source: "qrc:///editor/format_color_fill"
					icon.color: clientColorDialog.color
					onClicked: clientColorDialog.open();
					Platform.ColorDialog {
						id: clientColorDialog
						color: Material.accent
					}
				}
				TextField {
					id: ipTextField
					anchors.left: parent.left
					anchors.right: parent.right
					placeholderText: "IP (IPv4 or IPv6)"
					text: "127.0.0.1"
				}
				TextField {
					id: portTextField
					anchors.left: parent.left
					anchors.right: parent.right
					placeholderText: "Port"
					onAccepted: buttonJoin.clicked();
				}
				Button {
					id: buttonJoin
					enabled: clientDialog.joinStatus === Client.NONE || clientDialog.joinStatus === Client.FAILED || clientDialog.joinStatus === Client.JOINED
					text: enabled ? "Join" : clientDialog.joinStatus === Client.DNS_PENDING ? "Looking up hostname..." : clientDialog.joinStatus === Client.TCP_PENDING ? "Connecting TCP..." : clientDialog.joinStatus === Client.UDP_PENDING ? "Connecting UDP..." : "Connected"
					anchors.left: parent.left
					anchors.right: parent.right
					onClicked: {
						c_settings.setClientName(nameTextField.text);
						c_settings.setClientColor(clientColorDialog.color);
						game.connectToHost(ipTextField.text, portTextField.text);
					}
				}
				BusyIndicator {
					id: busyIndicatorJoining
					running: !buttonJoin.enabled
					visible: running
					anchors.horizontalCenter: parent.horizontalCenter
				}
			}
			onOpened: portTextField.forceActiveFocus();
		}
	}
}
