import QtQuick 2.7
import QtQuick.Layouts 1.1
import Material 0.3
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import QtQuick.Controls.Material 2.0

TabbedPage {
    id: settings
    title: "Settings"

    onGoBack: {
        settings.forcePop();
    }
    Tab {
        title: "General"
        ColumnLayout {
            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumHeight: settingsGrid.height + dp(80)
                Layout.minimumWidth: settingsGrid.width + dp(80)
                color: "#333"

                GridLayout {
                    id: settingsGrid
                    anchors.centerIn: parent
                    rowSpacing: dp(20)
                    columnSpacing: dp(10)
                    columns: 2

                    Label {
                        text: "Round timeout"
                        color: Theme.dark.textColor
                    }
                    Slider {
                        value: 2
                        tickmarksEnabled: true
                        stepSize: 1
                        numericValueLabel: true
                        minimumValue: 0
                        maximumValue: 10
                        darkBackground: true
                        onValueChanged: game.setRoundTimeout(value)
                    }
                    Label {
                        text: "Base speed"
                        color: Theme.dark.textColor
                    }
                    Slider {
                        value: 128
                        tickmarksEnabled: true
                        stepSize: 32
                        numericValueLabel: true
                        minimumValue: 64
                        maximumValue: 512
                        darkBackground: true
                        onValueChanged: game.setBaseSpeed(value)
                    }
                }
            }
        }
    }
    Tab {
        property var itemNames: ["Faster Item", "Cleaninstall"]
        property var defaultValues: [3, 1]
        title: "Item Spawn Probabilities"
        ColumnLayout {
            spacing: 0
            Repeater {
                model: 2
                Card {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: dp(10)

                    GridLayout {
                        anchors.centerIn: parent
                        rowSpacing: dp(20)
                        columnSpacing: dp(10)
                        columns: 2

                        Label {
                            text: itemNames[index]
                        }

                        Slider {
                            Layout.alignment: Qt.AlignCenter
                            tickmarksEnabled: true
                            numericValueLabel: true
                            minimumValue: 0
                            maximumValue: 10
                            value: defaultValues[index]
                            onValueChanged: game.setItemPriority(index, value)
                        }
                    }
                }
            }
        }
    }
}
