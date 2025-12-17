import QtQuick
import QtQuick.Controls
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 800
    title: "CPPProjekt – mapa"
    property int currentCellSize: 40

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: menuScreen
    }

    MessageDialog {
        id: messageDialog
        title: "Chyba vstupu"
        text: "Zadejte platná čísla (Mapa: 5-20)."
    }

    MessageDialog {
        id: gameOverDialog
        title: "Konec hry"
        onAccepted: {
            stack.pop()
        }
    }

    Connections {
        target: gameController

        function onGameOver(msg) {
            gameOverDialog.text = msg
            gameOverDialog.open()
        }
    }

    // --- MENU ---
    Component {
        id: menuScreen
        Rectangle {
            color: "#1f1f1f"
            border.color: "#00173b"
            border.width: 20
            radius: 15

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 30

                Text {
                    text: "Tanky a lodě"
                    font.pixelSize: 36
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: "Zadej velikost mapy (5–20)"
                    font.pixelSize: 20
                    color: "#bdc3c7"
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: mapSizeInput
                    width: 200
                    height: 50
                    font.pixelSize: 24
                    text: "10"
                    inputMethodHints: Qt.ImhDigitsOnly
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                    validator: IntValidator { bottom: 5; top: 20 }
                }

                Text {
                    text: "Zadej velikost políčka"
                    font.pixelSize: 20
                    color: "#bdc3c7"
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: cellSizeInput
                    width: 200
                    height: 50
                    font.pixelSize: 24
                    text: "50"
                    inputMethodHints: Qt.ImhDigitsOnly
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                    validator: IntValidator { bottom: 20; top: 100 }
                }

                RowLayout {
                    spacing: 20

                    Button {
                        text: "Začít hru"
                        font.pixelSize: 24
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 60
                        background: Rectangle {
                            color: "#27ae60"
                            radius: 10
                        }

                        onClicked: {
                            var mapSize = parseInt(mapSizeInput.text)
                            root.currentCellSize = parseInt(cellSizeInput.text)

                            if (mapSize >= 5 && mapSize <= 20) {
                                mapModel.generate(mapSize)
                                stack.push(gameScreen)
                            } else {
                                messageDialog.open()
                            }
                        }
                    }

                    Button {
                        text: "Konec"
                        font.pixelSize: 24
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 60
                        background: Rectangle {
                            color: "#e74c3c"
                            radius: 10
                        }

                        onClicked: {
                            Qt.quit()
                        }
                    }
                }
            }
        }
    }

    // --- HRA ---
    Component {
        id: gameScreen
        Item {
            Rectangle {
                id: topBar
                width: parent.width
                height: 60
                color: "#2c3e50"
                z: 100

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    Text {
                        text: gameController.statusMessage
                        color: "white"
                        font.pixelSize: 20
                        font.bold: true
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Rectangle {
                        width: 30; height: 30
                        radius: 15
                        color: gameController.currentPlayer === 1 ? "#3498db" : "#e74c3c"
                        border.color: "white"
                        border.width: 2
                    }
                }
            }

            Column {
                anchors.top: topBar.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                Item {
                    id: mapArea
                    width: grid.width
                    height: grid.height
                    anchors.centerIn: parent

                    GridView {
                        id: grid
                        interactive: false
                        model: mapModel
                        cellWidth: root.currentCellSize
                        cellHeight: root.currentCellSize
                        width: mapModel.size * cellWidth
                        height: mapModel.size * cellHeight

                        delegate: Rectangle {
                            width: grid.cellWidth
                            height: grid.cellHeight
                            border.width: 1
                            border.color: rgba(1,1,1,0.1)
                            color: terrain === 1 ? "dodgerblue" : "lightgreen"

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor

                                onClicked: {
                                    var col = index % mapModel.size
                                    var row = Math.floor(index / mapModel.size)
                                    console.log("Klik: [" + col + ", " + row + "]")
                                    gameController.handleTileClick(col, row)
                                }
                                onEntered: {
                                    parent.border.color = "yellow"
                                    parent.border.width = 3
                                }
                                onExited: {
                                    parent.border.width = 1
                                    parent.border.color = "rgba(0,0,0,0.1)"
                                }
                            }
                        }

                        Repeater {
                            model: unitModel
                            Rectangle {
                                id: unitRect
                                width: grid.cellWidth * 0.8
                                height: grid.cellHeight * 0.8
                                radius: width / 2

                                x: grid.x + ux * grid.cellWidth + (grid.cellWidth * 0.1)
                                y: grid.y + uy * grid.cellHeight + (grid.cellHeight * 0.1)
                                z: 10

                                color: ownerId === 1 ? "#3498db" : "#e74c3c"

                                border.color: "white"
                                border.width: 2

                                Text {
                                    anchors.centerIn: parent
                                    text: health
                                    color: "white"
                                    font.bold: true
                                    font.pixelSize: parent.width * 0.4
                                }
                            }
                        }
                    }
                }
            }
            Button {
                text: "X"
                font.pixelSize: 20
                width: 50
                height: 50
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 30
                background: Rectangle {
                    color: "#e74c3c"
                    radius: 25
                    opacity: 0.8
                }
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: {
                    stack.pop()
                }
            }
        }
    }
}
