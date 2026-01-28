import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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
            gameController.resetGame()
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

                            if (mapSize >= 10 && mapSize <= 30) {
                                mapModel.generate(mapSize)
                                gameController.startGame()
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
                id: topPanel
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
                    Text {
                        text: "P1: " + gameController.p1Gold + "$ | P2: " + gameController.p2Gold + "$"
                        color: "gold"
                        font.pixelSize: 18
                        font.bold: true
                    }
                }
            }

            Flickable {
                anchors.top: topPanel.bottom
                anchors.bottom: bottomPanel.top
                anchors.left: parent.left
                anchors.right: shopPanel.left
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
                            width: currentCellSize
                            height: currentCellSize
                            color: terrain === 1 ? "#1a3c5e" : (terrain === 2 ? "#555" : "#2d5a27")

                            Text {
                                text: "^"
                                visible: terrain === 2
                                anchors.centerIn: parent
                                color: "#888";
                                font.pixelSize: 20
                            }
                            Rectangle {
                                anchors.fill: parent
                                z: 5
                                color: "transparent"
                                border.color: highlight === 1 ? "#00ff00" :
                                              highlight === 2 ? "#ff0000" : "transparent"
                                border.width: highlight > 0 ? 3 : 0
                            }

                            TapHandler {
                                onTapped: gameController.handleTileClick(index % mapModel.size, Math.floor(index / mapModel.size))
                            }
                        }
                    }
                }

                Repeater {
                    model: unitModel
                    delegate: Item {
                        x: ux * (currentCellSize + 1)
                        y: uy * (currentCellSize + 1)
                        width: currentCellSize
                        height: currentCellSize

                        Rectangle {
                            anchors.centerIn: parent
                            width: currentCellSize * 0.8
                            height: currentCellSize * 0.8
                            radius: type === 4 || type === 5 ? 0 : width/2

                            color: ownerId === 1 ? "#ff3333" : "#3333ff"
                            border.color: gameController.selectedUnitIndex === index ? "white" : "black"
                            border.width: 3

                            Column {
                                anchors.centerIn: parent
                                Text {
                                    text: getUnitShort(type)
                                    color: "white"
                                    font.bold: true
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                                Text {
                                    text: health
                                    color: "white"
                                    font.pixelSize: 10
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                            }
                        }
                    }
                }
            }

            function getUnitShort(t) {
                if (t === 0) return "VOJ";
                if (t === 1) return "SNP";
                if (t === 2) return "TNK";
                if (t === 3) return "LOD";
                if (t === 4) return "BASE";
                if (t === 5) return "DUL";
                return "?";
            }

            Rectangle {
                id: shopPanel
                anchors.right: parent.right
                anchors.top: topPanel.bottom
                anchors.bottom: bottomPanel.top
                width: 200
                color: "#151515"
                visible: gameController.isShopOpen || !gameController.isPlacementPhase

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Text {
                        text: "OBCHOD"
                        color: "white"
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Repeater {
                        model: [
                            {name: "Voják (100)", type: 0},
                            {name: "Sniper (200)", type: 1},
                            {name: "Tank (400)", type: 2},
                            {name: "Loď (350)", type: 3},
                            {name: "Důl (300)", type: 5}
                        ]

                        delegate: Button {
                            id: shopButton
                            text: modelData.name
                            Layout.fillWidth: true

                            enabled: (gameController.currentPlayer === 1 ? gameController.p1Gold : gameController.p2Gold) >=
                                     (modelData.type === 0 ? 100 : modelData.type === 1 ? 200 : modelData.type === 2 ? 400 : modelData.type === 3 ? 350 : 300)

                            onClicked: gameController.selectUnitToBuy(modelData.type)

                            background: Rectangle {
                                implicitWidth: 100
                                implicitHeight: 40
                                color: shopButton.down ? "#333" : (shopButton.enabled ? "#222" : "#111")
                                border.color: (gameController.selectedBuyTypeInt === modelData.type && gameController.isBuyingActive) ? "yellow" : "#444"
                                border.width: (gameController.selectedBuyTypeInt === modelData.type && gameController.isBuyingActive) ? 3 : 1
                                radius: 4
                            }

                            contentItem: Text {
                                text: shopButton.text
                                font: shopButton.font
                                color: shopButton.enabled ? "white" : "#666"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }

                    Button {
                        text: gameController.isPlacementPhase ? "HOTOVO / DALŠÍ" : "ZRUŠIT VÝBĚR"
                        Layout.fillWidth: true
                        onClicked: gameController.cancelBuy()
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
                    text: "X pro návrat do menu"
                    color: "#555"
                }
            }

            Button {
                text: "X"
                anchors.right: parent.right
                anchors.top: parent.top
                width: 30
                height: 30
                onClicked: {
                    gameController.resetGame()
                    stack.pop()
                }
                background: Rectangle { color: "red"; radius: 10 }
                anchors.margins: 10
            }

        }
    }
}
