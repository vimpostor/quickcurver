import QtQuick 2.7
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Page {
    backgroundColor: "#31363b"
    focus: false
    canGoBack: false
    onGoBack: pageStack.push(this) //as of now, canGoBack = false does not work, this disables it manually
    Sidebar {
        mode: "right"
        width: parent.width - 1000
        ListItem.Subheader {
            id: scoreListHeader
            text: "Players"
            anchors.top: parent.top

        }
        Label {
            text: "Here you will see the score later!"
        }
    }
}
