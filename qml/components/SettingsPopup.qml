import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Popup {
    id: settingsPopup
    
    width: 400
    // Use a maximum height to ensure popup fits in the window with scrolling
    height: Math.min(contentColumn.implicitHeight + 48, parent ? parent.height - 80 : 700)
    padding: 24
    
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    
    // Theme properties
    property color cardBackground: ThemeManager.cardBackground
    property color primaryColor: ThemeManager.primaryColor
    property color accentColor: ThemeManager.accentColor
    property color successColor: ThemeManager.successColor
    property color warningColor: ThemeManager.warningColor
    property color accent2Color: ThemeManager.accent2Color
    property color accent3Color: ThemeManager.accent3Color
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color borderColor: ThemeManager.borderColor
    property color hoverBackground: ThemeManager.hoverBackground
    property color dividerColor: ThemeManager.dividerColor
    property color gradientStart: ThemeManager.gradientStart
    property color gradientEnd: ThemeManager.gradientEnd
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
    
    contentItem: Flickable {
        id: flickableContent
        contentWidth: width
        contentHeight: contentColumn.implicitHeight
        clip: true
        boundsBehavior: Flickable.StopAtBounds
        
        // Enable smooth scrolling
        ScrollBar.vertical: ScrollBar {
            id: verticalScrollBar
            policy: flickableContent.contentHeight > flickableContent.height ? ScrollBar.AsNeeded : ScrollBar.AlwaysOff
            
            contentItem: Rectangle {
                implicitWidth: 6
                implicitHeight: 100
                radius: 3
                color: verticalScrollBar.pressed ? primaryColor : (verticalScrollBar.hovered ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.7) : borderColor)
                opacity: verticalScrollBar.policy === ScrollBar.AlwaysOff ? 0 : 1
                
                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }
            
            background: Rectangle {
                implicitWidth: 6
                radius: 3
                color: "transparent"
            }
        }
        
        ColumnLayout {
            id: contentColumn
            width: flickableContent.width - (verticalScrollBar.policy === ScrollBar.AsNeeded ? 12 : 0)
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
                        font.pixelSize: 20
                        font.weight: Font.DemiBold
                        font.family: fontFamily
                        color: textPrimary
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                    
                    Text {
                        text: "Customize your experience"
                        font.pixelSize: 16
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
                        font.pixelSize: 16
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
                    font.pixelSize: 13
                font.weight: Font.DemiBold
                font.family: fontFamily
                font.letterSpacing: 1
                color: textSecondary
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }
            
            // Theme Toggle - Elegant monochrome design
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
                    
                    // Theme icon - elegant monochrome design
                    Rectangle {
                        width: 40
                        height: 40
                        radius: 10
                        color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                        border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.2)
                        border.width: 1
                        
                        // Half-filled circle representing theme
                        Canvas {
                            anchors.centerIn: parent
                            width: 20
                            height: 20
                            
                            property color iconColor: primaryColor
                            property bool darkMode: ThemeManager.isDarkTheme
                            
                            onIconColorChanged: requestPaint()
                            onDarkModeChanged: requestPaint()
                            Component.onCompleted: requestPaint()
                            
                            onPaint: {
                                var ctx = getContext("2d");
                                ctx.reset();
                                var cx = width / 2;
                                var cy = height / 2;
                                var r = 8;
                                
                                // Draw circle outline
                                ctx.strokeStyle = iconColor;
                                ctx.lineWidth = 1.5;
                                ctx.beginPath();
                                ctx.arc(cx, cy, r, 0, 2 * Math.PI);
                                ctx.stroke();
                                
                                // Fill half based on theme
                                ctx.fillStyle = iconColor;
                                ctx.beginPath();
                                if (darkMode) {
                                    // Fill left half for dark mode
                                    ctx.arc(cx, cy, r - 1, Math.PI / 2, 3 * Math.PI / 2);
                                } else {
                                    // Fill right half for light mode  
                                    ctx.arc(cx, cy, r - 1, -Math.PI / 2, Math.PI / 2);
                                }
                                ctx.fill();
                            }
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
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            font.family: fontFamily
                            color: textPrimary
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                        
                        Text {
                            text: ThemeManager.isDarkTheme ? "Dark mode enabled" : "Light mode enabled"
                            font.pixelSize: 14
                            font.family: fontFamily
                            color: textSecondary
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                    }
                    
                    // Toggle Switch - monochrome design
                    Rectangle {
                        width: 52
                        height: 28
                        radius: 14
                        color: ThemeManager.isDarkTheme ? primaryColor : ThemeManager.trackOffColor
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                        
                        Rectangle {
                            x: ThemeManager.isDarkTheme ? parent.width - width - 4 : 4
                            y: 4
                            width: 20
                            height: 20
                            radius: 10
                            color: ThemeManager.switchKnobColor
                            
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
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                    }
                }
            }
            
            // Theme Preview Cards - Clean monochrome design
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                
                // Light theme preview
                Rectangle {
                    Layout.fillWidth: true
                    height: 80
                    radius: 10
                    color: "#fafafa"
                    border.color: !ThemeManager.isDarkTheme ? "#1a1a1a" : "#e5e5e5"
                    border.width: !ThemeManager.isDarkTheme ? 2 : 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ThemeManager.setTheme(false)
                    }
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 10
                        
                        // Elegant icon representation
                        Rectangle {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 28
                            height: 28
                            radius: 14
                            color: "#1a1a1a"
                            
                            // Inner white circle
                            Rectangle {
                                anchors.centerIn: parent
                                width: 12
                                height: 12
                                radius: 6
                                color: "#fafafa"
                            }
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Light"
                            font.pixelSize: 13
                            font.weight: !ThemeManager.isDarkTheme ? Font.DemiBold : Font.Normal
                            font.family: fontFamily
                            font.letterSpacing: 0.5
                            color: "#1a1a1a"
                        }
                    }
                    
                    // Checkmark for selected
                    Rectangle {
                        visible: !ThemeManager.isDarkTheme
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: 6
                        width: 18
                        height: 18
                        radius: 9
                        color: "#1a1a1a"
                        
                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 11
                            font.weight: Font.Bold
                            color: "#fafafa"
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
                    color: "#171717"
                    border.color: ThemeManager.isDarkTheme ? "#fafafa" : "#404040"
                    border.width: ThemeManager.isDarkTheme ? 2 : 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ThemeManager.setTheme(true)
                    }
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 10
                        
                        // Elegant icon representation (inverted)
                        Rectangle {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 28
                            height: 28
                            radius: 14
                            color: "#fafafa"
                            
                            // Inner dark circle
                            Rectangle {
                                anchors.centerIn: parent
                                width: 12
                                height: 12
                                radius: 6
                                color: "#171717"
                            }
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Dark"
                            font.pixelSize: 13
                            font.weight: ThemeManager.isDarkTheme ? Font.DemiBold : Font.Normal
                            font.family: fontFamily
                            font.letterSpacing: 0.5
                            color: "#fafafa"
                        }
                    }
                    
                    // Checkmark for selected
                    Rectangle {
                        visible: ThemeManager.isDarkTheme
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: 6
                        width: 18
                        height: 18
                        radius: 9
                        color: "#fafafa"
                        
                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 11
                            font.weight: Font.Bold
                            color: "#171717"
                        }
                    }
                    
                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
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
        
        // ============================================
        // DESIGN MODE SECTION
        // ============================================
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 16
            
            // Section Title
            Text {
                text: "DESIGN"
                font.pixelSize: 13
                font.weight: Font.DemiBold
                font.family: fontFamily
                font.letterSpacing: 1
                color: textSecondary
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }
            
            // Design Mode Toggle
            Rectangle {
                Layout.fillWidth: true
                height: 72
                radius: 12
                color: designRowMouseArea.containsMouse ? hoverBackground : "transparent"
                border.color: borderColor
                border.width: 1
                
                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
                
                MouseArea {
                    id: designRowMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: ThemeManager.toggleDesignMode()
                }
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16
                    
                    // Design mode icon - palette/color wheel design
                    Rectangle {
                        width: 40
                        height: 40
                        radius: 10
                        color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                        border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.2)
                        border.width: 1
                        
                        // Design mode icon using Canvas
                        Canvas {
                            anchors.centerIn: parent
                            width: 22
                            height: 22
                            
                            property color iconColor: primaryColor
                            property bool colorMode: ThemeManager.isColorMode
                            property color accentC: ThemeManager.accentColor
                            property color successC: ThemeManager.successColor
                            property color warningC: ThemeManager.warningColor
                            
                            onIconColorChanged: requestPaint()
                            onColorModeChanged: requestPaint()
                            onAccentCChanged: requestPaint()
                            Component.onCompleted: requestPaint()
                            
                            onPaint: {
                                var ctx = getContext("2d");
                                ctx.reset();
                                var cx = width / 2;
                                var cy = height / 2;
                                var r = 9;
                                
                                if (colorMode) {
                                    // Draw colorful palette in color mode
                                    var colors = ["#4f46e5", "#10b981", "#f59e0b", "#ef4444"];
                                    for (var i = 0; i < 4; i++) {
                                        ctx.fillStyle = colors[i];
                                        ctx.beginPath();
                                        ctx.arc(cx, cy, r, (i * Math.PI / 2) - Math.PI / 4, ((i + 1) * Math.PI / 2) - Math.PI / 4);
                                        ctx.lineTo(cx, cy);
                                        ctx.closePath();
                                        ctx.fill();
                                    }
                                    // Center circle
                                    ctx.fillStyle = "#ffffff";
                                    ctx.beginPath();
                                    ctx.arc(cx, cy, 3, 0, 2 * Math.PI);
                                    ctx.fill();
                                } else {
                                    // Draw monochrome palette
                                    var grays = ["#1a1a1a", "#525252", "#a3a3a3", "#e5e5e5"];
                                    for (var j = 0; j < 4; j++) {
                                        ctx.fillStyle = grays[j];
                                        ctx.beginPath();
                                        ctx.arc(cx, cy, r, (j * Math.PI / 2) - Math.PI / 4, ((j + 1) * Math.PI / 2) - Math.PI / 4);
                                        ctx.lineTo(cx, cy);
                                        ctx.closePath();
                                        ctx.fill();
                                    }
                                    // Center circle
                                    ctx.fillStyle = ThemeManager.isDarkTheme ? "#0a0a0a" : "#ffffff";
                                    ctx.beginPath();
                                    ctx.arc(cx, cy, 3, 0, 2 * Math.PI);
                                    ctx.fill();
                                }
                            }
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
                            text: "Design Mode"
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            font.family: fontFamily
                            color: textPrimary
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                        
                        Text {
                            text: ThemeManager.isColorMode ? "Vibrant color palette" : "Classic monochrome"
                            font.pixelSize: 14
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
                        color: ThemeManager.isColorMode ? primaryColor : ThemeManager.trackOffColor
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                        
                        Rectangle {
                            x: ThemeManager.isColorMode ? parent.width - width - 4 : 4
                            y: 4
                            width: 20
                            height: 20
                            radius: 10
                            color: ThemeManager.switchKnobColor
                            
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
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                    }
                }
            }
            
            // Design Mode Preview Cards
            RowLayout {
                Layout.fillWidth: true
                spacing: 12
                
                // Monochrome preview
                Rectangle {
                    Layout.fillWidth: true
                    height: 100
                    radius: 10
                    color: ThemeManager.isDarkTheme ? "#171717" : "#fafafa"
                    border.color: !ThemeManager.isColorMode ? primaryColor : borderColor
                    border.width: !ThemeManager.isColorMode ? 2 : 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ThemeManager.setDesignMode(false)
                    }
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 8
                        
                        // Monochrome color swatches
                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 4
                            
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#1a1a1a"
                            }
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#525252"
                            }
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#a3a3a3"
                            }
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#e5e5e5"
                                border.color: "#d4d4d4"
                                border.width: 1
                            }
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "MonoChrome Theme"
                            font.pixelSize: 13
                            font.weight: !ThemeManager.isColorMode ? Font.DemiBold : Font.Normal
                            font.family: fontFamily
                            font.letterSpacing: 0.5
                            color: ThemeManager.isDarkTheme ? "#fafafa" : "#1a1a1a"
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Classic & Elegant"
                            font.pixelSize: 11
                            font.family: fontFamily
                            color: ThemeManager.isDarkTheme ? "#a3a3a3" : "#737373"
                        }
                    }
                    
                    // Checkmark for selected
                    Rectangle {
                        visible: !ThemeManager.isColorMode
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: 6
                        width: 18
                        height: 18
                        radius: 9
                        color: primaryColor
                        
                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 11
                            font.weight: Font.Bold
                            color: ThemeManager.buttonTextColor
                        }
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                    
                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
                
                // Color mode preview
                Rectangle {
                    Layout.fillWidth: true
                    height: 100
                    radius: 10
                    color: ThemeManager.isDarkTheme ? "#1e293b" : "#f8fafc"
                    border.color: ThemeManager.isColorMode ? primaryColor : borderColor
                    border.width: ThemeManager.isColorMode ? 2 : 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ThemeManager.setDesignMode(true)
                    }
                    
                    Column {
                        anchors.centerIn: parent
                        spacing: 8
                        
                        // Colorful swatches
                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 4
                            
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#4f46e5"
                            }
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#10b981"
                            }
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#f59e0b"
                            }
                            Rectangle {
                                width: 16
                                height: 16
                                radius: 3
                                color: "#ef4444"
                            }
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Colored Theme"
                            font.pixelSize: 13
                            font.weight: ThemeManager.isColorMode ? Font.DemiBold : Font.Normal
                            font.family: fontFamily
                            font.letterSpacing: 0.5
                            color: ThemeManager.isDarkTheme ? "#f8fafc" : "#0f172a"
                        }
                        
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Vibrant & Modern"
                            font.pixelSize: 11
                            font.family: fontFamily
                            color: ThemeManager.isDarkTheme ? "#94a3b8" : "#64748b"
                        }
                    }
                    
                    // Checkmark for selected
                    Rectangle {
                        visible: ThemeManager.isColorMode
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.margins: 6
                        width: 18
                        height: 18
                        radius: 9
                        color: "#4f46e5"
                        
                        Text {
                            anchors.centerIn: parent
                            text: "✓"
                            font.pixelSize: 11
                            font.weight: Font.Bold
                            color: "#ffffff"
                        }
                    }
                    
                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                    
                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                }
            }
            
            // Color palette preview (only visible in Color mode)
            Rectangle {
                Layout.fillWidth: true
                height: colorPaletteVisible ? 60 : 0
                radius: 10
                color: hoverBackground
                border.color: borderColor
                border.width: 1
                clip: true
                visible: height > 0
                
                property bool colorPaletteVisible: ThemeManager.isColorMode
                
                Behavior on height {
                    NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                }
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8
                    
                    Text {
                        text: "Palette:"
                        font.pixelSize: 12
                        font.family: fontFamily
                        font.weight: Font.Medium
                        color: textSecondary
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                    
                    // Color swatches showing current palette
                    Row {
                        spacing: 6
                        Layout.fillWidth: true
                        
                        Repeater {
                            model: [
                                { color: ThemeManager.primaryColor, name: "Primary" },
                                { color: ThemeManager.accentColor, name: "Accent" },
                                { color: ThemeManager.successColor, name: "Success" },
                                { color: ThemeManager.warningColor, name: "Warning" },
                                { color: ThemeManager.errorColor, name: "Error" },
                                { color: ThemeManager.infoColor, name: "Info" }
                            ]
                            
                            Rectangle {
                                width: 28
                                height: 28
                                radius: 6
                                color: modelData.color
                                border.color: Qt.darker(modelData.color, 1.1)
                                border.width: 1
                                
                                Behavior on color {
                                    ColorAnimation { duration: 200 }
                                }
                                
                                ToolTip {
                                    visible: swatchMouseArea.containsMouse
                                    text: modelData.name
                                    delay: 300
                                    
                                    background: Rectangle {
                                        color: cardBackground
                                        radius: 4
                                        border.color: borderColor
                                        border.width: 1
                                    }
                                    
                                    contentItem: Text {
                                        text: modelData.name
                                        font.pixelSize: 11
                                        font.family: fontFamily
                                        color: textPrimary
                                    }
                                }
                                
                                MouseArea {
                                    id: swatchMouseArea
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }
                            }
                        }
                    }
                }
            }
        }
        }  // Close ColumnLayout (contentColumn)
    }  // Close Flickable
}
