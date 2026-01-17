import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "components"

Item {
    id: mainView
    
    // Theme properties
    property color backgroundColor: "#f8fafc"
    property color cardBackground: "#ffffff"
    property color primaryColor: "#3b82f6"
    property color primaryHover: "#2563eb"
    property color primaryPressed: "#1d4ed8"
    property color accentColor: "#06b6d4"
    property color successColor: "#10b981"
    property color warningColor: "#f59e0b"
    property color errorColor: "#ef4444"
    property color textPrimary: "#1e293b"
    property color textSecondary: "#64748b"
    property color textMuted: "#94a3b8"
    property color borderColor: "#e2e8f0"
    property color borderFocus: "#3b82f6"
    property string fontFamily: "Segoe UI"
    
    // Expose panels for external access
    property alias udpConfigPanel: udpConfig
    
    // 2x2 Grid Layout
    GridLayout {
        anchors.fill: parent
        columns: 2
        rows: 2
        columnSpacing: 20
        rowSpacing: 20
        
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
}
