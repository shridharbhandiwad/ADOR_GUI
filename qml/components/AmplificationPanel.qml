import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: amplificationPanel
    
    // Theme properties
    property color cardBackground: ThemeManager.cardBackground
    property color primaryColor: ThemeManager.primaryColor
    property color primaryHover: ThemeManager.primaryHover
    property color primaryPressed: ThemeManager.primaryPressed
    property color accentColor: ThemeManager.accentColor
    property color successColor: ThemeManager.successColor
    property color warningColor: ThemeManager.warningColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color textMuted: ThemeManager.textMuted
    property color borderColor: ThemeManager.borderColor
    property color borderFocus: ThemeManager.borderFocus
    property color inputBackground: ThemeManager.inputBackground
    property color trackColor: ThemeManager.trackColor
    property string fontFamily: ThemeManager.fontFamily
    
    // State
    property bool autoEnabled: false
    property int amplificationValue: 20
    
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
        
        // Compact header
        Text {
            text: "Amplification"
            font.pixelSize: 14
            font.weight: Font.DemiBold
            font.family: fontFamily
            color: textPrimary
            
            Behavior on color { ColorAnimation { duration: 200 } }
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
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }
                    
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
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
                            }
                            
                            Text {
                                text: "dB"
                                font.pixelSize: 20
                                font.weight: Font.Medium
                                font.family: fontFamily
                                color: primaryColor
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 10
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
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
                            trackColor: amplificationPanel.trackColor
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
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }
                    
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
                        trackOffColor: ThemeManager.trackOffColor
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
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                        Behavior on border.color { ColorAnimation { duration: 200 } }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            
                            Text {
                                text: "Inner Threshold"
                                font.pixelSize: 12
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
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
                                backgroundColor: amplificationPanel.inputBackground
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
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                        Behavior on border.color { ColorAnimation { duration: 200 } }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            
                            Text {
                                text: "Outer Threshold"
                                font.pixelSize: 12
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Behavior on color { ColorAnimation { duration: 200 } }
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
                                backgroundColor: amplificationPanel.inputBackground
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
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                    }
                    
                    // Outer threshold marker
                    Rectangle {
                        x: (outerThreshold.value / 100.0) * parent.width - 2
                        width: 4
                        height: parent.height + 8
                        y: -4
                        radius: 2
                        color: warningColor
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                    }
                }
            }
        }
        
        // Store to EEPROM button (panel-specific)
        ModernButton {
            text: "Store to EEPROM"
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            
            primaryColor: successColor
            primaryHover: Qt.darker(successColor, 1.1)
            primaryPressed: Qt.darker(successColor, 1.2)
            fontFamily: amplificationPanel.fontFamily
        }
    }
}
