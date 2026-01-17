import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: panelCard
    
    // Theme properties
    property color cardBackground: "#ffffff"
    property color primaryColor: "#3b82f6"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color borderColor: "#e2e8f0"
    property string fontFamily: "Segoe UI"
    
    // Content properties
    property string title: "Panel Title"
    property string subtitle: ""
    property string iconText: ""
    property alias content: contentContainer.children
    
    color: cardBackground
    radius: 16
    border.color: borderColor
    border.width: 1
    
    // Shadow effect
    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        horizontalOffset: 0
        verticalOffset: 4
        radius: 12
        samples: 25
        color: "#0000001a"
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
                
                Text {
                    anchors.centerIn: parent
                    text: panelCard.iconText
                    font.pixelSize: 24
                    color: primaryColor
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
                }
                
                Text {
                    text: panelCard.subtitle
                    font.pixelSize: 13
                    font.family: panelCard.fontFamily
                    color: textSecondary
                    visible: panelCard.subtitle.length > 0
                }
            }
        }
        
        // Separator
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: borderColor
        }
        
        // Content container
        Item {
            id: contentContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
