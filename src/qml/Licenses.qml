import QtQuick 2.7
import QtQuick.Controls 2.1
import Fluid.Controls 1.0
import Fluid.Material 1.0
import QtQuick.Layouts 1.1

Flickable {
	anchors.fill: parent
	clip: true
	contentHeight: licensesLayout.height
	ColumnLayout {
		id: licensesLayout
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.margins: 16
		spacing: 10
		TitleLabel {
			text: "Quick Curver uses the following software:"
		}
		LicenseCard {
			cardTitle: "Qt"
			cardDescription: "C++ Framework. Licensed under GNU GPL v. 3.0"
			cardLink: "http://code.qt.io"
		}
		LicenseCard {
			cardTitle: "Fluid"
			cardDescription: "Beatiful Material design library. Licensed under MPL v. 2.0"
			cardLink: "https://github.com/lirios/fluid"
		}
		LicenseCard {
			cardTitle: "Google Material Icons"
			cardDescription: "Material Icons. Licensed under Apache License Version 2.0"
			cardLink: "https://design.google.com/icons/"
		}
		LicenseCard {
			cardTitle: "Quick Curver"
			cardDescription: "Quick Curver is free software, licensed under GNU GPL v. 3.0"
			cardLink: "https://github.com/magnus-gross/quickcurver"
		}
	}

}
