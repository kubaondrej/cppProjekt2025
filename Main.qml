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
    height: 700
    title: "CPPProjekt – mapa"
    property int currentCellSize: 40
    StackView {
            id: stack
            anchors.fill: parent
            initialItem: menuScreen
        }

    Component {
            id: menuScreen
            Rectangle {
                color: "#1f1f1f"
                border.color: "#00173b"
                border.width: 20
                radius:15


                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 30

                    Text {
                        text: "Tanky a lodě, zabili koně"
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
                        text: "15"
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
                        text: "45"
                        inputMethodHints: Qt.ImhDigitsOnly
                        horizontalAlignment: Text.AlignHCenter
                        Layout.alignment: Qt.AlignHCenter

                        validator: IntValidator { bottom: 5; top: 20 }
                    }

            RowLayout {
                    Button {
                        text: "Začít hru"
                        font.pixelSize: 24
                        width: 250
                        height: 60

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
                            } else
                            {

                                messageDialog.open()
                            }
                            console.log("Aktuální velikost mapModel.size:", cellSize)
                        }
                    }
                    Button
                    {
                        text: "Konec"
                        font.pixelSize: 24
                        width: 250
                        height: 60

                        background: Rectangle
                        {
                            color: "#e74c3c"
                            radius: 10
                        }

                        onClicked:
                        {
                            console.log("KONEC")
                            Qt.quit()
                        }
                    }
                }
            }
        }
    }
    Component {
            id: gameScreen

            Item {

                Column {
                    anchors.fill: parent
                    spacing: 10
                    padding: 10


                    Item {
                        id: mapArea
                        width: grid.width
                        height: grid.height
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.horizontalCenter

                        GridView {
                            id: grid
                            interactive: false //pry zakaze "scrollovani" mapy?
                            model: mapModel
                            cellWidth: root.currentCellSize
                            cellHeight: root.currentCellSize
                            width: mapModel.size * cellWidth
                            height: mapModel.size * cellHeight
                            anchors.centerIn: parent

                            delegate: Rectangle {
                                width: grid.cellWidth
                                height: grid.cellHeight
                                border.width: 1
                                color: {
                                    if (terrain === 1) return "dodgerblue"
                                    else if (terrain === 2) return "gray"
                                    return "lightgreen"
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
                            radius: 10
                        }

                        onClicked: {
                            console.log("MENU")
                            stack.pop()
                        }
                    }
            }
        }
    MessageDialog {
        id: messageDialog
        title: "Chyba vstupu"
        text: "Chyba vstupu"

    }
}
