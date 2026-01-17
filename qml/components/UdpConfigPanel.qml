import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: udpConfigPanel
    
    // Theme properties
    property color cardBackground: "#ffffff"
    property color primaryColor: "#3b82f6"
    property color primaryHover: "#2563eb"
    property color primaryPressed: "#1d4ed8"
    property color successColor: "#10b981"
    property color errorColor: "#ef4444"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color textMuted: "#94a3b8"
    property color borderColor: "#e2e8f0"
    property color borderFocus: "#3b82f6"
    property string fontFamily: "Segoe UI"
    
    // State
    property bool isConnected: false
    property int packetsReceived: 0
    property int packetsDropped: 0
    property real packetRate: 0.0
    
    color: cardBackground
    radius: 16
    border.color: borderColor
    border.width: 1
    
    // Shadow
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
        anchors.margins: 16
        spacing: 12
        
        // Compact header with title and status
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            
            Text {
                text: "UDP Configuration"
                font.pixelSize: 14
                font.weight: Font.DemiBold
                font.family: fontFamily
                color: textPrimary
            }
            
            Item { Layout.fillWidth: true }
            
            // Status badge
            Rectangle {
                width: statusRow.width + 12
                height: 24
                radius: 12
                color: isConnected ? Qt.rgba(successColor.r, successColor.g, successColor.b, 0.15) :
                                    Qt.rgba(errorColor.r, errorColor.g, errorColor.b, 0.15)
                
                Row {
                    id: statusRow
                    anchors.centerIn: parent
                    spacing: 6
                    
                    Rectangle {
                        width: 6
                        height: 6
                        radius: 3
                        color: isConnected ? successColor : errorColor
                        anchors.verticalCenter: parent.verticalCenter
                        
                        SequentialAnimation on opacity {
                            running: true
                            loops: Animation.Infinite
                            NumberAnimation { to: 0.5; duration: 1000 }
                            NumberAnimation { to: 1.0; duration: 1000 }
                        }
                    }
                    
                    Text {
                        text: isConnected ? "Connected" : "Disconnected"
                        font.pixelSize: 11
                        font.weight: Font.Medium
                        font.family: fontFamily
                        color: isConnected ? successColor : errorColor
                    }
                }
            }
        }
        
        // Content
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentHeight: contentColumn.height
            clip: true
            
            ColumnLayout {
                id: contentColumn
                width: parent.width
                spacing: 20
                
                // Connection Section
                SectionHeader {
                    Layout.fillWidth: true
                    title: "CONNECTION SETTINGS"
                    textColor: textSecondary
                    fontFamily: udpConfigPanel.fontFamily
                }
                
                // Host and Port
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16
                    
                    ModernTextField {
                        id: hostField
                        Layout.fillWidth: true
                        Layout.preferredWidth: 200
                        label: "Host Address"
                        placeholderText: "127.0.0.1"
                        text: "127.0.0.1"
                        
                        primaryColor: udpConfigPanel.primaryColor
                        textPrimary: udpConfigPanel.textPrimary
                        textSecondary: udpConfigPanel.textSecondary
                        borderColor: udpConfigPanel.borderColor
                        borderFocus: udpConfigPanel.borderFocus
                        fontFamily: udpConfigPanel.fontFamily
                    }
                    
                    ModernSpinBox {
                        id: portField
                        Layout.preferredWidth: 140
                        label: "Port"
                        from: 1
                        to: 65535
                        value: 5000
                        
                        primaryColor: udpConfigPanel.primaryColor
                        textPrimary: udpConfigPanel.textPrimary
                        textSecondary: udpConfigPanel.textSecondary
                        borderColor: udpConfigPanel.borderColor
                        borderFocus: udpConfigPanel.borderFocus
                        fontFamily: udpConfigPanel.fontFamily
                    }
                }
                
                // Statistics Section
                SectionHeader {
                    Layout.fillWidth: true
                    title: "STATISTICS"
                    textColor: textSecondary
                    fontFamily: udpConfigPanel.fontFamily
                }
                
                // Stats cards
                GridLayout {
                    Layout.fillWidth: true
                    columns: 3
                    rowSpacing: 12
                    columnSpacing: 12
                    
                    // Packets received
                    Rectangle {
                        Layout.fillWidth: true
                        height: 80
                        radius: 12
                        color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.05)
                        border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.2)
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 4
                            
                            Text {
                                text: packetsReceived.toLocaleString()
                                font.pixelSize: 24
                                font.weight: Font.Bold
                                font.family: fontFamily
                                color: primaryColor
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            Text {
                                text: "Received"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                    
                    // Packets dropped
                    Rectangle {
                        Layout.fillWidth: true
                        height: 80
                        radius: 12
                        color: Qt.rgba(errorColor.r, errorColor.g, errorColor.b, 0.05)
                        border.color: Qt.rgba(errorColor.r, errorColor.g, errorColor.b, 0.2)
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 4
                            
                            Text {
                                text: packetsDropped.toLocaleString()
                                font.pixelSize: 24
                                font.weight: Font.Bold
                                font.family: fontFamily
                                color: errorColor
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            Text {
                                text: "Dropped"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                    
                    // Packet rate
                    Rectangle {
                        Layout.fillWidth: true
                        height: 80
                        radius: 12
                        color: Qt.rgba(successColor.r, successColor.g, successColor.b, 0.05)
                        border.color: Qt.rgba(successColor.r, successColor.g, successColor.b, 0.2)
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 4
                            
                            Text {
                                text: packetRate.toFixed(1)
                                font.pixelSize: 24
                                font.weight: Font.Bold
                                font.family: fontFamily
                                color: successColor
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            Text {
                                text: "pps"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }
            }
        }
        
        // Connect button (kept as it's specific to UDP)
        ModernButton {
            text: isConnected ? "Disconnect" : "Connect"
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            
            primaryColor: isConnected ? errorColor : udpConfigPanel.primaryColor
            primaryHover: isConnected ? Qt.darker(errorColor, 1.1) : udpConfigPanel.primaryHover
            primaryPressed: isConnected ? Qt.darker(errorColor, 1.2) : udpConfigPanel.primaryPressed
            fontFamily: udpConfigPanel.fontFamily
            
            onClicked: {
                isConnected = !isConnected
                if (isConnected) {
                    // Simulate connection
                    packetsReceived = 0
                    packetsDropped = 0
                    packetRate = 0
                }
            }
        }
    }
    
    // Simulation timer for demo
    Timer {
        interval: 100
        running: isConnected
        repeat: true
        onTriggered: {
            packetsReceived += Math.floor(Math.random() * 5)
            if (Math.random() < 0.01) packetsDropped++
            packetRate = 10 + Math.random() * 40
        }
    }
}
