import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: amplificationPanel
    
    // Theme properties - Adaptive palette (monochrome/color)
    property color cardBackground: ThemeManager.cardBackground
    property color primaryColor: ThemeManager.primaryColor
    property color primaryHover: ThemeManager.primaryHover
    property color primaryPressed: ThemeManager.primaryPressed
    property color accentColor: ThemeManager.accentColor
    property color successColor: ThemeManager.successColor
    property color warningColor: ThemeManager.warningColor
    property color infoColor: ThemeManager.infoColor
    property color gradientStart: ThemeManager.gradientStart
    property color gradientEnd: ThemeManager.gradientEnd
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color textMuted: ThemeManager.textMuted
    property color borderColor: ThemeManager.borderColor
    property color borderFocus: ThemeManager.borderFocus
    property color inputBackground: ThemeManager.inputBackground
    property color trackColor: ThemeManager.trackColor
    property string fontFamily: ThemeManager.fontFamily
    
    // Color mode flag
    property bool isColorMode: ThemeManager.isColorMode
    
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
    
    // Enhanced shadow for depth
    layer.enabled: true
    layer.effect: DropShadow {
        transparentBorder: true
        horizontalOffset: 0
        verticalOffset: 6
        radius: 20
        samples: 32
        color: ThemeManager.shadowColor
        spread: 0.1
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16
        
        // Enhanced header
        Text {
            text: "Amplification"
            font.pixelSize: 15
            font.weight: Font.DemiBold
            font.family: fontFamily
            font.letterSpacing: -0.2
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
                
                // Amplification display - Classic unified design
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
                
                // Auto enable switch - Uses primary color for unified look
                Rectangle {
                    Layout.fillWidth: true
                    height: 70
                    radius: 12
                    color: autoEnabled ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08) : "transparent"
                    border.color: autoEnabled ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.3) : borderColor
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }
                    
                    ModernSwitch {
                        anchors.fill: parent
                        anchors.margins: 16
                        
                        label: "Enable Automatic Amplification"
                        description: "Automatically adjusts gain based on signal strength"
                        checked: autoEnabled
                        
                        primaryColor: amplificationPanel.primaryColor
                        successColor: amplificationPanel.primaryColor
                        textPrimary: amplificationPanel.textPrimary
                        textSecondary: amplificationPanel.textSecondary
                        trackOffColor: ThemeManager.trackOffColor
                        fontFamily: amplificationPanel.fontFamily
                        
                        onCheckedChanged: autoEnabled = checked
                    }
                }
                
                // Threshold settings - Both use primary color for unified classic look
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16
                    opacity: autoEnabled ? 1.0 : 0.5
                    
                    Rectangle {
                        Layout.fillWidth: true
                        height: 100
                        radius: 12
                        color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.03)
                        border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                        
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
                                
                                primaryColor: amplificationPanel.primaryColor
                                textPrimary: amplificationPanel.textPrimary
                                textSecondary: amplificationPanel.textSecondary
                                borderColor: amplificationPanel.borderColor
                                borderFocus: amplificationPanel.primaryColor
                                backgroundColor: amplificationPanel.inputBackground
                                fontFamily: amplificationPanel.fontFamily
                            }
                        }
                    }
                    
                    Rectangle {
                        Layout.fillWidth: true
                        height: 100
                        radius: 12
                        color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.03)
                        border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                        
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
                                
                                primaryColor: amplificationPanel.primaryColor
                                textPrimary: amplificationPanel.textPrimary
                                textSecondary: amplificationPanel.textSecondary
                                borderColor: amplificationPanel.borderColor
                                borderFocus: amplificationPanel.primaryColor
                                backgroundColor: amplificationPanel.inputBackground
                                fontFamily: amplificationPanel.fontFamily
                            }
                        }
                    }
                }
                
                // Visual indicator - Adaptive gradient (monochrome/color)
                Rectangle {
                    Layout.fillWidth: true
                    height: 12
                    radius: 6
                    
                    // Use LinearGradient for richer color mode experience
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { 
                            position: 0.0
                            color: isColorMode ? Qt.rgba(successColor.r, successColor.g, successColor.b, 0.3) : 
                                                Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15) 
                        }
                        GradientStop { 
                            position: innerThreshold.value / 100.0
                            color: isColorMode ? Qt.rgba(successColor.r, successColor.g, successColor.b, 0.6) :
                                                Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.4) 
                        }
                        GradientStop { 
                            position: innerThreshold.value / 100.0 + 0.01
                            color: isColorMode ? gradientStart : primaryColor 
                        }
                        GradientStop { 
                            position: (innerThreshold.value + outerThreshold.value) / 200.0
                            color: isColorMode ? gradientEnd : primaryColor 
                        }
                        GradientStop { 
                            position: outerThreshold.value / 100.0
                            color: isColorMode ? warningColor : primaryColor 
                        }
                        GradientStop { 
                            position: outerThreshold.value / 100.0 + 0.01
                            color: isColorMode ? Qt.rgba(ThemeManager.errorColor.r, ThemeManager.errorColor.g, ThemeManager.errorColor.b, 0.7) :
                                                Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.6) 
                        }
                        GradientStop { 
                            position: 1.0
                            color: isColorMode ? Qt.rgba(ThemeManager.errorColor.r, ThemeManager.errorColor.g, ThemeManager.errorColor.b, 0.4) :
                                                Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.25) 
                        }
                    }
                    
                    // Inner threshold marker
                    Rectangle {
                        x: (innerThreshold.value / 100.0) * parent.width - 2
                        width: 4
                        height: parent.height + 8
                        y: -4
                        radius: 2
                        color: isColorMode ? successColor : primaryColor
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                    }
                    
                    // Outer threshold marker
                    Rectangle {
                        x: (outerThreshold.value / 100.0) * parent.width - 2
                        width: 4
                        height: parent.height + 8
                        y: -4
                        radius: 2
                        color: isColorMode ? warningColor : primaryColor
                        
                        Behavior on color { ColorAnimation { duration: 200 } }
                    }
                }
            }
        }
        
        // Store to EEPROM button - Uses primary color for classic look
        ModernButton {
            text: "Store to EEPROM"
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            
            primaryColor: amplificationPanel.primaryColor
            primaryHover: amplificationPanel.primaryHover
            primaryPressed: amplificationPanel.primaryPressed
            fontFamily: amplificationPanel.fontFamily
        }
    }
}
