import QtQuick 2.7
import Fluid.Controls 1.0
import QtQuick.Layouts 1.1

Page {
	Card {
		anchors.fill: parent
		anchors.margins: 32
		GridLayout {
			anchors.fill: parent
			anchors.margins: 16
			columnSpacing: 32
			rowSpacing: 32
			columns: 3
			LicenseCard {
				cardTitle: "Help"
				cardDescription: "This help page has yet to be written..."
			}
		}
	}
}
