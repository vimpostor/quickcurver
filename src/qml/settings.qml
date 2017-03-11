import QtQuick 2.7
import QtQuick.Layouts 1.1
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

TabbedPage {
	id: settings
	title: "Settings"

	onGoBack: {
		settings.forcePop();
	}
	Tab {
		title: "General"
		ColumnLayout {
			Rectangle {
				Layout.fillHeight: true
				Layout.fillWidth: true
				Layout.minimumHeight: settingsGrid.height + dp(80)
				Layout.minimumWidth: settingsGrid.width + dp(80)
				color: "#333"

				GridLayout {
					id: settingsGrid
					anchors.centerIn: parent
					rowSpacing: dp(20)
					columnSpacing: dp(10)
					columns: 2

					Label {
						text: "Round timeout"
						color: Theme.dark.textColor
					}
					Slider {
						value: 2
						tickmarksEnabled: true
						stepSize: 1
						numericValueLabel: true
						minimumValue: 0
						maximumValue: 10
						darkBackground: true
						onValueChanged: game.setRoundTimeout(value)
					}
					Label {
						text: "Base speed"
						color: Theme.dark.textColor
					}
					Slider {
						value: 128
						tickmarksEnabled: true
						stepSize: 32
						numericValueLabel: true
						minimumValue: 64
						maximumValue: 512
						darkBackground: true
						onValueChanged: game.setBaseSpeed(value)
					}
					Label {
						text: "Field size"
						color: Theme.dark.textColor
					}
					Slider {
						value: 800
						tickmarksEnabled: true
						stepSize: 50
						numericValueLabel: true
						minimumValue: 300
						maximumValue: 1000
						darkBackground: true
						onValueChanged: game.setFieldSize(value)
					}
					Label {
						text: "Time Multiplier when only bots are alive (Experimental)"
						color: Theme.dark.textColor
					}
					Slider {
						value: 1
						tickmarksEnabled: true
						stepSize: 1
						numericValueLabel: true
						minimumValue: 1
						maximumValue: 5
						darkBackground: true
						onValueChanged: game.setTimeMultiplier(value)
					}
					Label {
						text: "Show winner as chat message"
						color: Theme.dark.textColor
					}
					Switch {
						checked: false
						darkBackground: true
						onCheckedChanged: game.setSendWinnerMessages(checked)
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
					eIconName: "content/speedometer"
				}
				ListElement {
					name: "Faster Item"
					description: "Makes you faster for some time"
					defaultValue: 8
					eIconName: "action/clock_fast"
				}
				ListElement {
					name: "Slower Item"
					description: "Makes you slower for some time"
					defaultValue: 0
					eIconName: "content/timer_sand"
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
					eIconName: "action/arrow_expand"
				}
			}

			delegate: ListItem.Subtitled {
				text: name == "Header" ? "Item Spawn Rate" : name
				subText: description
				enabled: defaultValue != 0 //some items are not implemented yet
				iconName: eIconName
				secondaryItem: Slider {
					value: defaultValue
					anchors.verticalCenter: parent.verticalCenter
					tickmarksEnabled: true
					numericValueLabel: true
					minimumValue: 0
					maximumValue: name == "Header" ? 100 : 10
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
