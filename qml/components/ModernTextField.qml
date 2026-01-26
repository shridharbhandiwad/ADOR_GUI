import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: modernTextField
    
    // Theme properties
    property color primaryColor: ThemeManager.primaryColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color borderColor: ThemeManager.borderColor
    property color borderFocus: ThemeManager.borderFocus
    property color backgroundColor: ThemeManager.inputBackground
    property color cardBackground: ThemeManager.cardBackground
    property string fontFamily: ThemeManager.fontFamily
    
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
            font.pixelSize: 15
            font.weight: Font.Medium
            font.family: modernTextField.fontFamily
            color: textSecondary
            visible: modernTextField.label.length > 0
            
            Behavior on color { ColorAnimation { duration: 200 } }
        }
        
        // Text field
        TextField {
            id: textField
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            
            font.pixelSize: 16
            font.family: modernTextField.fontFamily
            color: textPrimary
            placeholderText: modernTextField.placeholderText
            placeholderTextColor: ThemeManager.textMuted
            
            background: Rectangle {
                radius: 10
                color: textField.activeFocus ? cardBackground : backgroundColor
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
