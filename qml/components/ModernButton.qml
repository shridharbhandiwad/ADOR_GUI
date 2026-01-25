import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Button {
    id: modernButton
    
    // Theme properties
    property color primaryColor: ThemeManager.primaryColor
    property color primaryHover: ThemeManager.primaryHover
    property color primaryPressed: ThemeManager.primaryPressed
    property color textColor: "#ffffff"
    property string fontFamily: ThemeManager.fontFamily
    property bool outline: false
    property bool small: false
    
    implicitHeight: small ? 36 : 44
    implicitWidth: Math.max(100, contentItem.implicitWidth + 32)
    
    contentItem: Text {
        text: modernButton.text
        font.pixelSize: small ? 13 : 14
        font.weight: Font.Medium
        font.family: modernButton.fontFamily
        color: outline ? (modernButton.down ? primaryPressed : 
               modernButton.hovered ? primaryHover : primaryColor) : textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        
        Behavior on color { ColorAnimation { duration: 150 } }
    }
    
    background: Rectangle {
        radius: 10
        color: {
            if (outline) {
                if (modernButton.down) return Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                if (modernButton.hovered) return Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08)
                return "transparent"
            } else {
                if (modernButton.down) return primaryPressed
                if (modernButton.hovered) return primaryHover
                return primaryColor
            }
        }
        border.color: outline ? (modernButton.hovered ? primaryHover : primaryColor) : "transparent"
        border.width: outline ? 1.5 : 0
        
        Behavior on color {
            ColorAnimation { duration: 150 }
        }
        
        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }
        
        // Shadow for non-outline buttons
        layer.enabled: !outline && modernButton.enabled
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: modernButton.down ? 1 : 2
            radius: modernButton.down ? 4 : 8
            samples: 17
            color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.35)
        }
    }
    
    opacity: enabled ? 1.0 : 0.5
    
    Behavior on opacity {
        NumberAnimation { duration: 150 }
    }
}
