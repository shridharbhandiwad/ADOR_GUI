import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"
import "../components"

DialogCard {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Configuration Dialog - System Settings & Parameters
    // ═══════════════════════════════════════════════════════════════════════════
    
    title: "Configuration"
    subtitle: "System settings and parameters"
    iconSource: "⚙"
    accentColor: Theme.configCardAccent
    
    contentItem: [
        // Connection Settings Section
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: sectionHeader1.height
            
            Text {
                id: sectionHeader1
                text: "Connection Settings"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
                textFormat: Text.PlainText
            }
            
            Rectangle {
                anchors.left: sectionHeader1.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: sectionHeader1.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        ModernTextField {
            Layout.fillWidth: true
            label: "Host Address"
            placeholderText: "192.168.1.100"
            iconText: "🌐"
            helperText: "Enter the device IP address or hostname"
        },
        
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingM
            
            ModernTextField {
                Layout.fillWidth: true
                Layout.preferredWidth: 150
                label: "Port"
                placeholderText: "5000"
                text: "5000"
            },
            
            ModernComboBox {
                Layout.fillWidth: true
                Layout.preferredWidth: 150
                label: "Protocol"
                model: ["TCP/IP", "UDP", "Modbus TCP", "OPC UA"]
                currentIndex: 0
            }
        },
        
        // Device Settings Section
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: sectionHeader2.height
            Layout.topMargin: Theme.spacingM
            
            Text {
                id: sectionHeader2
                text: "Device Settings"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
                textFormat: Text.PlainText
            }
            
            Rectangle {
                anchors.left: sectionHeader2.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: sectionHeader2.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        ModernComboBox {
            Layout.fillWidth: true
            label: "Device Type"
            model: ["ADOR Controller v1", "ADOR Controller v2", "ADOR Sensor Module", "Custom Device"]
            currentIndex: 0
            helperText: "Select your connected device type"
        },
        
        ModernTextField {
            Layout.fillWidth: true
            label: "Device Name"
            placeholderText: "My ADOR Device"
            text: "ADOR-001"
        },
        
        ModernSwitch {
            Layout.fillWidth: true
            label: "Auto-reconnect"
            description: "Automatically reconnect if connection is lost"
            checked: true
        },
        
        ModernSwitch {
            Layout.fillWidth: true
            label: "Enable Logging"
            description: "Log all communication for debugging"
            checked: false
        },
        
        // Action Buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Theme.spacingL
            spacing: Theme.spacingM
            
            Item { Layout.fillWidth: true }
            
            ModernButton {
                text: "Reset"
                variant: "outline"
            }
            
            ModernButton {
                text: "Apply"
                variant: "primary"
                iconText: "✓"
            }
        }
    ]
}
