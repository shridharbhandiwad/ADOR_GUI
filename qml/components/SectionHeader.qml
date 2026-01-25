import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: sectionHeader
    
    property string title: "Section"
    property color textColor: ThemeManager.textSecondary
    property string fontFamily: ThemeManager.fontFamily
    
    height: 32
    
    RowLayout {
        anchors.fill: parent
        spacing: 12
        
        Text {
            text: sectionHeader.title
            font.pixelSize: 12
            font.weight: Font.DemiBold
            font.family: sectionHeader.fontFamily
            font.letterSpacing: 0.5
            color: textColor
            Layout.alignment: Qt.AlignVCenter
            
            Behavior on color { ColorAnimation { duration: 200 } }
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: Qt.rgba(textColor.r, textColor.g, textColor.b, 0.3)
            Layout.alignment: Qt.AlignVCenter
            
            Behavior on color { ColorAnimation { duration: 200 } }
        }
    }
}
