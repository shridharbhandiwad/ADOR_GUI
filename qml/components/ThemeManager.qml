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
    // CLASSIC MONOCHROME PALETTE
    // Elegant two-tone design for timeless professional look
    // Light Theme: Dark grey/black accents on light background
    // Dark Theme: White/light grey accents on dark background
    // No blue, green, red - all meanings derived from grey tones
    // ============================================
    
    // ============================================
    // LIGHT THEME COLORS - Classic Monochrome
    // Primary accent: Dark charcoal grey
    // ============================================
    readonly property color lightBackgroundColor: "#fafafa"
    readonly property color lightCardBackground: "#ffffff"
    // Primary - Dark charcoal for elegant contrast
    readonly property color lightPrimaryColor: "#1a1a1a"
    readonly property color lightPrimaryHover: "#333333"
    readonly property color lightPrimaryPressed: "#0a0a0a"
    // All semantic colors unified as shades of grey
    readonly property color lightAccentColor: "#1a1a1a"
    readonly property color lightSuccessColor: "#1a1a1a"     // Active/On state - solid dark
    readonly property color lightWarningColor: "#525252"     // Attention - medium grey
    readonly property color lightErrorColor: "#737373"       // Inactive/Off - lighter grey
    // Text hierarchy using neutral grays
    readonly property color lightTextPrimary: "#0a0a0a"
    readonly property color lightTextSecondary: "#525252"
    readonly property color lightTextMuted: "#a3a3a3"
    // Borders and UI elements
    readonly property color lightBorderColor: "#e5e5e5"
    readonly property color lightBorderFocus: "#1a1a1a"
    readonly property color lightShadowColor: "#00000012"
    readonly property color lightInputBackground: "#fafafa"
    readonly property color lightTrackOffColor: "#d4d4d4"
    readonly property color lightTrackColor: "#e5e5e5"
    readonly property color lightHoverBackground: "#f5f5f5"
    readonly property color lightDividerColor: "#e5e5e5"
    
    // ============================================
    // DARK THEME COLORS - Classic Monochrome
    // Primary accent: Pure white/off-white
    // ============================================
    readonly property color darkBackgroundColor: "#0a0a0a"
    readonly property color darkCardBackground: "#171717"
    // Primary - Clean white for dark theme
    readonly property color darkPrimaryColor: "#fafafa"
    readonly property color darkPrimaryHover: "#e5e5e5"
    readonly property color darkPrimaryPressed: "#ffffff"
    // All semantic colors unified as shades of white/grey
    readonly property color darkAccentColor: "#fafafa"
    readonly property color darkSuccessColor: "#fafafa"      // Active/On state - bright white
    readonly property color darkWarningColor: "#a3a3a3"      // Attention - medium grey
    readonly property color darkErrorColor: "#737373"        // Inactive/Off - dimmer grey
    // Text hierarchy using light grays
    readonly property color darkTextPrimary: "#fafafa"
    readonly property color darkTextSecondary: "#a3a3a3"
    readonly property color darkTextMuted: "#525252"
    // Borders and UI elements
    readonly property color darkBorderColor: "#262626"
    readonly property color darkBorderFocus: "#fafafa"
    readonly property color darkShadowColor: "#00000060"
    readonly property color darkInputBackground: "#0a0a0a"
    readonly property color darkTrackOffColor: "#404040"
    readonly property color darkTrackColor: "#262626"
    readonly property color darkHoverBackground: "#262626"
    readonly property color darkDividerColor: "#262626"
    
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
    
    // Button specific colors - inverted for contrast
    readonly property color buttonTextColor: isDarkTheme ? "#0a0a0a" : "#ffffff"
    readonly property color outlineButtonText: primaryColor
    
    // Switch knob color - contrasting with track
    readonly property color switchKnobColor: isDarkTheme ? "#0a0a0a" : "#ffffff"
    
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
