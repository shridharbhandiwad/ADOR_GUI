import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: modernTextField
    
    // Theme properties
    property color primaryColor: "#3b82f6"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color borderColor: "#e2e8f0"
    property color borderFocus: "#3b82f6"
    property color backgroundColor: "#f8fafc"
    property string fontFamily: "Segoe UI"
    
    // Field properties
    property string label: ""
    property string placeholderText: ""
    property alias text: textField.text
    property alias readOnly: textField.readOnly
    property alias validator: textField.validator
    property alias inputMethodHints: textField.inputMethodHints
    property alias echoMode: textField.echoMode
    
    implicitHeight: label.length > 0 ? 72 : 48
    implicitWidth: 200
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 8
        
        // Label
        Text {
            text: modernTextField.label
            font.pixelSize: 13
            font.weight: Font.Medium
            font.family: modernTextField.fontFamily
            color: textSecondary
            visible: modernTextField.label.length > 0
        }
        
        // Text field
        TextField {
            id: textField
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            
            font.pixelSize: 14
            font.family: modernTextField.fontFamily
            color: textPrimary
            placeholderText: modernTextField.placeholderText
            placeholderTextColor: "#94a3b8"
            
            background: Rectangle {
                radius: 10
                color: textField.activeFocus ? "#ffffff" : backgroundColor
                border.color: textField.activeFocus ? borderFocus : borderColor
                border.width: textField.activeFocus ? 2 : 1
                
                Behavior on border.color {
                    ColorAnimation { duration: 150 }
                }
                
                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }
            
            leftPadding: 16
            rightPadding: 16
            verticalAlignment: Text.AlignVCenter
            
            selectByMouse: true
        }
    }
}
