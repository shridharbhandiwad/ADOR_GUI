import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "components"

Item {
    id: mainView
    
    // Theme properties with smooth transitions
    property color backgroundColor: ThemeManager.backgroundColor
    property color cardBackground: ThemeManager.cardBackground
    property color primaryColor: ThemeManager.primaryColor
    property color primaryHover: ThemeManager.primaryHover
    property color primaryPressed: ThemeManager.primaryPressed
    property color accentColor: ThemeManager.accentColor
    property color successColor: ThemeManager.successColor
    property color warningColor: ThemeManager.warningColor
    property color errorColor: ThemeManager.errorColor
    property color textPrimary: ThemeManager.textPrimary
    property color textSecondary: ThemeManager.textSecondary
    property color textMuted: ThemeManager.textMuted
    property color borderColor: ThemeManager.borderColor
    property color borderFocus: ThemeManager.borderFocus
    property string fontFamily: ThemeManager.fontFamily
    
    // Expose panels for external access
    property alias udpConfigPanel: udpConfig
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 16
        
        // 2x2 Grid Layout for panels
        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 2
            rows: 2
            columnSpacing: 16
            rowSpacing: 16
            
            // Top Left: UDP Configuration
            UdpConfigPanel {
                id: udpConfig
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width / 2
                Layout.preferredHeight: parent.height / 2
                
                cardBackground: mainView.cardBackground
                primaryColor: mainView.primaryColor
                primaryHover: mainView.primaryHover
                primaryPressed: mainView.primaryPressed
                successColor: mainView.successColor
                errorColor: mainView.errorColor
                textPrimary: mainView.textPrimary
                textSecondary: mainView.textSecondary
                textMuted: mainView.textMuted
                borderColor: mainView.borderColor
                borderFocus: mainView.borderFocus
                fontFamily: mainView.fontFamily
            }
            
            // Top Right: Output Configuration
            OutputConfigPanel {
                id: outputConfig
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width / 2
                Layout.preferredHeight: parent.height / 2
                
                cardBackground: mainView.cardBackground
                primaryColor: mainView.primaryColor
                primaryHover: mainView.primaryHover
                primaryPressed: mainView.primaryPressed
                accentColor: mainView.accentColor
                textPrimary: mainView.textPrimary
                textSecondary: mainView.textSecondary
                textMuted: mainView.textMuted
                borderColor: mainView.borderColor
                borderFocus: mainView.borderFocus
                fontFamily: mainView.fontFamily
            }
            
            // Bottom Left: Angle Correction
            AngleCorrectionPanel {
                id: angleCorrection
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width / 2
                Layout.preferredHeight: parent.height / 2
                
                cardBackground: mainView.cardBackground
                primaryColor: mainView.primaryColor
                primaryHover: mainView.primaryHover
                primaryPressed: mainView.primaryPressed
                accentColor: mainView.accentColor
                textPrimary: mainView.textPrimary
                textSecondary: mainView.textSecondary
                textMuted: mainView.textMuted
                borderColor: mainView.borderColor
                borderFocus: mainView.borderFocus
                fontFamily: mainView.fontFamily
            }
            
            // Bottom Right: Amplification
            AmplificationPanel {
                id: amplification
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width / 2
                Layout.preferredHeight: parent.height / 2
                
                cardBackground: mainView.cardBackground
                primaryColor: mainView.primaryColor
                primaryHover: mainView.primaryHover
                primaryPressed: mainView.primaryPressed
                accentColor: mainView.accentColor
                successColor: mainView.successColor
                warningColor: mainView.warningColor
                textPrimary: mainView.textPrimary
                textSecondary: mainView.textSecondary
                textMuted: mainView.textMuted
                borderColor: mainView.borderColor
                borderFocus: mainView.borderFocus
                fontFamily: mainView.fontFamily
            }
        }
        
        // Independent Apply and Reset buttons - centered below both divisions
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 8
            spacing: 16
            
            ModernButton {
                text: "Apply"
                Layout.preferredWidth: 120
                Layout.preferredHeight: 44
                
                primaryColor: mainView.primaryColor
                primaryHover: mainView.primaryHover
                primaryPressed: mainView.primaryPressed
                fontFamily: mainView.fontFamily
                
                onClicked: {
                    console.log("Apply settings")
                }
            }
            
            ModernButton {
                text: "Reset"
                Layout.preferredWidth: 120
                Layout.preferredHeight: 44
                outline: true
                
                primaryColor: mainView.primaryColor
                primaryHover: mainView.primaryHover
                primaryPressed: mainView.primaryPressed
                textColor: mainView.textPrimary
                fontFamily: mainView.fontFamily
                
                onClicked: {
                    console.log("Reset settings")
                }
            }
        }
    }
}
