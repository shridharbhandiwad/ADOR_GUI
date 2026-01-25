import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: modernComboBox
    
    // Theme properties
    property color primaryColor: ThemeManager.primaryColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color borderColor: ThemeManager.borderColor
    property color borderFocus: ThemeManager.borderFocus
    property color backgroundColor: ThemeManager.inputBackground
    property color cardBackground: ThemeManager.cardBackground
    property color hoverBackground: ThemeManager.hoverBackground
    property string fontFamily: ThemeManager.fontFamily
    
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
            
            Behavior on color { ColorAnimation { duration: 200 } }
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
                
                Behavior on color { ColorAnimation { duration: 200 } }
            }
            
            indicator: Canvas {
                id: canvas
                x: comboBox.width - width - 12
                y: comboBox.topPadding + (comboBox.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"
                
                property color indicatorColor: textSecondary
                
                Connections {
                    target: comboBox
                    function onPressedChanged() { canvas.requestPaint(); }
                }
                
                Connections {
                    target: modernComboBox
                    function onTextSecondaryChanged() { canvas.requestPaint(); }
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
                color: comboBox.pressed ? cardBackground : backgroundColor
                border.color: comboBox.pressed || comboBox.popup.visible ? borderFocus : borderColor
                border.width: comboBox.pressed || comboBox.popup.visible ? 2 : 1
                
                Behavior on border.color {
                    ColorAnimation { duration: 150 }
                }
                
                Behavior on color {
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
                    
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }
                    
                    layer.enabled: true
                    layer.effect: DropShadow {
                        transparentBorder: true
                        horizontalOffset: 0
                        verticalOffset: 4
                        radius: 16
                        samples: 33
                        color: ThemeManager.shadowColor
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
                    
                    Behavior on color { ColorAnimation { duration: 150 } }
                }
                
                background: Rectangle {
                    radius: 8
                    color: highlighted ? Qt.rgba(primaryColor.r, primaryColor.g, primaryColor.b, 0.1) : "transparent"
                    
                    Behavior on color { ColorAnimation { duration: 150 } }
                }
                
                highlighted: comboBox.highlightedIndex === index
            }
        }
    }
}
