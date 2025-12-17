import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 900
    height: 700
    title: "Mapa"

    Column {
        anchors.fill: parent
        spacing: 10
        padding: 10

        Row {
            spacing: 10

            TextField {
                id: sizeInput
                width: 140
                placeholderText: "velikost (např. 10)"
                text: "10"
                inputMethodHints: Qt.ImhDigitsOnly
            }

            Button {
                text: "Vytvořit mapu"
                onClicked: mapModel.generate(parseInt(sizeInput.text))
            }

            Label { text: "Aktuálně: " + mapModel.size + "×" + mapModel.size }
        }

        GridView {
            id: grid
            model: mapModel
            cellWidth: 24
            cellHeight: 24

            width: mapModel.size * cellWidth
            height: parent.height - 80
            anchors.horizontalCenter: parent.horizontalCenter

            delegate: Rectangle {
                width: grid.cellWidth
                height: grid.cellHeight
                border.width: 1

                color: {
                    if (terrain === 1) return "dodgerblue"
                    if (terrain === 2) return "gray"
                    return "lightgreen"
                }
            }
        }
    }
}
