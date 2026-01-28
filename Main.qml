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
    width: 1280
    height: 900
    title: "PCP Projekt C&C hra"
    property int currentCellSize: 60

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: menuScreen
    }

    MessageDialog {
        id: messageDialog
        title: "Info"
        text: "Zadejte velikost mapy (10-30)."
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

    Rectangle {
        id: playerBorder
        anchors.fill: parent
        z: 999
        color: "transparent"
        border.width: 8
        border.color: gameController.currentPlayer === 1 ? "#ff3333" : "#3333ff"
        visible: stack.depth > 1
    }

    Component {
        id: menuScreen
        Rectangle {
            color: "grey"
            border.width: 20
            radius: 15

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 30

                Text {
                    text: "C&C - Tanky a lodě"
                    font.pixelSize: 48
                    color: "white"
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: mapSizeInput
                    placeholderText: "Velikost (10-30)"
                    color: "white"
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    spacing: 20

                    Button {
                        text: "Začít hru"
                        font.pixelSize: 24
                        Layout.preferredWidth: 200
                        Layout.preferredHeight: 60
                        background: Rectangle {
                            color: "green"
                            radius: 10
                        }

                        onClicked: {
                            var mapSize = parseInt(mapSizeInput.text)

                            if (mapSize >= 5 && mapSize <= 30) {
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
                            color: "red"
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

    Component {
        id: gameScreen
        Item {
            Rectangle {
                id: topBar
                width: parent.width
                height: 50
                color: "grey"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    anchors.rightMargin: 60
                    Text {
                        text: gameController.statusMessage
                        color: "black"
                        font.pixelSize: 18
                        font.bold: true
                        Layout.fillWidth: true
                    }
                }
            }

            Flickable {
                anchors.top: topBar.bottom
                anchors.bottom: bottomPanel.top
                anchors.left: parent.left
                anchors.right: parent.right
                contentWidth: mapGrid.width
                contentHeight: mapGrid.height
                anchors.leftMargin: 100
                clip: true

                contentX: (contentWidth - width) / 2
                contentY: (contentHeight - height) / 2

                GridLayout {
                    id: mapGrid
                    columns: mapModel.size
                    columnSpacing: 1
                    rowSpacing: 1

                    Repeater {
                        model: mapModel
                        delegate: Rectangle {
                            width: currentCellSize; height: currentCellSize
                            color: terrain === 1 ? "#1a3c5e" : (terrain === 2 ? "#555" : "#2d5a27")

                            Text {
                                text: "^"
                                visible: terrain === 2
                                anchors.centerIn: parent
                                color: "#888"; font.pixelSize: 20
                            }
                        }
                    }
                }

                Repeater {
                    model: unitModel
                    //vojáci
                    Rectangle {
                        id: unitRect
                        width: grid.cellWidth * 0.8
                        height: grid.cellHeight * 0.8
                        radius: width / 2

                        x: grid.x + ux * grid.cellWidth + (grid.cellWidth * 0.1)
                        y: grid.y + uy * grid.cellHeight + (grid.cellHeight * 0.1)
                        z: 10

                        color: ownerId === 1 ? "blue" : "red"
                        border.width: index === gameController.selectedUnitIndex ? 6 : 0
                        border.color: "yellow"

                        Text{
                            text:"hp"
                            anchors.centerIn: parent.top
                            color: "white"
                            font.bold: true
                            font.pixelSize: parent.width * 0.4}
                        Text {
                            anchors.bottom: parent.bottom
                            text: health
                            color: "white"
                            font.bold: true
                            font.pixelSize: parent.width * 0.4
                        }
                    }
                }
            }
            Rectangle {
                id: bottomPanel
                anchors.bottom: parent.bottom
                width: parent.width
                height: 40
                color: "grey"
                Text {
                    anchors.centerIn: parent
                    text: "ESC / X pro menu"
                    color: "#555"
                }
            }

            Button {
                text: "X"
                anchors.right: parent.right
                anchors.top: parent.top
                width: 30
                height: 30
                onClicked: stack.pop()
                background: Rectangle {
                    color: "red";
                    radius: 10
                }
                anchors.margins: 10
            }

        }
    }
}
