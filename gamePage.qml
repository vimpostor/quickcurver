import QtQuick 2.7
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Page {
    backgroundColor: "#31363b"
    focus: false
    canGoBack: false
    onGoBack: pageStack.push(this) //as of now, canGoBack = false does not work, this disables it manually
    PageSidebar {
//        mode: "right"
        width: parent.width - 1000
        backgroundColor: Theme.backgroundColor
        Card {
            anchors.fill: parent
            anchors.margins: dp(32)
            ListItem.Subheader {
                id: scoreListHeader
                text: "Players"
                anchors.top: parent.top

            }
            View {
                anchors.top: scoreListHeader.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                ListView {
                    anchors.fill: parent
                    model: playerListModel
                    delegate: scoreListDelegate
                }
            }
            Component {
                id: scoreListDelegate
                ListItem.Standard {
                    text: ename
                    secondaryItem: Label {
                        anchors.verticalCenter: parent.verticalCenter
                        text: escore
                    }
                }
            }
        }


    }
}
