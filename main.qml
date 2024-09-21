import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("DHA")
    Pane {
        anchors.fill: parent
        // Панель с параметрами
        Pane {
            height: parent.height
            width: 400
            ColumnLayout{
                id: columnParametrs
                width: parent.width - 10
                height: parent.height
                spacing: 30
                Connections {
                    target: backend
                    function onConfigChanged(Thetas)
                    {
                        listModelThetas.clear();
                        listModelThetas.append(Thetas);
                    }
                }
                ListModel {
                    id: listModelThetas
                }
                ListView {
                    id: listViewThetas
                    model: listModelThetas
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    boundsBehavior: ListView.StopAtBounds
                    spacing: 30
                    currentIndex: -1
                    delegate: ItemDelegate {
                        width: listViewThetas.width
                        height: 150
                        background: Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                        }
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 10
                            Text {
                                text: "Сечение " + (index+1)
                                font.bold: Font.Bold
                                font.pointSize: 16
                                Layout.fillWidth: true
                            }
                            Item {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Text {
                                    text: "θ:"
                                    font.pointSize: 16
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                }
                                Text {
                                    font.pointSize: 16
                                    text: sliderJoint.value
                                    anchors.centerIn: parent
                                }
                            }
                            Slider {
                                id: sliderJoint
                                from: -180
                                to: 180
                                value: model.value
                                stepSize: 1
                                Layout.fillWidth: true
                                onValueChanged:
                                {
                                    var tmp = listModelThetas.get(index)
                                    tmp.value = value
                                    listModelThetas.set(index, tmp)
                                    var list = [];
                                    for(var i = 0; i < listModelThetas.count; ++i)
                                    {
                                        list.push(listModelThetas.get(i).value)
                                    }
                                    backend.orderToCalc(list)
                                }
                            }
                        }
                    }
                }
                Button{
                    Layout.fillWidth: true
                    highlighted: true
                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/dha_qml/icons/upload.svg"
                    }
                    onClicked: {
                        fileDialogDHConfig.open()
                    }
                }
            }
        }
        // Панель с отображением координат
        Pane {
            anchors.right: parent.right
            width: window.width - 400
            height: parent.height
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 50
                spacing: 50
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "x: " + backend.xyz[0]
                    font.pointSize: 32
                    font.bold: Font.Bold
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "y: " + backend.xyz[1]
                    font.pointSize: 32
                    font.bold: Font.Bold
                }
                Text {
                    Layout.alignment: Qt.AlignHCenter
                    text: "z: " + backend.xyz[2]
                    font.pointSize: 32
                    font.bold: Font.Bold
                }
            }
        }
    }
    FileDialog {
        id: fileDialogDHConfig
        nameFilters: "JSON (*.json)"
        onAccepted: {
            backend.loadConfig(selectedFile);
        }
    }
}
