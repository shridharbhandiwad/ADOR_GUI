import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Popup {
    id: settingsPopup
    
    width: 320
    height: contentColumn.implicitHeight + 48
    padding: 24
    
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    
    // Theme properties
    property color cardBackground: ThemeManager.cardBackground
    property color primaryColor: ThemeManager.primaryColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color borderColor: ThemeManager.borderColor
    property color hoverBackground: ThemeManager.hoverBackground
    property color dividerColor: ThemeManager.dividerColor
    property string fontFamily: ThemeManager.fontFamily
    
    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
        NumberAnimation { property: "scale"; from: 0.95; to: 1.0; duration: 200; easing.type: Easing.OutCubic }
    }
    
    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 150; easing.type: Easing.InCubic }
        NumberAnimation { property: "scale"; from: 1.0; to: 0.95; duration: 150; easing.type: Easing.InCubic }
    }
    
    background: Rectangle {
        color: cardBackground
        radius: 16
        border.color: borderColor
        border.width: 1
        
        layer.enabled: true
        layer.effect: DropShadow {
            transparentBorder: true
            horizontalOffset: 0
            verticalOffset: 8
            radius: 24
            samples: 49
            color: ThemeManager.shadowColor
        }
        
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }
    
    contentItem: ColumnLayout {
        id: contentColumn
        spacing: 20
        
        // Header
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            
            // Settings icon
            Rectangle {
                width: 40
                height: 40
                radius: 10
                color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                
                // Gear icon using Canvas for reliable rendering
                Canvas {
                    anchors.centerIn: parent
                    width: 20
                    height: 20
                    
                    property color iconColor: primaryColor
                    
                    onIconColorChanged: requestPaint()
                    Component.onCompleted: requestPaint()
                    
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.reset();
                        ctx.fillStyle = iconColor;
                        
                        var cx = width / 2;
                        var cy = height / 2;
                        var outerRadius = 9;
                        var innerRadius = 3.5;
                        var teethCount = 8;
                        var toothDepth = 2.5;
                        
                        ctx.beginPath();
                        for (var i = 0; i < teethCount * 2; i++) {
                            var angle = (i * Math.PI) / teethCount;
                            var radius = (i % 2 === 0) ? outerRadius : outerRadius - toothDepth;
                            var x = cx + radius * Math.cos(angle - Math.PI / 2);
                            var y = cy + radius * Math.sin(angle - Math.PI / 2);
                            if (i === 0) {
                                ctx.moveTo(x, y);
                            } else {
                                ctx.lineTo(x, y);
                            }
                        }
                        ctx.closePath();
                        ctx.fill();
                        
                        // Center hole
                        ctx.globalCompositeOperation = "destination-out";
                        ctx.beginPath();
                        ctx.arc(cx, cy, innerRadius, 0, 2 * Math.PI);
                        ctx.fill();
                    }
                }
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }
            
            Column {
                Layout.fillWidth: true
                spacing: 2
                
                Text {
                    text: "Settings"
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    font.family: fontFamily
                    color: textPrimary
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
                
                Text {
                    text: "Customize your experience"
                    font.pixelSize: 12
                    font.family: fontFamily
                    color: textSecondary
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
            }
            
            // Close button
            Rectangle {
                width: 32
                height: 32
                radius: 8
                color: closeMouseArea.containsMouse ? hoverBackground : "transparent"
                
                Text {
                    anchors.centerIn: parent
                    text: "✕"
                    font.pixelSize: 14
                    color: textSecondary
                }
                
                MouseArea {
                    id: closeMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: settingsPopup.close()
                }
                
                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }
        }
        
        // Divider
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: dividerColor
            
            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }
        
        // Theme Section
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 16
            
            // Section Title
            Text {
                text: "APPEARANCE"
                font.pixelSize: 11
                font.weight: Font.DemiBold
                font.family: fontFamily
                font.letterSpacing: 1
                color: textSecondary
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }
            
            // Theme Toggle
            Rectangle {
                Layout.fillWidth: true
                height: 72
                radius: 12
                color: themeRowMouseArea.containsMouse ? hoverBackground : "transparent"
                border.color: borderColor
                border.width: 1
                
                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
                
                MouseArea {
                    id: themeRowMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: ThemeManager.toggleTheme()
                }
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16
                    
                    // Theme icon
                    Rectangle {
                        width: 40
                        height: 40
                        radius: 10
                        color: ThemeManager.isDarkTheme ? "#1e293b" : "#fef3c7"
                        border.color: ThemeManager.isDarkTheme ? "#475569" : "#fcd34d"
                        border.width: 1
                        
                        Text {
                            anchors.centerIn: parent
                            text: ThemeManager.isDarkTheme ? "🌙" : "☀️"
                            font.pixelSize: 18
                        }
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                        
                        Behavior on border.color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                    
                    Column {
                        Layout.fillWidth: true
                        spacing: 4
                        
                        Text {
                            text: "Theme"
                            font.pixelSize: 14
                            font.weight: Font.Medium
                            font.family: fontFamily
                            color: textPrimary
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                        
                        Text {
                            text: ThemeManager.isDarkTheme ? "Dark mode enabled" : "Light mode enabled"
                            font.pixelSize: 12
                            font.family: fontFamily
                            color: textSecondary
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                    }
                    
                    // Toggle Switch
                    Rectangle {
                        width: 52
                        height: 28
                        radius: 14
                        color: ThemeManager.isDarkTheme ? primaryColor : "#cbd5e1"
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                        
                        Rectangle {
                            x: ThemeManager.isDarkTheme ? parent.width - width - 4 : 4
                            y: 4
                            width: 20
                            height: 20
                            radius: 10
                            color: "#ffffff"
                            
                            layer.enabled: true
                            layer.effect: DropShadow {
                                transparentBorder: true
                                horizontalOffset: 0
                                verticalOffset: 1
                                radius: 4
                                samples: 9
                                color: "#0000002a"
                            }
                            
                            Behavior on x {
                                NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                            }
                        }
                    }
                }
            }
            
            // Theme Preview Cards
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                
                // Light theme preview
                Rectangle {
                    Layout.fillWidth: true
                    height: 80
                    radius: 10
                    color: "#ffffff"
                    border.color: !ThemeManager.isDarkTheme ? primaryColor : "#e2e8f0"
                    border.width: !ThemeManager.isDarkTheme ? 2 : 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ThemeManager.setTheme(false)
                    }
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 8
                        
                        Rectangle {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 32
                            height: 32
                            radius: 8
                            color: "#f8fafc"
                            border.color: "#e2e8f0"
                            border.width: 1
                            
                            Text {
                                anchors.centerIn: parent
                                text: "☀️"
                                font.pixelSize: 14
                            }
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Light"
                            font.pixelSize: 12
                            font.weight: !ThemeManager.isDarkTheme ? Font.DemiBold : Font.Normal
                            font.family: fontFamily
                            color: "#1e293b"
                        }
                    }
                    
                    // Checkmark for selected
                    Rectangle {
                        visible: !ThemeManager.isDarkTheme
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: 6
                        width: 20
                        height: 20
                        radius: 10
                        color: primaryColor
                        
                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 12
                            font.weight: Font.Bold
                            color: "#ffffff"
                        }
                    }
                    
                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                }
                
                // Dark theme preview
                Rectangle {
                    Layout.fillWidth: true
                    height: 80
                    radius: 10
                    color: "#1e293b"
                    border.color: ThemeManager.isDarkTheme ? primaryColor : "#334155"
                    border.width: ThemeManager.isDarkTheme ? 2 : 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ThemeManager.setTheme(true)
                    }
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 8
                        
                        Rectangle {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 32
                            height: 32
                            radius: 8
                            color: "#0f172a"
                            border.color: "#334155"
                            border.width: 1
                            
                            Text {
                                anchors.centerIn: parent
                                text: "🌙"
                                font.pixelSize: 14
                            }
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Dark"
                            font.pixelSize: 12
                            font.weight: ThemeManager.isDarkTheme ? Font.DemiBold : Font.Normal
                            font.family: fontFamily
                            color: "#f1f5f9"
                        }
                    }
                    
                    // Checkmark for selected
                    Rectangle {
                        visible: ThemeManager.isDarkTheme
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: 6
                        width: 20
                        height: 20
                        radius: 10
                        color: primaryColor
                        
                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 12
                            font.weight: Font.Bold
                            color: "#ffffff"
                        }
                    }
                    
                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                }
            }
        }
    }
}
