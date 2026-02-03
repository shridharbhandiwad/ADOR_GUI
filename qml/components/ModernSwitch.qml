import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: modernSwitch
    
    // Theme properties - Simplified classic palette
    property color primaryColor: ThemeManager.primaryColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color trackOffColor: ThemeManager.trackOffColor
    property string fontFamily: ThemeManager.fontFamily
    
    // Deprecated - kept for compatibility, now uses primaryColor
    property color successColor: primaryColor
    
    // Switch properties
    property string label: ""
    property string description: ""
    property alias checked: switchControl.checked
    
    implicitHeight: description.length > 0 ? 64 : 44
    implicitWidth: 200
    
    RowLayout {
        anchors.fill: parent
        spacing: 16
        
        Column {
            Layout.fillWidth: true
            spacing: 4
            
            Text {
                text: modernSwitch.label
                font.pixelSize: 16
                font.weight: Font.Medium
                font.family: modernSwitch.fontFamily
                color: textPrimary
                visible: modernSwitch.label.length > 0
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
            
            Text {
                text: modernSwitch.description
                font.pixelSize: 14
                font.family: modernSwitch.fontFamily
                color: textSecondary
                visible: modernSwitch.description.length > 0
                width: parent.width
                wrapMode: Text.WordWrap
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
        }
        
        Switch {
            id: switchControl
            
            indicator: Rectangle {
                implicitWidth: 52
                implicitHeight: 28
                x: switchControl.leftPadding
                y: parent.height / 2 - height / 2
                radius: 14
                // Uses primary color for classic unified look
                color: switchControl.checked ? primaryColor : trackOffColor
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
                
                Rectangle {
                    x: switchControl.checked ? parent.width - width - 4 : 4
                    y: 4
                    width: 20
                    height: 20
                    radius: 10
                    color: "#ffffff"
                    
                    layer.enabled: true
                    layer.effect: DropShadow {
                        transparentBorder: true
                        horizontalOffset: 0
                        verticalOffset: 1
                        radius: 4
                        samples: 9
                        color: "#0000002a"
                    }
                    
                    Behavior on x {
                        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                    }
                }
            }
            
            contentItem: Item {}
        }
    }
}
