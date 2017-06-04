import QtQuick 2.7
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import Fluid.Controls 1.0
import Fluid.Material 1.0
import Fluid.Core 1.0

Page {
	onGoBack: game.setTouchPoint(leftTP.x / touchArea.width, leftTP.y / touchArea.height, rightTP.x / touchArea.width, rightTP.y/touchArea.height)
	Card {
		id: touchDescriptionCard
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.margins: Units.smallSpacing
		anchors.right: parent.right
		height: touchPointDescription.implicitHeight + Units.smallSpacing
		ColumnLayout {
			id: touchPointDescription
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.margins: Units.smallSpacing
			spacing: Units.smallSpacing
			TitleLabel {
				text: "Choose the touchpoints"
			}
			BodyLabel {
				text: "Whenever you touch a point inside the gamefield, the touchpoint closes to that will trigger. The green point will trigger the left direction and the red point will trigger the right direction. Use the touchscreen with two fingers to move both points."
				wrapMode: Text.WordWrap
				Layout.fillWidth: true
			}
		}
	}
	Rectangle {
		anchors {
			top: touchDescriptionCard.bottom
			left: parent.left
			right: parent.right
			bottom: parent.bottom
			margins: Units.smallSpacing
		}
		color: Material.color(Material.BlueGrey, Material.Shade900)
		MultiPointTouchArea {
			id: touchArea
			anchors.fill: parent
			maximumTouchPoints: 2
			touchPoints: [
				TouchPoint { id: leftTP },
				TouchPoint { id: rightTP }
			]
		}
		Rectangle {
			width: Units.mediumSpacing
			height: width
			color: Material.color(Material.Red, Material.Shade500)
			x: leftTP.x
			y: leftTP.y
		}
		Rectangle {
			width: Units.mediumSpacing
			height: width
			color: Material.color(Material.Green, Material.Shade500)
			x: rightTP.x
			y: rightTP.y
		}
	}
}
