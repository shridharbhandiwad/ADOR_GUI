import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Item {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Modern Switch Component - Elegant Toggle Switch
    // ═══════════════════════════════════════════════════════════════════════════
    
    property alias checked: switchControl.checked
    property string label: ""
    property string description: ""
    property color accentColor: Theme.primaryColor
    property bool labelOnLeft: true
    
    implicitWidth: 200
    implicitHeight: Math.max(switchControl.height, labelColumn.height)
    
    RowLayout {
        anchors.fill: parent
        spacing: Theme.spacingM
        layoutDirection: labelOnLeft ? Qt.LeftToRight : Qt.RightToLeft
        
        // Label and description
        ColumnLayout {
            id: labelColumn
            Layout.fillWidth: true
            spacing: 2
            visible: label !== ""
            
            Text {
                text: label
                font.pixelSize: Theme.fontSizeM
                font.weight: Font.Medium
                color: Theme.textPrimary
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
            
            Text {
                text: description
                font.pixelSize: Theme.fontSizeS
                color: Theme.textSecondary
                visible: description !== ""
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
        }
        
        // Switch control
        Switch {
            id: switchControl
            
            indicator: Rectangle {
                implicitWidth: 48
                implicitHeight: 26
                x: switchControl.leftPadding
                y: parent.height / 2 - height / 2
                radius: 13
                color: switchControl.checked ? accentColor : Theme.borderColor
                
                Behavior on color {
                    ColorAnimation { duration: Theme.animationNormal }
                }
                
                Rectangle {
                    x: switchControl.checked ? parent.width - width - 3 : 3
                    y: 3
                    width: 20
                    height: 20
                    radius: 10
                    color: Theme.surfaceColor
                    
                    // Inner shadow for depth
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 1
                        radius: parent.radius - 1
                        color: "transparent"
                        border.width: 1
                        border.color: Qt.alpha(Theme.shadowLight, 0.5)
                    }
                    
                    Behavior on x {
                        NumberAnimation { duration: Theme.animationNormal; easing.type: Easing.OutCubic }
                    }
                }
            }
            
            contentItem: Item {}
            
            background: Rectangle {
                color: "transparent"
            }
        }
    }
    
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: switchControl.toggle()
    }
}
