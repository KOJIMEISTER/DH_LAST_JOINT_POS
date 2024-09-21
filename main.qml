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
        Pane {
            height: parent.height
            width: 400
            Flickable {
                id: flickableParametrs
                anchors.fill: parent
                anchors.margins: 10
                contentHeight: columnParametrs.height
                contentWidth: columnParametrs.width
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                ColumnLayout{
                    id: columnParametrs
                    width: flickableParametrs.width - 10
                    spacing: 30
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
        }
        Pane {
            anchors.right: parent.right
            width: window.width - 400
        }
    }
    FileDialog {
        id: fileDialogDHConfig
        onAccepted: backend.loadConfig(selectedFile);
    }
}
