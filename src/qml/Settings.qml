import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Quartz

import Backend

Page {
	id: settings
	title: "Settings"
	TabBar {
		id: bar
		anchors.left: parent.left
		anchors.right: closeButton.left
		TabButton {
			text: "General"
		}
		TabButton {
			text: "Item spawn probabilities"
		}
	}
	IconButton {
		id: closeButton
		anchors.right: parent.right
		anchors.top: parent.top
		ico.name: "close"
		onClicked: pageStack.clear();
	}
	StackLayout {
		anchors {top: bar.bottom; left: parent.left; right: parent.right; bottom: parent.bottom}
		currentIndex: bar.currentIndex
		Item {
			GridLayout {
				anchors.centerIn: parent
				columns: 2
				Label {
					text: "Round time out"
				}
				Slider {
					height: 24
					value: Settings.getRoundTimeOut();
					from: 0
					to: 5000
					onValueChanged: Settings.setRoundTimeOut(value);
				}
				Label {
					text: "Item spawn"
				}
				RangeSlider {
					height: 24
					from: 50
					to: 10000
					first.value: Settings.getItemSpawnIntervalMin();
					second.value: Settings.getItemSpawnIntervalMax();
					first.onValueChanged: Settings.setItemSpawnIntervalMin(first.value);
					second.onValueChanged: Settings.setItemSpawnIntervalMax(second.value);
				}
				Label {
					text: "Score to win"
				}
				TextField {
					text: Settings.getTargetScore()
					inputMethodHints: Qt.ImhDigitsOnly
					onTextChanged: Settings.setTargetScore(text);
				}
				Label {
					text: "Logic update rate"
				}
				Slider {
					height: 24
					value: Settings.getUpdatesPerSecond();
					from: 30
					to: 144
					snapMode: Slider.SnapAlways
					stepSize: 1
					onValueChanged: Settings.setUpdatesPerSecond(value);
				}
				Label {
					text: "Network update rate"
				}
				Slider {
					height: 24
					value: Settings.getNetworkCurverBlock();
					from: 1
					to: 8
					snapMode: Slider.SnapAlways
					stepSize: 1
					onValueChanged: Settings.setNetworkCurverBlock(value);
				}
			}
		}
		Item {
			ListView {
				id: itemListView
				anchors.fill: parent
				model: ItemModel
				delegate: RowLayout {
					width: parent.width
					IconButton {
						ico.name: model.iconName
					}
					Label {
						text: model.name + " (" + model.description + ")"
						Layout.fillWidth: true
					}
					Slider {
						id: probabilitySlider
						value: model.probability
						onValueChanged: ItemModel.setProbability(index, value);
					}
					ComboBox {
						model: ListModel {
							ListElement { name: "Allow all" }
							ListElement { name: "Allow others" }
							ListElement { name: "Allow collector" }
						}
						implicitWidth: 160
						currentIndex: allowedUsers
						onCurrentIndexChanged: ItemModel.setAllowedUsers(index, currentIndex);
					}
				}
			}
		}
	}
}
