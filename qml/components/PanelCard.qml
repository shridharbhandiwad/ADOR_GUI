import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: panelCard
    
    // Theme properties
    property color cardBackground: ThemeManager.cardBackground
    property color primaryColor: ThemeManager.primaryColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color borderColor: ThemeManager.borderColor
    property string fontFamily: ThemeManager.fontFamily
    
    // Content properties
    property string title: "Panel Title"
    property string subtitle: ""
    property string iconText: ""
    property alias content: contentContainer.children
    
    color: cardBackground
    radius: 16
    border.color: borderColor
    border.width: 1
    
    // Smooth theme transitions
    Behavior on color { ColorAnimation { duration: 200 } }
    Behavior on border.color { ColorAnimation { duration: 200 } }
    
    // Enhanced shadow with subtle depth
    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        horizontalOffset: 0
        verticalOffset: 6
        radius: 20
        samples: 32
        color: ThemeManager.shadowColor
        spread: 0.1
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 24
        
        // Header - Enhanced premium design
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            // Icon with refined gradient background
            Rectangle {
                width: 52
                height: 52
                radius: 14
                
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.12) }
                    GradientStop { position: 1.0; color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.06) }
                }
                
                border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                border.width: 1
                
                Behavior on color { ColorAnimation { duration: 200 } }
                Behavior on border.color { ColorAnimation { duration: 200 } }
                
                Text {
                    anchors.centerIn: parent
                    text: panelCard.iconText
                    font.pixelSize: 28
                    color: primaryColor
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
            }
            
            Column {
                Layout.fillWidth: true
                spacing: 6
                
                Text {
                    text: panelCard.title
                    font.pixelSize: 19
                    font.weight: Font.DemiBold
                    font.family: panelCard.fontFamily
                    font.letterSpacing: -0.3
                    color: textPrimary
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
                
                Text {
                    text: panelCard.subtitle
                    font.pixelSize: 14
                    font.weight: Font.Normal
                    font.family: panelCard.fontFamily
                    color: textSecondary
                    visible: panelCard.subtitle.length > 0
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
            }
        }
        
        // Refined separator with gradient fade
        Rectangle {
            Layout.fillWidth: true
            height: 1
            
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 0.1; color: borderColor }
                GradientStop { position: 0.9; color: borderColor }
                GradientStop { position: 1.0; color: "transparent" }
            }
            
            Behavior on color { ColorAnimation { duration: 200 } }
        }
        
        // Content container
        Item {
            id: contentContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
