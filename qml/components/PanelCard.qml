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
    
    // Shadow effect
    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        horizontalOffset: 0
        verticalOffset: 4
        radius: 12
        samples: 25
        color: ThemeManager.shadowColor
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20
        
        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            // Icon
            Rectangle {
                width: 48
                height: 48
                radius: 12
                color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                
                Behavior on color { ColorAnimation { duration: 200 } }
                
                Text {
                    anchors.centerIn: parent
                    text: panelCard.iconText
                    font.pixelSize: 24
                    color: primaryColor
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
            }
            
            Column {
                Layout.fillWidth: true
                spacing: 4
                
                Text {
                    text: panelCard.title
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    font.family: panelCard.fontFamily
                    color: textPrimary
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
                
                Text {
                    text: panelCard.subtitle
                    font.pixelSize: 13
                    font.family: panelCard.fontFamily
                    color: textSecondary
                    visible: panelCard.subtitle.length > 0
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
            }
        }
        
        // Separator
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: borderColor
            
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
