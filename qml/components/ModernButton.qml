import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Button {
    id: control
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Modern Button Component - Elegant Styled Button
    // ═══════════════════════════════════════════════════════════════════════════
    
    property string variant: "primary"  // primary, secondary, outline, ghost, success, danger
    property string size: "medium"      // small, medium, large
    property string iconText: ""        // Unicode icon character
    property bool iconOnly: false
    property bool loading: false
    
    implicitWidth: iconOnly ? implicitHeight : Math.max(100, contentLayout.implicitWidth + Theme.spacingXL * 2)
    implicitHeight: size === "small" ? Theme.buttonHeightS : 
                    size === "large" ? Theme.buttonHeightL : Theme.buttonHeightM
    
    font.pixelSize: size === "small" ? Theme.fontSizeS : 
                    size === "large" ? Theme.fontSizeL : Theme.fontSizeM
    font.weight: Font.Medium
    
    // Variant colors
    readonly property color variantBg: {
        switch(variant) {
            case "primary": return Theme.primaryColor
            case "secondary": return Theme.surfaceVariant
            case "outline": return "transparent"
            case "ghost": return "transparent"
            case "success": return Theme.successColor
            case "danger": return Theme.errorColor
            default: return Theme.primaryColor
        }
    }
    
    readonly property color variantHoverBg: {
        switch(variant) {
            case "primary": return Theme.primaryLight
            case "secondary": return Theme.borderColor
            case "outline": return Theme.surfaceVariant
            case "ghost": return Theme.surfaceVariant
            case "success": return Theme.successLight
            case "danger": return Theme.errorLight
            default: return Theme.primaryLight
        }
    }
    
    readonly property color variantText: {
        switch(variant) {
            case "primary": return Theme.textOnPrimary
            case "secondary": return Theme.textPrimary
            case "outline": return Theme.primaryColor
            case "ghost": return Theme.textPrimary
            case "success": return Theme.textOnPrimary
            case "danger": return Theme.textOnPrimary
            default: return Theme.textOnPrimary
        }
    }
    
    readonly property color variantBorder: {
        switch(variant) {
            case "outline": return Theme.primaryColor
            default: return "transparent"
        }
    }
    
    contentItem: RowLayout {
        id: contentLayout
        spacing: Theme.spacingS
        opacity: loading ? 0.5 : 1
        
        // Loading spinner
        Item {
            Layout.preferredWidth: Theme.iconSizeS
            Layout.preferredHeight: Theme.iconSizeS
            visible: loading
            
            Rectangle {
                id: spinner
                anchors.centerIn: parent
                width: parent.width
                height: parent.height
                radius: width / 2
                color: "transparent"
                border.width: 2
                border.color: control.variantText
                
                Rectangle {
                    width: 4
                    height: 4
                    radius: 2
                    color: control.variantText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                }
                
                RotationAnimation on rotation {
                    running: loading
                    from: 0
                    to: 360
                    duration: 1000
                    loops: Animation.Infinite
                }
            }
        }
        
        // Icon
        Text {
            text: iconText
            font.pixelSize: size === "small" ? Theme.iconSizeS : Theme.iconSizeM
            font.family: "Segoe Fluent Icons, Segoe MDL2 Assets"
            color: control.variantText
            visible: iconText !== "" && !loading
            Layout.alignment: Qt.AlignVCenter
        }
        
        // Text
        Text {
            text: control.text
            font: control.font
            color: control.variantText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            visible: !iconOnly && text !== ""
            Layout.alignment: Qt.AlignVCenter
        }
    }
    
    background: Rectangle {
        radius: Theme.radiusM
        color: control.pressed ? Qt.darker(variantBg, 1.1) :
               control.hovered ? variantHoverBg : variantBg
        border.width: variant === "outline" ? 1.5 : 0
        border.color: variantBorder
        
        Behavior on color {
            ColorAnimation { duration: Theme.animationFast }
        }
        
        // Ripple effect placeholder
        Rectangle {
            id: ripple
            anchors.centerIn: parent
            width: 0
            height: 0
            radius: width / 2
            color: Qt.alpha(Theme.textOnPrimary, 0.2)
            opacity: 0
            
            states: State {
                name: "pressed"
                when: control.pressed
                PropertyChanges {
                    target: ripple
                    width: control.width * 2
                    height: control.width * 2
                    opacity: 0
                }
            }
            
            transitions: Transition {
                ParallelAnimation {
                    NumberAnimation { property: "width"; duration: 300 }
                    NumberAnimation { property: "height"; duration: 300 }
                    NumberAnimation { property: "opacity"; from: 0.3; to: 0; duration: 300 }
                }
            }
        }
    }
    
    // Hover cursor
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        acceptedButtons: Qt.NoButton
    }
}
