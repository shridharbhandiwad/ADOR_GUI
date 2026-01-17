import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../style"

Item {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Status Indicator Component - Elegant Status Display
    // ═══════════════════════════════════════════════════════════════════════════
    
    property string status: "idle"     // idle, active, success, warning, error, loading
    property string label: ""
    property string value: ""
    property bool showPulse: status === "active" || status === "loading"
    property bool compact: false
    
    readonly property color statusColor: {
        switch(status) {
            case "active": return Theme.primaryColor
            case "success": return Theme.successColor
            case "warning": return Theme.warningColor
            case "error": return Theme.errorColor
            case "loading": return Theme.infoColor
            default: return Theme.textTertiary
        }
    }
    
    readonly property color statusBgColor: {
        switch(status) {
            case "active": return Theme.primarySurface
            case "success": return Theme.successSurface
            case "warning": return Theme.warningSurface
            case "error": return Theme.errorSurface
            case "loading": return Theme.infoSurface
            default: return Theme.surfaceVariant
        }
    }
    
    implicitWidth: compact ? indicatorRow.implicitWidth : 200
    implicitHeight: compact ? 28 : 40
    
    Rectangle {
        anchors.fill: parent
        radius: compact ? Theme.radiusFull : Theme.radiusM
        color: statusBgColor
        border.width: 1
        border.color: Qt.alpha(statusColor, 0.3)
        
        Behavior on color {
            ColorAnimation { duration: Theme.animationNormal }
        }
        
        Behavior on border.color {
            ColorAnimation { duration: Theme.animationNormal }
        }
        
        RowLayout {
            id: indicatorRow
            anchors.fill: parent
            anchors.leftMargin: compact ? Theme.spacingS : Theme.spacingM
            anchors.rightMargin: compact ? Theme.spacingS : Theme.spacingM
            spacing: Theme.spacingS
            
            // Status dot with optional pulse
            Item {
                Layout.preferredWidth: compact ? 8 : 12
                Layout.preferredHeight: compact ? 8 : 12
                Layout.alignment: Qt.AlignVCenter
                
                // Pulse animation
                Rectangle {
                    id: pulseCircle
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    radius: width / 2
                    color: statusColor
                    opacity: 0.4
                    visible: showPulse
                    
                    SequentialAnimation on scale {
                        running: showPulse
                        loops: Animation.Infinite
                        NumberAnimation { to: 2; duration: 1000; easing.type: Easing.OutCubic }
                        NumberAnimation { to: 1; duration: 0 }
                    }
                    
                    SequentialAnimation on opacity {
                        running: showPulse
                        loops: Animation.Infinite
                        NumberAnimation { to: 0; duration: 1000; easing.type: Easing.OutCubic }
                        NumberAnimation { to: 0.4; duration: 0 }
                    }
                }
                
                // Main dot
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    radius: width / 2
                    color: statusColor
                    
                    Behavior on color {
                        ColorAnimation { duration: Theme.animationNormal }
                    }
                }
            }
            
            // Label
            Text {
                text: label
                font.pixelSize: compact ? Theme.fontSizeS : Theme.fontSizeM
                font.weight: Font.Medium
                color: Theme.textPrimary
                visible: label !== ""
                Layout.fillWidth: value === ""
            }
            
            // Spacer
            Item {
                Layout.fillWidth: true
                visible: value !== "" && label !== ""
            }
            
            // Value
            Text {
                text: value
                font.pixelSize: compact ? Theme.fontSizeS : Theme.fontSizeM
                font.weight: Font.DemiBold
                color: statusColor
                visible: value !== ""
                
                Behavior on color {
                    ColorAnimation { duration: Theme.animationNormal }
                }
            }
        }
    }
}
