import QtQuick 2.12
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import Fluid.Controls 1.0
import Fluid.Core 1.0

TabbedPage {
	id: settings
	title: "Settings"
	Tab {
		title: "General"
		ColumnLayout {
			anchors.centerIn: parent
			ListItem {
				text: "Round time out"
				rightItem: Slider {
					height: 24
					value: c_settings.getRoundTimeOut();
					from: 0
					to: 5000
					onValueChanged: c_settings.setRoundTimeOut(value);
				}
			}
			ListItem {
				text: "Item spawn"
				rightItem: RangeSlider {
					height: 24
					from: 50
					to: 10000
					first.value: c_settings.getItemSpawnIntervalMin();
					second.value: c_settings.getItemSpawnIntervalMax();
					first.onValueChanged: c_settings.setItemSpawnIntervalMin(first.value);
					second.onValueChanged: c_settings.setItemSpawnIntervalMax(second.value);
				}
			}
			ListItem {
				text: "Score to win"
				implicitHeight: 50
				rightItem: TextField {
					text: c_settings.getTargetScore()
					inputMethodHints: Qt.ImhDigitsOnly
					onTextChanged: c_settings.setTargetScore(text);
				}
			}
			ListItem {
				text: "Logic update rate"
				rightItem: Slider {
					height: 24
					value: c_settings.getUpdatesPerSecond();
					from: 30
					to: 144
					snapMode: Slider.SnapAlways
					stepSize: 1
					onValueChanged: c_settings.setUpdatesPerSecond(value);
				}
			}
			ListItem {
				text: "Network update rate"
				rightItem: Slider {
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
	Tab {
		title: "Item Spawn Probabilities"
		ListView {
			id: itemListView
			anchors.fill: parent
			model: c_itemModel
			delegate: ListItem {
				height: 108
				text: model.name
				subText: model.description
				icon.source: Utils.iconUrl(model.iconName)
				rightItem: Column {
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
