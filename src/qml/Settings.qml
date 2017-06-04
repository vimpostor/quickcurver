import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import Fluid.Controls 1.0
import Fluid.Material 1.0

TabbedPage {
	id: settings
	title: "Settings"

	onGoBack: {
		settings.forcePop();
	}
	Tab {
		title: "General"
		Flickable {
			anchors.fill: parent
			contentHeight: Math.max(generalLayout.implicitHeight, height)
			clip: true
			ColumnLayout {
				id: generalLayout
				anchors.centerIn: parent
				GridLayout {
					id: settingsGrid
					rowSpacing: 20
					columns: 2
					Label {
						text: "Round timeout"
					}
					Slider {
						id: timeoutSlider
						value: 2
						stepSize: 1
						from: 0
						to: 10
						onValueChanged: game.setRoundTimeout(value)
					}
					Label {
						text: "Base speed"
					}
					Slider {
						value: 128
						stepSize: 32
						from: 64
						to: 512
						onValueChanged: game.setBaseSpeed(value)
					}
					Label {
						text: "Field size"
					}
					Slider {
						value: 800
						stepSize: 50
						from: 300
						to: 1000
						onValueChanged: {
							game.setFieldSize(value);
							gameWidth = value;
						}
					}
					Label {
						text: "Fast forward at death"
					}
					Slider {
						value: 1
						stepSize: 1
						from: 1
						to: 5
						onValueChanged: game.setTimeMultiplier(value)
					}
					Label {
						text: "Show winner in chat"
					}
					Switch {
						checked: false
						onCheckedChanged: game.setSendWinnerMessages(checked)
					}
					Label {
						text: "Endless game"
					}
					Row {
						id: endlessRow
						anchors.left: timeoutSlider.left
						anchors.right: timeoutSlider.right
						spacing: 16
						Switch {
							id: endlessGameSwitch
							checked: true
							onCheckedChanged: game.setScoreToFinish(checked? 0 : scoreToFinishTextField.text)
						}
						TextField {
							id: scoreToFinishTextField
							enabled: !endlessGameSwitch.checked
							text: ""
							placeholderText: "Score to finish"
							Layout.fillWidth: true
							onTextChanged: game.setScoreToFinish(text)
						}
					}
				}
			}
		}
	}
	Tab {
		title: "Item Spawn Probabilities"
		ListItem {
			id: spawnRate
			text: settings.width > 380 ? "General item Spawn Rate" : ""
			iconName: "action/timeline"
			rightItem: Slider {
				value: 20
				from: 0
				to: 100
				onValueChanged: game.setItemSpawnrate(value)
			}
		}
		ListView {
			anchors.top: spawnRate.bottom
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			clip: true
			model: ListModel {
				ListElement {
					name: "Faster Item"
					description: "Makes you faster"
					defaultValue: 8
					eIconName: "maps/directions_bike"
				}
				ListElement {
					name: "Slower Item"
					description: "Makes you slower"
					defaultValue: 0
					eIconName: "action/bug_report"
				}
				ListElement {
					name: "Cleaninstall"
					description: "Deletes every line"
					defaultValue: 3
					eIconName: "communication/clear_all"
				}
				ListElement {
					name: "Wall Hack"
					description: "Opens the wall"
					defaultValue: 0
					eIconName: "editor/border_clear"
				}
				ListElement {
					name: "Invisibility"
					description: "Disables collisions"
					defaultValue: 4
					eIconName: "action/visibility_off"
				}
				ListElement {
					name: "Fatter Item"
					description: "Increases the line width"
					defaultValue: 1
					eIconName: "maps/local_dining"
				}
			}
			delegate: ListItem {
				text: settings.width > 380 ? name : ""
				subText: settings.width > 450 ? description : ""
				enabled: defaultValue != 0 //some items are not implemented yet
				iconName: eIconName
				rightItem: Slider {
					value: defaultValue
					from: 0
					to: 10
					onValueChanged: game.setItemPriority(index, value);
				}
			}
		}
	}
}
