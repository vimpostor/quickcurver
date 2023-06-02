import QtQuick
import QtQuick.Window
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

import Game
import Client
import Backend

ApplicationWindow {
	property int connectedToServer: game ? game.client.joinStatus === Client.JOINED : 0
	onClosing: {
		game.destroy();
	}
	id: root
	visible: true
	width: 1200
	height: 900
	title: "Quick Curver"
	Material.theme: Material.System
	Material.primary: Material.Yellow
	Material.accent: Material.Blue
	onWidthChanged: game.checkDimension();
	Action {
		shortcut: StandardKey.Quit
		onTriggered: close();
	}
	Item {
		id: initialPage
		anchors.fill: parent
		SplitView {
			anchors.fill: parent
			MouseArea {
				id: gameWave
				SplitView.preferredWidth: c_settings.width
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
					onPostInfoBar: (msg) => infoBar.open(msg);
					Keys.onPressed: (event) => {
						game.processKey(event.key, false);
					}
					Keys.onReleased: (event) => {
						game.processKey(event.key, true);
					}
					onGameStarted: {
						game.forceActiveFocus();
						/* gameWave.openWave(); */
					}
				}
				MouseArea {
					anchors.fill: parent
					onClicked: game.forceActiveFocus();
				}
				MouseArea {
					enabled: Backend.isMobile
					anchors.fill: parent
					anchors.rightMargin: parent.width / 2
					onPressedChanged: game.processKey(Qt.Key_Left, !pressed);
				}
				MouseArea {
					enabled: Backend.isMobile
					anchors.fill: parent
					anchors.leftMargin: parent.width / 2
					onPressedChanged: game.processKey(Qt.Key_Right, !pressed);
				}
			}
			Item {
				Item {
					id: options
					height: optionsLayout.implicitHeight
					anchors {top: parent.top; left: parent.left; right: parent.right; margins: 8}
					RowLayout {
						id: optionsLayout
						anchors {left: parent.left; right: parent.right}
						Button {
							icon.source: "qrc:///delete"
							enabled: !root.connectedToServer
							onClicked: game.resetGame();
							ToolTip.text: "Reset game"
							ToolTip.visible: pressed
							ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
						}
						Button {
							icon.source: "qrc:///upload-cloud"
							onClicked: clientDialog.open();
							ToolTip.text: "Join game"
							ToolTip.visible: pressed
							ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
						}
						Button {
							icon.source: "qrc:///settings"
							enabled: !root.connectedToServer
							onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
							ToolTip.text: "Settings"
							ToolTip.visible: pressed
							ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
						}
						Button {
							icon.source: "qrc:///info"
							onClicked: licenseDialog.open();
							ToolTip.text: "About"
							ToolTip.visible: pressed
							ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
						}
						Button {
							id: startButton
							Layout.fillWidth: true
							icon.source: "qrc:///navigation"
							enabled: !root.connectedToServer && (game ? !game.isStarted : false)
							text: "Start!"
							highlighted: true
							onClicked: {
								game.startGame();
							}
						}
						MessageDialog {
							id: licenseDialog
							text: "This software is free software licensed under the GNU GPL3.\nThe source code repository is available at https://github.com/vimpostor/quickcurver\nYou can also report any issues on the same website."
							buttons: MessageDialog.Ok
						}
					}
				}
				Chat {
					id: chat
					anchors {top: options.bottom; left: parent.left; right: parent.right; margins: 8}
					height: parent.height / 3
				}
				Players {
					id: players
					anchors {top: chat.bottom; left: parent.left; right: parent.right; bottom: parent.bottom; margins: 8}
				}
			}
		}
		StackView {
			id: pageStack
			anchors.fill: parent
		}
		SnackBar {
			id: infoBar
		}
		SnackBar {
			id: resizeSnackbar
			duration: 10000
			onAccepted: {
				gameWave.width =  Math.min(c_settings.width + 16, Screen.width - 16);
				root.height = Math.min(c_settings.height + 3 * 16, Screen.height - 16);
				if (root.width < gameWave.width) {
					root.width = gameWave.width + 14 * 16;
				}
			}
		}
		Dialog {
			property int joinStatus: game ? game.client.joinStatus : 0

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
					icon.source: "qrc:///droplet"
					icon.color: clientColorDialog.selectedColor
					onClicked: clientColorDialog.open();
					ColorDialog {
						id: clientColorDialog
						selectedColor: Material.accent
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
						c_settings.setClientColor(clientColorDialog.selectedColor);
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
