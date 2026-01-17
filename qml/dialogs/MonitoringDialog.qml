import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"
import "../components"

DialogCard {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Monitoring Dialog - Real-time Data & Metrics
    // ═══════════════════════════════════════════════════════════════════════════
    
    title: "Monitoring"
    subtitle: "Real-time data and metrics"
    iconSource: "📊"
    accentColor: Theme.monitorCardAccent
    
    // Simulated data for demonstration
    property real cpuUsage: 45
    property real memoryUsage: 62
    property real temperature: 38.5
    property int activeConnections: 3
    
    Timer {
        interval: 2000
        running: true
        repeat: true
        onTriggered: {
            cpuUsage = Math.min(100, Math.max(0, cpuUsage + (Math.random() - 0.5) * 10))
            memoryUsage = Math.min(100, Math.max(0, memoryUsage + (Math.random() - 0.5) * 5))
            temperature = Math.min(80, Math.max(20, temperature + (Math.random() - 0.5) * 2))
        }
    }
    
    contentItem: [
        // Status Overview
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingS
            
            StatusIndicator {
                Layout.fillWidth: true
                status: "active"
                label: "System"
                value: "Online"
            }
            
            StatusIndicator {
                Layout.fillWidth: true
                status: "success"
                label: "Connection"
                value: "Stable"
            }
        },
        
        // Metrics Section
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: sectionHeader.height
            Layout.topMargin: Theme.spacingS
            
            Text {
                id: sectionHeader
                text: "System Metrics"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: sectionHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: sectionHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        // CPU Usage
        MetricBar {
            Layout.fillWidth: true
            label: "CPU Usage"
            value: cpuUsage
            unit: "%"
            color: cpuUsage > 80 ? Theme.errorColor : cpuUsage > 60 ? Theme.warningColor : Theme.successColor
        },
        
        // Memory Usage
        MetricBar {
            Layout.fillWidth: true
            label: "Memory Usage"
            value: memoryUsage
            unit: "%"
            color: memoryUsage > 85 ? Theme.errorColor : memoryUsage > 70 ? Theme.warningColor : Theme.primaryColor
        },
        
        // Temperature
        MetricBar {
            Layout.fillWidth: true
            label: "Temperature"
            value: temperature
            maxValue: 100
            unit: "°C"
            color: temperature > 70 ? Theme.errorColor : temperature > 50 ? Theme.warningColor : Theme.infoColor
        },
        
        // Statistics Grid
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: statsHeader.height
            Layout.topMargin: Theme.spacingM
            
            Text {
                id: statsHeader
                text: "Statistics"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: statsHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: statsHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        GridLayout {
            Layout.fillWidth: true
            columns: 2
            rowSpacing: Theme.spacingS
            columnSpacing: Theme.spacingS
            
            StatCard {
                Layout.fillWidth: true
                label: "Active Connections"
                value: activeConnections.toString()
                iconText: "🔗"
            }
            
            StatCard {
                Layout.fillWidth: true
                label: "Uptime"
                value: "4h 23m"
                iconText: "⏱"
            }
            
            StatCard {
                Layout.fillWidth: true
                label: "Data Received"
                value: "1.2 GB"
                iconText: "📥"
            }
            
            StatCard {
                Layout.fillWidth: true
                label: "Data Sent"
                value: "856 MB"
                iconText: "📤"
            }
        },
        
        // Refresh button
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Theme.spacingM
            
            Item { Layout.fillWidth: true }
            
            ModernButton {
                text: "Refresh"
                variant: "outline"
                iconText: "🔄"
            }
        }
    ]
    
    // ───────────────────────────────────────────────────────────────────────────
    // Custom Components
    // ───────────────────────────────────────────────────────────────────────────
    
    component MetricBar: Item {
        property string label: ""
        property real value: 0
        property real maxValue: 100
        property string unit: ""
        property color color: Theme.primaryColor
        
        implicitHeight: 48
        
        ColumnLayout {
            anchors.fill: parent
            spacing: Theme.spacingXS
            
            RowLayout {
                Layout.fillWidth: true
                
                Text {
                    text: label
                    font.pixelSize: Theme.fontSizeS
                    color: Theme.textSecondary
                }
                
                Item { Layout.fillWidth: true }
                
                Text {
                    text: value.toFixed(1) + unit
                    font.pixelSize: Theme.fontSizeM
                    font.weight: Font.DemiBold
                    color: color
                    
                    Behavior on color {
                        ColorAnimation { duration: Theme.animationNormal }
                    }
                }
            }
            
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 8
                radius: 4
                color: Theme.surfaceVariant
                
                Rectangle {
                    width: parent.width * Math.min(1, value / maxValue)
                    height: parent.height
                    radius: parent.radius
                    color: color
                    
                    Behavior on width {
                        NumberAnimation { duration: Theme.animationNormal; easing.type: Easing.OutCubic }
                    }
                    
                    Behavior on color {
                        ColorAnimation { duration: Theme.animationNormal }
                    }
                }
            }
        }
    }
    
    component StatCard: Rectangle {
        property string label: ""
        property string value: ""
        property string iconText: ""
        
        implicitHeight: 60
        radius: Theme.radiusM
        color: Theme.surfaceVariant
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingS
            spacing: Theme.spacingS
            
            Text {
                text: iconText
                font.pixelSize: Theme.iconSizeL
            }
            
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                
                Text {
                    text: value
                    font.pixelSize: Theme.fontSizeL
                    font.weight: Font.Bold
                    color: Theme.textPrimary
                }
                
                Text {
                    text: label
                    font.pixelSize: Theme.fontSizeXS
                    color: Theme.textSecondary
                }
            }
        }
    }
}
