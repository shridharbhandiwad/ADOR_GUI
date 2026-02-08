# Color Theme Visibility Guide

## IMPORTANT: How to See the Color Theme

### The Issue
If you're not seeing the color theme, it's because the application has **two separate windows**:

1. **Visualization Window** (Default) - C++ Qt Widgets interface with monochrome theme
2. **Configuration Panel** (QML) - Modern color-themed interface ← **THIS HAS THE COLORS!**

### The Solution

**Run the application with the `--config` flag:**

```bash
# Linux/Mac
./RadarVisualization --config

# Windows  
RadarVisualization.exe --config

# Or use the provided scripts:
./run_config_panel.sh      # Linux/Mac
run_config_panel.bat       # Windows
```

## Step-by-Step: See the Color Theme Now

### Step 1: Rebuild the Application

The QML files (including ThemeManager.qml) are compiled into the application as resources. After any changes, you MUST rebuild:

**Windows:**
```cmd
REM Clean old build
del /S /Q *.o moc_* qrc_* Makefile*

REM Rebuild
qmake RadarVisualization.pro
nmake
```

**Linux/Mac:**
```bash
# Clean old build
make clean
rm -f *.o moc_* qrc_* Makefile*

# Rebuild
qmake RadarVisualization.pro
make
```

### Step 2: Run with --config Flag

```bash
./RadarVisualization --config
```

### Step 3: Verify Colors

You should see:
- ✅ **Indigo blue** buttons and primary color (#4c51bf)
- ✅ **Green** success indicators  
- ✅ **Red** error states
- ✅ **Purple** accent colors
- ✅ **Gradient effects** on buttons and panels
- ✅ **Colorful status indicators**

Look at the top-right corner - it should say: **"Light · Colored"**

### Step 4: Check Console Output

When you run with `--config`, you should see debug output like:

```
===========================================
ThemeManager Initialized
Color Mode: ENABLED ✓
Theme: Light
Primary Color: #4c51bf
Expected Primary: #4c51bf (Indigo)
===========================================
```

## What You Should See

### Color Mode (isColorMode: true) ✓ DEFAULT
- **Primary**: Indigo Blue (#4c51bf)
- **Success**: Emerald Green (#059669)
- **Warning**: Amber (#d97706)
- **Error**: Red (#dc2626)
- **Accent**: Deep Purple (#7c3aed)
- **Info**: Sky Blue (#0284c7)

### Monochrome Mode (isColorMode: false)
- **Everything**: Shades of black, grey, and white only
- Professional two-tone design

## Common Issues & Solutions

### Issue 1: "I only see grey/black colors"

**Causes:**
- Running the wrong window (visualization instead of config)
- Application wasn't rebuilt after theme changes
- QML cache issues

**Solutions:**
1. Make sure you're running with `--config` flag
2. Rebuild completely (clean + qmake + make)
3. Clear QML cache: `rm -rf ~/.cache/QtQuick`

### Issue 2: "Application shows visualization window only"

**Cause:** No command-line flags means it shows the C++ widgets window by default.

**Solution:** Add `--config` flag to see the QML color-themed UI.

### Issue 3: "Changes to ThemeManager.qml don't appear"

**Cause:** QML files are compiled into the binary via qml.qrc resource file.

**Solution:** Full rebuild required:
```bash
make clean
rm -f qrc_qml.cpp moc_*.cpp *.o
qmake && make
```

## Current Configuration

As of the latest changes:

```qml
// qml/components/ThemeManager.qml
property bool isDarkTheme: false    // Light theme
property bool isColorMode: true     // COLOR MODE ENABLED ✓
```

This means:
- ✅ Color mode is **ENABLED** by default
- ✅ Light theme is active (not dark)
- ✅ You should see **rich colors** when running with `--config`

## Architecture Overview

```
RadarVisualization Application
│
├─ Visualization Window (C++ Qt Widgets)
│  ├─ Default when running without flags
│  ├─ Shows radar plots, FFT, PPI, etc.
│  └─ Has monochrome theme (by design)
│
└─ Configuration Panel (QML) ← **COLOR THEME HERE!**
   ├─ Run with --config flag
   ├─ Modern, colorful UI
   ├─ Settings and configuration
   └─ Uses ThemeManager singleton
```

## Quick Commands Reference

```bash
# See COLOR theme (QML Config Panel)
./RadarVisualization --config

# See visualization only (monochrome by design)
./RadarVisualization

# See BOTH windows
./RadarVisualization --both

# Help
./RadarVisualization --help
```

## File Locations

| File | Purpose |
|------|---------|
| `qml/components/ThemeManager.qml` | Color theme configuration (isColorMode: true) |
| `qml/main.qml` | Main QML window entry point |
| `qml.qrc` | QML resource file (must be recompiled) |
| `main.cpp` | Application entry point with command-line parsing |

## Verification Checklist

- [ ] Application rebuilt after theme changes
- [ ] Running with `--config` flag
- [ ] Console shows "Color Mode: ENABLED ✓"
- [ ] Top-right corner shows "Light · Colored"
- [ ] Buttons are indigo blue, not grey
- [ ] Status indicators show green/red, not grey
- [ ] Gradient effects visible on buttons

## Still Not Working?

If you've done all of the above and still see only grey/black:

1. **Check the source code:**
   ```bash
   grep "isColorMode" qml/components/ThemeManager.qml
   ```
   Should show: `property bool isColorMode: true`

2. **Verify qml.qrc includes ThemeManager:**
   ```bash
   grep "ThemeManager" qml.qrc
   ```
   Should show: `<file>qml/components/ThemeManager.qml</file>`

3. **Check for QML errors:**
   Run with debug output:
   ```bash
   QT_LOGGING_RULES="*.debug=true" ./RadarVisualization --config
   ```

4. **Nuclear option - complete clean:**
   ```bash
   git clean -fdx
   qmake
   make
   ./RadarVisualization --config
   ```

## Success Indicators

When everything is working correctly, you'll see:

1. **Console Output:**
   ```
   ThemeManager Initialized
   Color Mode: ENABLED ✓
   Primary Color: #4c51bf
   ```

2. **Visual UI:**
   - Indigo blue buttons with gradients
   - Green success colors
   - Colorful status badges
   - "Light · Colored" in top-right
   - Color wheel icon next to theme name

3. **Settings Panel:**
   - Theme toggle shows "Light" (moon icon)
   - Style name shows "Light · Colored"
   - All buttons and controls are colorful

---

**Remember:** The color theme is in the **QML Configuration Panel** which requires the `--config` flag. The default visualization window uses a monochrome theme by design.
