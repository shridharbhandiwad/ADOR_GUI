import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: udpConfigPanel
    
    // Theme properties - Simplified classic palette
    property color cardBackground: ThemeManager.cardBackground
    property color primaryColor: ThemeManager.primaryColor
    property color primaryHover: ThemeManager.primaryHover
    property color primaryPressed: ThemeManager.primaryPressed
    property color errorColor: ThemeManager.errorColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color textMuted: ThemeManager.textMuted
    property color borderColor: ThemeManager.borderColor
    property color borderFocus: ThemeManager.borderFocus
    property color inputBackground: ThemeManager.inputBackground
    property string fontFamily: ThemeManager.fontFamily
    
    // Deprecated - kept for compatibility, now uses primaryColor
    property color successColor: primaryColor
    
    // State
    property bool isConnected: false
    property int packetsReceived: 0
    property int packetsDropped: 0
    property real packetRate: 0.0
    
    color: cardBackground
    radius: 16
    border.color: borderColor
    border.width: 1
    
    // Smooth theme transitions
    Behavior on color { ColorAnimation { duration: 200 } }
    Behavior on border.color { ColorAnimation { duration: 200 } }
    
    // Shadow
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
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
            
            Item { Layout.fillWidth: true }
            
            // Status badge - Classic design with primary/error colors only
            Rectangle {
                width: statusRow.width + 12
                height: 24
                radius: 12
                color: isConnected ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15) :
                                    Qt.rgba(errorColor.r, errorColor.g, errorColor.b, 0.15)
                
                Behavior on color { ColorAnimation { duration: 200 } }
                
                Row {
                    id: statusRow
                    anchors.centerIn: parent
                    spacing: 6
                    
                    Rectangle {
                        width: 6
                        height: 6
                        radius: 3
                        color: isConnected ? primaryColor : errorColor
                        anchors.verticalCenter: parent.verticalCenter
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                        
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
                        color: isConnected ? primaryColor : errorColor
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
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
                        backgroundColor: udpConfigPanel.inputBackground
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
                        backgroundColor: udpConfigPanel.inputBackground
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
                
                // Stats cards - Classic unified design with primary color
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
                        border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                        Behavior on border.color { ColorAnimation { duration: 200 } }
                        
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
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
                            }
                            
                            Text {
                                text: "Received"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
                            }
                        }
                    }
                    
                    // Packets dropped - Keep error color as this is a critical indicator
                    Rectangle {
                        Layout.fillWidth: true
                        height: 80
                        radius: 12
                        color: packetsDropped > 0 ? Qt.rgba(errorColor.r, errorColor.g, errorColor.b, 0.05) : 
                               Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.03)
                        border.color: packetsDropped > 0 ? Qt.rgba(errorColor.r, errorColor.g, errorColor.b, 0.2) :
                                      Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                        Behavior on border.color { ColorAnimation { duration: 200 } }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 4
                            
                            Text {
                                text: packetsDropped.toLocaleString()
                                font.pixelSize: 24
                                font.weight: Font.Bold
                                font.family: fontFamily
                                color: packetsDropped > 0 ? errorColor : textMuted
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
                            }
                            
                            Text {
                                text: "Dropped"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
                            }
                        }
                    }
                    
                    // Packet rate - Uses primary color for unified classic look
                    Rectangle {
                        Layout.fillWidth: true
                        height: 80
                        radius: 12
                        color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.05)
                        border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                        Behavior on border.color { ColorAnimation { duration: 200 } }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 4
                            
                            Text {
                                text: packetRate.toFixed(1)
                                font.pixelSize: 24
                                font.weight: Font.Bold
                                font.family: fontFamily
                                color: primaryColor
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
                            }
                            
                            Text {
                                text: "pps"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
                            }
                        }
                    }
                }
            }
        }
        
        // Connect button - Uses primary color for both states (classic unified look)
        ModernButton {
            text: isConnected ? "Disconnect" : "Connect"
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            outline: isConnected  // Outline style when connected for visual distinction
            
            primaryColor: udpConfigPanel.primaryColor
            primaryHover: udpConfigPanel.primaryHover
            primaryPressed: udpConfigPanel.primaryPressed
            textColor: udpConfigPanel.textPrimary
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
