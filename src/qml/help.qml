import QtQuick 2.7
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Layouts 1.1

Page {
	Card {
		anchors.fill: parent
		anchors.margins: dp(32)
		GridLayout {
			anchors.fill: parent
			anchors.margins: dp(16)
			columnSpacing: dp(32)
			rowSpacing: dp(32)
			columns: 3
			LicenseCard {
				cardTitle: "Help"
				cardDescription: "This help page has yet to be written..."
			}
		}
	}
}
