import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import "components"

ApplicationWindow {
    id: root
    visible: true
    width: 1400
    height: 900
    minimumWidth: 1200
    minimumHeight: 800
    title: qsTr("Radar Data Visualization - Configuration Panel")
    
    // Light theme colors
    readonly property color backgroundColor: "#f8fafc"
    readonly property color cardBackground: "#ffffff"
    readonly property color primaryColor: "#3b82f6"
    readonly property color primaryHover: "#2563eb"
    readonly property color primaryPressed: "#1d4ed8"
    readonly property color accentColor: "#06b6d4"
    readonly property color successColor: "#10b981"
    readonly property color warningColor: "#f59e0b"
    readonly property color errorColor: "#ef4444"
    readonly property color textPrimary: "#1e293b"
    readonly property color textSecondary: "#64748b"
    readonly property color textMuted: "#94a3b8"
    readonly property color borderColor: "#e2e8f0"
    readonly property color borderFocus: "#3b82f6"
    readonly property color shadowColor: "#00000014"
    
    // Fonts
    readonly property string fontFamily: "Segoe UI, Inter, -apple-system, BlinkMacSystemFont, sans-serif"
    
    // Set application background
    color: backgroundColor
    
    // Header
    header: ToolBar {
        height: 64
        background: Rectangle {
            color: cardBackground
            border.color: borderColor
            border.width: 1
            
            // Subtle shadow
            layer.enabled: true
            layer.effect: Rectangle {
                color: shadowColor
                y: 2
                height: parent.height
                width: parent.width
            }
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 24
            anchors.rightMargin: 24
            spacing: 16
            
            // Logo/Title
            Row {
                spacing: 12
                
                Rectangle {
                    width: 40
                    height: 40
                    radius: 8
                    color: primaryColor
                    
                    Text {
                        anchors.centerIn: parent
                        text: "R"
                        font.pixelSize: 20
                        font.weight: Font.Bold
                        color: "#ffffff"
                    }
                }
                
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2
                    
                    Text {
                        text: "Radar Visualization"
                        font.pixelSize: 18
                        font.weight: Font.DemiBold
                        font.family: root.fontFamily
                        color: textPrimary
                    }
                    
                    Text {
                        text: "Configuration Dashboard"
                        font.pixelSize: 12
                        font.family: root.fontFamily
                        color: textSecondary
                    }
                }
            }
            
            Item { Layout.fillWidth: true }
            
            // Status indicators
            Row {
                spacing: 24
                
                // Connection status
                Row {
                    spacing: 8
                    
                    Rectangle {
                        width: 10
                        height: 10
                        radius: 5
                        anchors.verticalCenter: parent.verticalCenter
                        color: udpPanel.isConnected ? successColor : errorColor
                        
                        SequentialAnimation on opacity {
                            running: !udpPanel.isConnected
                            loops: Animation.Infinite
                            NumberAnimation { to: 0.4; duration: 800; easing.type: Easing.InOutQuad }
                            NumberAnimation { to: 1.0; duration: 800; easing.type: Easing.InOutQuad }
                        }
                    }
                    
                    Text {
                        text: udpPanel.isConnected ? "Connected" : "Disconnected"
                        font.pixelSize: 13
                        font.family: root.fontFamily
                        color: textSecondary
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                
                // Separator
                Rectangle {
                    width: 1
                    height: 24
                    color: borderColor
                }
                
                // Time
                Text {
                    text: Qt.formatDateTime(new Date(), "hh:mm:ss")
                    font.pixelSize: 13
                    font.family: root.fontFamily
                    font.weight: Font.Medium
                    color: textSecondary
                    
                    Timer {
                        interval: 1000
                        running: true
                        repeat: true
                        onTriggered: parent.text = Qt.formatDateTime(new Date(), "hh:mm:ss")
                    }
                }
            }
        }
    }
    
    // Main content - 2x2 Grid Layout
    MainView {
        id: mainView
        anchors.fill: parent
        anchors.margins: 24
        
        backgroundColor: root.backgroundColor
        cardBackground: root.cardBackground
        primaryColor: root.primaryColor
        primaryHover: root.primaryHover
        primaryPressed: root.primaryPressed
        accentColor: root.accentColor
        successColor: root.successColor
        warningColor: root.warningColor
        errorColor: root.errorColor
        textPrimary: root.textPrimary
        textSecondary: root.textSecondary
        textMuted: root.textMuted
        borderColor: root.borderColor
        borderFocus: root.borderFocus
        fontFamily: root.fontFamily
        
        // Reference to UDP panel for status
        property alias udpPanel: mainView.udpConfigPanel
    }
    
    // Alias for header status
    property alias udpPanel: mainView.udpConfigPanel
}
