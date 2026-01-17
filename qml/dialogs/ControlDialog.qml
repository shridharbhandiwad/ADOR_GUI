import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"
import "../components"

DialogCard {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Control Dialog - Device Commands & Operations
    // ═══════════════════════════════════════════════════════════════════════════
    
    title: "Control"
    subtitle: "Device commands and operations"
    iconSource: "🎛"
    accentColor: Theme.controlCardAccent
    
    property bool isRunning: false
    property bool isConnected: true
    property int operationMode: 0
    
    contentItem: [
        // Quick Actions
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: quickActionsHeader.height
            
            Text {
                id: quickActionsHeader
                text: "Quick Actions"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: quickActionsHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: quickActionsHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        // Start/Stop Row
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingM
            
            ModernButton {
                Layout.fillWidth: true
                text: isRunning ? "Stop" : "Start"
                variant: isRunning ? "danger" : "success"
                iconText: isRunning ? "⏹" : "▶"
                onClicked: isRunning = !isRunning
            }
            
            ModernButton {
                Layout.fillWidth: true
                text: "Pause"
                variant: "secondary"
                iconText: "⏸"
                enabled: isRunning
            }
            
            ModernButton {
                Layout.fillWidth: true
                text: "Reset"
                variant: "outline"
                iconText: "🔄"
            }
        },
        
        // Operation Mode
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: modeHeader.height
            Layout.topMargin: Theme.spacingM
            
            Text {
                id: modeHeader
                text: "Operation Mode"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: modeHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: modeHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        // Mode selector buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingS
            
            Repeater {
                model: ["Auto", "Manual", "Schedule", "Remote"]
                
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    radius: Theme.radiusM
                    color: operationMode === index ? Theme.controlCardAccent : Theme.surfaceVariant
                    border.width: 1
                    border.color: operationMode === index ? Theme.controlCardAccent : Theme.borderColor
                    
                    Behavior on color {
                        ColorAnimation { duration: Theme.animationFast }
                    }
                    
                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        font.pixelSize: Theme.fontSizeS
                        font.weight: operationMode === index ? Font.DemiBold : Font.Normal
                        color: operationMode === index ? Theme.textOnPrimary : Theme.textPrimary
                        
                        Behavior on color {
                            ColorAnimation { duration: Theme.animationFast }
                        }
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: operationMode = index
                    }
                }
            }
        },
        
        // Manual Controls (visible when Manual mode selected)
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: manualHeader.height
            Layout.topMargin: Theme.spacingM
            visible: operationMode === 1
            
            Text {
                id: manualHeader
                text: "Manual Controls"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: manualHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: manualHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        // Slider controls (visible in manual mode)
        ColumnLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingM
            visible: operationMode === 1
            
            SliderControl {
                Layout.fillWidth: true
                label: "Speed"
                value: 50
                unit: "%"
                accentColor: Theme.controlCardAccent
            }
            
            SliderControl {
                Layout.fillWidth: true
                label: "Power"
                value: 75
                unit: "%"
                accentColor: Theme.successColor
            }
            
            SliderControl {
                Layout.fillWidth: true
                label: "Sensitivity"
                value: 30
                unit: "%"
                accentColor: Theme.infoColor
            }
        },
        
        // Device Toggles
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: toggleHeader.height
            Layout.topMargin: Theme.spacingM
            
            Text {
                id: toggleHeader
                text: "Device Features"
                font.pixelSize: Theme.fontSizeS
                font.weight: Font.Bold
                color: Theme.textSecondary
            }
            
            Rectangle {
                anchors.left: toggleHeader.right
                anchors.leftMargin: Theme.spacingM
                anchors.right: parent.right
                anchors.verticalCenter: toggleHeader.verticalCenter
                height: 1
                color: Theme.dividerColor
            }
        },
        
        ModernSwitch {
            Layout.fillWidth: true
            label: "LED Indicator"
            checked: true
            accentColor: Theme.controlCardAccent
        },
        
        ModernSwitch {
            Layout.fillWidth: true
            label: "Buzzer Alerts"
            checked: true
            accentColor: Theme.controlCardAccent
        },
        
        ModernSwitch {
            Layout.fillWidth: true
            label: "Auto-shutdown"
            description: "Shutdown after 30 min of inactivity"
            checked: false
            accentColor: Theme.controlCardAccent
        },
        
        // Command Buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Theme.spacingL
            spacing: Theme.spacingM
            
            ModernButton {
                text: "Send Command"
                variant: "primary"
                iconText: "📤"
            }
            
            Item { Layout.fillWidth: true }
            
            ModernButton {
                text: "Emergency Stop"
                variant: "danger"
                iconText: "🛑"
            }
        }
    ]
    
    // ───────────────────────────────────────────────────────────────────────────
    // Custom Slider Control Component
    // ───────────────────────────────────────────────────────────────────────────
    
    component SliderControl: Item {
        property string label: ""
        property real value: 50
        property real minValue: 0
        property real maxValue: 100
        property string unit: "%"
        property color accentColor: Theme.primaryColor
        
        implicitHeight: 50
        
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
                    text: slider.value.toFixed(0) + unit
                    font.pixelSize: Theme.fontSizeM
                    font.weight: Font.DemiBold
                    color: accentColor
                }
            }
            
            Slider {
                id: slider
                Layout.fillWidth: true
                from: minValue
                to: maxValue
                value: parent.parent.value
                stepSize: 1
                
                background: Rectangle {
                    x: slider.leftPadding
                    y: slider.topPadding + slider.availableHeight / 2 - height / 2
                    width: slider.availableWidth
                    height: 6
                    radius: 3
                    color: Theme.surfaceVariant
                    
                    Rectangle {
                        width: slider.visualPosition * parent.width
                        height: parent.height
                        radius: parent.radius
                        color: accentColor
                    }
                }
                
                handle: Rectangle {
                    x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                    y: slider.topPadding + slider.availableHeight / 2 - height / 2
                    width: 18
                    height: 18
                    radius: 9
                    color: slider.pressed ? Qt.darker(accentColor, 1.1) : Theme.surfaceColor
                    border.width: 3
                    border.color: accentColor
                    
                    Behavior on color {
                        ColorAnimation { duration: Theme.animationFast }
                    }
                }
            }
        }
    }
}
