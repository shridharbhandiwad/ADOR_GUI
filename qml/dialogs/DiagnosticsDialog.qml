import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"
import "../components"

DialogCard {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Diagnostics Dialog - System Health & Troubleshooting
    // ═══════════════════════════════════════════════════════════════════════════
    
    title: "Diagnostics"
    subtitle: "System health and troubleshooting"
    iconSource: "🔧"
    accentColor: Theme.diagCardAccent
    
    property bool isScanning: false
    
    contentItem: [
        // System Health Overview
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: healthHeader.height
            
            Text {
                id: healthHeader
                text: "System Health"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: healthHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: healthHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        // Health Status Cards
        GridLayout {
            Layout.fillWidth: true
            columns: 2
            rowSpacing: Theme.spacingS
            columnSpacing: Theme.spacingS
            
            HealthStatusCard {
                Layout.fillWidth: true
                label: "Hardware"
                status: "success"
                details: "All components OK"
            }
            
            HealthStatusCard {
                Layout.fillWidth: true
                label: "Firmware"
                status: "success"
                details: "v2.4.1 (Latest)"
            }
            
            HealthStatusCard {
                Layout.fillWidth: true
                label: "Network"
                status: "warning"
                details: "High latency"
            }
            
            HealthStatusCard {
                Layout.fillWidth: true
                label: "Storage"
                status: "success"
                details: "78% available"
            }
        },
        
        // Diagnostic Tests
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: testsHeader.height
            Layout.topMargin: Theme.spacingM
            
            Text {
                id: testsHeader
                text: "Diagnostic Tests"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: testsHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: testsHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingS
            
            DiagnosticTestRow {
                Layout.fillWidth: true
                testName: "Self-test"
                status: "passed"
                lastRun: "2 min ago"
            }
            
            DiagnosticTestRow {
                Layout.fillWidth: true
                testName: "Connectivity check"
                status: "passed"
                lastRun: "5 min ago"
            }
            
            DiagnosticTestRow {
                Layout.fillWidth: true
                testName: "Sensor calibration"
                status: "warning"
                lastRun: "1 hour ago"
            }
            
            DiagnosticTestRow {
                Layout.fillWidth: true
                testName: "Memory test"
                status: "idle"
                lastRun: "Never"
            }
        },
        
        // Run Tests Button
        ModernButton {
            Layout.fillWidth: true
            Layout.topMargin: Theme.spacingS
            text: isScanning ? "Scanning..." : "Run All Tests"
            variant: "primary"
            iconText: "🔍"
            loading: isScanning
            onClicked: {
                isScanning = true
                scanTimer.start()
            }
        },
        
        // Event Log
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: logHeader.height
            Layout.topMargin: Theme.spacingM
            
            Text {
                id: logHeader
                text: "Recent Events"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: logHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: logHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            radius: Theme.radiusM
            color: Theme.surfaceVariant
            border.width: 1
            border.color: Theme.borderColor
            
            ListView {
                anchors.fill: parent
                anchors.margins: Theme.spacingS
                clip: true
                spacing: 4
                model: eventLogModel
                
                delegate: RowLayout {
                    width: ListView.view.width
                    spacing: Theme.spacingS
                    
                    Text {
                        text: model.time
                        font.pixelSize: Theme.fontSizeXS
                        font.family: "Consolas, Monaco, monospace"
                        color: Theme.textTertiary
                        Layout.preferredWidth: 60
                    }
                    
                    Rectangle {
                        Layout.preferredWidth: 6
                        Layout.preferredHeight: 6
                        radius: 3
                        color: model.level === "error" ? Theme.errorColor :
                               model.level === "warning" ? Theme.warningColor :
                               model.level === "info" ? Theme.infoColor : Theme.textTertiary
                    }
                    
                    Text {
                        text: model.message
                        font.pixelSize: Theme.fontSizeXS
                        color: Theme.textPrimary
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }
                }
            }
        },
        
        // Export & Clear Buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Theme.spacingM
            spacing: Theme.spacingM
            
            ModernButton {
                text: "Export Log"
                variant: "outline"
                iconText: "📋"
            }
            
            Item { Layout.fillWidth: true }
            
            ModernButton {
                text: "Clear Log"
                variant: "ghost"
            }
        }
    ]
    
    // ───────────────────────────────────────────────────────────────────────────
    // Data Models & Timers
    // ───────────────────────────────────────────────────────────────────────────
    
    Timer {
        id: scanTimer
        interval: 3000
        onTriggered: isScanning = false
    }
    
    ListModel {
        id: eventLogModel
        
        ListElement { time: "14:32:01"; level: "info"; message: "System initialized successfully" }
        ListElement { time: "14:32:05"; level: "info"; message: "Connection established" }
        ListElement { time: "14:35:22"; level: "warning"; message: "High network latency detected (>100ms)" }
        ListElement { time: "14:36:10"; level: "info"; message: "Auto-calibration completed" }
        ListElement { time: "14:40:00"; level: "info"; message: "Heartbeat OK" }
    }
    
    // ───────────────────────────────────────────────────────────────────────────
    // Custom Components
    // ───────────────────────────────────────────────────────────────────────────
    
    component HealthStatusCard: Rectangle {
        property string label: ""
        property string status: "idle"  // success, warning, error, idle
        property string details: ""
        
        readonly property color statusColor: status === "success" ? Theme.successColor :
                                            status === "warning" ? Theme.warningColor :
                                            status === "error" ? Theme.errorColor : Theme.textTertiary
        
        readonly property color bgColor: status === "success" ? Theme.successSurface :
                                        status === "warning" ? Theme.warningSurface :
                                        status === "error" ? Theme.errorSurface : Theme.surfaceVariant
        
        implicitHeight: 56
        radius: Theme.radiusM
        color: bgColor
        border.width: 1
        border.color: Qt.alpha(statusColor, 0.3)
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingS
            spacing: Theme.spacingS
            
            Rectangle {
                Layout.preferredWidth: 8
                Layout.preferredHeight: 8
                radius: 4
                color: statusColor
            }
            
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                
                Text {
                    text: label
                    font.pixelSize: Theme.fontSizeS
                    font.weight: Font.Medium
                    color: Theme.textPrimary
                }
                
                Text {
                    text: details
                    font.pixelSize: Theme.fontSizeXS
                    color: Theme.textSecondary
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }
        }
    }
    
    component DiagnosticTestRow: Rectangle {
        property string testName: ""
        property string status: "idle"  // passed, failed, warning, running, idle
        property string lastRun: ""
        
        readonly property color statusColor: status === "passed" ? Theme.successColor :
                                            status === "failed" ? Theme.errorColor :
                                            status === "warning" ? Theme.warningColor :
                                            status === "running" ? Theme.infoColor : Theme.textTertiary
        
        readonly property string statusIcon: status === "passed" ? "✓" :
                                            status === "failed" ? "✗" :
                                            status === "warning" ? "⚠" :
                                            status === "running" ? "⟳" : "○"
        
        implicitHeight: 44
        radius: Theme.radiusS
        color: Theme.surfaceVariant
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: Theme.spacingM
            anchors.rightMargin: Theme.spacingM
            spacing: Theme.spacingM
            
            Text {
                text: statusIcon
                font.pixelSize: Theme.fontSizeM
                color: statusColor
            }
            
            Text {
                text: testName
                font.pixelSize: Theme.fontSizeM
                color: Theme.textPrimary
                Layout.fillWidth: true
            }
            
            Text {
                text: lastRun
                font.pixelSize: Theme.fontSizeXS
                color: Theme.textTertiary
            }
            
            ModernButton {
                text: "Run"
                variant: "ghost"
                size: "small"
            }
        }
    }
}
