import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import "components"

ApplicationWindow {
    id: root
    visible: true
    width: 1400
    height: 900
    minimumWidth: 1200
    minimumHeight: 800
    title: qsTr("Radar Data Visualization - Configuration Panel")
    
    // Dynamic theme colors from ThemeManager
    readonly property color backgroundColor: ThemeManager.backgroundColor
    readonly property color cardBackground: ThemeManager.cardBackground
    readonly property color primaryColor: ThemeManager.primaryColor
    readonly property color primaryHover: ThemeManager.primaryHover
    readonly property color primaryPressed: ThemeManager.primaryPressed
    readonly property color accentColor: ThemeManager.accentColor
    readonly property color successColor: ThemeManager.successColor
    readonly property color warningColor: ThemeManager.warningColor
    readonly property color errorColor: ThemeManager.errorColor
    readonly property color textPrimary: ThemeManager.textPrimary
    readonly property color textSecondary: ThemeManager.textSecondary
    readonly property color textMuted: ThemeManager.textMuted
    readonly property color borderColor: ThemeManager.borderColor
    readonly property color borderFocus: ThemeManager.borderFocus
    readonly property color shadowColor: ThemeManager.shadowColor
    readonly property color hoverBackground: ThemeManager.hoverBackground
    
    // Fonts
    readonly property string fontFamily: ThemeManager.fontFamily
    
    // Set application background with smooth transition
    color: backgroundColor
    
    Behavior on color {
        ColorAnimation { duration: 200 }
    }
    
    // Header
    header: ToolBar {
        height: 64
        background: Rectangle {
            color: cardBackground
            border.color: borderColor
            border.width: 1
            
            Behavior on color {
                ColorAnimation { duration: 200 }
            }
            
            Behavior on border.color {
                ColorAnimation { duration: 200 }
            }
            
            // Subtle shadow
            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 0
                verticalOffset: 2
                radius: 8
                samples: 17
                color: shadowColor
            }
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 24
            anchors.rightMargin: 24
            spacing: 16
            
            // Logo/Title
            Row {
                spacing: 12
                
                Rectangle {
                    width: 40
                    height: 40
                    radius: 8
                    color: primaryColor
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                    
                    Text {
                        anchors.centerIn: parent
                        text: "R"
                        font.pixelSize: 20
                        font.weight: Font.Bold
                        color: "#ffffff"
                    }
                }
                
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2
                    
                    Text {
                        text: "Radar Visualization"
                        font.pixelSize: 18
                        font.weight: Font.DemiBold
                        font.family: root.fontFamily
                        color: textPrimary
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                    
                    Text {
                        text: "Configuration Dashboard"
                        font.pixelSize: 12
                        font.family: root.fontFamily
                        color: textSecondary
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                }
            }
            
            Item { Layout.fillWidth: true }
            
            // Status indicators
            Row {
                spacing: 24
                
                // Connection status
                Row {
                    spacing: 8
                    
                    Rectangle {
                        width: 10
                        height: 10
                        radius: 5
                        anchors.verticalCenter: parent.verticalCenter
                        color: udpPanel.isConnected ? successColor : errorColor
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                        
                        SequentialAnimation on opacity {
                            running: !udpPanel.isConnected
                            loops: Animation.Infinite
                            NumberAnimation { to: 0.4; duration: 800; easing.type: Easing.InOutQuad }
                            NumberAnimation { to: 1.0; duration: 800; easing.type: Easing.InOutQuad }
                        }
                    }
                    
                    Text {
                        text: udpPanel.isConnected ? "Connected" : "Disconnected"
                        font.pixelSize: 13
                        font.family: root.fontFamily
                        color: textSecondary
                        anchors.verticalCenter: parent.verticalCenter
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                }
                
                // Separator
                Rectangle {
                    width: 1
                    height: 24
                    color: borderColor
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
                
                // Time
                Text {
                    text: Qt.formatDateTime(new Date(), "hh:mm:ss")
                    font.pixelSize: 13
                    font.family: root.fontFamily
                    font.weight: Font.Medium
                    color: textSecondary
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                    
                    Timer {
                        interval: 1000
                        running: true
                        repeat: true
                        onTriggered: parent.text = Qt.formatDateTime(new Date(), "hh:mm:ss")
                    }
                }
                
                // Separator
                Rectangle {
                    width: 1
                    height: 24
                    color: borderColor
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
                
                // Theme indicator
                Row {
                    spacing: 8
                    
                    Text {
                        text: ThemeManager.isDarkTheme ? "🌙" : "☀️"
                        font.pixelSize: 14
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Text {
                        text: ThemeManager.themeName
                        font.pixelSize: 13
                        font.family: root.fontFamily
                        color: textSecondary
                        anchors.verticalCenter: parent.verticalCenter
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                }
                
                // Separator
                Rectangle {
                    width: 1
                    height: 24
                    color: borderColor
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
                
                // Settings button
                Rectangle {
                    id: settingsButton
                    width: 40
                    height: 40
                    radius: 10
                    color: settingsMouseArea.containsMouse ? hoverBackground : "transparent"
                    border.color: settingsMouseArea.containsMouse ? borderColor : "transparent"
                    border.width: 1
                    
                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                    
                    Behavior on border.color {
                        ColorAnimation { duration: 150 }
                    }
                    
                    Text {
                        anchors.centerIn: parent
                        text: "⚙"
                        font.pixelSize: 20
                        color: settingsMouseArea.containsMouse ? primaryColor : textSecondary
                        
                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                        
                        // Subtle rotation animation on hover
                        rotation: settingsMouseArea.containsMouse ? 45 : 0
                        
                        Behavior on rotation {
                            NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                        }
                    }
                    
                    MouseArea {
                        id: settingsMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: settingsPopup.open()
                    }
                    
                    // Tooltip
                    ToolTip {
                        visible: settingsMouseArea.containsMouse
                        text: "Settings"
                        delay: 500
                        
                        background: Rectangle {
                            color: cardBackground
                            radius: 6
                            border.color: borderColor
                            border.width: 1
                            
                            layer.enabled: true
                            layer.effect: DropShadow {
                                transparentBorder: true
                                horizontalOffset: 0
                                verticalOffset: 2
                                radius: 8
                                samples: 17
                                color: shadowColor
                            }
                        }
                        
                        contentItem: Text {
                            text: "Settings"
                            font.pixelSize: 12
                            font.family: root.fontFamily
                            color: textPrimary
                        }
                    }
                }
            }
        }
    }
    
    // Main content - 2x2 Grid Layout
    MainView {
        id: mainView
        anchors.fill: parent
        anchors.margins: 24
        
        backgroundColor: root.backgroundColor
        cardBackground: root.cardBackground
        primaryColor: root.primaryColor
        primaryHover: root.primaryHover
        primaryPressed: root.primaryPressed
        accentColor: root.accentColor
        successColor: root.successColor
        warningColor: root.warningColor
        errorColor: root.errorColor
        textPrimary: root.textPrimary
        textSecondary: root.textSecondary
        textMuted: root.textMuted
        borderColor: root.borderColor
        borderFocus: root.borderFocus
        fontFamily: root.fontFamily
        
        // Reference to UDP panel for status
        property alias udpPanel: mainView.udpConfigPanel
    }
    
    // Alias for header status
    property alias udpPanel: mainView.udpConfigPanel
    
    // Settings popup
    SettingsPopup {
        id: settingsPopup
        anchors.centerIn: parent
    }
}
