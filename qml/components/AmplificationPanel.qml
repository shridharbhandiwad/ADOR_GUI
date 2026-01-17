import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: amplificationPanel
    
    // Theme properties
    property color cardBackground: "#ffffff"
    property color primaryColor: "#3b82f6"
    property color primaryHover: "#2563eb"
    property color primaryPressed: "#1d4ed8"
    property color accentColor: "#06b6d4"
    property color successColor: "#10b981"
    property color warningColor: "#f59e0b"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color textMuted: "#94a3b8"
    property color borderColor: "#e2e8f0"
    property color borderFocus: "#3b82f6"
    property string fontFamily: "Segoe UI"
    
    // State
    property bool autoEnabled: false
    property int amplificationValue: 20
    
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
        anchors.margins: 24
        spacing: 20
        
        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            Rectangle {
                width: 48
                height: 48
                radius: 12
                color: Qt.rgba(warningColor.r, warningColor.g, warningColor.b, 0.1)
                
                Text {
                    anchors.centerIn: parent
                    text: "📊"
                    font.pixelSize: 24
                }
            }
            
            Column {
                Layout.fillWidth: true
                spacing: 4
                
                Text {
                    text: "Amplification"
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    font.family: fontFamily
                    color: textPrimary
                }
                
                Text {
                    text: "Signal amplification and gain control"
                    font.pixelSize: 13
                    font.family: fontFamily
                    color: textSecondary
                }
            }
        }
        
        // Separator
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: borderColor
        }
        
        // Content
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentHeight: ampContent.height
            clip: true
            
            ColumnLayout {
                id: ampContent
                width: parent.width
                spacing: 20
                
                // Manual Amplification Section
                SectionHeader {
                    Layout.fillWidth: true
                    title: "MANUAL AMPLIFICATION"
                    textColor: textSecondary
                    fontFamily: amplificationPanel.fontFamily
                }
                
                // Amplification display
                Rectangle {
                    Layout.fillWidth: true
                    height: 140
                    radius: 16
                    color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.05)
                    border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                    opacity: autoEnabled ? 0.5 : 1.0
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 16
                        
                        // Large value display
                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 4
                            
                            Text {
                                text: amplificationValue
                                font.pixelSize: 48
                                font.weight: Font.Bold
                                font.family: fontFamily
                                color: primaryColor
                            }
                            
                            Text {
                                text: "dB"
                                font.pixelSize: 20
                                font.weight: Font.Medium
                                font.family: fontFamily
                                color: primaryColor
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 10
                            }
                        }
                        
                        // Slider
                        ModernSlider {
                            width: 300
                            from: 0
                            to: 60
                            value: amplificationValue
                            suffix: " dB"
                            showValue: false
                            enabled: !autoEnabled
                            
                            primaryColor: amplificationPanel.primaryColor
                            textPrimary: amplificationPanel.textPrimary
                            textSecondary: amplificationPanel.textSecondary
                            borderColor: amplificationPanel.borderColor
                            fontFamily: amplificationPanel.fontFamily
                            
                            onValueChanged: amplificationValue = value
                        }
                    }
                }
                
                // Auto Amplification Section
                SectionHeader {
                    Layout.fillWidth: true
                    title: "AUTOMATIC AMPLIFICATION"
                    textColor: textSecondary
                    fontFamily: amplificationPanel.fontFamily
                }
                
                // Auto enable switch
                Rectangle {
                    Layout.fillWidth: true
                    height: 70
                    radius: 12
                    color: autoEnabled ? Qt.rgba(successColor.r, successColor.g, successColor.b, 0.08) : "transparent"
                    border.color: autoEnabled ? Qt.rgba(successColor.r, successColor.g, successColor.b, 0.3) : borderColor
                    
                    ModernSwitch {
                        anchors.fill: parent
                        anchors.margins: 16
                        
                        label: "Enable Automatic Amplification"
                        description: "Automatically adjusts gain based on signal strength"
                        checked: autoEnabled
                        
                        primaryColor: amplificationPanel.primaryColor
                        successColor: amplificationPanel.successColor
                        textPrimary: amplificationPanel.textPrimary
                        textSecondary: amplificationPanel.textSecondary
                        fontFamily: amplificationPanel.fontFamily
                        
                        onCheckedChanged: autoEnabled = checked
                    }
                }
                
                // Threshold settings
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16
                    opacity: autoEnabled ? 1.0 : 0.5
                    
                    Rectangle {
                        Layout.fillWidth: true
                        height: 100
                        radius: 12
                        color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.05)
                        border.color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.2)
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            
                            Text {
                                text: "Inner Threshold"
                                font.pixelSize: 12
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            ModernSpinBox {
                                id: innerThreshold
                                width: 120
                                from: 0
                                to: 100
                                value: 30
                                suffix: " dB"
                                enabled: autoEnabled
                                
                                primaryColor: accentColor
                                textPrimary: amplificationPanel.textPrimary
                                textSecondary: amplificationPanel.textSecondary
                                borderColor: amplificationPanel.borderColor
                                borderFocus: accentColor
                                fontFamily: amplificationPanel.fontFamily
                            }
                        }
                    }
                    
                    Rectangle {
                        Layout.fillWidth: true
                        height: 100
                        radius: 12
                        color: Qt.rgba(warningColor.r, warningColor.g, warningColor.b, 0.05)
                        border.color: Qt.rgba(warningColor.r, warningColor.g, warningColor.b, 0.2)
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            
                            Text {
                                text: "Outer Threshold"
                                font.pixelSize: 12
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            ModernSpinBox {
                                id: outerThreshold
                                width: 120
                                from: 0
                                to: 100
                                value: 70
                                suffix: " dB"
                                enabled: autoEnabled
                                
                                primaryColor: warningColor
                                textPrimary: amplificationPanel.textPrimary
                                textSecondary: amplificationPanel.textSecondary
                                borderColor: amplificationPanel.borderColor
                                borderFocus: warningColor
                                fontFamily: amplificationPanel.fontFamily
                            }
                        }
                    }
                }
                
                // Visual indicator
                Rectangle {
                    Layout.fillWidth: true
                    height: 12
                    radius: 6
                    
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.3) }
                        GradientStop { position: innerThreshold.value / 100.0; color: accentColor }
                        GradientStop { position: innerThreshold.value / 100.0 + 0.01; color: successColor }
                        GradientStop { position: outerThreshold.value / 100.0; color: successColor }
                        GradientStop { position: outerThreshold.value / 100.0 + 0.01; color: warningColor }
                        GradientStop { position: 1.0; color: Qt.rgba(warningColor.r, warningColor.g, warningColor.b, 0.3) }
                    }
                    
                    // Inner threshold marker
                    Rectangle {
                        x: (innerThreshold.value / 100.0) * parent.width - 2
                        width: 4
                        height: parent.height + 8
                        y: -4
                        radius: 2
                        color: accentColor
                    }
                    
                    // Outer threshold marker
                    Rectangle {
                        x: (outerThreshold.value / 100.0) * parent.width - 2
                        width: 4
                        height: parent.height + 8
                        y: -4
                        radius: 2
                        color: warningColor
                    }
                }
            }
        }
        
        // Footer buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            
            ModernButton {
                text: "Store to EEPROM"
                Layout.fillWidth: true
                
                primaryColor: successColor
                primaryHover: Qt.darker(successColor, 1.1)
                primaryPressed: Qt.darker(successColor, 1.2)
                fontFamily: amplificationPanel.fontFamily
            }
            
            ModernButton {
                text: "Apply"
                Layout.preferredWidth: 100
                
                primaryColor: amplificationPanel.primaryColor
                primaryHover: amplificationPanel.primaryHover
                primaryPressed: amplificationPanel.primaryPressed
                fontFamily: amplificationPanel.fontFamily
            }
            
            ModernButton {
                text: "Reset"
                Layout.preferredWidth: 100
                outline: true
                
                primaryColor: amplificationPanel.primaryColor
                primaryHover: amplificationPanel.primaryHover
                primaryPressed: amplificationPanel.primaryPressed
                fontFamily: amplificationPanel.fontFamily
                
                onClicked: {
                    amplificationValue = 20
                    autoEnabled = false
                    innerThreshold.value = 30
                    outerThreshold.value = 70
                }
            }
        }
    }
}
