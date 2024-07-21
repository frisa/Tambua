import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

GridLayout {
    columns: 3
    Label {
        Layout.row: 0
        Layout.column: 0 // Corrected from Layout.Column to Layout.column
        text: "Image:"
    }
    TextField {
        id: imagePathField
        placeholderText: "Enter Bitmap Path to Classify"
        text: logic.parameter1
        Layout.row: 0
        Layout.column: 1
        Layout.fillWidth: true
    }
    Label {
        Layout.row: 1
        Layout.column: 0
        text: "Model:"
    }
    TextField {
        id: modelPathField
        placeholderText: "Enter Model Path"
        text: logic.parameter2
        Layout.row: 1
        Layout.column: 1
        Layout.fillWidth: true
    }
    Label {
        Layout.row: 2
        Layout.column: 0
        text: "Name:"
    }
    TextField {
        id: lablesPathField
        placeholderText: "Enter Labels Path"
        text: logic.parameter3
        Layout.row: 2
        Layout.column: 1
        Layout.fillWidth: true
    }
    Button {
        text: "Classify"
        width: parent.width
        Layout.row: 3
        Layout.column: 0
        Layout.fillWidth: true
        Layout.columnSpan: 2
        onClicked: {
            console.log("Classify: " + imagePathField.text + ", " + modelPathField.text + ", " + lablesPathField.text);
            logic.DoClassificationCmd();
        }
    }
    Image {
        id: image
        source: "file://" + imagePathField.text
        Layout.row: 0
        Layout.column: 2
        Layout.rowSpan: 3
        Layout.preferredHeight: 150
        Layout.preferredWidth: 150
        width: 100
        fillMode: Image.PreserveAspectFit
    }
}
