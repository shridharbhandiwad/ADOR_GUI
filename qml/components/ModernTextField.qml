import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Item {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Modern Text Field Component - Elegant Input with Label
    // ═══════════════════════════════════════════════════════════════════════════
    
    property alias text: textField.text
    property alias placeholderText: textField.placeholderText
    property alias echoMode: textField.echoMode
    property alias readOnly: textField.readOnly
    property alias validator: textField.validator
    property string label: ""
    property string helperText: ""
    property string errorText: ""
    property string iconText: ""
    property string suffixText: ""
    property bool hasError: errorText !== ""
    property color accentColor: Theme.primaryColor
    
    implicitWidth: 200
    implicitHeight: labelText.visible ? labelText.height + Theme.spacingXS + inputContainer.height + 
                   (helperContainer.visible ? Theme.spacingXS + helperContainer.height : 0) :
                   inputContainer.height + (helperContainer.visible ? Theme.spacingXS + helperContainer.height : 0)
    
    ColumnLayout {
        anchors.fill: parent
        spacing: Theme.spacingXS
        
        // Label
        Text {
            id: labelText
            text: label
            font.pixelSize: Theme.fontSizeS
            font.weight: Font.Medium
            color: hasError ? Theme.errorColor : 
                   textField.activeFocus ? accentColor : Theme.textSecondary
            visible: label !== ""
            
            Behavior on color {
                ColorAnimation { duration: Theme.animationFast }
            }
        }
        
        // Input container
        Rectangle {
            id: inputContainer
            Layout.fillWidth: true
            Layout.preferredHeight: Theme.inputHeight
            radius: Theme.radiusM
            color: textField.activeFocus ? Theme.surfaceColor : Theme.surfaceVariant
            border.width: textField.activeFocus ? 2 : 1
            border.color: hasError ? Theme.errorColor :
                         textField.activeFocus ? accentColor : Theme.borderColor
            
            Behavior on color {
                ColorAnimation { duration: Theme.animationFast }
            }
            
            Behavior on border.color {
                ColorAnimation { duration: Theme.animationFast }
            }
            
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: Theme.spacingM
                anchors.rightMargin: Theme.spacingM
                spacing: Theme.spacingS
                
                // Prefix icon
                Text {
                    text: iconText
                    font.pixelSize: Theme.iconSizeM
                    font.family: "Segoe Fluent Icons, Segoe MDL2 Assets"
                    color: textField.activeFocus ? accentColor : Theme.textTertiary
                    visible: iconText !== ""
                    Layout.alignment: Qt.AlignVCenter
                    
                    Behavior on color {
                        ColorAnimation { duration: Theme.animationFast }
                    }
                }
                
                // Text input
                TextField {
                    id: textField
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    font.pixelSize: Theme.fontSizeM
                    color: Theme.textPrimary
                    placeholderTextColor: Theme.textTertiary
                    verticalAlignment: Text.AlignVCenter
                    selectByMouse: true
                    
                    background: Rectangle {
                        color: "transparent"
                    }
                }
                
                // Suffix text/icon
                Text {
                    text: suffixText
                    font.pixelSize: Theme.fontSizeS
                    color: Theme.textTertiary
                    visible: suffixText !== ""
                    Layout.alignment: Qt.AlignVCenter
                }
            }
        }
        
        // Helper/Error text
        RowLayout {
            id: helperContainer
            Layout.fillWidth: true
            visible: helperText !== "" || hasError
            spacing: Theme.spacingXS
            
            Text {
                text: hasError ? "⚠" : "ℹ"
                font.pixelSize: Theme.fontSizeXS
                color: hasError ? Theme.errorColor : Theme.textTertiary
            }
            
            Text {
                text: hasError ? errorText : helperText
                font.pixelSize: Theme.fontSizeXS
                color: hasError ? Theme.errorColor : Theme.textTertiary
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
        }
    }
}
