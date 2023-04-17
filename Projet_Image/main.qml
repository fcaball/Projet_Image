import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Make'saĩques")

    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }

        Rectangle {
            id: rectangle1
            x: 354
            y: 140
            width: 200
            height: 200
            color: "#c0bfbb"
        }
    }
}
