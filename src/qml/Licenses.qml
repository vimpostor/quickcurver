import QtQuick 2.12
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

import Backend

Flickable {
	anchors.fill: parent
	contentHeight: Math.max(licensesLayout.implicitHeight, height) + 10
	AutomaticGrid {
		id: licensesLayout
		anchors.fill: parent
		anchors.margins: 8
		cellWidth: Backend.isMobile ? parent.width - 16 : 400
		cellHeight: 156
		model: ListModel {
			ListElement {
				cardTitle: "Qt"
				cardDescription: "C++ Framework. Licensed under GNU GPL v. 3.0"
				cardHomepage: "https://www.qt.io/"
				cardLicense: "http://code.qt.io/cgit/qt/qtbase.git/tree/LICENSE.GPL3"
				cardSource: "http://code.qt.io"
			}
			ListElement {
				cardTitle: "Google Material Icons"
				cardDescription: "Material Icons. Licensed under Apache License Version 2.0"
				cardHomepage: "https://design.google.com/icons/"
				cardLicense: "https://github.com/google/material-design-icons/blob/master/LICENSE"
				cardSource: "https://github.com/google/material-design-icons/"
			}
			ListElement {
				cardTitle: "Quick Curver"
				cardDescription: "Quick Curver is free software, licensed under GNU GPL v. 3.0"
				cardHomepage: "https://github.com/magnus-gross/quickcurver"
				cardLicense: "https://github.com/magnus-gross/quickcurver/blob/master/LICENSE.txt"
				cardSource: "https://github.com/magnus-gross/quickcurver"
			}
		}
		delegate: Item {
			width: licensesLayout.cellWidth
			height: licensesLayout.cellHeight
			Card {
				width: parent.width - 8
				height: parent.height - 8
				anchors.centerIn: parent
				Column {
					anchors.fill: parent
					anchors.margins: 8
					spacing: 16
					TitleLabel {
						text: cardTitle
					}
					BodyLabel {
						text: cardDescription
						wrapMode: Text.WordWrap
						width: parent.width
					}
				}
				RowLayout {
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.bottom: parent.bottom
					anchors.margins: 8
					spacing: 8
					Button {
						text: qsTr("Website")
						flat: true
						highlighted: true
						Layout.fillWidth: true
						onClicked: Qt.openUrlExternally(cardHomepage)
					}
					Button {
						text: qsTr("Source code")
						flat: true
						Layout.fillWidth: true
						onClicked: Qt.openUrlExternally(cardSource)
					}
					Button {
						text: qsTr("License")
						flat: true
						Layout.fillWidth: true
						onClicked: Qt.openUrlExternally(cardLicense)
					}
				}
			}
		}
	}
}
