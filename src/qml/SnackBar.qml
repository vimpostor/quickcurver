import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
	id: snack
	property int duration: 2000
	function open(text) {
		snackLabel.text = text;
		offset = 0;
		snackTimer.start();
	}
	function close() {
		offset = height;
	}
	signal accepted()

	width: snackLayout.implicitWidth
	height: snackLayout.implicitHeight
	property int offset: height
	anchors.horizontalCenter: parent.horizontalCenter
	anchors.bottom: parent.bottom
	anchors.margins: -offset
	RowLayout {
		id: snackLayout
		Label {
			id: snackLabel
			text: ""
		}
		DialogButtonBox {
			standardButtons: DialogButtonBox.Ok
			onAccepted: {
				snack.accepted();
				close();
			}
		}
	}
	Timer {
		id: snackTimer
		interval: duration
		onTriggered: close();
	}
	Behavior on offset {
		NumberAnimation {
			easing.type: Easing.OutCubic
		}
	}
}
