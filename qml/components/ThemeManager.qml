pragma Singleton
import QtQuick 2.15

QtObject {
    id: themeManager
    
    // Theme state
    property bool isDarkTheme: false
    
    // Design mode state: false = Monochrome, true = Color
    property bool isColorMode: true
    
    // Function to toggle theme
    function toggleTheme() {
        isDarkTheme = !isDarkTheme
    }
    
    // Function to set specific theme
    function setTheme(dark) {
        isDarkTheme = dark
    }
    
    // Function to toggle design mode
    function toggleDesignMode() {
        isColorMode = !isColorMode
    }
    
    // Function to set specific design mode
    function setDesignMode(color) {
        isColorMode = color
    }
    
    // Design mode name for display
    readonly property string designModeName: isColorMode ? "Colored" : "MonoChrome"
    
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
    readonly property color monoLightBackgroundColor: "#fafafa"
    readonly property color monoLightCardBackground: "#ffffff"
    // Primary - Dark charcoal for elegant contrast
    readonly property color monoLightPrimaryColor: "#1a1a1a"
    readonly property color monoLightPrimaryHover: "#333333"
    readonly property color monoLightPrimaryPressed: "#0a0a0a"
    // All semantic colors unified as shades of grey
    readonly property color monoLightAccentColor: "#1a1a1a"
    readonly property color monoLightSuccessColor: "#1a1a1a"     // Active/On state - solid dark
    readonly property color monoLightWarningColor: "#525252"     // Attention - medium grey
    readonly property color monoLightErrorColor: "#737373"       // Inactive/Off - lighter grey
    readonly property color monoLightInfoColor: "#404040"        // Info - dark grey
    // Text hierarchy using neutral grays
    readonly property color monoLightTextPrimary: "#0a0a0a"
    readonly property color monoLightTextSecondary: "#525252"
    readonly property color monoLightTextMuted: "#a3a3a3"
    // Borders and UI elements
    readonly property color monoLightBorderColor: "#e5e5e5"
    readonly property color monoLightBorderFocus: "#1a1a1a"
    readonly property color monoLightShadowColor: "#00000012"
    readonly property color monoLightInputBackground: "#fafafa"
    readonly property color monoLightTrackOffColor: "#d4d4d4"
    readonly property color monoLightTrackColor: "#e5e5e5"
    readonly property color monoLightHoverBackground: "#f5f5f5"
    readonly property color monoLightDividerColor: "#e5e5e5"
    // Gradient colors for monochrome
    readonly property color monoLightGradientStart: "#1a1a1a"
    readonly property color monoLightGradientEnd: "#404040"
    
    // ============================================
    // DARK THEME COLORS - Classic Monochrome
    // Primary accent: Pure white/off-white
    // ============================================
    readonly property color monoDarkBackgroundColor: "#0a0a0a"
    readonly property color monoDarkCardBackground: "#171717"
    // Primary - Clean white for dark theme
    readonly property color monoDarkPrimaryColor: "#fafafa"
    readonly property color monoDarkPrimaryHover: "#e5e5e5"
    readonly property color monoDarkPrimaryPressed: "#ffffff"
    // All semantic colors unified as shades of white/grey
    readonly property color monoDarkAccentColor: "#fafafa"
    readonly property color monoDarkSuccessColor: "#fafafa"      // Active/On state - bright white
    readonly property color monoDarkWarningColor: "#a3a3a3"      // Attention - medium grey
    readonly property color monoDarkErrorColor: "#737373"        // Inactive/Off - dimmer grey
    readonly property color monoDarkInfoColor: "#d4d4d4"         // Info - light grey
    // Text hierarchy using light grays
    readonly property color monoDarkTextPrimary: "#fafafa"
    readonly property color monoDarkTextSecondary: "#a3a3a3"
    readonly property color monoDarkTextMuted: "#525252"
    // Borders and UI elements
    readonly property color monoDarkBorderColor: "#262626"
    readonly property color monoDarkBorderFocus: "#fafafa"
    readonly property color monoDarkShadowColor: "#00000060"
    readonly property color monoDarkInputBackground: "#0a0a0a"
    readonly property color monoDarkTrackOffColor: "#404040"
    readonly property color monoDarkTrackColor: "#262626"
    readonly property color monoDarkHoverBackground: "#262626"
    readonly property color monoDarkDividerColor: "#262626"
    // Gradient colors for monochrome dark
    readonly property color monoDarkGradientStart: "#fafafa"
    readonly property color monoDarkGradientEnd: "#a3a3a3"
    
    // ============================================
    // VIBRANT COLOR PALETTE - LIGHT THEME
    // Modern, rich colors with beautiful aesthetics
    // Inspired by premium design systems
    // ============================================
    readonly property color colorLightBackgroundColor: "#f8fafc"
    readonly property color colorLightCardBackground: "#ffffff"
    // Primary - Vibrant Indigo Blue
    readonly property color colorLightPrimaryColor: "#4f46e5"
    readonly property color colorLightPrimaryHover: "#4338ca"
    readonly property color colorLightPrimaryPressed: "#3730a3"
    // Semantic colors - rich and distinctive
    readonly property color colorLightAccentColor: "#8b5cf6"      // Purple accent
    readonly property color colorLightSuccessColor: "#10b981"     // Emerald green
    readonly property color colorLightWarningColor: "#f59e0b"     // Amber warning
    readonly property color colorLightErrorColor: "#ef4444"       // Red error
    readonly property color colorLightInfoColor: "#0ea5e9"        // Sky blue info
    // Text hierarchy
    readonly property color colorLightTextPrimary: "#0f172a"
    readonly property color colorLightTextSecondary: "#475569"
    readonly property color colorLightTextMuted: "#94a3b8"
    // Borders and UI elements
    readonly property color colorLightBorderColor: "#e2e8f0"
    readonly property color colorLightBorderFocus: "#4f46e5"
    readonly property color colorLightShadowColor: "#0f172a15"
    readonly property color colorLightInputBackground: "#f8fafc"
    readonly property color colorLightTrackOffColor: "#cbd5e1"
    readonly property color colorLightTrackColor: "#e2e8f0"
    readonly property color colorLightHoverBackground: "#f1f5f9"
    readonly property color colorLightDividerColor: "#e2e8f0"
    // Gradient colors for color mode
    readonly property color colorLightGradientStart: "#4f46e5"
    readonly property color colorLightGradientEnd: "#7c3aed"
    // Additional accent colors for visual richness
    readonly property color colorLightAccent2: "#06b6d4"          // Cyan
    readonly property color colorLightAccent3: "#ec4899"          // Pink
    readonly property color colorLightAccent4: "#f97316"          // Orange
    
    // ============================================
    // VIBRANT COLOR PALETTE - DARK THEME
    // Rich, luminous colors on dark backgrounds
    // High contrast and accessibility focused
    // ============================================
    readonly property color colorDarkBackgroundColor: "#0f172a"
    readonly property color colorDarkCardBackground: "#1e293b"
    // Primary - Bright Indigo for dark backgrounds
    readonly property color colorDarkPrimaryColor: "#818cf8"
    readonly property color colorDarkPrimaryHover: "#a5b4fc"
    readonly property color colorDarkPrimaryPressed: "#6366f1"
    // Semantic colors - luminous on dark
    readonly property color colorDarkAccentColor: "#a78bfa"       // Bright purple
    readonly property color colorDarkSuccessColor: "#34d399"      // Bright emerald
    readonly property color colorDarkWarningColor: "#fbbf24"      // Bright amber
    readonly property color colorDarkErrorColor: "#f87171"        // Bright red
    readonly property color colorDarkInfoColor: "#38bdf8"         // Bright sky blue
    // Text hierarchy
    readonly property color colorDarkTextPrimary: "#f8fafc"
    readonly property color colorDarkTextSecondary: "#94a3b8"
    readonly property color colorDarkTextMuted: "#475569"
    // Borders and UI elements
    readonly property color colorDarkBorderColor: "#334155"
    readonly property color colorDarkBorderFocus: "#818cf8"
    readonly property color colorDarkShadowColor: "#00000040"
    readonly property color colorDarkInputBackground: "#0f172a"
    readonly property color colorDarkTrackOffColor: "#475569"
    readonly property color colorDarkTrackColor: "#334155"
    readonly property color colorDarkHoverBackground: "#334155"
    readonly property color colorDarkDividerColor: "#334155"
    // Gradient colors for color mode dark
    readonly property color colorDarkGradientStart: "#818cf8"
    readonly property color colorDarkGradientEnd: "#c084fc"
    // Additional accent colors for visual richness
    readonly property color colorDarkAccent2: "#22d3ee"           // Bright cyan
    readonly property color colorDarkAccent3: "#f472b6"           // Bright pink
    readonly property color colorDarkAccent4: "#fb923c"           // Bright orange
    
    // ============================================
    // COMPUTED LIGHT THEME COLORS (based on design mode)
    // ============================================
    readonly property color lightBackgroundColor: isColorMode ? colorLightBackgroundColor : monoLightBackgroundColor
    readonly property color lightCardBackground: isColorMode ? colorLightCardBackground : monoLightCardBackground
    readonly property color lightPrimaryColor: isColorMode ? colorLightPrimaryColor : monoLightPrimaryColor
    readonly property color lightPrimaryHover: isColorMode ? colorLightPrimaryHover : monoLightPrimaryHover
    readonly property color lightPrimaryPressed: isColorMode ? colorLightPrimaryPressed : monoLightPrimaryPressed
    readonly property color lightAccentColor: isColorMode ? colorLightAccentColor : monoLightAccentColor
    readonly property color lightSuccessColor: isColorMode ? colorLightSuccessColor : monoLightSuccessColor
    readonly property color lightWarningColor: isColorMode ? colorLightWarningColor : monoLightWarningColor
    readonly property color lightErrorColor: isColorMode ? colorLightErrorColor : monoLightErrorColor
    readonly property color lightInfoColor: isColorMode ? colorLightInfoColor : monoLightInfoColor
    readonly property color lightTextPrimary: isColorMode ? colorLightTextPrimary : monoLightTextPrimary
    readonly property color lightTextSecondary: isColorMode ? colorLightTextSecondary : monoLightTextSecondary
    readonly property color lightTextMuted: isColorMode ? colorLightTextMuted : monoLightTextMuted
    readonly property color lightBorderColor: isColorMode ? colorLightBorderColor : monoLightBorderColor
    readonly property color lightBorderFocus: isColorMode ? colorLightBorderFocus : monoLightBorderFocus
    readonly property color lightShadowColor: isColorMode ? colorLightShadowColor : monoLightShadowColor
    readonly property color lightInputBackground: isColorMode ? colorLightInputBackground : monoLightInputBackground
    readonly property color lightTrackOffColor: isColorMode ? colorLightTrackOffColor : monoLightTrackOffColor
    readonly property color lightTrackColor: isColorMode ? colorLightTrackColor : monoLightTrackColor
    readonly property color lightHoverBackground: isColorMode ? colorLightHoverBackground : monoLightHoverBackground
    readonly property color lightDividerColor: isColorMode ? colorLightDividerColor : monoLightDividerColor
    readonly property color lightGradientStart: isColorMode ? colorLightGradientStart : monoLightGradientStart
    readonly property color lightGradientEnd: isColorMode ? colorLightGradientEnd : monoLightGradientEnd
    
    // ============================================
    // COMPUTED DARK THEME COLORS (based on design mode)
    // ============================================
    readonly property color darkBackgroundColor: isColorMode ? colorDarkBackgroundColor : monoDarkBackgroundColor
    readonly property color darkCardBackground: isColorMode ? colorDarkCardBackground : monoDarkCardBackground
    readonly property color darkPrimaryColor: isColorMode ? colorDarkPrimaryColor : monoDarkPrimaryColor
    readonly property color darkPrimaryHover: isColorMode ? colorDarkPrimaryHover : monoDarkPrimaryHover
    readonly property color darkPrimaryPressed: isColorMode ? colorDarkPrimaryPressed : monoDarkPrimaryPressed
    readonly property color darkAccentColor: isColorMode ? colorDarkAccentColor : monoDarkAccentColor
    readonly property color darkSuccessColor: isColorMode ? colorDarkSuccessColor : monoDarkSuccessColor
    readonly property color darkWarningColor: isColorMode ? colorDarkWarningColor : monoDarkWarningColor
    readonly property color darkErrorColor: isColorMode ? colorDarkErrorColor : monoDarkErrorColor
    readonly property color darkInfoColor: isColorMode ? colorDarkInfoColor : monoDarkInfoColor
    readonly property color darkTextPrimary: isColorMode ? colorDarkTextPrimary : monoDarkTextPrimary
    readonly property color darkTextSecondary: isColorMode ? colorDarkTextSecondary : monoDarkTextSecondary
    readonly property color darkTextMuted: isColorMode ? colorDarkTextMuted : monoDarkTextMuted
    readonly property color darkBorderColor: isColorMode ? colorDarkBorderColor : monoDarkBorderColor
    readonly property color darkBorderFocus: isColorMode ? colorDarkBorderFocus : monoDarkBorderFocus
    readonly property color darkShadowColor: isColorMode ? colorDarkShadowColor : monoDarkShadowColor
    readonly property color darkInputBackground: isColorMode ? colorDarkInputBackground : monoDarkInputBackground
    readonly property color darkTrackOffColor: isColorMode ? colorDarkTrackOffColor : monoDarkTrackOffColor
    readonly property color darkTrackColor: isColorMode ? colorDarkTrackColor : monoDarkTrackColor
    readonly property color darkHoverBackground: isColorMode ? colorDarkHoverBackground : monoDarkHoverBackground
    readonly property color darkDividerColor: isColorMode ? colorDarkDividerColor : monoDarkDividerColor
    readonly property color darkGradientStart: isColorMode ? colorDarkGradientStart : monoDarkGradientStart
    readonly property color darkGradientEnd: isColorMode ? colorDarkGradientEnd : monoDarkGradientEnd
    
    // ============================================
    // DYNAMIC THEME PROPERTIES (auto-switch based on isDarkTheme and isColorMode)
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
    readonly property color infoColor: isDarkTheme ? darkInfoColor : lightInfoColor
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
    readonly property color gradientStart: isDarkTheme ? darkGradientStart : lightGradientStart
    readonly property color gradientEnd: isDarkTheme ? darkGradientEnd : lightGradientEnd
    
    // Additional accent colors (available in color mode, fallback to primary in mono)
    readonly property color accent2Color: isColorMode ? (isDarkTheme ? colorDarkAccent2 : colorLightAccent2) : primaryColor
    readonly property color accent3Color: isColorMode ? (isDarkTheme ? colorDarkAccent3 : colorLightAccent3) : primaryColor
    readonly property color accent4Color: isColorMode ? (isDarkTheme ? colorDarkAccent4 : colorLightAccent4) : primaryColor
    
    // Button specific colors - adjusted for color mode
    readonly property color buttonTextColor: {
        if (isColorMode) {
            return "#ffffff"  // Always white text on colored buttons
        }
        return isDarkTheme ? "#0a0a0a" : "#ffffff"
    }
    readonly property color outlineButtonText: primaryColor
    
    // Switch knob color - contrasting with track
    readonly property color switchKnobColor: {
        if (isColorMode) {
            return "#ffffff"  // White knob in color mode
        }
        return isDarkTheme ? "#0a0a0a" : "#ffffff"
    }
    
    // Status indicator colors - more semantic in color mode
    readonly property color statusConnected: isColorMode ? successColor : primaryColor
    readonly property color statusDisconnected: isColorMode ? errorColor : textMuted
    readonly property color statusWarning: isColorMode ? warningColor : textSecondary
    readonly property color statusInfo: isColorMode ? infoColor : textSecondary
    
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
    
    // Full style name combining theme and design mode
    readonly property string fullStyleName: themeName + " · " + designModeName
}
