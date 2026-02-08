# Comprehensive Theming Guide

## Overview

This guide documents the comprehensive theming system implemented in the Radar Visualization application. Every UI component now reflects the selected color theme, providing a professional and modern user experience.

## Theme Options

### 1. Theme Mode (Light/Dark)
- **Light Theme**: Clean, bright interface with dark text on light backgrounds
- **Dark Theme**: Professional dark interface with light text on dark backgrounds
- **Toggle**: Press `Ctrl+T` or use Menu Bar → Theme → Toggle Theme

### 2. Design Mode (Colored/Monochrome)
- **Colored Mode**: Rich, vibrant color palette with semantic colors
  - Primary: Premium Indigo Blue (#4c51bf)
  - Success: Professional Emerald Green (#059669)
  - Warning: Refined Amber (#d97706)
  - Error: Professional Red (#dc2626)
  - Info: Deep Sky Blue (#0284c7)
  - Accent: Deep Purple (#7c3aed)
  
- **Monochrome Mode**: Classic, elegant grayscale palette
  - Primary: Deep Charcoal/Refined White (theme-dependent)
  - All semantic colors unified as shades of gray
  - Timeless, professional aesthetic
  
- **Toggle**: Press `Ctrl+D` or use Menu Bar → Theme → Toggle Design Mode

## Theming Coverage

### Core UI Components

#### 1. Main Window
- ✅ Background color transitions
- ✅ Header with gradient logo
- ✅ Menu bar styling
- ✅ Status indicators (connection, time, theme)
- ✅ Settings button with hover effects

#### 2. Menu System
All menus fully themed with:
- ✅ Background colors from theme
- ✅ Border colors with transitions
- ✅ Menu items with hover highlights
- ✅ Checkmarks for theme menu items
- ✅ Separators using theme divider colors
- ✅ Shadow effects matching theme

**Themed Menus:**
- File Menu
- View Menu
- Connection Menu
- Theme Menu (with checkable items)
- Tools Menu

#### 3. Panels

**UDP Configuration Panel:**
- ✅ Card background and borders
- ✅ Status badge (Connected/Disconnected) with semantic colors
- ✅ Statistics cards with adaptive colors
  - Received: Success color (green in colored mode, primary in mono)
  - Dropped: Error color when packets dropped (red in colored mode)
  - Rate: Info color (blue in colored mode, primary in mono)
- ✅ Connect/Disconnect button with semantic colors
- ✅ All text elements using theme typography
- ✅ Smooth color transitions on theme changes

**Output Configuration Panel:**
- ✅ Tab bar with active/hover states
- ✅ Radio buttons with theme colors
- ✅ Selection cards with border highlights
- ✅ All input fields themed
- ✅ Dividers using theme colors

**Angle Correction Panel:**
- ✅ Method selection cards
- ✅ Icon backgrounds with theme colors
- ✅ Active/inactive state indication
- ✅ Parameter cards with adaptive opacity
- ✅ Info hints using theme colors

**Amplification Panel:**
- ✅ Large value display with theme colors
- ✅ Auto-enable switch with theme colors
- ✅ Threshold cards using primary color
- ✅ Visual gradient indicator (adaptive to color/mono mode)
  - Colored Mode: Multi-color gradient (green → purple → amber → red)
  - Monochrome Mode: Unified primary color gradient
- ✅ EEPROM button themed

#### 4. Input Components

**ModernButton:**
- ✅ Filled variant with gradient
- ✅ Outline variant with border
- ✅ Hover and pressed states
- ✅ Text color adapts to background
- ✅ Shadow effects using theme colors
- ✅ Scale animation on press

**ModernTextField:**
- ✅ Background color transitions
- ✅ Border colors (normal/focus)
- ✅ Label color changes on focus
- ✅ Placeholder text using muted color
- ✅ Focus shadow effect

**ModernSlider:**
- ✅ Track colors from theme
- ✅ Handle with theme border
- ✅ Filled track with gradient
- ✅ Value display using primary color
- ✅ Min/max labels using secondary color

**ModernSwitch:**
- ✅ Track color (on: primary, off: track off color)
- ✅ Knob color adapts to theme
- ✅ Border colors with transitions
- ✅ Label and description text themed
- ✅ Shadow effect on knob

**ModernSpinBox:**
- ✅ Input background and borders
- ✅ Up/down button hovers
- ✅ Text and selection colors
- ✅ Focus state with shadow
- ✅ Label color changes

**ModernComboBox:**
- ✅ Dropdown background
- ✅ Selected item highlighting
- ✅ Popup menu styling
- ✅ Border and focus states
- ✅ Indicator arrow color

#### 5. Dialogs

**Settings Popup:**
- ✅ Background and border colors
- ✅ Header with icons
- ✅ Section headers
- ✅ Theme toggle with icon
- ✅ Theme preview cards (Light/Dark)
- ✅ Design Mode toggle with palette icon
- ✅ Design Mode preview cards (Colored/Monochrome)
  - Color preview: Shows 4 vibrant colors
  - Mono preview: Shows 4 shades of gray
- ✅ Scrollbar styling
- ✅ Close button with hover
- ✅ Dividers using theme colors

**About Dialog:**
- ✅ Background with gradient
- ✅ Header with logo
- ✅ All text elements themed
- ✅ Divider colors
- ✅ Button styling
- ✅ Shadow effects

#### 6. Header Components

**Logo:**
- ✅ Gradient background using primary color
- ✅ Text color adapts to theme
- ✅ Shadow effect

**Connection Status Badge:**
- ✅ Background tint using semantic color
- ✅ Border color with semantic meaning
- ✅ Status indicator dot with pulse animation
- ✅ Text color from theme

**Style Indicator:**
- ✅ Color palette icon (shows 4 colors or 4 grays)
- ✅ Text showing current theme combination
- ✅ Updates dynamically

**Settings Button:**
- ✅ Background hover effect
- ✅ Border colors
- ✅ Gear icon using primary color
- ✅ Rotation animation on hover
- ✅ Tooltip styled

## Color Transitions

All color changes feature smooth 200ms transitions:
```qml
Behavior on color {
    ColorAnimation { duration: 200 }
}
```

This applies to:
- Background colors
- Border colors
- Text colors
- Button colors
- Icon colors
- Shadow colors
- Gradient colors

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl+T` | Toggle Theme (Light/Dark) |
| `Ctrl+D` | Toggle Design Mode (Colored/Monochrome) |
| `Ctrl+Shift+L` | Set Light Theme |
| `Ctrl+Shift+D` | Set Dark Theme |
| `Ctrl+Shift+C` | Set Colored Mode |
| `Ctrl+Shift+M` | Set Monochrome Mode |

## Theme Manager Properties

The `ThemeManager` singleton provides all color properties:

### Dynamic Colors (Auto-switch based on theme)
- `backgroundColor` - Main window background
- `cardBackground` - Card/panel backgrounds
- `primaryColor` - Primary brand color
- `primaryHover` - Primary hover state
- `primaryPressed` - Primary pressed state
- `accentColor` - Accent highlights
- `successColor` - Success/connected states
- `warningColor` - Warning states
- `errorColor` - Error/disconnected states
- `infoColor` - Info/neutral states
- `textPrimary` - Primary text
- `textSecondary` - Secondary text
- `textMuted` - Muted/disabled text
- `borderColor` - Standard borders
- `borderFocus` - Focused element borders
- `shadowColor` - Shadow effects
- `inputBackground` - Input field backgrounds
- `trackColor` - Slider/switch tracks
- `hoverBackground` - Hover backgrounds
- `dividerColor` - Divider lines
- `gradientStart` - Gradient start color
- `gradientEnd` - Gradient end color

### Special Colors
- `buttonTextColor` - Button text (white on colored buttons)
- `switchKnobColor` - Switch knob (contrasting)
- `statusConnected` - Connection success indicator
- `statusDisconnected` - Connection error indicator

### Typography
- `fontFamily` - Inter, Segoe UI, SF Pro Display
- Font sizes: xs(11), sm(13), md(15), lg(17), xl(20), 2xl(28), 3xl(36)
- Font weights: Light, Regular, Medium, SemiBold, Bold

### Spacing
- xs(4), sm(8), md(16), lg(24), xl(32), 2xl(48)

### Border Radius
- sm(8), md(12), lg(16), xl(24)

### Animation Durations
- fast(120ms), normal(200ms), slow(350ms)

## Implementation Best Practices

### 1. Always Add Color Transitions
```qml
property color myColor: ThemeManager.primaryColor

Behavior on myColor {
    ColorAnimation { duration: 200 }
}
```

### 2. Use Theme Properties
```qml
// Good
color: ThemeManager.textPrimary

// Avoid
color: "#0f1419"
```

### 3. Adaptive Design
```qml
// Use isColorMode for conditional styling
color: ThemeManager.isColorMode ? ThemeManager.successColor : ThemeManager.primaryColor
```

### 4. Semantic Colors
Use semantic colors appropriately:
- `successColor` for positive/connected states
- `errorColor` for negative/disconnected states
- `warningColor` for attention states
- `infoColor` for neutral information
- `primaryColor` for general highlights

### 5. Gradients
```qml
gradient: Gradient {
    GradientStop { position: 0.0; color: ThemeManager.gradientStart }
    GradientStop { position: 1.0; color: ThemeManager.gradientEnd }
}
```

## Testing Themes

To test all theme combinations:

1. **Light + Colored** (Default)
   - Navigate through all panels
   - Hover over buttons and menu items
   - Test all input components

2. **Light + Monochrome**
   - Press `Ctrl+D` to toggle
   - Verify grayscale palette
   - Check semantic color unification

3. **Dark + Colored**
   - Press `Ctrl+T` to toggle theme
   - Verify color contrast
   - Check visibility

4. **Dark + Monochrome**
   - Press `Ctrl+D` again
   - Verify complete grayscale
   - Test all interactive elements

## Debugging Theme Issues

### Console Logging
The app logs theme status on startup:
```
===========================================
ThemeManager Initialized
Color Mode: ENABLED ✓
Theme: Light
Primary Color: #4c51bf (Indigo)
===========================================

Main Window Loaded
Background Color: #f6f8fc
Primary Color: #4c51bf
Color Mode Active: true
===========================================
```

### Visual Indicators
- Settings icon in header toolbar
- Theme name display (e.g., "Light · Colored")
- Color palette icon in header

### Common Issues
1. **Component not changing color**: Add `Behavior on color`
2. **Harsh transitions**: Check animation duration (should be 200ms)
3. **Wrong color in monochrome**: Use conditional check with `isColorMode`

## Architecture

### Theme Manager (`ThemeManager.qml`)
- Singleton QML component
- Central source of truth for all colors
- Computed properties for light/dark themes
- Design mode toggle affects all semantic colors

### Component Pattern
```qml
Rectangle {
    // 1. Declare theme properties
    property color primaryColor: ThemeManager.primaryColor
    
    // 2. Use in component
    color: primaryColor
    
    // 3. Add transition
    Behavior on color {
        ColorAnimation { duration: 200 }
    }
}
```

## Benefits

✅ **Consistency**: All components use the same theme system
✅ **Flexibility**: Easy to switch between 4 theme combinations
✅ **Professional**: Smooth transitions and polished appearance
✅ **Accessible**: Both light and dark themes for different preferences
✅ **Maintainable**: Centralized theme management
✅ **Modern**: Latest design trends and best practices

## Future Enhancements

Potential improvements:
- [ ] Custom color themes (user-defined palettes)
- [ ] High contrast mode for accessibility
- [ ] Accent color picker
- [ ] Theme import/export
- [ ] Per-panel theme overrides
- [ ] Theme scheduling (auto dark mode at night)

## Conclusion

Every UI component in the application now comprehensively reflects the selected theme. The implementation provides a professional, modern, and highly polished user experience with smooth transitions and consistent styling throughout.

**Theme Combinations Available:**
1. Light + Colored (Default) - Vibrant and modern
2. Light + Monochrome - Classic and professional
3. Dark + Colored - Rich and engaging
4. Dark + Monochrome - Elegant and timeless

All components, from buttons to menus to dialogs, seamlessly adapt to any theme combination.
