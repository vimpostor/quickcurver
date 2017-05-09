import QtQuick 2.7
import QtQuick.Controls 2.1
import Fluid.Controls 1.0
import Fluid.Material 1.0
import QtQuick.Layouts 1.1

Card {
	property string cardTitle
	property string cardDescription
	property string cardLink
	Layout.fillWidth: true
	height: openExternallyButton.height + 16
	ActionButton {
		id: openExternallyButton
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.margins: 8
		iconName: "action/open_in_browser"
		onClicked: Qt.openUrlExternally(cardLink)
	}
	ColumnLayout {
		id: column
		anchors.left: parent.left
		anchors.right: openExternallyButton.left
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		TitleLabel {
			Layout.alignment: Qt.AlignHCenter
			text: cardTitle
		}
		BodyLabel {
			Layout.alignment: Qt.AlignHCenter
			text: cardDescription
		}
	}
}
