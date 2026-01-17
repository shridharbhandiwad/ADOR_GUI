import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls.Material
import "style"
import "components"
import "dialogs"

ApplicationWindow {
    id: mainWindow
    
    // ═══════════════════════════════════════════════════════════════════════════
    // ADOR GUI - Main Application Window
    // Elegant Light Theme with 2x2 Dialog Layout
    // ═══════════════════════════════════════════════════════════════════════════
    
    width: 1400
    height: 900
    minimumWidth: 1000
    minimumHeight: 700
    visible: true
    title: qsTr("ADOR Control Panel")
    
    // Material Design Light Theme Configuration
    Material.theme: Material.Light
    Material.accent: Theme.primaryColor
    Material.primary: Theme.primaryColor
    Material.background: Theme.backgroundColor
    Material.foreground: Theme.textPrimary
    
    // Window background
    color: Theme.backgroundColor
    
    // ───────────────────────────────────────────────────────────────────────────
    // Main Container
    // ───────────────────────────────────────────────────────────────────────────
    
    Item {
        anchors.fill: parent
        
        // Subtle background pattern
        Rectangle {
            anchors.fill: parent
            color: Theme.backgroundColor
            
            // Subtle grid pattern
            Canvas {
                anchors.fill: parent
                opacity: 0.3
                
                onPaint: {
                    var ctx = getContext("2d")
                    ctx.clearRect(0, 0, width, height)
                    ctx.strokeStyle = Theme.borderLight
                    ctx.lineWidth = 0.5
                    
                    var gridSize = 40
                    
                    for (var x = 0; x < width; x += gridSize) {
                        ctx.beginPath()
                        ctx.moveTo(x, 0)
                        ctx.lineTo(x, height)
                        ctx.stroke()
                    }
                    
                    for (var y = 0; y < height; y += gridSize) {
                        ctx.beginPath()
                        ctx.moveTo(0, y)
                        ctx.lineTo(width, y)
                        ctx.stroke()
                    }
                }
            }
        }
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 0
            spacing: 0
            
            // ═══════════════════════════════════════════════════════════════════
            // Application Header
            // ═══════════════════════════════════════════════════════════════════
            
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 64
                color: Theme.surfaceColor
                
                // Bottom border
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: Theme.dividerColor
                }
                
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: Theme.spacingXL
                    anchors.rightMargin: Theme.spacingXL
                    spacing: Theme.spacingL
                    
                    // Logo and Title
                    RowLayout {
                        spacing: Theme.spacingM
                        
                        // Logo icon
                        Rectangle {
                            Layout.preferredWidth: 40
                            Layout.preferredHeight: 40
                            radius: Theme.radiusM
                            gradient: Gradient {
                                orientation: Gradient.Horizontal
                                GradientStop { position: 0.0; color: Theme.primaryColor }
                                GradientStop { position: 1.0; color: Theme.accentColor }
                            }
                            
                            Text {
                                anchors.centerIn: parent
                                text: "A"
                                font.pixelSize: 20
                                font.weight: Font.Bold
                                color: Theme.textOnPrimary
                            }
                        }
                        
                        ColumnLayout {
                            spacing: 0
                            
                            Text {
                                text: "ADOR Control Panel"
                                font.pixelSize: Theme.fontSizeXL
                                font.weight: Font.DemiBold
                                color: Theme.textPrimary
                            }
                            
                            Text {
                                text: "Professional Device Management"
                                font.pixelSize: Theme.fontSizeS
                                color: Theme.textSecondary
                            }
                        }
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    // Connection Status
                    StatusIndicator {
                        status: "active"
                        label: "Connected"
                        value: "192.168.1.100"
                        compact: true
                    }
                    
                    // Separator
                    Rectangle {
                        Layout.preferredWidth: 1
                        Layout.preferredHeight: 32
                        color: Theme.dividerColor
                    }
                    
                    // Settings Button
                    Rectangle {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 40
                        radius: Theme.radiusM
                        color: settingsBtn.containsMouse ? Theme.surfaceVariant : "transparent"
                        
                        Text {
                            anchors.centerIn: parent
                            text: "⚙"
                            font.pixelSize: Theme.iconSizeL
                            color: Theme.textSecondary
                        }
                        
                        MouseArea {
                            id: settingsBtn
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                    
                    // Help Button
                    Rectangle {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 40
                        radius: Theme.radiusM
                        color: helpBtn.containsMouse ? Theme.surfaceVariant : "transparent"
                        
                        Text {
                            anchors.centerIn: parent
                            text: "?"
                            font.pixelSize: Theme.fontSizeL
                            font.weight: Font.Bold
                            color: Theme.textSecondary
                        }
                        
                        MouseArea {
                            id: helpBtn
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                        }
                    }
                }
            }
            
            // ═══════════════════════════════════════════════════════════════════
            // Main Content Area - 2x2 Grid Layout
            // ═══════════════════════════════════════════════════════════════════
            
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: Theme.spacingM
                
                GridLayout {
                    anchors.fill: parent
                    columns: 2
                    rows: 2
                    columnSpacing: Theme.spacingS
                    rowSpacing: Theme.spacingS
                    
                    // ─────────────────────────────────────────────────────────────
                    // Top-Left: Configuration Dialog
                    // ─────────────────────────────────────────────────────────────
                    ConfigurationDialog {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.row: 0
                        Layout.column: 0
                    }
                    
                    // ─────────────────────────────────────────────────────────────
                    // Top-Right: Monitoring Dialog
                    // ─────────────────────────────────────────────────────────────
                    MonitoringDialog {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.row: 0
                        Layout.column: 1
                    }
                    
                    // ─────────────────────────────────────────────────────────────
                    // Bottom-Left: Control Dialog
                    // ─────────────────────────────────────────────────────────────
                    ControlDialog {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.row: 1
                        Layout.column: 0
                    }
                    
                    // ─────────────────────────────────────────────────────────────
                    // Bottom-Right: Diagnostics Dialog
                    // ─────────────────────────────────────────────────────────────
                    DiagnosticsDialog {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.row: 1
                        Layout.column: 1
                    }
                }
            }
            
            // ═══════════════════════════════════════════════════════════════════
            // Footer / Status Bar
            // ═══════════════════════════════════════════════════════════════════
            
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 32
                color: Theme.surfaceColor
                
                // Top border
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: Theme.dividerColor
                }
                
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: Theme.spacingL
                    anchors.rightMargin: Theme.spacingL
                    spacing: Theme.spacingXL
                    
                    // Status items
                    RowLayout {
                        spacing: Theme.spacingS
                        
                        Rectangle {
                            width: 8
                            height: 8
                            radius: 4
                            color: Theme.successColor
                        }
                        
                        Text {
                            text: "System Ready"
                            font.pixelSize: Theme.fontSizeS
                            color: Theme.textSecondary
                        }
                    }
                    
                    Rectangle {
                        width: 1
                        height: 16
                        color: Theme.dividerColor
                    }
                    
                    Text {
                        text: "Last sync: 2 seconds ago"
                        font.pixelSize: Theme.fontSizeS
                        color: Theme.textTertiary
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Text {
                        text: "ADOR GUI v" + (typeof appVersion !== 'undefined' ? appVersion : "1.0.0")
                        font.pixelSize: Theme.fontSizeS
                        color: Theme.textTertiary
                    }
                }
            }
        }
    }
}
