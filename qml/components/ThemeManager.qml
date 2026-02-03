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
    // CLASSIC COLOR PALETTE - SIMPLIFIED
    // Using only 2-3 core colors for a clean, professional UI
    // Primary: Blue (#2563eb) - Main interactive elements
    // Neutral: Slate grays - Backgrounds, text, borders
    // ============================================
    
    // ============================================
    // LIGHT THEME COLORS - Classic Clean Look
    // ============================================
    readonly property color lightBackgroundColor: "#f8fafc"
    readonly property color lightCardBackground: "#ffffff"
    // Primary blue - the main accent color
    readonly property color lightPrimaryColor: "#2563eb"
    readonly property color lightPrimaryHover: "#1d4ed8"
    readonly property color lightPrimaryPressed: "#1e40af"
    // Accent, Success, Warning, Error all derive from primary for consistency
    readonly property color lightAccentColor: "#2563eb"      // Same as primary for unified look
    readonly property color lightSuccessColor: "#2563eb"     // Use primary instead of green
    readonly property color lightWarningColor: "#2563eb"     // Use primary instead of orange
    readonly property color lightErrorColor: "#dc2626"       // Keep red only for critical errors
    // Text hierarchy using slate grays
    readonly property color lightTextPrimary: "#1e293b"
    readonly property color lightTextSecondary: "#475569"
    readonly property color lightTextMuted: "#94a3b8"
    // Borders and UI elements
    readonly property color lightBorderColor: "#e2e8f0"
    readonly property color lightBorderFocus: "#2563eb"
    readonly property color lightShadowColor: "#00000014"
    readonly property color lightInputBackground: "#f8fafc"
    readonly property color lightTrackOffColor: "#cbd5e1"
    readonly property color lightTrackColor: "#e2e8f0"
    readonly property color lightHoverBackground: "#f1f5f9"
    readonly property color lightDividerColor: "#e2e8f0"
    
    // ============================================
    // DARK THEME COLORS - Classic Dark Look
    // ============================================
    readonly property color darkBackgroundColor: "#0f172a"
    readonly property color darkCardBackground: "#1e293b"
    // Primary blue - lighter for dark backgrounds
    readonly property color darkPrimaryColor: "#3b82f6"
    readonly property color darkPrimaryHover: "#2563eb"
    readonly property color darkPrimaryPressed: "#1d4ed8"
    // Accent, Success, Warning, Error all derive from primary for consistency
    readonly property color darkAccentColor: "#3b82f6"       // Same as primary for unified look
    readonly property color darkSuccessColor: "#3b82f6"      // Use primary instead of green
    readonly property color darkWarningColor: "#3b82f6"      // Use primary instead of orange
    readonly property color darkErrorColor: "#ef4444"        // Keep red only for critical errors
    // Text hierarchy using slate grays
    readonly property color darkTextPrimary: "#f1f5f9"
    readonly property color darkTextSecondary: "#94a3b8"
    readonly property color darkTextMuted: "#64748b"
    // Borders and UI elements
    readonly property color darkBorderColor: "#334155"
    readonly property color darkBorderFocus: "#3b82f6"
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
    readonly property int fontSizeXs: 13
    readonly property int fontSizeSm: 14
    readonly property int fontSizeMd: 16
    readonly property int fontSizeLg: 18
    readonly property int fontSizeXl: 20
    readonly property int fontSize2Xl: 28
    
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
