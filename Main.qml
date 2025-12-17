import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 900
    height: 700
    title: "CPPProjekt – mapa"

    Column {
        anchors.fill: parent
        spacing: 10
        padding: 10

        Row {
            spacing: 10

            TextField {
                id: sizeInput
                width: 160
                placeholderText: "velikost (např. 10)"
                text: "10"
                inputMethodHints: Qt.ImhDigitsOnly
            }

            Button {
                text: "Vytvořit mapu"
                onClicked: mapModel.generate(parseInt(sizeInput.text))
            }

            Label {
                text: "Mapa: " + mapModel.size + " × " + mapModel.size
            }
        }

        Item {
            id: mapArea
            width: grid.width
            height: grid.height
            anchors.horizontalCenter: parent.horizontalCenter

            // MAPA
            GridView {
                id: grid
                model: mapModel

                cellWidth: 24
                cellHeight: 24

                width: mapModel.size * cellWidth
                height: mapModel.size * cellHeight
                anchors.centerIn: parent

                delegate: Rectangle {
                    width: grid.cellWidth
                    height: grid.cellHeight
                    border.width: 1

                    color: {
                        if (terrain === 1) return "dodgerblue" // Water
                        if (terrain === 2) return "gray"       // Mountain
                        return "lightgreen"                    // Grass
                    }
                }
            }

            Repeater {
                model: unitModel

                Rectangle {
                    width: grid.cellWidth
                    height: grid.cellHeight
                    x: grid.x + ux * grid.cellWidth
                    y: grid.y + uy * grid.cellHeight
                    color: "yellow"
                    radius: 4
                    z: 10
                }
            }
        }
    }
}
