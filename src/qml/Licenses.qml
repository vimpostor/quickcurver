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
			cardDescription: "Various different licenses, click for more information"
			cardLink: "http://doc.qt.io/qt-5/licensing.html"
		}
		LicenseCard {
			cardTitle: "Qt Creator"
			cardDescription: "Licensed under GNU LGPL"
			cardLink: "http://doc.qt.io/qt-5/opensourcelicense.html"
		}
		LicenseCard {
			cardTitle: "QML Material"
			cardDescription: "Beatiful Material library\nLicensed under GNU LGPL"
			cardLink: "https://github.com/papyros/qml-material/tree/master"
		}
		LicenseCard {
			cardTitle: "Google Material Icons"
			cardDescription: "Material Icons, kindly open sourced by Google\nLicensed under Apache License Version 2.0"
			cardLink: "https://design.google.com/icons/"
		}
		LicenseCard {
			cardTitle: "Quick Curver"
			cardDescription: "This software is licensed under GNU GPL v3.0"
			cardLink: "https://github.com/Magnus2552/quickcurver"
		}
	}
}
