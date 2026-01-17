import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Item {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Modern ComboBox Component - Elegant Dropdown Select
    // ═══════════════════════════════════════════════════════════════════════════
    
    property alias model: comboBox.model
    property alias currentIndex: comboBox.currentIndex
    property alias currentText: comboBox.currentText
    property string label: ""
    property string helperText: ""
    property color accentColor: Theme.primaryColor
    
    implicitWidth: 200
    implicitHeight: labelText.visible ? labelText.height + Theme.spacingXS + comboContainer.height +
                   (helperText !== "" ? Theme.spacingXS + helperTextItem.height : 0) :
                   comboContainer.height + (helperText !== "" ? Theme.spacingXS + helperTextItem.height : 0)
    
    ColumnLayout {
        anchors.fill: parent
        spacing: Theme.spacingXS
        
        // Label
        Text {
            id: labelText
            text: label
            font.pixelSize: Theme.fontSizeS
            font.weight: Font.Medium
            color: comboBox.popup.visible ? accentColor : Theme.textSecondary
            visible: label !== ""
            
            Behavior on color {
                ColorAnimation { duration: Theme.animationFast }
            }
        }
        
        // ComboBox container
        Rectangle {
            id: comboContainer
            Layout.fillWidth: true
            Layout.preferredHeight: Theme.inputHeight
            radius: Theme.radiusM
            color: comboBox.popup.visible ? Theme.surfaceColor : Theme.surfaceVariant
            border.width: comboBox.popup.visible ? 2 : 1
            border.color: comboBox.popup.visible ? accentColor : Theme.borderColor
            
            Behavior on color {
                ColorAnimation { duration: Theme.animationFast }
            }
            
            Behavior on border.color {
                ColorAnimation { duration: Theme.animationFast }
            }
            
            ComboBox {
                id: comboBox
                anchors.fill: parent
                anchors.leftMargin: Theme.spacingM
                anchors.rightMargin: Theme.spacingM
                font.pixelSize: Theme.fontSizeM
                
                contentItem: Text {
                    leftPadding: 0
                    rightPadding: Theme.spacingXL
                    text: comboBox.displayText
                    font: comboBox.font
                    color: Theme.textPrimary
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                
                indicator: Text {
                    x: comboBox.width - width - Theme.spacingS
                    y: (comboBox.height - height) / 2
                    text: comboBox.popup.visible ? "▲" : "▼"
                    font.pixelSize: 10
                    color: comboBox.popup.visible ? accentColor : Theme.textSecondary
                    
                    Behavior on color {
                        ColorAnimation { duration: Theme.animationFast }
                    }
                }
                
                background: Rectangle {
                    color: "transparent"
                }
                
                popup: Popup {
                    y: comboBox.height + Theme.spacingXS
                    width: comboBox.width + Theme.spacingXL
                    implicitHeight: Math.min(contentItem.implicitHeight + Theme.spacingS * 2, 300)
                    padding: Theme.spacingS
                    
                    contentItem: ListView {
                        clip: true
                        implicitHeight: contentHeight
                        model: comboBox.popup.visible ? comboBox.delegateModel : null
                        currentIndex: comboBox.highlightedIndex
                        
                        ScrollIndicator.vertical: ScrollIndicator { }
                    }
                    
                    background: Rectangle {
                        color: Theme.surfaceColor
                        radius: Theme.radiusM
                        border.color: Theme.borderColor
                        border.width: 1
                        
                        // Shadow
                        Rectangle {
                            anchors.fill: parent
                            anchors.topMargin: 4
                            z: -1
                            radius: parent.radius
                            color: Theme.shadowMedium
                        }
                    }
                }
                
                delegate: ItemDelegate {
                    width: comboBox.width + Theme.spacingXL - Theme.spacingS * 2
                    height: 36
                    
                    contentItem: Text {
                        text: modelData
                        font.pixelSize: Theme.fontSizeM
                        color: highlighted ? accentColor : Theme.textPrimary
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    
                    highlighted: comboBox.highlightedIndex === index
                    
                    background: Rectangle {
                        color: highlighted ? Theme.primarySurface : 
                               hovered ? Theme.surfaceVariant : "transparent"
                        radius: Theme.radiusS
                        
                        Behavior on color {
                            ColorAnimation { duration: Theme.animationFast }
                        }
                    }
                }
            }
        }
        
        // Helper text
        Text {
            id: helperTextItem
            text: helperText
            font.pixelSize: Theme.fontSizeXS
            color: Theme.textTertiary
            visible: helperText !== ""
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
        }
    }
}
