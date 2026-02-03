pragma Singleton
import QtQuick 2.15

QtObject {
    // Monochrome Light Theme Colors (fallback/static version)
    readonly property color backgroundColor: "#fafafa"
    readonly property color cardBackground: "#ffffff"
    readonly property color primaryColor: "#1a1a1a"
    readonly property color primaryHover: "#333333"
    readonly property color primaryPressed: "#0a0a0a"
    readonly property color accentColor: "#1a1a1a"
    readonly property color successColor: "#1a1a1a"
    readonly property color warningColor: "#525252"
    readonly property color errorColor: "#737373"
    readonly property color textPrimary: "#0a0a0a"
    readonly property color textSecondary: "#525252"
    readonly property color textMuted: "#a3a3a3"
    readonly property color borderColor: "#e5e5e5"
    readonly property color borderFocus: "#1a1a1a"
    readonly property color shadowColor: "#00000012"
    
    // Typography
    readonly property string fontFamily: "Segoe UI, Inter, -apple-system, BlinkMacSystemFont, sans-serif"
    readonly property int fontSizeXs: 13
    readonly property int fontSizeSm: 14
    readonly property int fontSizeMd: 16
    readonly property int fontSizeLg: 18
    readonly property int fontSizeXl: 20
    readonly property int fontSize2Xl: 28
    
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
