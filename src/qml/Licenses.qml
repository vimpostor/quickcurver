import QtQuick 2.7
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Layouts 1.1

Flickable {
	anchors.fill: parent
	ColumnLayout {
		spacing: dp(10)
		anchors.fill: parent
		anchors.margins: dp(70)
		LicenseCard {
			cardTitle: "Qt"
			cardDescription: "C++ Framework. Licensed under GNU GPL v. 3.0"
			cardLink: "http://code.qt.io"
		}
		LicenseCard {
			cardTitle: "QML Material"
			cardDescription: "Beatiful Material design library. Licensed under GNU LGPL v. 2.1"
			cardLink: "https://github.com/papyros/qml-material"
		}
		LicenseCard {
			cardTitle: "Google Material Icons"
			cardDescription: "Material Icons. Licensed under Apache License Version 2.0"
			cardLink: "https://design.google.com/icons/"
		}
		LicenseCard {
			cardTitle: "Quick Curver"
			cardDescription: "Quick Curver is free software, licensed under GNU GPL v. 3.0"
			cardLink: "https://github.com/Magnus2552/quickcurver"
		}
	}
}
