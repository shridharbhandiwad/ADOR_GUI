pragma Singleton
import QtQuick

QtObject {
    // ═══════════════════════════════════════════════════════════════════════════
    // ADOR GUI Light Theme - Elegant & Modern Design System
    // ═══════════════════════════════════════════════════════════════════════════
    
    // ───────────────────────────────────────────────────────────────────────────
    // Primary Colors - Professional Blue Palette
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color primaryColor: "#2563EB"        // Vibrant blue
    readonly property color primaryLight: "#3B82F6"        // Lighter blue for hover
    readonly property color primaryDark: "#1D4ED8"         // Darker blue for pressed
    readonly property color primarySurface: "#EFF6FF"      // Very light blue surface
    
    // ───────────────────────────────────────────────────────────────────────────
    // Accent Colors - Complementary Palette
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color accentColor: "#8B5CF6"         // Purple accent
    readonly property color accentLight: "#A78BFA"         // Light purple
    readonly property color accentSurface: "#F5F3FF"       // Light purple surface
    
    // ───────────────────────────────────────────────────────────────────────────
    // Status Colors - Semantic Colors
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color successColor: "#10B981"        // Emerald green
    readonly property color successLight: "#34D399"        // Light green
    readonly property color successSurface: "#ECFDF5"      // Very light green
    
    readonly property color warningColor: "#F59E0B"        // Amber
    readonly property color warningLight: "#FBBF24"        // Light amber
    readonly property color warningSurface: "#FFFBEB"      // Very light amber
    
    readonly property color errorColor: "#EF4444"          // Red
    readonly property color errorLight: "#F87171"          // Light red
    readonly property color errorSurface: "#FEF2F2"        // Very light red
    
    readonly property color infoColor: "#06B6D4"           // Cyan
    readonly property color infoLight: "#22D3EE"           // Light cyan
    readonly property color infoSurface: "#ECFEFF"         // Very light cyan
    
    // ───────────────────────────────────────────────────────────────────────────
    // Background Colors - Clean Light Theme
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color backgroundColor: "#F8FAFC"     // Main background
    readonly property color surfaceColor: "#FFFFFF"        // Card/Panel surface
    readonly property color surfaceVariant: "#F1F5F9"      // Alternate surface
    readonly property color elevatedSurface: "#FFFFFF"     // Elevated components
    
    // ───────────────────────────────────────────────────────────────────────────
    // Text Colors - High Contrast for Readability
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color textPrimary: "#0F172A"         // Main text (slate-900)
    readonly property color textSecondary: "#475569"       // Secondary text (slate-600)
    readonly property color textTertiary: "#94A3B8"        // Tertiary text (slate-400)
    readonly property color textOnPrimary: "#FFFFFF"       // Text on primary color
    readonly property color textOnAccent: "#FFFFFF"        // Text on accent color
    
    // ───────────────────────────────────────────────────────────────────────────
    // Border & Divider Colors
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color borderColor: "#E2E8F0"         // Default border
    readonly property color borderLight: "#F1F5F9"         // Light border
    readonly property color borderFocus: "#2563EB"         // Focus border
    readonly property color dividerColor: "#E2E8F0"        // Divider lines
    
    // ───────────────────────────────────────────────────────────────────────────
    // Shadow Colors
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color shadowLight: "#0F172A10"       // Light shadow (6% opacity)
    readonly property color shadowMedium: "#0F172A15"      // Medium shadow (8% opacity)
    readonly property color shadowDark: "#0F172A20"        // Dark shadow (12% opacity)
    
    // ───────────────────────────────────────────────────────────────────────────
    // Typography - Modern Sans-Serif
    // ───────────────────────────────────────────────────────────────────────────
    readonly property string fontFamily: "Segoe UI, Roboto, -apple-system, BlinkMacSystemFont, sans-serif"
    
    readonly property int fontSizeXS: 10
    readonly property int fontSizeS: 11
    readonly property int fontSizeM: 13
    readonly property int fontSizeL: 15
    readonly property int fontSizeXL: 18
    readonly property int fontSizeXXL: 24
    readonly property int fontSizeDisplay: 32
    
    readonly property int fontWeightLight: 300
    readonly property int fontWeightRegular: 400
    readonly property int fontWeightMedium: 500
    readonly property int fontWeightSemiBold: 600
    readonly property int fontWeightBold: 700
    
    // ───────────────────────────────────────────────────────────────────────────
    // Spacing System (8px base grid)
    // ───────────────────────────────────────────────────────────────────────────
    readonly property int spacingXXS: 2
    readonly property int spacingXS: 4
    readonly property int spacingS: 8
    readonly property int spacingM: 12
    readonly property int spacingL: 16
    readonly property int spacingXL: 24
    readonly property int spacingXXL: 32
    readonly property int spacingXXXL: 48
    
    // ───────────────────────────────────────────────────────────────────────────
    // Border Radius - Smooth Rounded Corners
    // ───────────────────────────────────────────────────────────────────────────
    readonly property int radiusXS: 4
    readonly property int radiusS: 6
    readonly property int radiusM: 8
    readonly property int radiusL: 12
    readonly property int radiusXL: 16
    readonly property int radiusXXL: 24
    readonly property int radiusFull: 9999
    
    // ───────────────────────────────────────────────────────────────────────────
    // Component Heights
    // ───────────────────────────────────────────────────────────────────────────
    readonly property int buttonHeightS: 28
    readonly property int buttonHeightM: 36
    readonly property int buttonHeightL: 44
    readonly property int inputHeight: 40
    readonly property int headerHeight: 56
    readonly property int cardMinHeight: 200
    
    // ───────────────────────────────────────────────────────────────────────────
    // Animation Durations
    // ───────────────────────────────────────────────────────────────────────────
    readonly property int animationFast: 150
    readonly property int animationNormal: 250
    readonly property int animationSlow: 400
    
    // ───────────────────────────────────────────────────────────────────────────
    // Icon Sizes
    // ───────────────────────────────────────────────────────────────────────────
    readonly property int iconSizeXS: 12
    readonly property int iconSizeS: 16
    readonly property int iconSizeM: 20
    readonly property int iconSizeL: 24
    readonly property int iconSizeXL: 32
    readonly property int iconSizeXXL: 48
    
    // ───────────────────────────────────────────────────────────────────────────
    // Dialog Card Colors (for 2x2 layout differentiation)
    // ───────────────────────────────────────────────────────────────────────────
    readonly property color configCardAccent: "#2563EB"    // Blue - Configuration
    readonly property color monitorCardAccent: "#10B981"   // Green - Monitoring
    readonly property color controlCardAccent: "#8B5CF6"   // Purple - Control
    readonly property color diagCardAccent: "#F59E0B"      // Amber - Diagnostics
    
    // Helper function to get subtle gradient backgrounds
    function cardGradient(accentColor) {
        return [
            Qt.lighter(accentColor, 1.95),
            surfaceColor
        ]
    }
}
