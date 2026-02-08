import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Button {
    id: modernButton
    
    // Theme properties
    property color primaryColor: ThemeManager.primaryColor
    property color primaryHover: ThemeManager.primaryHover
    property color primaryPressed: ThemeManager.primaryPressed
    property color textColor: ThemeManager.buttonTextColor  // Dynamic based on theme
    property string fontFamily: ThemeManager.fontFamily
    property bool outline: false
    property bool small: false
    
    implicitHeight: small ? 36 : 44
    implicitWidth: Math.max(100, contentItem.implicitWidth + 36)
    
    contentItem: Text {
        text: modernButton.text
        font.pixelSize: small ? 14 : 15
        font.weight: Font.Medium
        font.family: modernButton.fontFamily
        font.letterSpacing: 0.3
        color: outline ? (modernButton.down ? primaryPressed : 
               modernButton.hovered ? primaryHover : primaryColor) : textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        
        Behavior on color { ColorAnimation { duration: 200 } }
    }
    
    background: Rectangle {
        radius: 10
        
        // Enhanced gradient for filled buttons
        gradient: !outline ? Gradient {
            GradientStop { 
                position: 0.0
                color: {
                    if (modernButton.down) return Qt.darker(primaryPressed, 1.05)
                    if (modernButton.hovered) return primaryHover
                    return primaryColor
                }
            }
            GradientStop { 
                position: 1.0
                color: {
                    if (modernButton.down) return primaryPressed
                    if (modernButton.hovered) return Qt.darker(primaryHover, 1.08)
                    return Qt.darker(primaryColor, 1.12)
                }
            }
        } : null
        
        // Solid color for outline buttons
        color: {
            if (!outline) return "transparent"  // Gradient handles filled buttons
            if (modernButton.down) return Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.12)
            if (modernButton.hovered) return Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08)
            return "transparent"
        }
        
        border.color: outline ? (modernButton.hovered ? primaryHover : primaryColor) : "transparent"
        border.width: outline ? 1.5 : 0
        
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
        
        Behavior on border.color {
            ColorAnimation { duration: 200 }
        }
        
        // Premium shadow for filled buttons
        layer.enabled: !outline && modernButton.enabled
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: modernButton.down ? 2 : 4
            radius: modernButton.down ? 8 : 16
            samples: 25
            color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, modernButton.down ? 0.35 : 0.45)
            spread: 0.1
        }
    }
    
    // Subtle scale effect on press
    scale: modernButton.down ? 0.98 : 1.0
    
    Behavior on scale {
        NumberAnimation { duration: 100; easing.type: Easing.OutCubic }
    }
    
    opacity: enabled ? 1.0 : 0.5
    
    Behavior on opacity {
        NumberAnimation { duration: 150 }
    }
}
