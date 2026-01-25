pragma Singleton
import QtQuick 2.15

QtObject {
    id: themeManager
    
    // Theme state
    property bool isDarkTheme: false
    
    // Function to toggle theme
    function toggleTheme() {
        isDarkTheme = !isDarkTheme
    }
    
    // Function to set specific theme
    function setTheme(dark) {
        isDarkTheme = dark
    }
    
    // ============================================
    // LIGHT THEME COLORS
    // ============================================
    readonly property color lightBackgroundColor: "#f8fafc"
    readonly property color lightCardBackground: "#ffffff"
    readonly property color lightPrimaryColor: "#3b82f6"
    readonly property color lightPrimaryHover: "#2563eb"
    readonly property color lightPrimaryPressed: "#1d4ed8"
    readonly property color lightAccentColor: "#06b6d4"
    readonly property color lightSuccessColor: "#10b981"
    readonly property color lightWarningColor: "#f59e0b"
    readonly property color lightErrorColor: "#ef4444"
    readonly property color lightTextPrimary: "#1e293b"
    readonly property color lightTextSecondary: "#64748b"
    readonly property color lightTextMuted: "#94a3b8"
    readonly property color lightBorderColor: "#e2e8f0"
    readonly property color lightBorderFocus: "#3b82f6"
    readonly property color lightShadowColor: "#00000014"
    readonly property color lightInputBackground: "#f8fafc"
    readonly property color lightTrackOffColor: "#cbd5e1"
    readonly property color lightTrackColor: "#e2e8f0"
    readonly property color lightHoverBackground: "#f1f5f9"
    readonly property color lightDividerColor: "#e2e8f0"
    
    // ============================================
    // DARK THEME COLORS
    // ============================================
    readonly property color darkBackgroundColor: "#0f172a"
    readonly property color darkCardBackground: "#1e293b"
    readonly property color darkPrimaryColor: "#60a5fa"
    readonly property color darkPrimaryHover: "#3b82f6"
    readonly property color darkPrimaryPressed: "#2563eb"
    readonly property color darkAccentColor: "#22d3ee"
    readonly property color darkSuccessColor: "#34d399"
    readonly property color darkWarningColor: "#fbbf24"
    readonly property color darkErrorColor: "#f87171"
    readonly property color darkTextPrimary: "#f1f5f9"
    readonly property color darkTextSecondary: "#94a3b8"
    readonly property color darkTextMuted: "#64748b"
    readonly property color darkBorderColor: "#334155"
    readonly property color darkBorderFocus: "#60a5fa"
    readonly property color darkShadowColor: "#00000040"
    readonly property color darkInputBackground: "#0f172a"
    readonly property color darkTrackOffColor: "#475569"
    readonly property color darkTrackColor: "#334155"
    readonly property color darkHoverBackground: "#334155"
    readonly property color darkDividerColor: "#334155"
    
    // ============================================
    // DYNAMIC THEME PROPERTIES (auto-switch based on isDarkTheme)
    // ============================================
    readonly property color backgroundColor: isDarkTheme ? darkBackgroundColor : lightBackgroundColor
    readonly property color cardBackground: isDarkTheme ? darkCardBackground : lightCardBackground
    readonly property color primaryColor: isDarkTheme ? darkPrimaryColor : lightPrimaryColor
    readonly property color primaryHover: isDarkTheme ? darkPrimaryHover : lightPrimaryHover
    readonly property color primaryPressed: isDarkTheme ? darkPrimaryPressed : lightPrimaryPressed
    readonly property color accentColor: isDarkTheme ? darkAccentColor : lightAccentColor
    readonly property color successColor: isDarkTheme ? darkSuccessColor : lightSuccessColor
    readonly property color warningColor: isDarkTheme ? darkWarningColor : lightWarningColor
    readonly property color errorColor: isDarkTheme ? darkErrorColor : lightErrorColor
    readonly property color textPrimary: isDarkTheme ? darkTextPrimary : lightTextPrimary
    readonly property color textSecondary: isDarkTheme ? darkTextSecondary : lightTextSecondary
    readonly property color textMuted: isDarkTheme ? darkTextMuted : lightTextMuted
    readonly property color borderColor: isDarkTheme ? darkBorderColor : lightBorderColor
    readonly property color borderFocus: isDarkTheme ? darkBorderFocus : lightBorderFocus
    readonly property color shadowColor: isDarkTheme ? darkShadowColor : lightShadowColor
    readonly property color inputBackground: isDarkTheme ? darkInputBackground : lightInputBackground
    readonly property color trackOffColor: isDarkTheme ? darkTrackOffColor : lightTrackOffColor
    readonly property color trackColor: isDarkTheme ? darkTrackColor : lightTrackColor
    readonly property color hoverBackground: isDarkTheme ? darkHoverBackground : lightHoverBackground
    readonly property color dividerColor: isDarkTheme ? darkDividerColor : lightDividerColor
    
    // Button specific colors
    readonly property color buttonTextColor: "#ffffff"
    readonly property color outlineButtonText: primaryColor
    
    // Switch knob color (stays white in both themes)
    readonly property color switchKnobColor: "#ffffff"
    
    // ============================================
    // TYPOGRAPHY
    // ============================================
    readonly property string fontFamily: "Segoe UI, Inter, -apple-system, BlinkMacSystemFont, sans-serif"
    readonly property int fontSizeXs: 11
    readonly property int fontSizeSm: 12
    readonly property int fontSizeMd: 14
    readonly property int fontSizeLg: 16
    readonly property int fontSizeXl: 18
    readonly property int fontSize2Xl: 24
    
    // ============================================
    // SPACING
    // ============================================
    readonly property int spacingXs: 4
    readonly property int spacingSm: 8
    readonly property int spacingMd: 16
    readonly property int spacingLg: 24
    readonly property int spacingXl: 32
    
    // ============================================
    // BORDER RADIUS
    // ============================================
    readonly property int radiusSm: 6
    readonly property int radiusMd: 10
    readonly property int radiusLg: 16
    readonly property int radiusXl: 24
    
    // ============================================
    // ANIMATION DURATIONS
    // ============================================
    readonly property int animationFast: 100
    readonly property int animationNormal: 200
    readonly property int animationSlow: 300
    
    // Theme name for display
    readonly property string themeName: isDarkTheme ? "Dark" : "Light"
}
