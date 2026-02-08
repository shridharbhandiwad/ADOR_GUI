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
    title: qsTr("ZopplerSystems RadarGUI")

    // Menu Bar at the top
    menuBar: MenuBar {
        id: mainMenuBar

        background: Rectangle {
            color: cardBackground
            border.color: borderColor
            border.width: 0

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: borderColor
            }

            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }

        // File Menu
        Menu {
            id: fileMenu
            title: qsTr("&File")

            background: Rectangle {
                implicitWidth: 200
                color: cardBackground
                border.color: borderColor
                border.width: 1
                radius: 8

                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 4
                    radius: 12
                    samples: 17
                    color: shadowColor
                }
            }

            Action {
                text: qsTr("&New Configuration")
                shortcut: "Ctrl+N"
                onTriggered: {
                    console.log("New Configuration")
                }
            }

            Action {
                text: qsTr("&Open Configuration...")
                shortcut: "Ctrl+O"
                onTriggered: {
                    console.log("Open Configuration")
                }
            }

            Action {
                text: qsTr("&Save Configuration")
                shortcut: "Ctrl+S"
                onTriggered: {
                    console.log("Save Configuration")
                }
            }

            Action {
                text: qsTr("Save Configuration &As...")
                shortcut: "Ctrl+Shift+S"
                onTriggered: {
                    console.log("Save Configuration As")
                }
            }

            MenuSeparator {
                contentItem: Rectangle {
                    implicitWidth: 180
                    implicitHeight: 1
                    color: borderColor
                }
            }

            Action {
                text: qsTr("&Export Data...")
                shortcut: "Ctrl+E"
                onTriggered: {
                    console.log("Export Data")
                }
            }

            MenuSeparator {
                contentItem: Rectangle {
                    implicitWidth: 180
                    implicitHeight: 1
                    color: borderColor
                }
            }

            Action {
                text: qsTr("E&xit")
                shortcut: "Alt+F4"
                onTriggered: Qt.quit()
            }
        }

        // View Menu
        Menu {
            id: viewMenu
            title: qsTr("&View")

            background: Rectangle {
                implicitWidth: 220
                color: cardBackground
                border.color: borderColor
                border.width: 1
                radius: 8

                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 4
                    radius: 12
                    samples: 17
                    color: shadowColor
                }
            }

            Action {
                text: qsTr("&Full Screen")
                shortcut: "F11"
                checkable: true
                checked: root.visibility === Window.FullScreen
                onTriggered: {
                    if (root.visibility === Window.FullScreen) {
                        root.showNormal()
                    } else {
                        root.showFullScreen()
                    }
                }
            }

            MenuSeparator {
                contentItem: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 1
                    color: borderColor
                }
            }

            Action {
                text: qsTr("&Refresh")
                shortcut: "F5"
                onTriggered: {
                    console.log("Refresh display")
                }
            }
        }

        // Connection Menu
        Menu {
            id: connectionMenu
            title: qsTr("&Connection")

            background: Rectangle {
                implicitWidth: 200
                color: cardBackground
                border.color: borderColor
                border.width: 1
                radius: 8

                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 4
                    radius: 12
                    samples: 17
                    color: shadowColor
                }
            }

            Action {
                text: udpPanel.isConnected ? qsTr("&Disconnect") : qsTr("&Connect")
                shortcut: "Ctrl+K"
                onTriggered: {
                    console.log("Toggle connection")
                }
            }

            MenuSeparator {
                contentItem: Rectangle {
                    implicitWidth: 180
                    implicitHeight: 1
                    color: borderColor
                }
            }

            Action {
                text: qsTr("&UDP Settings...")
                onTriggered: {
                    console.log("UDP Settings")
                }
            }

            Action {
                text: qsTr("&Output Settings...")
                onTriggered: {
                    console.log("Output Settings")
                }
            }
        }

        // Tools Menu
        Menu {
            id: toolsMenu
            title: qsTr("&Tools")

            background: Rectangle {
                implicitWidth: 200
                color: cardBackground
                border.color: borderColor
                border.width: 1
                radius: 8

                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 4
                    radius: 12
                    samples: 17
                    color: shadowColor
                }
            }

            Action {
                text: qsTr("&Angle Correction...")
                onTriggered: {
                    console.log("Angle Correction")
                }
            }

            Action {
                text: qsTr("A&mplification Settings...")
                onTriggered: {
                    console.log("Amplification Settings")
                }
            }

            MenuSeparator {
                contentItem: Rectangle {
                    implicitWidth: 180
                    implicitHeight: 1
                    color: borderColor
                }
            }

            Action {
                text: qsTr("&Settings...")
                shortcut: "Ctrl+,"
                onTriggered: settingsPopup.open()
            }
        }

        // Design Mode Menu - Contains themes and design mode options
        Menu {
            id: designMenu
            title: qsTr("Design &Mode")

            background: Rectangle {
                implicitWidth: 220
                color: cardBackground
                border.color: borderColor
                border.width: 1
                radius: 8

                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 4
                    radius: 12
                    samples: 17
                    color: shadowColor
                }
            }

            // Theme submenu
            Menu {
                id: themeSubmenu
                title: qsTr("Theme")

                background: Rectangle {
                    implicitWidth: 180
                    color: cardBackground
                    border.color: borderColor
                    border.width: 1
                    radius: 8

                    layer.enabled: true
                    layer.effect: DropShadow {
                        transparentBorder: true
                        horizontalOffset: 0
                        verticalOffset: 4
                        radius: 12
                        samples: 17
                        color: shadowColor
                    }
                }

                MenuItem {
                    text: qsTr("Light Theme")
                    checkable: true
                    checked: !ThemeManager.isDarkTheme
                    onTriggered: ThemeManager.setTheme(false)

                    contentItem: Row {
                        spacing: 10
                        leftPadding: 12
                        
                        // Light theme indicator
                        Rectangle {
                            width: 14
                            height: 14
                            radius: 7
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#1a1a1a"
                            
                            Rectangle {
                                anchors.centerIn: parent
                                width: 6
                                height: 6
                                radius: 3
                                color: "#fafafa"
                            }
                        }
                        
                        Text {
                            text: "Light Theme"
                            font.pixelSize: 13
                            font.family: root.fontFamily
                            font.weight: !ThemeManager.isDarkTheme ? Font.Medium : Font.Normal
                            color: parent.parent.highlighted ? primaryColor : textPrimary
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    background: Rectangle {
                        implicitWidth: 160
                        implicitHeight: 36
                        color: parent.highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                        radius: 6

                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                }

                MenuItem {
                    text: qsTr("Dark Theme")
                    checkable: true
                    checked: ThemeManager.isDarkTheme
                    onTriggered: ThemeManager.setTheme(true)

                    contentItem: Row {
                        spacing: 10
                        leftPadding: 12
                        
                        // Dark theme indicator
                        Rectangle {
                            width: 14
                            height: 14
                            radius: 7
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#fafafa"
                            border.color: "#d4d4d4"
                            border.width: 1
                            
                            Rectangle {
                                anchors.centerIn: parent
                                width: 6
                                height: 6
                                radius: 3
                                color: "#1a1a1a"
                            }
                        }
                        
                        Text {
                            text: "Dark Theme"
                            font.pixelSize: 13
                            font.family: root.fontFamily
                            font.weight: ThemeManager.isDarkTheme ? Font.Medium : Font.Normal
                            color: parent.parent.highlighted ? primaryColor : textPrimary
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    background: Rectangle {
                        implicitWidth: 160
                        implicitHeight: 36
                        color: parent.highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                        radius: 6

                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                }

                MenuSeparator {
                    contentItem: Rectangle {
                        implicitWidth: 160
                        implicitHeight: 1
                        color: borderColor
                    }
                }

                MenuItem {
                    text: qsTr("Toggle Theme")
                    onTriggered: ThemeManager.toggleTheme()

                    contentItem: Row {
                        spacing: 8
                        leftPadding: 12

                        Text {
                            text: parent.parent.text
                            font.pixelSize: 13
                            font.family: root.fontFamily
                            color: parent.parent.highlighted ? primaryColor : textPrimary
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Text {
                            text: "Ctrl+T"
                            font.pixelSize: 11
                            font.family: root.fontFamily
                            color: textMuted
                            verticalAlignment: Text.AlignVCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }

                    background: Rectangle {
                        implicitWidth: 160
                        implicitHeight: 36
                        color: parent.highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                        radius: 6

                        Behavior on color {
                            ColorAnimation { duration: 150 }
                        }
                    }
                }
            }

            MenuSeparator {
                contentItem: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 1
                    color: borderColor
                }
            }

            MenuItem {
                text: qsTr("MonoChrome Mode")
                checkable: true
                checked: !ThemeManager.isColorMode
                onTriggered: ThemeManager.setDesignMode(false)

                contentItem: Row {
                    spacing: 10
                    leftPadding: 12
                    
                    // Monochrome indicator - grayscale swatches
                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2
                        
                        Rectangle {
                            width: 8
                            height: 14
                            radius: 2
                            color: "#1a1a1a"
                        }
                        Rectangle {
                            width: 8
                            height: 14
                            radius: 2
                            color: "#737373"
                        }
                    }
                    
                    Text {
                        text: "MonoChrome Mode"
                        font.pixelSize: 13
                        font.family: root.fontFamily
                        font.weight: !ThemeManager.isColorMode ? Font.Medium : Font.Normal
                        color: parent.parent.highlighted ? primaryColor : textPrimary
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 36
                    color: parent.highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                    radius: 6

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }
            }

            MenuItem {
                text: qsTr("Colored Mode")
                checkable: true
                checked: ThemeManager.isColorMode
                onTriggered: ThemeManager.setDesignMode(true)

                contentItem: Row {
                    spacing: 10
                    leftPadding: 12
                    
                    // Color indicator - colorful swatches
                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 2
                        
                        Rectangle {
                            width: 8
                            height: 14
                            radius: 2
                            color: "#4f46e5"
                        }
                        Rectangle {
                            width: 8
                            height: 14
                            radius: 2
                            color: "#10b981"
                        }
                    }
                    
                    Text {
                        text: "Colored Mode"
                        font.pixelSize: 13
                        font.family: root.fontFamily
                        font.weight: ThemeManager.isColorMode ? Font.Medium : Font.Normal
                        color: parent.parent.highlighted ? primaryColor : textPrimary
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 36
                    color: parent.highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                    radius: 6

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }
            }

            MenuSeparator {
                contentItem: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 1
                    color: borderColor
                }
            }

            MenuItem {
                text: qsTr("Toggle Design Mode")
                onTriggered: ThemeManager.toggleDesignMode()

                contentItem: Row {
                    spacing: 8
                    leftPadding: 12

                    Text {
                        text: parent.parent.text
                        font.pixelSize: 13
                        font.family: root.fontFamily
                        color: parent.parent.highlighted ? primaryColor : textPrimary
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: "Ctrl+D"
                        font.pixelSize: 11
                        font.family: root.fontFamily
                        color: textMuted
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                background: Rectangle {
                    implicitWidth: 200
                    implicitHeight: 36
                    color: parent.highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                    radius: 6

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }
                }
            }
        }

        // Custom styling for menu items
        delegate: MenuBarItem {
            id: menuBarItem

            contentItem: Text {
                text: menuBarItem.text
                font.pixelSize: 13
                font.weight: Font.Medium
                font.family: root.fontFamily
                opacity: enabled ? 1.0 : 0.3
                color: menuBarItem.highlighted ? primaryColor : textPrimary
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }

            background: Rectangle {
                implicitWidth: 60
                implicitHeight: 36
                color: menuBarItem.highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                radius: 6

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }
        }
    }

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

    // Header - Enhanced enterprise design
    header: ToolBar {
        height: 72
        background: Rectangle {
            color: cardBackground
            border.color: borderColor
            border.width: 0

            Behavior on color {
                ColorAnimation { duration: 200 }
            }

            Behavior on border.color {
                ColorAnimation { duration: 200 }
            }

            // Bottom border separator
            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: borderColor
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
            }

            // Enhanced shadow for depth
            layer.enabled: true
            layer.effect: DropShadow {
                transparentBorder: true
                horizontalOffset: 0
                verticalOffset: 2
                radius: 16
                samples: 25
                color: Qt.rgba(shadowColor.r, shadowColor.g, shadowColor.b, 0.08)
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 24
            anchors.rightMargin: 24
            spacing: 16

            // Logo/Title - Enhanced enterprise branding
            Row {
                spacing: 16

                // Premium logo with gradient
                Rectangle {
                    width: 48
                    height: 48
                    radius: 12
                    
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: primaryColor }
                        GradientStop { position: 1.0; color: Qt.darker(primaryColor, 1.15) }
                    }

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }
                    
                    // Subtle inner shadow effect
                    layer.enabled: true
                    layer.effect: DropShadow {
                        transparentBorder: true
                        horizontalOffset: 0
                        verticalOffset: 2
                        radius: 8
                        samples: 17
                        color: Qt.rgba(0, 0, 0, 0.25)
                    }

                    Text {
                        anchors.centerIn: parent
                        text: "R"
                        font.pixelSize: 24
                        font.weight: Font.Bold
                        font.family: root.fontFamily
                        color: ThemeManager.buttonTextColor
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4

                    Text {
                        text: "Radar Visualization"
                        font.pixelSize: 19
                        font.weight: Font.DemiBold
                        font.family: root.fontFamily
                        color: textPrimary
                        font.letterSpacing: -0.5

                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    Text {
                        text: "Configuration Dashboard"
                        font.pixelSize: 13
                        font.weight: Font.Normal
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

                // Connection status - Enhanced enterprise badge
                Rectangle {
                    height: 32
                    width: connectionStatusRow.width + 24
                    radius: 8
                    anchors.verticalCenter: parent.verticalCenter
                    color: udpPanel.isConnected ? 
                           Qt.rgba(ThemeManager.statusConnected.r, ThemeManager.statusConnected.g, ThemeManager.statusConnected.b, 0.08) :
                           Qt.rgba(ThemeManager.statusDisconnected.r, ThemeManager.statusDisconnected.g, ThemeManager.statusDisconnected.b, 0.08)
                    border.color: udpPanel.isConnected ? 
                                 Qt.rgba(ThemeManager.statusConnected.r, ThemeManager.statusConnected.g, ThemeManager.statusConnected.b, 0.2) :
                                 Qt.rgba(ThemeManager.statusDisconnected.r, ThemeManager.statusDisconnected.g, ThemeManager.statusDisconnected.b, 0.2)
                    border.width: 1

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }

                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }

                    Row {
                        id: connectionStatusRow
                        anchors.centerIn: parent
                        spacing: 8

                        // Status indicator with ring design
                        Rectangle {
                            width: 10
                            height: 10
                            radius: 5
                            anchors.verticalCenter: parent.verticalCenter
                            color: "transparent"
                            border.color: udpPanel.isConnected ? ThemeManager.statusConnected : ThemeManager.statusDisconnected
                            border.width: 2

                            Behavior on border.color {
                                ColorAnimation { duration: 200 }
                            }

                            // Inner filled circle when connected
                            Rectangle {
                                anchors.centerIn: parent
                                width: 5
                                height: 5
                                radius: 2.5
                                color: ThemeManager.statusConnected
                                visible: udpPanel.isConnected
                                
                                Behavior on color {
                                    ColorAnimation { duration: 200 }
                                }
                            }

                            // Pulsing animation only when disconnected
                            SequentialAnimation on opacity {
                                running: !udpPanel.isConnected
                                loops: Animation.Infinite
                                NumberAnimation { to: 0.4; duration: 1000; easing.type: Easing.InOutSine }
                                NumberAnimation { to: 1.0; duration: 1000; easing.type: Easing.InOutSine }
                            }
                        }

                        Text {
                            text: udpPanel.isConnected ? "Connected" : "Disconnected"
                            font.pixelSize: 13
                            font.weight: Font.Medium
                            font.family: root.fontFamily
                            color: udpPanel.isConnected ? ThemeManager.statusConnected : ThemeManager.statusDisconnected
                            anchors.verticalCenter: parent.verticalCenter

                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
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

                // Style indicator - Theme + Design mode
                Row {
                    spacing: 8

                    // Design mode icon - color palette indicator
                    Canvas {
                        width: 16
                        height: 16
                        anchors.verticalCenter: parent.verticalCenter
                        
                        property bool colorMode: ThemeManager.isColorMode
                        property color pColor: primaryColor
                        
                        onColorModeChanged: requestPaint()
                        onPColorChanged: requestPaint()
                        Component.onCompleted: requestPaint()
                        
                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.reset();
                            var cx = width / 2;
                            var cy = height / 2;
                            var r = 7;
                            
                            if (colorMode) {
                                // Draw colorful quadrants
                                var colors = ["#4f46e5", "#10b981", "#f59e0b", "#ef4444"];
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

                    Text {
                        text: ThemeManager.fullStyleName
                        font.pixelSize: 13
                        font.weight: Font.Medium
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

                // Settings button - Enhanced premium design
                Rectangle {
                    id: settingsButton
                    width: 44
                    height: 44
                    radius: 12
                    color: settingsMouseArea.containsMouse ? 
                           Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.12) : 
                           Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.06)
                    border.color: settingsMouseArea.containsMouse ? 
                                 Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.3) : 
                                 Qt.rgba(borderColor.r, borderColor.g, borderColor.b, 0.5)
                    border.width: 1
                    visible: true

                    Behavior on color {
                        ColorAnimation { duration: 200 }
                    }

                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                    
                    // Subtle scale animation on hover
                    scale: settingsMouseArea.containsMouse ? 1.05 : 1.0
                    
                    Behavior on scale {
                        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                    }

                    // Gear icon using Canvas for reliable rendering
                    Canvas {
                        id: gearIcon
                        anchors.centerIn: parent
                        width: 20
                        height: 20

                        property color iconColor: settingsMouseArea.containsMouse ? primaryColor : textSecondary

                        onIconColorChanged: requestPaint()

                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.reset();
                            ctx.fillStyle = iconColor;
                            ctx.strokeStyle = iconColor;
                            ctx.lineWidth = 1.5;

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

    // Main content - 2x2 Grid Layout with enhanced spacing
    MainView {
        id: mainView
        anchors.fill: parent
        anchors.margins: 32
        anchors.topMargin: 28
        anchors.bottomMargin: 28

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

    // About Dialog
    Dialog {
        id: aboutDialog
        title: "About Radar Visualization"
        anchors.centerIn: parent
        width: 400
        height: 280
        modal: true

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
                samples: 25
                color: shadowColor
            }
        }

        header: Rectangle {
            height: 50
            color: "transparent"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20

                Rectangle {
                    width: 36
                    height: 36
                    radius: 8
                    color: primaryColor

                    Text {
                        anchors.centerIn: parent
                        text: "R"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        color: ThemeManager.buttonTextColor
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
                    }
                }

                Text {
                    text: "About Radar Visualization"
                    font.pixelSize: 16
                    font.weight: Font.DemiBold
                    font.family: root.fontFamily
                    color: textPrimary
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                }
            }
        }

        contentItem: ColumnLayout {
            spacing: 16

            Text {
                text: "Radar Data Visualization"
                font.pixelSize: 20
                font.weight: Font.Bold
                font.family: root.fontFamily
                color: textPrimary
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Version 1.0.0"
                font.pixelSize: 14
                font.family: root.fontFamily
                color: textSecondary
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: borderColor
                Layout.leftMargin: 20
                Layout.rightMargin: 20
            }

            Text {
                text: "A modern radar data visualization and\nconfiguration dashboard for real-time\nradar signal processing and display."
                font.pixelSize: 13
                font.family: root.fontFamily
                color: textSecondary
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
                lineHeight: 1.4
            }

            Item { Layout.fillHeight: true }
        }

        footer: Rectangle {
            height: 60
            color: "transparent"

            ModernButton {
                text: "Close"
                anchors.centerIn: parent
                width: 100
                height: 36

                primaryColor: root.primaryColor
                primaryHover: root.primaryHover
                primaryPressed: root.primaryPressed
                fontFamily: root.fontFamily

                onClicked: aboutDialog.close()
            }
        }
    }

    // Global keyboard shortcuts
    Shortcut {
        sequence: "Ctrl+T"
        onActivated: ThemeManager.toggleTheme()
    }
    
    Shortcut {
        sequence: "Ctrl+D"
        onActivated: ThemeManager.toggleDesignMode()
    }
}
