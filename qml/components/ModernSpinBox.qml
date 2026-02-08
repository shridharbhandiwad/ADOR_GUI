import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: modernSpinBox
    
    // Theme properties
    property color primaryColor: ThemeManager.primaryColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color borderColor: ThemeManager.borderColor
    property color borderFocus: ThemeManager.borderFocus
    property color backgroundColor: ThemeManager.inputBackground
    property color cardBackground: ThemeManager.cardBackground
    property string fontFamily: ThemeManager.fontFamily
    
    // SpinBox properties
    property string label: ""
    property alias value: spinBox.value
    property alias from: spinBox.from
    property alias to: spinBox.to
    property alias stepSize: spinBox.stepSize
    property string suffix: ""
    
    implicitHeight: label.length > 0 ? 72 : 48
    implicitWidth: 200
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 8
        
        // Label - Enhanced styling
        Text {
            text: modernSpinBox.label
            font.pixelSize: 13
            font.weight: Font.Medium
            font.family: modernSpinBox.fontFamily
            font.letterSpacing: 0.3
            color: spinBox.activeFocus ? primaryColor : textSecondary
            visible: modernSpinBox.label.length > 0
            
            Behavior on color { ColorAnimation { duration: 200 } }
        }
        
        // SpinBox
        SpinBox {
            id: spinBox
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            
            editable: true
            
            contentItem: TextInput {
                z: 2
                text: spinBox.textFromValue(spinBox.value, spinBox.locale) + modernSpinBox.suffix
                
                font.pixelSize: 15
                font.family: modernSpinBox.fontFamily
                color: textPrimary
                selectionColor: primaryColor
                selectedTextColor: ThemeManager.buttonTextColor
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                
                readOnly: !spinBox.editable
                validator: spinBox.validator
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                selectByMouse: true
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
            
            up.indicator: Rectangle {
                x: spinBox.mirrored ? 0 : parent.width - width
                height: parent.height
                width: 38
                radius: 10
                color: spinBox.up.pressed ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15) :
                       spinBox.up.hovered ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.10) : "transparent"
                
                Behavior on color { ColorAnimation { duration: 200 } }
                
                Text {
                    text: "+"
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: spinBox.up.pressed ? primaryColor : 
                           spinBox.up.hovered ? primaryColor : textSecondary
                    anchors.centerIn: parent
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
            }
            
            down.indicator: Rectangle {
                x: spinBox.mirrored ? parent.width - width : 0
                height: parent.height
                width: 38
                radius: 10
                color: spinBox.down.pressed ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15) :
                       spinBox.down.hovered ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.10) : "transparent"
                
                Behavior on color { ColorAnimation { duration: 200 } }
                
                Text {
                    text: "-"
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: spinBox.down.pressed ? primaryColor : 
                           spinBox.down.hovered ? primaryColor : textSecondary
                    anchors.centerIn: parent
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                }
            }
            
            background: Rectangle {
                radius: 10
                color: spinBox.activeFocus ? cardBackground : backgroundColor
                border.color: spinBox.activeFocus ? borderFocus : borderColor
                border.width: spinBox.activeFocus ? 2 : 1
                
                Behavior on border.color {
                    ColorAnimation { duration: 200 }
                }
                
                Behavior on color {
                    ColorAnimation { duration: 200 }
                }
                
                // Subtle shadow on focus
                layer.enabled: spinBox.activeFocus
                layer.effect: DropShadow {
                    transparentBorder: true
                    horizontalOffset: 0
                    verticalOffset: 2
                    radius: 8
                    samples: 17
                    color: Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15)
                }
            }
        }
    }
}
