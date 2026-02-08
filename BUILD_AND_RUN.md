# Build and Run Guide for Color Theme

## Overview
This application has two UI components:
- **Visualization Window** (C++ Qt Widgets) - Shows radar data visualizations
- **Configuration Panel** (QML) - Shows the modern color-themed configuration interface

## Building the Application

### Windows (Using Qt Creator or Command Line)

1. **Clean Previous Build:**
   ```cmd
   del /S /Q debug release Makefile* *.o moc_* qrc_*
   ```

2. **Build with Qt:**
   ```cmd
   build_qmake.bat
   ```
   
   OR manually:
   ```cmd
   qmake RadarVisualization.pro
   nmake
   ```

### Linux/Mac

1. **Clean Previous Build:**
   ```bash
   make clean
   rm -f Makefile* *.o moc_* qrc_*
   ```

2. **Build with Qt:**
   ```bash
   ./build_qmake.sh
   ```
   
   OR manually:
   ```bash
   qmake RadarVisualization.pro
   make
   ```

## Running the Application to See Color Theme

### Show ONLY the Color-Themed Configuration Panel:
```bash
./RadarVisualization --config
```
or
```bash
./RadarVisualization -c
```

### Show ONLY the Visualization Window (default):
```bash
./RadarVisualization
```
or
```bash
./RadarVisualization --visualization
```

### Show BOTH Windows:
```bash
./RadarVisualization --both
```
or
```bash
./RadarVisualization -b
```

## Color Theme Details

The color theme is **enabled by default** in the QML configuration panel with:
- **isColorMode: true** in ThemeManager.qml
- Premium indigo blue primary color (#4c51bf)
- Professional emerald green for success states
- Refined amber for warnings
- Professional red for errors
- Deep purple accent colors

## Troubleshooting

### "I don't see the color theme"

**Solution:** Make sure you're running with the `--config` flag:
```bash
./RadarVisualization --config
```

The visualization window (shown by default) uses Qt Widgets and has a monochrome theme. The color theme is in the QML configuration panel.

### "Colors are still monochrome after building"

**Solution:** The .qrc resource file needs to be recompiled. Make sure to:
1. Clean all build artifacts
2. Rebuild completely using qmake
3. Run with `--config` flag

### "Build fails"

**Solution:** Make sure you have Qt 5.15+ installed with:
- Qt Quick Controls 2
- Qt Quick Layouts
- Qt Graphical Effects

## Quick Reference

| Command | What You See |
|---------|-------------|
| `./RadarVisualization` | Visualization window only (monochrome) |
| `./RadarVisualization --config` | **Configuration panel with COLOR THEME** |
| `./RadarVisualization --both` | Both windows |

## Color vs Monochrome

- **Color Mode (Default in QML)**: Rich UI with professional colors
  - Indigo blue buttons and accents
  - Green/Red status indicators
  - Colorful gradient effects
  
- **Monochrome Mode**: Elegant two-tone design
  - Black and grey tones only
  - Professional enterprise look
  - Can be toggled in settings (when implemented)

---

**Important:** After any changes to QML files (including ThemeManager.qml), you MUST rebuild the application because QML files are compiled into the binary via the .qrc resource system.
