import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 600
    height: 200
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
                text: "Open Model"
            }
            ToolButton {
                text: "Open Image"
            }
            ToolButton {
                text: "Open Labels"
            }
            ToolButton {
                text: "Exit"
                onClicked: Qt.quit();
            }
        }
    }
    footer: Label {
        text: "Result: " + logic.parameter4
    }
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Form {
            id: classifierForm
        }
    }
}
