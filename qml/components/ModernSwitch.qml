import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: modernSwitch
    
    // Theme properties
    property color primaryColor: "#3b82f6"
    property color successColor: "#10b981"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color trackOffColor: "#cbd5e1"
    property string fontFamily: "Segoe UI"
    
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
                font.pixelSize: 14
                font.weight: Font.Medium
                font.family: modernSwitch.fontFamily
                color: textPrimary
                visible: modernSwitch.label.length > 0
            }
            
            Text {
                text: modernSwitch.description
                font.pixelSize: 12
                font.family: modernSwitch.fontFamily
                color: textSecondary
                visible: modernSwitch.description.length > 0
                width: parent.width
                wrapMode: Text.WordWrap
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
                color: switchControl.checked ? successColor : trackOffColor
                
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
