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

        // Theme Menu - FIXED: Now properly visible as standalone menu
        Menu {
            id: themeMenu
            title: qsTr("&Theme")

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
                        color: ThemeManager.buttonTextColor
                        
                        Behavior on color {
                            ColorAnimation { duration: 200 }
                        }
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

                // Connection status - Monochrome design
                Row {
                    spacing: 8

                    // Status indicator with ring design for elegance
                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        anchors.verticalCenter: parent.verticalCenter
                        color: "transparent"
                        border.color: udpPanel.isConnected ? primaryColor : textMuted
                        border.width: 2

                        Behavior on border.color {
                            ColorAnimation { duration: 200 }
                        }

                        // Inner filled circle when connected
                        Rectangle {
                            anchors.centerIn: parent
                            width: 6
                            height: 6
                            radius: 3
                            color: primaryColor
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
                        font.weight: udpPanel.isConnected ? Font.Medium : Font.Normal
                        font.family: root.fontFamily
                        color: udpPanel.isConnected ? textPrimary : textMuted
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

                // Theme indicator - Elegant monochrome design
                Row {
                    spacing: 8

                    // Theme icon - minimalist circle design
                    Rectangle {
                        width: 16
                        height: 16
                        radius: 8
                        anchors.verticalCenter: parent.verticalCenter
                        color: "transparent"
                        border.color: primaryColor
                        border.width: 1.5
                        
                        // Half fill based on theme
                        Rectangle {
                            anchors.left: ThemeManager.isDarkTheme ? parent.left : undefined
                            anchors.right: ThemeManager.isDarkTheme ? undefined : parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.margins: 2
                            width: 5
                            height: 10
                            radius: ThemeManager.isDarkTheme ? Qt.size(2.5, 0).width : Qt.size(0, 2.5).width
                            color: primaryColor
                            
                            Behavior on color {
                                ColorAnimation { duration: 200 }
                            }
                        }
                        
                        Behavior on border.color {
                            ColorAnimation { duration: 200 }
                        }
                    }

                    Text {
                        text: ThemeManager.themeName
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

                // Settings button
                Rectangle {
                    id: settingsButton
                    width: 40
                    height: 40
                    radius: 10
                    color: settingsMouseArea.containsMouse ? hoverBackground : Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1)
                    border.color: settingsMouseArea.containsMouse ? primaryColor : borderColor
                    border.width: 1
                    visible: true

                    Behavior on color {
                        ColorAnimation { duration: 150 }
                    }

                    Behavior on border.color {
                        ColorAnimation { duration: 150 }
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
}
