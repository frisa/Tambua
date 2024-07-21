import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

GridLayout {

    columns: 2
    Label {
        text: "Image:"
    }
    TextField {
        id: imagePathField
        placeholderText: "Enter Bitmap Path to Classify"
        text: logic.parameter1
        Layout.fillWidth: true
    }
    Label {
        text: "Model:"
    }
    TextField {
        id: modelPathField
        placeholderText: "Enter Model Path"
        text: logic.parameter2
        Layout.fillWidth: true
    }
    Label {
        text: "Name:"
    }
    TextField {
        id: lablesPathField
        placeholderText: "Enter Labels Path"
        text: logic.parameter3
        Layout.fillWidth: true
    }
    Button {
        text: "Classify"
        width: parent.width
        Layout.columnSpan: 2
        onClicked: {
            console.log("Classify: " + imagePathField.text + ", " + modelPathField.text + ", " + lablesPathField.text);
            logic.DoClassificationCmd();
        }
        Layout.fillWidth: true
    }
}
