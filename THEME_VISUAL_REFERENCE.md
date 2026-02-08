# Theme Visual Reference Guide

## Quick Visual Guide to All Theme Combinations

This guide provides a visual reference for how different UI components look in each theme combination.

---

## 🎨 Theme Combinations Overview

### 1. Light + Colored (Default) ☀️🌈
**Description**: Bright, vibrant, and energetic
- Background: Light blue-gray (#f6f8fc)
- Primary: Premium Indigo (#4c51bf)
- Success: Emerald Green (#059669)
- Warning: Amber (#d97706)
- Error: Red (#dc2626)
- Info: Sky Blue (#0284c7)

**Best For**: 
- Daytime use
- Colorful data visualization
- Modern, energetic feel

**Visual Characteristics**:
- Buttons: Indigo gradient with white text
- Connected badge: Green background with green text
- Statistics cards: Color-coded (green, red, blue)
- Sliders: Indigo track fill
- Switches: Indigo when ON
- Menu highlights: Light indigo tint
- Logos: Indigo gradient

---

### 2. Light + Monochrome ☀️⚫
**Description**: Classic, elegant, timeless
- Background: Light gray (#f5f7fa)
- Primary: Deep Charcoal (#1c1e21)
- All semantic colors: Shades of gray
- Success: Dark gray
- Warning: Medium gray
- Error: Light gray

**Best For**:
- Professional presentations
- Print-friendly interface
- Classic, timeless aesthetic
- Reduced visual distraction

**Visual Characteristics**:
- Buttons: Dark gradient with white text
- Connected badge: Dark background
- Statistics cards: Unified dark tints
- Sliders: Dark track fill
- Switches: Dark when ON
- Menu highlights: Light gray tint
- Logos: Dark gradient

---

### 3. Dark + Colored 🌙🌈
**Description**: Rich, engaging, modern dark
- Background: Deep space (#0d1117)
- Primary: Luminous Indigo (#8b92ff)
- Success: Bright Emerald (#10b981)
- Warning: Bright Amber (#f59e0b)
- Error: Soft Red (#f87171)
- Info: Bright Sky Blue (#38bdf8)

**Best For**:
- Night time use
- Reduced eye strain
- Modern, engaging interface
- Rich color on dark

**Visual Characteristics**:
- Buttons: Bright indigo gradient with dark text
- Connected badge: Bright green on dark
- Statistics cards: Vivid color tints
- Sliders: Bright indigo track
- Switches: Luminous indigo when ON
- Menu highlights: Subtle indigo glow
- Logos: Bright indigo gradient

---

### 4. Dark + Monochrome 🌙⚫
**Description**: Elegant, refined, sophisticated
- Background: Deep space (#0d1117)
- Primary: Refined White (#f6f8fa)
- All semantic colors: Shades of white/gray
- Success: Bright white
- Warning: Medium gray
- Error: Dim gray

**Best For**:
- Maximum focus
- Elegant presentations
- Professional dark theme
- Minimal distraction
- Terminal/coding aesthetic

**Visual Characteristics**:
- Buttons: White gradient with dark text
- Connected badge: White on dark
- Statistics cards: Unified white tints
- Sliders: White track fill
- Switches: White when ON
- Menu highlights: Subtle white glow
- Logos: White gradient

---

## 🎯 Component-by-Component Visual Guide

### Header Section

#### Logo Badge
- **Light + Colored**: Indigo square with white "R"
- **Light + Monochrome**: Dark charcoal square with white "R"
- **Dark + Colored**: Luminous indigo square with dark "R"
- **Dark + Monochrome**: White square with dark "R"

#### Connection Status Badge
- **Connected (Colored)**: Green tint background, green border, green dot with inner fill
- **Connected (Monochrome)**: Primary color tint, primary border, primary dot
- **Disconnected**: Muted color tint, no inner dot fill, pulsing animation

#### Style Indicator Icon
- **Colored Mode**: 4-color wheel (indigo, green, amber, red)
- **Monochrome Mode**: 4-shade wheel (black, dark gray, light gray, lightest gray)

---

### Panels

#### UDP Configuration Panel

**Statistics Cards**:

*Light + Colored*:
- Received: Light green tint (#05966910), green text
- Dropped: Light red tint (#dc262610), red text (when > 0)
- Rate: Light blue tint (#0284c710), blue text

*Light + Monochrome*:
- All cards: Light charcoal tint, charcoal text
- Unified appearance, distinguished by position

*Dark + Colored*:
- Received: Bright green tint, bright green text
- Dropped: Soft red tint, soft red text (when > 0)
- Rate: Bright blue tint, bright blue text

*Dark + Monochrome*:
- All cards: Light white tint, white text
- Elegant unified appearance

**Connect Button**:
- Colored Mode: Green (not connected), Red outline (connected)
- Monochrome Mode: Primary color for both states

---

#### Amplification Panel

**Gradient Indicator** (showing thresholds):

*Colored Mode*:
```
[Light Green] → [Green] | [Indigo] → [Purple] | [Amber] | [Red] → [Light Red]
   0%         30%      |    40%        50%     |  70%   |  80%      100%
```

*Monochrome Mode*:
```
[Light Primary] → [Primary] → [Primary] → [Light Primary]
      0%            30%         70%           100%
```

**Threshold Markers**:
- Colored: Green line (inner), Amber line (outer)
- Monochrome: Primary color for both

---

### Buttons

#### Filled Buttons (e.g., Apply, Connect)

*Light + Colored*:
- Background: Indigo gradient (#4c51bf → darker)
- Text: White
- Hover: Lighter gradient
- Shadow: Indigo tint

*Light + Monochrome*:
- Background: Charcoal gradient (#1c1e21 → darker)
- Text: White
- Hover: Lighter charcoal
- Shadow: Dark tint

*Dark + Colored*:
- Background: Luminous indigo gradient (#8b92ff → darker)
- Text: Dark
- Hover: Brighter gradient
- Shadow: Bright indigo glow

*Dark + Monochrome*:
- Background: White gradient (#f6f8fa → darker)
- Text: Dark
- Hover: Brighter white
- Shadow: White glow

#### Outline Buttons (e.g., Reset)

All themes:
- Background: Transparent with hover tint
- Border: Primary color (2px)
- Text: Primary color
- Hover: Light primary tint background

---

### Input Fields

#### Text Fields (focused)

*All Light Themes*:
- Background: Pure white
- Border: Primary color (2px)
- Shadow: Primary color glow (15% opacity)

*All Dark Themes*:
- Background: Deep space
- Border: Primary color (2px)
- Shadow: Primary color glow (15% opacity)

#### Sliders

*Active Track* (filled portion):
- All themes: Gradient using primary color
  - Start: Lighter primary
  - End: Primary

*Handle*:
- Background: White (light) or Dark (dark) - switchKnobColor
- Border: Primary color (3px)
- Shadow: Subtle dark shadow

---

### Switches

#### ON State
- Track: Primary color
- Knob: White (light) or Dark (dark)
- Border: Primary color (subtle)

#### OFF State
- Track: Track off color (light gray)
- Knob: White (light) or Dark (dark)
- Border: Track off color

---

### Menus

#### Menu Background
- Light themes: White (#ffffff)
- Dark themes: Dark card (#161b22)
- Border: Theme border color
- Shadow: Theme shadow color (soft)

#### Menu Items

*Default*:
- Background: Transparent
- Text: Primary text color

*Hovered*:
- Background: Primary color tint (10% opacity)
- Text: Primary color
- Radius: 6px

*Checkmarks* (Theme menu):
- Box: Primary color when checked
- Check: Button text color (white on primary)
- Size: 16x16px

---

### Dialogs

#### Settings Popup

**Theme Preview Cards**:

*Light Theme Card*:
- Background: #fafafa
- Icon: Dark circle with white inner circle
- Text: "Light"
- Border: Dark when selected (2px)
- Checkmark: Dark circle with white ✓

*Dark Theme Card*:
- Background: #171717
- Icon: White circle with dark inner circle
- Text: "Dark"
- Border: White when selected (2px)
- Checkmark: White circle with dark ✓

**Design Mode Preview Cards**:

*Colored Card*:
- 4 Color Squares: Indigo, Green, Amber, Red
- Text: "Colored"
- Border: Primary when selected (2px)
- Checkmark: Primary circle with button text ✓

*Monochrome Card*:
- 4 Gray Squares: Black, Dark gray, Medium gray, Light gray
- Text: "Monochrome"
- Border: Primary when selected (2px)
- Checkmark: Primary circle with button text ✓

---

## 🚀 How Colors Transition

When switching themes, all colors transition smoothly over **200ms**:

```
State A (Light + Colored)
    ↓ User clicks Dark Theme
    ↓ (200ms smooth transition)
State B (Dark + Colored)
```

### Animation Details
- **Duration**: 200ms for colors, 150ms for quick interactions
- **Easing**: Default Qt easing (smooth curve)
- **Properties Animated**:
  - background colors
  - text colors
  - border colors
  - shadow colors
  - gradient colors

### Example Transition Flow

When toggling from **Light + Colored** to **Dark + Monochrome**:

1. Background: #f6f8fc → #0d1117 (200ms)
2. Primary buttons: #4c51bf → #f6f8fa (200ms)
3. Success indicators: #059669 → #f6f8fa (200ms)
4. Text: #0f1419 → #f6f8fa (200ms)
5. Borders: #e5e7eb → #21262d (200ms)
6. Shadows: #0f141918 → #00000070 (200ms)

All changes happen **simultaneously** creating a smooth, professional transition.

---

## 📋 Quick Reference Table

| Component | Light+Color | Light+Mono | Dark+Color | Dark+Mono |
|-----------|-------------|------------|------------|-----------|
| Primary Button | Indigo gradient | Charcoal gradient | Bright indigo | White gradient |
| Connected Badge | Green tint | Dark tint | Bright green | White tint |
| Slider Track | Indigo fill | Dark fill | Bright indigo | White fill |
| Menu Highlight | Light indigo | Light gray | Indigo glow | White glow |
| Focus Border | Indigo 2px | Charcoal 2px | Bright indigo 2px | White 2px |
| Shadows | Subtle dark | Dark | Bright glow | White glow |

---

## 🎨 Color Palette Reference

### Light + Colored
```
Primary:    #4c51bf (Indigo)
Success:    #059669 (Emerald)
Warning:    #d97706 (Amber)
Error:      #dc2626 (Red)
Info:       #0284c7 (Sky Blue)
Background: #f6f8fc (Light Blue Gray)
Card:       #ffffff (White)
Text:       #0f1419 (Almost Black)
```

### Light + Monochrome
```
Primary:    #1c1e21 (Deep Charcoal)
Success:    #1c1e21 (Unified)
Warning:    #4a5568 (Medium Gray)
Error:      #6b7280 (Light Gray)
Info:       #374151 (Dark Gray)
Background: #f5f7fa (Light Gray)
Card:       #ffffff (White)
Text:       #0f1419 (Almost Black)
```

### Dark + Colored
```
Primary:    #8b92ff (Luminous Indigo)
Success:    #10b981 (Bright Emerald)
Warning:    #f59e0b (Bright Amber)
Error:      #f87171 (Soft Red)
Info:       #38bdf8 (Bright Sky)
Background: #0d1117 (Deep Space)
Card:       #161b22 (Dark Card)
Text:       #f6f8fa (Off White)
```

### Dark + Monochrome
```
Primary:    #f6f8fa (Refined White)
Success:    #f6f8fa (Unified)
Warning:    #9ca3af (Medium Gray)
Error:      #6b7280 (Dim Gray)
Info:       #d1d5db (Light Gray)
Background: #0d1117 (Deep Space)
Card:       #161b22 (Dark Card)
Text:       #f6f8fa (Off White)
```

---

## 💡 Design Principles

### Colored Mode
- **Purpose**: Rich visual feedback, semantic meaning through color
- **Usage**: When colors aid understanding (status, categories, alerts)
- **Benefit**: More engaging, easier to scan visually

### Monochrome Mode
- **Purpose**: Classic elegance, focus on content, professional aesthetic
- **Usage**: When colors are distracting or unnecessary
- **Benefit**: Timeless, sophisticated, print-friendly

### Theme Selection Tips

**Choose Light + Colored** when:
- Working in well-lit environments
- Need vibrant, energetic interface
- Colors help understand data

**Choose Light + Monochrome** when:
- Want classic, professional look
- Need print-friendly colors
- Prefer minimal visual distraction

**Choose Dark + Colored** when:
- Working in low-light environments
- Want modern, engaging dark theme
- Need rich colors on dark background

**Choose Dark + Monochrome** when:
- Want elegant, sophisticated dark theme
- Need maximum focus on content
- Prefer terminal/coding aesthetic

---

## 🔍 Where to See Themes

### Live Preview Areas
1. **Settings Popup**: Theme and Design Mode cards show previews
2. **Header Indicator**: Color wheel icon reflects current mode
3. **All Panels**: Instantly reflect theme changes
4. **Menus**: Open any menu to see themed items
5. **Buttons**: Hover to see theme-appropriate highlights

### Testing Checklist
- [ ] Open Settings, toggle all combinations
- [ ] Check all 4 panels
- [ ] Open all 5 menus
- [ ] Hover over buttons and menu items
- [ ] Test all input components
- [ ] View About dialog
- [ ] Check connection status badge

---

**Navigation**: Press Settings (⚙️) → Try all theme combinations → Watch the magic! ✨
