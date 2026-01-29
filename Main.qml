import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtMultimedia
ApplicationWindow {
    id: root
    visible: true
    width: 1280
    height: 900

    title: "!T&B - Tanky a lodě!"
    property int currentCellSize: 60

    MediaPlayer {
        id: backgroundMusic
        audioOutput: AudioOutput {}
        source: "qrc:Sounds/background-music.mp3"
        loops: MediaPlayer.Infinite
    }

    Component.onCompleted: {
        backgroundMusic.play()
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: menuScreen
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
            color: "#1c1c1c"

            Image {
                id: menuBackground
                source: "qrc:/Images/camoDigital.png"
                width: root.width - 30
                height: root.height - 30
                anchors.centerIn: parent
            }

            Rectangle {
                id: menuPanel
                color: "#1c1c1c"
                border.color: "#444"
                border.width: 2

                width: mainColumn.width + 60
                height: mainColumn.height + 60
                anchors.centerIn: parent

                ColumnLayout {
                    id: mainColumn
                    anchors.centerIn: parent
                    spacing: 25

                    Text {
                        id: textLabel
                        text: "T&B - Tanky a lodě"
                        font.pixelSize: 48
                        color: "white"
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    ButtonGroup { id: sizeGroup }

                    ColumnLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 10

                        Text {
                            text: "VELIKOST MAPY"
                            color: "white"
                            font.pixelSize: 20
                            font.bold: true
                            Layout.alignment: Qt.AlignHCenter
                        }

                        RowLayout {
                            spacing: 20
                            CheckBox {
                                id: smallMap
                                text: "Malá (15)"
                                checked: true
                                ButtonGroup.group: sizeGroup
                                property int val: 15
                                contentItem: Text { text: parent.text; color: "white"; leftPadding: 30; verticalAlignment: Text.AlignVCenter }
                            }
                            CheckBox {
                                id: mediumMap
                                text: "Střední (25)"
                                ButtonGroup.group: sizeGroup
                                property int val: 25
                                contentItem: Text { text: parent.text; color: "white"; leftPadding: 30; verticalAlignment: Text.AlignVCenter }
                            }
                            CheckBox {
                                id: largeMap
                                text: "Velká (35)"
                                ButtonGroup.group: sizeGroup
                                property int val: 35
                                contentItem: Text { text: parent.text; color: "white"; leftPadding: 30; verticalAlignment: Text.AlignVCenter }
                            }
                        }
                        Text {
                            text: "POČÁTEČNÍ ZLAŤÁKY"
                            color: "white"
                            font.pixelSize: 20
                            font.bold: true
                            Layout.alignment: Qt.AlignHCenter
                        }
                        TextField {
                                            id: coinCountTB
                                            placeholderText: "Celé čislo (0-99999)"
                                            text: "250"
                                            color: "white"
                                            Layout.alignment: Qt.AlignHCenter
                                            validator: IntValidator { bottom: 0; top: 99999 }

                                        }
                        CheckBox {
                            id: fullscreen
                            text: "Fullscreen?"

                            Layout.alignment: Qt.AlignHCenter


                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                leftPadding: 30
                                verticalAlignment: Text.AlignVCenter
                            }

                            onCheckedChanged: {
                                if (checked) {
                                    root.visibility = Window.FullScreen
                                } else {
                                    root.visibility = Window.Windowed
                                }
                            }
                        }

                        CheckBox {
                            id: music
                            text: "Zvuky?"

                            Layout.alignment: Qt.AlignHCenter
                            checked:true

                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                leftPadding: 30
                                verticalAlignment: Text.AlignVCenter

                            }

                            onCheckedChanged: {
                                if (checked) {
                                    backgroundMusic.play()
                                } else {
                                    backgroundMusic.stop()
                                }
                            }
                        }
                    }


                    RowLayout {
                        spacing: 20
                        Layout.alignment: Qt.AlignHCenter

                        Button {
                            text: "Začít hru"
                            font.bold: true
                            Layout.preferredWidth: 160
                            Layout.preferredHeight: 50
                            background: Rectangle { color: "green"; radius: 8 }
                            contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; font.pixelSize: 18 }
                            onClicked: {
                                var coinCount = parseInt(coinCountTB.text)
                                var selectedSize = sizeGroup.checkedButton.val
                                mapModel.generate(selectedSize)
                                gameController.startGame()
                                gameController.setStartingGold(coinCount)
                                stack.push(gameScreen)
                            }
                        }

                        Button {
                            text: "Konec"
                            font.bold: true
                            Layout.preferredWidth: 160
                            Layout.preferredHeight: 50
                            background: Rectangle { color: "red"; radius: 8 }
                            contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; font.pixelSize: 18 }
                            onClicked: Qt.quit()
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
                height: 65
                color: "#404040"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    anchors.rightMargin: 60

                    Text {
                        text: gameController.statusMessage
                        color:(gameController.currentPlayer=== 0 || gameController.currentPlayer=== 1) ? "#ff3333" : "#3333ff" //TADYY
                        font.pixelSize: 35
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

                            color: terrain === 1 ? "#1a3c5e" : (terrain === 2 ? "#555" : "#2d5a27") //jiz nepotrebne

                            Image {
                                id: terrainImage
                                source: terrain === 1 ? "qrc:/Images/water.png" : (terrain === 2 ? "qrc:/Images/rocks.png" : "qrc:/Images/grass.png")
                                width: currentCellSize
                                height: currentCellSize
                            }




                            Rectangle {
                                anchors.fill: parent
                                z: 5

                                color: {
                                    if (model.highlight === 1 || model.highlight === 3)
                                        return Qt.rgba(0, 1, 0, 0.3);

                                    if (model.highlight === 4)
                                        return Qt.rgba(1, 0, 1, 0.9);

                                    return "transparent";
                                }

                                border.color: {
                                    if (model.highlight === 2 || model.highlight === 3)
                                        return "#0000FF";
                                    return "transparent";
                                }

                                border.width: (model.highlight === 2 || model.highlight === 3) ? 3 : 0
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
                            radius: type === 4 || type === 5 ? 0 : width / 2

                            color: ownerId === 1 ? "#ff3333" : "#3333ff"
                            border.color: gameController.selectedUnitIndex === index ? "white" : "black"
                            border.width: 3

                            layer.enabled: true
                            layer.smooth: true




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
