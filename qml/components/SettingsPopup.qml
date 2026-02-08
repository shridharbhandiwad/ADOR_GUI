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
            
            // Design Mode Section
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 16
                
                // Section Title
                Text {
                    text: "COLOR THEME"
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
                    color: designModeMouseArea.containsMouse ? hoverBackground : "transparent"
                    border.color: borderColor
                    border.width: 1
                    
                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                    
                    MouseArea {
                        id: designModeMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: ThemeManager.toggleDesignMode()
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        // Design mode icon - color palette
                        Rectangle {
                            width: 40
                            height: 40
                            radius: 10
                            color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                            border.color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.2)
                            border.width: 1
                            
                            // Color palette icon
                            Canvas {
                                anchors.centerIn: parent
                                width: 20
                                height: 20
                                
                                property bool colorMode: ThemeManager.isColorMode
                                
                                onColorModeChanged: requestPaint()
                                Component.onCompleted: requestPaint()
                                
                                onPaint: {
                                    var ctx = getContext("2d");
                                    ctx.reset();
                                    var cx = width / 2;
                                    var cy = height / 2;
                                    var r = 7;
                                    
                                    if (colorMode) {
                                        // Draw colorful quadrants
                                        var colors = ["#4c51bf", "#059669", "#d97706", "#dc2626"];
                                        for (var i = 0; i < 4; i++) {
                                            ctx.fillStyle = colors[i];
                                            ctx.beginPath();
                                            ctx.arc(cx, cy, r, (i * Math.PI / 2) - Math.PI / 4, ((i + 1) * Math.PI / 2) - Math.PI / 4);
                                            ctx.lineTo(cx, cy);
                                            ctx.closePath();
                                            ctx.fill();
                                        }
                                    } else {
                                        // Draw monochrome quadrants
                                        var grays = ["#1a1a1a", "#525252", "#a3a3a3", "#d4d4d4"];
                                        for (var j = 0; j < 4; j++) {
                                            ctx.fillStyle = grays[j];
                                            ctx.beginPath();
                                            ctx.arc(cx, cy, r, (j * Math.PI / 2) - Math.PI / 4, ((j + 1) * Math.PI / 2) - Math.PI / 4);
                                            ctx.lineTo(cx, cy);
                                            ctx.closePath();
                                            ctx.fill();
                                        }
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
                                text: ThemeManager.isColorMode ? "Colored mode enabled" : "Monochrome mode enabled"
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
                    
                    // Colored mode preview
                    Rectangle {
                        Layout.fillWidth: true
                        height: 100
                        radius: 10
                        color: cardBackground
                        border.color: ThemeManager.isColorMode ? primaryColor : borderColor
                        border.width: ThemeManager.isColorMode ? 2 : 1
                        
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: ThemeManager.setDesignMode(true)
                        }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 10
                            
                            // Color palette preview
                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 4
                                
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#4c51bf"
                                }
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#059669"
                                }
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#d97706"
                                }
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#dc2626"
                                }
                            }
                            
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "Colored"
                                font.pixelSize: 13
                                font.weight: ThemeManager.isColorMode ? Font.DemiBold : Font.Normal
                                font.family: fontFamily
                                font.letterSpacing: 0.5
                                color: textPrimary
                                
                                Behavior on color {
                                    ColorAnimation { duration: 200 }
                                }
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
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                        
                        Behavior on border.color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                    
                    // Monochrome mode preview
                    Rectangle {
                        Layout.fillWidth: true
                        height: 100
                        radius: 10
                        color: cardBackground
                        border.color: !ThemeManager.isColorMode ? primaryColor : borderColor
                        border.width: !ThemeManager.isColorMode ? 2 : 1
                        
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: ThemeManager.setDesignMode(false)
                        }
                        
                        Column {
                            anchors.centerIn: parent
                            spacing: 10
                            
                            // Grayscale palette preview
                            Row {
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 4
                                
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#1a1a1a"
                                }
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#525252"
                                }
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#a3a3a3"
                                }
                                Rectangle {
                                    width: 16
                                    height: 16
                                    radius: 4
                                    color: "#d4d4d4"
                                }
                            }
                            
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: "Monochrome"
                                font.pixelSize: 13
                                font.weight: !ThemeManager.isColorMode ? Font.DemiBold : Font.Normal
                                font.family: fontFamily
                                font.letterSpacing: 0.5
                                color: textPrimary
                                
                                Behavior on color {
                                    ColorAnimation { duration: 200 }
                                }
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
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                        
                        Behavior on border.color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                }
            }
        }  // Close ColumnLayout (contentColumn)
    }  // Close Flickable
}
