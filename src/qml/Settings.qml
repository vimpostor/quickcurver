import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Quartz

Page {
	id: settings
	title: "Settings"
	TabBar {
		id: bar
		width: parent.width
		TabButton {
			text: "General"
		}
		TabButton {
			text: "Item spawn probabilities"
		}
	}
	IconButton {
		ico.name: "close"
		onClicked: pageStack.clear();
	}
	StackLayout {
		anchors {top: bar.bottom; left: parent.left; right: parent.right; bottom: parent.bottom}
		currentIndex: bar.currentIndex
		Item {
			ColumnLayout {
				anchors.centerIn: parent
				RowLayout {
					Label {
						text: "Round time out"
					}
					Slider {
						height: 24
						value: c_settings.getRoundTimeOut();
						from: 0
						to: 5000
						onValueChanged: c_settings.setRoundTimeOut(value);
					}
				}
				RowLayout {
					Label {
						text: "Item spawn"
					}
					RangeSlider {
						height: 24
						from: 50
						to: 10000
						first.value: c_settings.getItemSpawnIntervalMin();
						second.value: c_settings.getItemSpawnIntervalMax();
						first.onValueChanged: c_settings.setItemSpawnIntervalMin(first.value);
						second.onValueChanged: c_settings.setItemSpawnIntervalMax(second.value);
					}
				}
				RowLayout {
					Label {
						text: "Score to win"
					}
					TextField {
						text: c_settings.getTargetScore()
						inputMethodHints: Qt.ImhDigitsOnly
						onTextChanged: c_settings.setTargetScore(text);
					}
				}
				RowLayout {
					Label {
						text: "Logic update rate"
					}
					Slider {
						height: 24
						value: c_settings.getUpdatesPerSecond();
						from: 30
						to: 144
						snapMode: Slider.SnapAlways
						stepSize: 1
						onValueChanged: c_settings.setUpdatesPerSecond(value);
					}
				}
				RowLayout {
					Label {
						text: "Network update rate"
					}
					Slider {
						height: 24
						value: c_settings.getNetworkCurverBlock();
						from: 1
						to: 8
						snapMode: Slider.SnapAlways
						stepSize: 1
						onValueChanged: c_settings.setNetworkCurverBlock(value);
					}
				}
			}
		}
		Item {
			ListView {
				id: itemListView
				anchors.fill: parent
				model: c_itemModel
				delegate: Item {
					height: 108
					RowLayout {
						IconButton {
							ico.name: model.iconName
						}
						Label {
							text: model.name + " (" + model.description + ")"
						}
						Slider {
							id: probabilitySlider
							value: model.probability
							onValueChanged: c_itemModel.setProbability(index, value);
						}
						ComboBox {
							model: ListModel {
								ListElement { name: "Allow all" }
								ListElement { name: "Allow others" }
								ListElement { name: "Allow collector" }
							}
							width: probabilitySlider.width
							currentIndex: allowedUsers
							onCurrentIndexChanged: c_itemModel.setAllowedUsers(index, currentIndex);
						}
					}
				}
			}
		}
	}
}
