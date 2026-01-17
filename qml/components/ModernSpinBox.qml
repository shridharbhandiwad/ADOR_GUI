import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: modernSpinBox
    
    // Theme properties
    property color primaryColor: "#3b82f6"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color borderColor: "#e2e8f0"
    property color borderFocus: "#3b82f6"
    property color backgroundColor: "#f8fafc"
    property string fontFamily: "Segoe UI"
    
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
        
        // Label
        Text {
            text: modernSpinBox.label
            font.pixelSize: 13
            font.weight: Font.Medium
            font.family: modernSpinBox.fontFamily
            color: textSecondary
            visible: modernSpinBox.label.length > 0
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
                
                font.pixelSize: 14
                font.family: modernSpinBox.fontFamily
                color: textPrimary
                selectionColor: primaryColor
                selectedTextColor: "#ffffff"
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                
                readOnly: !spinBox.editable
                validator: spinBox.validator
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                selectByMouse: true
            }
            
            up.indicator: Rectangle {
                x: spinBox.mirrored ? 0 : parent.width - width
                height: parent.height
                width: 36
                radius: 10
                color: spinBox.up.pressed ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15) :
                       spinBox.up.hovered ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08) : "transparent"
                
                Text {
                    text: "+"
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: spinBox.up.pressed ? primaryColor : textSecondary
                    anchors.centerIn: parent
                }
            }
            
            down.indicator: Rectangle {
                x: spinBox.mirrored ? parent.width - width : 0
                height: parent.height
                width: 36
                radius: 10
                color: spinBox.down.pressed ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.15) :
                       spinBox.down.hovered ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.08) : "transparent"
                
                Text {
                    text: "-"
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: spinBox.down.pressed ? primaryColor : textSecondary
                    anchors.centerIn: parent
                }
            }
            
            background: Rectangle {
                radius: 10
                color: spinBox.activeFocus ? "#ffffff" : backgroundColor
                border.color: spinBox.activeFocus ? borderFocus : borderColor
                border.width: spinBox.activeFocus ? 2 : 1
                
                Behavior on border.color {
                    ColorAnimation { duration: 150 }
                }
            }
        }
    }
}
