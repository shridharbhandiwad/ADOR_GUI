import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: outputConfigPanel
    
    // Theme properties
    property color cardBackground: "#ffffff"
    property color primaryColor: "#3b82f6"
    property color primaryHover: "#2563eb"
    property color primaryPressed: "#1d4ed8"
    property color accentColor: "#06b6d4"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color textMuted: "#94a3b8"
    property color borderColor: "#e2e8f0"
    property color borderFocus: "#3b82f6"
    property string fontFamily: "Segoe UI"
    
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
                color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.1)
                
                Text {
                    anchors.centerIn: parent
                    text: "⚡"
                    font.pixelSize: 24
                }
            }
            
            Column {
                Layout.fillWidth: true
                spacing: 4
                
                Text {
                    text: "Output Configuration"
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    font.family: fontFamily
                    color: textPrimary
                }
                
                Text {
                    text: "Configure output channels and timing"
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
        
        // Tab bar
        TabBar {
            id: outputTabs
            Layout.fillWidth: true
            background: Rectangle { color: "transparent" }
            
            Repeater {
                model: ["Output 1", "Output 2", "Output 3"]
                
                TabButton {
                    text: modelData
                    width: implicitWidth
                    height: 40
                    
                    contentItem: Text {
                        text: parent.text
                        font.pixelSize: 13
                        font.weight: parent.checked ? Font.DemiBold : Font.Normal
                        font.family: fontFamily
                        color: parent.checked ? primaryColor : textSecondary
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    background: Rectangle {
                        radius: 8
                        color: parent.checked ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : 
                               parent.hovered ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.05) : "transparent"
                        
                        Rectangle {
                            width: parent.width
                            height: 3
                            radius: 1.5
                            color: primaryColor
                            anchors.bottom: parent.bottom
                            visible: parent.parent.checked
                        }
                    }
                }
            }
        }
        
        // Content
        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentHeight: outputContent.height
            clip: true
            
            ColumnLayout {
                id: outputContent
                width: parent.width
                spacing: 20
                
                // Output Type Section
                SectionHeader {
                    Layout.fillWidth: true
                    title: "OUTPUT TYPE"
                    textColor: textSecondary
                    fontFamily: outputConfigPanel.fontFamily
                }
                
                // Radio buttons for output type
                Column {
                    Layout.fillWidth: true
                    spacing: 12
                    
                    Repeater {
                        model: [
                            { text: "Digital", desc: "Standard digital output signal" },
                            { text: "PWM (Velocity)", desc: "Pulse width modulated by velocity" },
                            { text: "PWM (Range)", desc: "Pulse width modulated by range" }
                        ]
                        
                        Rectangle {
                            width: parent.width
                            height: 60
                            radius: 10
                            color: outputTypeGroup.checkedButton && outputTypeGroup.checkedButton.text === modelData.text ? 
                                   Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08) : "transparent"
                            border.color: outputTypeGroup.checkedButton && outputTypeGroup.checkedButton.text === modelData.text ? 
                                          primaryColor : borderColor
                            border.width: outputTypeGroup.checkedButton && outputTypeGroup.checkedButton.text === modelData.text ? 2 : 1
                            
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    for (var i = 0; i < outputTypeGroup.buttons.length; i++) {
                                        if (outputTypeGroup.buttons[i].text === modelData.text) {
                                            outputTypeGroup.buttons[i].checked = true
                                            break
                                        }
                                    }
                                }
                            }
                            
                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 12
                                
                                RadioButton {
                                    id: typeRadio
                                    text: modelData.text
                                    ButtonGroup.group: outputTypeGroup
                                    checked: index === 0
                                    
                                    indicator: Rectangle {
                                        width: 20
                                        height: 20
                                        radius: 10
                                        border.color: typeRadio.checked ? primaryColor : borderColor
                                        border.width: 2
                                        color: "transparent"
                                        
                                        Rectangle {
                                            anchors.centerIn: parent
                                            width: 10
                                            height: 10
                                            radius: 5
                                            color: primaryColor
                                            visible: typeRadio.checked
                                            
                                            scale: typeRadio.checked ? 1.0 : 0.0
                                            Behavior on scale {
                                                NumberAnimation { duration: 150; easing.type: Easing.OutCubic }
                                            }
                                        }
                                    }
                                    
                                    contentItem: Item {}
                                }
                                
                                Column {
                                    Layout.fillWidth: true
                                    spacing: 2
                                    
                                    Text {
                                        text: modelData.text
                                        font.pixelSize: 14
                                        font.weight: Font.Medium
                                        font.family: fontFamily
                                        color: textPrimary
                                    }
                                    
                                    Text {
                                        text: modelData.desc
                                        font.pixelSize: 12
                                        font.family: fontFamily
                                        color: textSecondary
                                    }
                                }
                            }
                        }
                    }
                }
                
                ButtonGroup { id: outputTypeGroup }
                
                // Pin Configuration
                SectionHeader {
                    Layout.fillWidth: true
                    title: "PIN CONFIGURATION"
                    textColor: textSecondary
                    fontFamily: outputConfigPanel.fontFamily
                }
                
                ModernComboBox {
                    Layout.fillWidth: true
                    label: "Configuration"
                    model: [
                        "Active Low - Normally Open",
                        "Active Low - Normally Closed",
                        "Active High - Normally Open",
                        "Active High - Normally Closed",
                        "Totem Pole - Normally Open",
                        "Totem Pole - Normally Closed"
                    ]
                    
                    primaryColor: outputConfigPanel.primaryColor
                    textPrimary: outputConfigPanel.textPrimary
                    textSecondary: outputConfigPanel.textSecondary
                    borderColor: outputConfigPanel.borderColor
                    borderFocus: outputConfigPanel.borderFocus
                    cardBackground: outputConfigPanel.cardBackground
                    fontFamily: outputConfigPanel.fontFamily
                }
                
                // Timing Settings
                SectionHeader {
                    Layout.fillWidth: true
                    title: "TIMING SETTINGS"
                    textColor: textSecondary
                    fontFamily: outputConfigPanel.fontFamily
                }
                
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16
                    
                    ModernSpinBox {
                        id: risingDelay
                        Layout.fillWidth: true
                        label: "Rising Delay (ms)"
                        from: 0
                        to: 5000
                        value: 375
                        
                        primaryColor: outputConfigPanel.primaryColor
                        textPrimary: outputConfigPanel.textPrimary
                        textSecondary: outputConfigPanel.textSecondary
                        borderColor: outputConfigPanel.borderColor
                        borderFocus: outputConfigPanel.borderFocus
                        fontFamily: outputConfigPanel.fontFamily
                    }
                    
                    ModernSpinBox {
                        id: fallingDelay
                        Layout.fillWidth: true
                        label: "Falling Delay (ms)"
                        from: 0
                        to: 5000
                        value: 75
                        
                        primaryColor: outputConfigPanel.primaryColor
                        textPrimary: outputConfigPanel.textPrimary
                        textSecondary: outputConfigPanel.textSecondary
                        borderColor: outputConfigPanel.borderColor
                        borderFocus: outputConfigPanel.borderFocus
                        fontFamily: outputConfigPanel.fontFamily
                    }
                }
            }
        }
        
        // Footer buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            
            Item { Layout.fillWidth: true }
            
            ModernButton {
                text: "Apply"
                Layout.preferredWidth: 100
                
                primaryColor: outputConfigPanel.primaryColor
                primaryHover: outputConfigPanel.primaryHover
                primaryPressed: outputConfigPanel.primaryPressed
                fontFamily: outputConfigPanel.fontFamily
            }
            
            ModernButton {
                text: "Save"
                Layout.preferredWidth: 100
                outline: true
                
                primaryColor: outputConfigPanel.primaryColor
                primaryHover: outputConfigPanel.primaryHover
                primaryPressed: outputConfigPanel.primaryPressed
                fontFamily: outputConfigPanel.fontFamily
            }
        }
    }
}
