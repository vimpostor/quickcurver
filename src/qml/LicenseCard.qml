import QtQuick 2.7
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Layouts 1.1

Card {
	property string cardTitle
	property string cardDescription
	property string cardLink
	Layout.fillWidth: true
	Layout.fillHeight: true
	Ink {
		anchors.fill: parent
		onClicked: Qt.openUrlExternally(cardLink)
	}

	ColumnLayout {
		id: column
		anchors.centerIn: parent
		Label {
			Layout.alignment: Qt.AlignHCenter
			text: cardTitle
			style: "title"
		}
		Label {
			Layout.alignment: Qt.AlignHCenter
			text: cardDescription
		}
	}
}
