import QtQuick
import QtQuick.Controls

Rectangle {
	function open(text) {
		snackLabel.text = text;
		offset = 0;
		snackTimer.start();
	}

	width: snackLabel.implicitWidth
	height: snackLabel.implicitHeight
	property int offset: height
	anchors.horizontalCenter: parent.horizontalCenter
	anchors.bottom: parent.bottom
	anchors.margins: -offset
	color: Material.background
	Label {
		id: snackLabel
		text: ""
	}
	Timer {
		id: snackTimer
		interval: 2000
		onTriggered: offset = height;
	}
	Behavior on offset {
		NumberAnimation {
			easing.type: Easing.OutCubic
		}
	}
}
