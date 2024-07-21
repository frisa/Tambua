import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 720
    height: 520
    title: "Classifier"
    menuBar: MenuBar {
        Menu {
            title: "&File"
            Action { text: "&New" }
            Action { text: "&Exit"; onTriggered: Qt.quit();
            }
        }
        Menu {
            title: "&Command"
        }
        Menu {
            title: "&Help"
        }
    }
    header: ToolBar {
        RowLayout {
            ToolButton {
                text: "Load Model"
            }
            ToolButton {
                text: "Open Image"
            }
            ToolButton {
                text: "Open Labels"
            }
            ToolButton {
                text: "Classify Image"
            }
            ToolButton {
                text: "Exit"
                onClicked: Qt.quit();
            }
        }
    }
    footer: Label {
        text: "Location: " + sensorPage.sensorLocation.latitude + ", " + sensorPage.sensorLocation.longitude
    }
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Sensor {
            id: sensorPage
        }
    }
}
