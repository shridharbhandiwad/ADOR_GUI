import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: modernSlider
    
    // Theme properties
    property color primaryColor: ThemeManager.primaryColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color borderColor: ThemeManager.borderColor
    property color trackColor: ThemeManager.trackColor
    property string fontFamily: ThemeManager.fontFamily
    
    // Slider properties
    property string label: ""
    property alias value: slider.value
    property alias from: slider.from
    property alias to: slider.to
    property alias stepSize: slider.stepSize
    property string suffix: ""
    property bool showValue: true
    
    implicitHeight: label.length > 0 ? 80 : 56
    implicitWidth: 200
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 12
        
        // Label row with value
        RowLayout {
            Layout.fillWidth: true
            visible: modernSlider.label.length > 0
            
            Text {
                text: modernSlider.label
                font.pixelSize: 13
                font.weight: Font.Medium
                font.family: modernSlider.fontFamily
                font.letterSpacing: 0.3
                color: textSecondary
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
            
            Item { Layout.fillWidth: true }
            
            Text {
                text: Math.round(slider.value) + modernSlider.suffix
                font.pixelSize: 15
                font.weight: Font.DemiBold
                font.family: modernSlider.fontFamily
                color: primaryColor
                visible: showValue
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
        }
        
        // Slider
        Slider {
            id: slider
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            
            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                width: 24
                height: 24
                radius: 12
                color: ThemeManager.switchKnobColor
                border.color: primaryColor
                border.width: 3
                
                Behavior on color { ColorAnimation { duration: 200 } }
                
                Behavior on border.color { ColorAnimation { duration: 200 } }
                
                layer.enabled: true
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 2
                    radius: 8
                    samples: 17
                    color: "#0000002a"
                }
                
                scale: slider.pressed ? 1.1 : 1.0
                
                Behavior on scale {
                    NumberAnimation { duration: 100; easing.type: Easing.OutCubic }
                }
            }
            
            background: Item {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - 4
                width: slider.availableWidth
                height: 8
                
                Rectangle {
                    width: parent.width
                    height: parent.height
                    radius: 4
                    color: trackColor
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
                
                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    radius: 4
                    color: primaryColor
                    
                    // Gradient effect
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: Qt.lighter(primaryColor, 1.1) }
                        GradientStop { position: 1.0; color: primaryColor }
                    }
                }
            }
        }
        
        // Min/Max labels
        RowLayout {
            Layout.fillWidth: true
            
            Text {
                text: slider.from + suffix
                font.pixelSize: 13
                font.family: modernSlider.fontFamily
                color: textSecondary
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
            
            Item { Layout.fillWidth: true }
            
            Text {
                text: slider.to + suffix
                font.pixelSize: 13
                font.family: modernSlider.fontFamily
                color: textSecondary
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
        }
    }
}
