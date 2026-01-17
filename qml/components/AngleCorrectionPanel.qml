import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: angleCorrectionPanel
    
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
    
    // State
    property bool useAngleMethod: true
    
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
                color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                
                Text {
                    anchors.centerIn: parent
                    text: "📐"
                    font.pixelSize: 24
                }
            }
            
            Column {
                Layout.fillWidth: true
                spacing: 4
                
                Text {
                    text: "Angle Correction"
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    font.family: fontFamily
                    color: textPrimary
                }
                
                Text {
                    text: "Mounting angle and height compensation"
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
            contentHeight: angleContent.height
            clip: true
            
            ColumnLayout {
                id: angleContent
                width: parent.width
                spacing: 20
                
                // Method Selection
                SectionHeader {
                    Layout.fillWidth: true
                    title: "CORRECTION METHOD"
                    textColor: textSecondary
                    fontFamily: angleCorrectionPanel.fontFamily
                }
                
                // Method cards
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    
                    // Angle method card
                    Rectangle {
                        Layout.fillWidth: true
                        height: 120
                        radius: 12
                        color: useAngleMethod ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08) : "transparent"
                        border.color: useAngleMethod ? primaryColor : borderColor
                        border.width: useAngleMethod ? 2 : 1
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: useAngleMethod = true
                            cursorShape: Qt.PointingHandCursor
                        }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            
                            Rectangle {
                                width: 40
                                height: 40
                                radius: 20
                                color: useAngleMethod ? primaryColor : borderColor
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: "∠"
                                    font.pixelSize: 20
                                    font.weight: Font.Bold
                                    color: "#ffffff"
                                }
                            }
                            
                            Text {
                                text: "Mounting Angle"
                                font.pixelSize: 14
                                font.weight: Font.Medium
                                font.family: fontFamily
                                color: useAngleMethod ? primaryColor : textPrimary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            Text {
                                text: "Correct using tilt angle"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                    
                    // Height method card
                    Rectangle {
                        Layout.fillWidth: true
                        height: 120
                        radius: 12
                        color: !useAngleMethod ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08) : "transparent"
                        border.color: !useAngleMethod ? primaryColor : borderColor
                        border.width: !useAngleMethod ? 2 : 1
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: useAngleMethod = false
                            cursorShape: Qt.PointingHandCursor
                        }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 8
                            
                            Rectangle {
                                width: 40
                                height: 40
                                radius: 20
                                color: !useAngleMethod ? primaryColor : borderColor
                                anchors.horizontalCenter: parent.horizontalCenter
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: "↕"
                                    font.pixelSize: 20
                                    font.weight: Font.Bold
                                    color: "#ffffff"
                                }
                            }
                            
                            Text {
                                text: "Mounting Height"
                                font.pixelSize: 14
                                font.weight: Font.Medium
                                font.family: fontFamily
                                color: !useAngleMethod ? primaryColor : textPrimary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            
                            Text {
                                text: "Correct using height"
                                font.pixelSize: 11
                                font.family: fontFamily
                                color: textSecondary
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }
                    }
                }
                
                // Parameters Section
                SectionHeader {
                    Layout.fillWidth: true
                    title: "PARAMETERS"
                    textColor: textSecondary
                    fontFamily: angleCorrectionPanel.fontFamily
                }
                
                // Angle parameter
                Rectangle {
                    Layout.fillWidth: true
                    height: 100
                    radius: 12
                    color: useAngleMethod ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.05) : "#f1f5f9"
                    border.color: useAngleMethod ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.2) : borderColor
                    opacity: useAngleMethod ? 1.0 : 0.5
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        Column {
                            Layout.fillWidth: true
                            spacing: 4
                            
                            Text {
                                text: "Mounting Angle"
                                font.pixelSize: 14
                                font.weight: Font.Medium
                                font.family: fontFamily
                                color: textPrimary
                            }
                            
                            Text {
                                text: "Tilt angle of the radar sensor in degrees"
                                font.pixelSize: 12
                                font.family: fontFamily
                                color: textSecondary
                            }
                        }
                        
                        ModernSpinBox {
                            id: angleSpinBox
                            Layout.preferredWidth: 120
                            from: 0
                            to: 90
                            value: 15
                            suffix: "°"
                            enabled: useAngleMethod
                            
                            primaryColor: angleCorrectionPanel.primaryColor
                            textPrimary: angleCorrectionPanel.textPrimary
                            textSecondary: angleCorrectionPanel.textSecondary
                            borderColor: angleCorrectionPanel.borderColor
                            borderFocus: angleCorrectionPanel.borderFocus
                            fontFamily: angleCorrectionPanel.fontFamily
                        }
                    }
                }
                
                // Height parameter
                Rectangle {
                    Layout.fillWidth: true
                    height: 100
                    radius: 12
                    color: !useAngleMethod ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.05) : "#f1f5f9"
                    border.color: !useAngleMethod ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.2) : borderColor
                    opacity: !useAngleMethod ? 1.0 : 0.5
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        Column {
                            Layout.fillWidth: true
                            spacing: 4
                            
                            Text {
                                text: "Mounting Height"
                                font.pixelSize: 14
                                font.weight: Font.Medium
                                font.family: fontFamily
                                color: textPrimary
                            }
                            
                            Text {
                                text: "Height of the radar sensor from ground"
                                font.pixelSize: 12
                                font.family: fontFamily
                                color: textSecondary
                            }
                        }
                        
                        ModernSpinBox {
                            id: heightSpinBox
                            Layout.preferredWidth: 120
                            from: 0
                            to: 500
                            value: 30
                            suffix: " m"
                            enabled: !useAngleMethod
                            
                            primaryColor: angleCorrectionPanel.primaryColor
                            textPrimary: angleCorrectionPanel.textPrimary
                            textSecondary: angleCorrectionPanel.textSecondary
                            borderColor: angleCorrectionPanel.borderColor
                            borderFocus: angleCorrectionPanel.borderFocus
                            fontFamily: angleCorrectionPanel.fontFamily
                        }
                    }
                }
                
                // Visualization hint
                Rectangle {
                    Layout.fillWidth: true
                    height: 60
                    radius: 10
                    color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.1)
                    border.color: Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.3)
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        
                        Text {
                            text: "💡"
                            font.pixelSize: 20
                        }
                        
                        Text {
                            Layout.fillWidth: true
                            text: "The correction ensures accurate distance measurements regardless of sensor mounting position."
                            font.pixelSize: 12
                            font.family: fontFamily
                            color: textSecondary
                            wrapMode: Text.WordWrap
                        }
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
                
                primaryColor: angleCorrectionPanel.primaryColor
                primaryHover: angleCorrectionPanel.primaryHover
                primaryPressed: angleCorrectionPanel.primaryPressed
                fontFamily: angleCorrectionPanel.fontFamily
            }
            
            ModernButton {
                text: "Reset"
                Layout.preferredWidth: 100
                outline: true
                
                primaryColor: angleCorrectionPanel.primaryColor
                primaryHover: angleCorrectionPanel.primaryHover
                primaryPressed: angleCorrectionPanel.primaryPressed
                fontFamily: angleCorrectionPanel.fontFamily
                
                onClicked: {
                    useAngleMethod = true
                    angleSpinBox.value = 0
                    heightSpinBox.value = 30
                }
            }
        }
    }
}
