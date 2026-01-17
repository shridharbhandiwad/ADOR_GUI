pragma Singleton
import QtQuick 2.15

QtObject {
    // Light Theme Colors
    readonly property color backgroundColor: "#f8fafc"
    readonly property color cardBackground: "#ffffff"
    readonly property color primaryColor: "#3b82f6"
    readonly property color primaryHover: "#2563eb"
    readonly property color primaryPressed: "#1d4ed8"
    readonly property color accentColor: "#06b6d4"
    readonly property color successColor: "#10b981"
    readonly property color warningColor: "#f59e0b"
    readonly property color errorColor: "#ef4444"
    readonly property color textPrimary: "#1e293b"
    readonly property color textSecondary: "#64748b"
    readonly property color textMuted: "#94a3b8"
    readonly property color borderColor: "#e2e8f0"
    readonly property color borderFocus: "#3b82f6"
    readonly property color shadowColor: "#00000014"
    
    // Typography
    readonly property string fontFamily: "Segoe UI, Inter, -apple-system, BlinkMacSystemFont, sans-serif"
    readonly property int fontSizeXs: 11
    readonly property int fontSizeSm: 12
    readonly property int fontSizeMd: 14
    readonly property int fontSizeLg: 16
    readonly property int fontSizeXl: 18
    readonly property int fontSize2Xl: 24
    
    // Spacing
    readonly property int spacingXs: 4
    readonly property int spacingSm: 8
    readonly property int spacingMd: 16
    readonly property int spacingLg: 24
    readonly property int spacingXl: 32
    
    // Border Radius
    readonly property int radiusSm: 6
    readonly property int radiusMd: 10
    readonly property int radiusLg: 16
    readonly property int radiusXl: 24
    
    // Animation Durations
    readonly property int animationFast: 100
    readonly property int animationNormal: 200
    readonly property int animationSlow: 300
}
