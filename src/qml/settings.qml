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
					columnSpacing: 10
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
						onValueChanged: game.setFieldSize(value)
					}
					Label {
						text: "Time Multiplier when only bots are alive (Experimental)"
					}
					Slider {
						value: 1
						stepSize: 1
						from: 1
						to: 5
						onValueChanged: game.setTimeMultiplier(value)
					}
					Label {
						text: "Show winner as chat message"
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
		ListView {
			anchors.fill: parent
			model: ListModel {
				ListElement {
					name: "Header"
					description: "General item spawn rate"
					defaultValue: 20
					eIconName: "action/timeline"
				}
				ListElement {
					name: "Faster Item"
					description: "Makes you faster for some time"
					defaultValue: 8
					eIconName: "maps/directions_bike"
				}
				ListElement {
					name: "Slower Item"
					description: "Makes you slower for some time"
					defaultValue: 0
					eIconName: "action/bug_report"
				}
				ListElement {
					name: "Cleaninstall"
					description: "Deletes every line drawn to this point"
					defaultValue: 3
					eIconName: "communication/clear_all"
				}
				ListElement {
					name: "Wall Hack"
					description: "Opens the wall for every player"
					defaultValue: 0
					eIconName: "editor/border_clear"
				}
				ListElement {
					name: "Invisibility"
					description: "Disables collisions for you"
					defaultValue: 4
					eIconName: "action/visibility_off"
				}
				ListElement {
					name: "Fatter Item"
					description: "Makes the enemy fatter"
					defaultValue: 1
					eIconName: "maps/local_dining"
				}
			}
			delegate: ListItem {
				text: name == "Header" ? "Item Spawn Rate" : name
				subText: description
				enabled: defaultValue != 0 //some items are not implemented yet
				iconName: eIconName
				rightItem: Slider {
					value: defaultValue
					from: 0
					to: name == "Header" ? 100 : 10
					onValueChanged: {
						if (name == "Header") {
							game.setItemSpawnrate(value);
						} else {
							game.setItemPriority(index-1, value);
						}
					}
				}
			}
		}
	}
}
