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
    // Primary accent: Deep charcoal with refined grays
    // Enterprise-grade professional aesthetic
    // ============================================
    readonly property color monoLightBackgroundColor: "#f5f7fa"
    readonly property color monoLightCardBackground: "#ffffff"
    // Primary - Deep charcoal for sophisticated contrast
    readonly property color monoLightPrimaryColor: "#1c1e21"
    readonly property color monoLightPrimaryHover: "#2d3137"
    readonly property color monoLightPrimaryPressed: "#0f1113"
    // All semantic colors unified as refined shades of grey
    readonly property color monoLightAccentColor: "#1c1e21"
    readonly property color monoLightSuccessColor: "#1c1e21"     // Active/On state - solid dark
    readonly property color monoLightWarningColor: "#4a5568"     // Attention - medium grey
    readonly property color monoLightErrorColor: "#6b7280"       // Inactive/Off - lighter grey
    readonly property color monoLightInfoColor: "#374151"        // Info - dark grey
    // Text hierarchy using sophisticated grays
    readonly property color monoLightTextPrimary: "#0f1419"
    readonly property color monoLightTextSecondary: "#4a5568"
    readonly property color monoLightTextMuted: "#9ca3af"
    // Borders and UI elements
    readonly property color monoLightBorderColor: "#e5e7eb"
    readonly property color monoLightBorderFocus: "#1c1e21"
    readonly property color monoLightShadowColor: "#0f141918"
    readonly property color monoLightInputBackground: "#f9fafb"
    readonly property color monoLightTrackOffColor: "#d1d5db"
    readonly property color monoLightTrackColor: "#e5e7eb"
    readonly property color monoLightHoverBackground: "#f3f4f6"
    readonly property color monoLightDividerColor: "#e5e7eb"
    // Gradient colors for monochrome
    readonly property color monoLightGradientStart: "#1c1e21"
    readonly property color monoLightGradientEnd: "#374151"
    
    // ============================================
    // DARK THEME COLORS - Classic Monochrome
    // Primary accent: Refined white with sophisticated grays
    // Enterprise-grade dark mode aesthetic
    // ============================================
    readonly property color monoDarkBackgroundColor: "#0d1117"
    readonly property color monoDarkCardBackground: "#161b22"
    // Primary - Refined white for dark theme
    readonly property color monoDarkPrimaryColor: "#f6f8fa"
    readonly property color monoDarkPrimaryHover: "#e6eaee"
    readonly property color monoDarkPrimaryPressed: "#ffffff"
    // All semantic colors unified as refined shades of white/grey
    readonly property color monoDarkAccentColor: "#f6f8fa"
    readonly property color monoDarkSuccessColor: "#f6f8fa"      // Active/On state - bright white
    readonly property color monoDarkWarningColor: "#9ca3af"      // Attention - medium grey
    readonly property color monoDarkErrorColor: "#6b7280"        // Inactive/Off - dimmer grey
    readonly property color monoDarkInfoColor: "#d1d5db"         // Info - light grey
    // Text hierarchy using refined light grays
    readonly property color monoDarkTextPrimary: "#f6f8fa"
    readonly property color monoDarkTextSecondary: "#9ca3af"
    readonly property color monoDarkTextMuted: "#4a5568"
    // Borders and UI elements
    readonly property color monoDarkBorderColor: "#21262d"
    readonly property color monoDarkBorderFocus: "#f6f8fa"
    readonly property color monoDarkShadowColor: "#00000070"
    readonly property color monoDarkInputBackground: "#0d1117"
    readonly property color monoDarkTrackOffColor: "#374151"
    readonly property color monoDarkTrackColor: "#21262d"
    readonly property color monoDarkHoverBackground: "#21262d"
    readonly property color monoDarkDividerColor: "#21262d"
    // Gradient colors for monochrome dark
    readonly property color monoDarkGradientStart: "#f6f8fa"
    readonly property color monoDarkGradientEnd: "#9ca3af"
    
    // ============================================
    // PREMIUM COLOR PALETTE - LIGHT THEME
    // Sophisticated colors inspired by enterprise design systems
    // Professional, refined, and accessible
    // ============================================
    readonly property color colorLightBackgroundColor: "#f6f8fc"
    readonly property color colorLightCardBackground: "#ffffff"
    // Primary - Premium Indigo Blue (refined for enterprise)
    readonly property color colorLightPrimaryColor: "#4c51bf"
    readonly property color colorLightPrimaryHover: "#434190"
    readonly property color colorLightPrimaryPressed: "#3730a3"
    // Semantic colors - refined and professional
    readonly property color colorLightAccentColor: "#7c3aed"      // Deep purple accent
    readonly property color colorLightSuccessColor: "#059669"     // Professional emerald green
    readonly property color colorLightWarningColor: "#d97706"     // Refined amber warning
    readonly property color colorLightErrorColor: "#dc2626"       // Professional red error
    readonly property color colorLightInfoColor: "#0284c7"        // Deep sky blue info
    // Text hierarchy - refined for readability
    readonly property color colorLightTextPrimary: "#0f1419"
    readonly property color colorLightTextSecondary: "#4a5568"
    readonly property color colorLightTextMuted: "#9ca3af"
    // Borders and UI elements
    readonly property color colorLightBorderColor: "#e5e7eb"
    readonly property color colorLightBorderFocus: "#4c51bf"
    readonly property color colorLightShadowColor: "#0f141918"
    readonly property color colorLightInputBackground: "#f9fafb"
    readonly property color colorLightTrackOffColor: "#cbd5e1"
    readonly property color colorLightTrackColor: "#e5e7eb"
    readonly property color colorLightHoverBackground: "#f3f4f6"
    readonly property color colorLightDividerColor: "#e5e7eb"
    // Gradient colors for color mode - sophisticated blend
    readonly property color colorLightGradientStart: "#4c51bf"
    readonly property color colorLightGradientEnd: "#7c3aed"
    // Additional accent colors for visual richness
    readonly property color colorLightAccent2: "#0891b2"          // Refined cyan
    readonly property color colorLightAccent3: "#db2777"          // Deep pink
    readonly property color colorLightAccent4: "#ea580c"          // Deep orange
    
    // ============================================
    // PREMIUM COLOR PALETTE - DARK THEME
    // Refined, luminous colors on dark backgrounds
    // Enterprise-grade with excellent contrast and accessibility
    // ============================================
    readonly property color colorDarkBackgroundColor: "#0d1117"
    readonly property color colorDarkCardBackground: "#161b22"
    // Primary - Refined Indigo for dark backgrounds
    readonly property color colorDarkPrimaryColor: "#8b92ff"
    readonly property color colorDarkPrimaryHover: "#a5acff"
    readonly property color colorDarkPrimaryPressed: "#6366f1"
    // Semantic colors - luminous yet refined on dark
    readonly property color colorDarkAccentColor: "#a78bfa"       // Elegant purple
    readonly property color colorDarkSuccessColor: "#10b981"      // Professional emerald
    readonly property color colorDarkWarningColor: "#f59e0b"      // Refined amber
    readonly property color colorDarkErrorColor: "#f87171"        // Clear red
    readonly property color colorDarkInfoColor: "#38bdf8"         // Clear sky blue
    // Text hierarchy - refined for dark mode readability
    readonly property color colorDarkTextPrimary: "#f6f8fa"
    readonly property color colorDarkTextSecondary: "#9ca3af"
    readonly property color colorDarkTextMuted: "#4a5568"
    // Borders and UI elements
    readonly property color colorDarkBorderColor: "#21262d"
    readonly property color colorDarkBorderFocus: "#8b92ff"
    readonly property color colorDarkShadowColor: "#00000070"
    readonly property color colorDarkInputBackground: "#0d1117"
    readonly property color colorDarkTrackOffColor: "#4a5568"
    readonly property color colorDarkTrackColor: "#21262d"
    readonly property color colorDarkHoverBackground: "#21262d"
    readonly property color colorDarkDividerColor: "#21262d"
    // Gradient colors for color mode dark
    readonly property color colorDarkGradientStart: "#8b92ff"
    readonly property color colorDarkGradientEnd: "#a78bfa"
    // Additional accent colors for visual richness
    readonly property color colorDarkAccent2: "#22d3ee"           // Bright cyan
    readonly property color colorDarkAccent3: "#ec4899"           // Elegant pink
    readonly property color colorDarkAccent4: "#fb923c"           // Warm orange
    
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
    // TYPOGRAPHY - Enterprise-grade font system
    // ============================================
    readonly property string fontFamily: "Inter, 'Segoe UI', -apple-system, BlinkMacSystemFont, 'SF Pro Display', sans-serif"
    readonly property int fontSizeXs: 11
    readonly property int fontSizeSm: 13
    readonly property int fontSizeMd: 15
    readonly property int fontSizeLg: 17
    readonly property int fontSizeXl: 20
    readonly property int fontSize2Xl: 28
    readonly property int fontSize3Xl: 36
    
    // Font weights for precise typography
    readonly property int fontWeightLight: Font.Light
    readonly property int fontWeightRegular: Font.Normal
    readonly property int fontWeightMedium: Font.Medium
    readonly property int fontWeightSemiBold: Font.DemiBold
    readonly property int fontWeightBold: Font.Bold
    
    // ============================================
    // SPACING - Refined spacing scale
    // ============================================
    readonly property int spacingXs: 4
    readonly property int spacingSm: 8
    readonly property int spacingMd: 16
    readonly property int spacingLg: 24
    readonly property int spacingXl: 32
    readonly property int spacing2Xl: 48
    
    // ============================================
    // BORDER RADIUS - Softer, more refined corners
    // ============================================
    readonly property int radiusSm: 8
    readonly property int radiusMd: 12
    readonly property int radiusLg: 16
    readonly property int radiusXl: 24
    
    // ============================================
    // SHADOWS - Multi-layered depth system
    // ============================================
    readonly property string shadowSmall: isDarkTheme ? "#00000050" : "#00000010"
    readonly property string shadowMedium: isDarkTheme ? "#00000070" : "#00000018"
    readonly property string shadowLarge: isDarkTheme ? "#00000090" : "#00000020"
    
    // ============================================
    // ANIMATION DURATIONS - Smooth micro-interactions
    // ============================================
    readonly property int animationFast: 120
    readonly property int animationNormal: 200
    readonly property int animationSlow: 350
    
    // Theme name for display
    readonly property string themeName: isDarkTheme ? "Dark" : "Light"
    
    // Full style name combining theme and design mode
    readonly property string fullStyleName: themeName + " · " + designModeName
}
