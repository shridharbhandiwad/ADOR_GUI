import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: modernComboBox
    
    // Theme properties
    property color primaryColor: "#3b82f6"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color borderColor: "#e2e8f0"
    property color borderFocus: "#3b82f6"
    property color backgroundColor: "#f8fafc"
    property color cardBackground: "#ffffff"
    property string fontFamily: "Segoe UI"
    
    // ComboBox properties
    property string label: ""
    property alias model: comboBox.model
    property alias currentIndex: comboBox.currentIndex
    property alias currentText: comboBox.currentText
    
    implicitHeight: label.length > 0 ? 72 : 48
    implicitWidth: 200
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 8
        
        // Label
        Text {
            text: modernComboBox.label
            font.pixelSize: 13
            font.weight: Font.Medium
            font.family: modernComboBox.fontFamily
            color: textSecondary
            visible: modernComboBox.label.length > 0
        }
        
        // ComboBox
        ComboBox {
            id: comboBox
            Layout.fillWidth: true
            Layout.preferredHeight: 44
            
            contentItem: Text {
                leftPadding: 16
                rightPadding: comboBox.indicator.width + 16
                
                text: comboBox.displayText
                font.pixelSize: 14
                font.family: modernComboBox.fontFamily
                color: textPrimary
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
            
            indicator: Canvas {
                id: canvas
                x: comboBox.width - width - 12
                y: comboBox.topPadding + (comboBox.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"
                
                Connections {
                    target: comboBox
                    function onPressedChanged() { canvas.requestPaint(); }
                }
                
                onPaint: {
                    context.reset();
                    context.moveTo(0, 0);
                    context.lineTo(width, 0);
                    context.lineTo(width / 2, height);
                    context.closePath();
                    context.fillStyle = textSecondary;
                    context.fill();
                }
            }
            
            background: Rectangle {
                radius: 10
                color: comboBox.pressed ? "#ffffff" : backgroundColor
                border.color: comboBox.pressed || comboBox.popup.visible ? borderFocus : borderColor
                border.width: comboBox.pressed || comboBox.popup.visible ? 2 : 1
                
                Behavior on border.color {
                    ColorAnimation { duration: 150 }
                }
            }
            
            popup: Popup {
                y: comboBox.height + 4
                width: comboBox.width
                implicitHeight: contentItem.implicitHeight + 16
                padding: 8
                
                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: comboBox.popup.visible ? comboBox.delegateModel : null
                    currentIndex: comboBox.highlightedIndex
                    
                    ScrollIndicator.vertical: ScrollIndicator { }
                }
                
                background: Rectangle {
                    radius: 12
                    color: cardBackground
                    border.color: borderColor
                    
                    layer.enabled: true
                    layer.effect: DropShadow {
                        transparentBorder: true
                        horizontalOffset: 0
                        verticalOffset: 4
                        radius: 16
                        samples: 33
                        color: "#0000002a"
                    }
                }
            }
            
            delegate: ItemDelegate {
                width: comboBox.width - 16
                height: 40
                
                contentItem: Text {
                    text: modelData
                    color: highlighted ? primaryColor : textPrimary
                    font.pixelSize: 14
                    font.family: modernComboBox.fontFamily
                    font.weight: highlighted ? Font.Medium : Font.Normal
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                
                background: Rectangle {
                    radius: 8
                    color: highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                }
                
                highlighted: comboBox.highlightedIndex === index
            }
        }
    }
}
