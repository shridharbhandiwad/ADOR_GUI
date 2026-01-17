import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "../style"

Item {
    id: root
    
    // ═══════════════════════════════════════════════════════════════════════════
    // Dialog Card Component - Elegant Container with Header
    // ═══════════════════════════════════════════════════════════════════════════
    
    property string title: "Dialog"
    property string subtitle: ""
    property string iconSource: ""
    property color accentColor: Theme.primaryColor
    property alias contentItem: contentArea.data
    property bool isExpanded: true
    property bool showShadow: true
    property bool interactive: true
    
    // Card container with shadow effect
    Rectangle {
        id: cardBackground
        anchors.fill: parent
        anchors.margins: Theme.spacingS
        radius: Theme.radiusXL
        color: Theme.surfaceColor
        border.color: hoverArea.containsMouse ? accentColor : Theme.borderColor
        border.width: hoverArea.containsMouse ? 2 : 1
        
        Behavior on border.color {
            ColorAnimation { duration: Theme.animationFast }
        }
        
        Behavior on border.width {
            NumberAnimation { duration: Theme.animationFast }
        }
        
        // Subtle gradient overlay at top
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 4
            radius: Theme.radiusXL
            
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: accentColor }
                GradientStop { position: 1.0; color: Qt.lighter(accentColor, 1.3) }
            }
        }
        
        // Main layout
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 0
            spacing: 0
            
            // ───────────────────────────────────────────────────────────────────
            // Header Section
            // ───────────────────────────────────────────────────────────────────
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: Theme.headerHeight
                color: "transparent"
                
                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: Theme.spacingL
                    anchors.rightMargin: Theme.spacingL
                    spacing: Theme.spacingM
                    
                    // Icon container with accent background
                    Rectangle {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 40
                        radius: Theme.radiusM
                        color: Qt.alpha(accentColor, 0.1)
                        
                        Text {
                            anchors.centerIn: parent
                            text: iconSource
                            font.pixelSize: Theme.iconSizeL
                            font.family: "Segoe Fluent Icons, Segoe MDL2 Assets, Material Icons"
                            color: accentColor
                        }
                    }
                    
                    // Title and subtitle
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        
                        Text {
                            text: title
                            font.pixelSize: Theme.fontSizeL
                            font.weight: Font.DemiBold
                            color: Theme.textPrimary
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                        
                        Text {
                            text: subtitle
                            font.pixelSize: Theme.fontSizeS
                            color: Theme.textSecondary
                            visible: subtitle !== ""
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }
                    
                    // Expand/Collapse button
                    Rectangle {
                        Layout.preferredWidth: 32
                        Layout.preferredHeight: 32
                        radius: Theme.radiusS
                        color: expandBtn.containsMouse ? Theme.surfaceVariant : "transparent"
                        visible: interactive
                        
                        Text {
                            anchors.centerIn: parent
                            text: isExpanded ? "▲" : "▼"
                            font.pixelSize: 10
                            color: Theme.textSecondary
                            
                            Behavior on text {
                                PropertyAnimation { duration: Theme.animationFast }
                            }
                        }
                        
                        MouseArea {
                            id: expandBtn
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: isExpanded = !isExpanded
                        }
                    }
                }
                
                // Header divider
                Rectangle {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: Theme.spacingL
                    anchors.rightMargin: Theme.spacingL
                    height: 1
                    color: Theme.dividerColor
                }
            }
            
            // ───────────────────────────────────────────────────────────────────
            // Content Section (Collapsible)
            // ───────────────────────────────────────────────────────────────────
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                
                opacity: isExpanded ? 1 : 0
                visible: opacity > 0
                
                Behavior on opacity {
                    NumberAnimation { duration: Theme.animationNormal; easing.type: Easing.OutCubic }
                }
                
                // Scrollable content area
                ScrollView {
                    id: scrollView
                    anchors.fill: parent
                    anchors.margins: Theme.spacingL
                    clip: true
                    
                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                        
                        contentItem: Rectangle {
                            implicitWidth: 6
                            implicitHeight: 100
                            radius: 3
                            color: parent.pressed ? Theme.textTertiary : 
                                   parent.hovered ? Theme.textTertiary : Theme.borderColor
                        }
                        
                        background: Rectangle {
                            implicitWidth: 6
                            color: "transparent"
                        }
                    }
                    
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    
                    ColumnLayout {
                        id: contentArea
                        width: scrollView.availableWidth
                        spacing: Theme.spacingM
                    }
                }
            }
        }
        
        // Hover detection for card
        MouseArea {
            id: hoverArea
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onPressed: mouse.accepted = false
        }
    }
    
    // Drop shadow effect
    Rectangle {
        id: shadowRect
        anchors.fill: cardBackground
        anchors.margins: -2
        z: -1
        radius: cardBackground.radius + 2
        color: "transparent"
        visible: showShadow
        
        Rectangle {
            anchors.fill: parent
            anchors.topMargin: 4
            radius: parent.radius
            color: Theme.shadowLight
            opacity: hoverArea.containsMouse ? 0.8 : 0.5
            
            Behavior on opacity {
                NumberAnimation { duration: Theme.animationFast }
            }
        }
    }
}
