# Cross-Platform Layout and Sizing Fixes

## Problem
The RadarGUI application had layout and sizing issues when ported to different PCs with varying:
- Screen resolutions
- DPI settings (96, 125%, 150%, etc.)
- Display sizes

This resulted in overlapped UI elements and incorrect widget sizing as shown in the reported screenshot.

## Root Causes
1. **Hard-coded pixel sizes** - Widgets used fixed pixel values without DPI scaling
2. **Fixed splitter sizes** - Splitter sizes used absolute pixel values calculated from screen geometry
3. **Non-responsive panel widths** - DSP Settings panel had overly restrictive max widths
4. **Widget minimum sizes** - SpeedMeasurementWidget and TimeSeriesPlotsWidget had large hardcoded minimum sizes

## Solutions Implemented (Updated 2026-02-17)

### 1. MainWindow.cpp
- **Responsive window sizing**: Window minimum size now based on 60% screen width, 70% screen height
- **DPI-aware calculations**: All sizes multiplied by DPI scale factor (logicalDotsPerInch / 96.0)
- **Stretch-factor based splitter sizing**: 
  - Top horizontal splitter: Uses stretch factors (2:1 ratio) instead of fixed pixel widths
  - Vertical splitter: Uses stretch factors (3:2 ratio) instead of fixed pixel heights
  - This ensures proportional scaling regardless of actual screen size
- **Improved DSP panel**: Min width 250px, max width 22% of screen (capped at 450px)
- **DPI-scaled widgets**: PPI and FFT widgets with flexible minimum sizes and expanding size policies
- **Responsive field sizes**: Input fields in DSP Settings panel scale with DPI
- **Size policies**: All main widgets now use QSizePolicy::Expanding for better responsiveness

### 2. Widget Updates (Updated 2026-02-17)
All custom widgets updated to remove hard-coded minimum sizes and use size policies:

#### PPIWidget.cpp
- Already had `setMinimumSize(400, 200)` commented out
- Uses `setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)`
- Minimum size now set in MainWindow with DPI scaling

#### FFTWidget.cpp
- No hardcoded minimum sizes
- Expanding size policy set in MainWindow
- Responsive to layout changes

#### TimeSeriesPlotsWidget.cpp
- **FIXED**: `DigitalRangeRateDisplay` - removed `setMinimumSize(240, 100)`
- Changed to `setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred)`
- Removed hardcoded size in setupFilterPanel
- Display now adapts to available space

#### SpeedMeasurementWidget.cpp
- **FIXED**: `SpeedometerGauge` - removed `setMinimumSize(600, 600)`
- **FIXED**: `DigitalSpeedDisplay` - removed `setMinimumSize(200, 80)`
- **FIXED**: `SpeedMeasurementWidget` - removed `setMinimumSize(1000, 700)`
- **FIXED**: `m_topSpeedCard` - removed `setMinimumSize(300, 240)`
- **FIXED**: `m_topSpeedDisplay` - removed `setMinimumSize(260, 100)`
- **FIXED**: `m_outputCard` - removed `setMinimumSize(300, 220)`
- All components now use QSizePolicy::Expanding to adapt to available space

### 3. Dialog Updates (dialogs.cpp)
All dialogs now calculate minimum sizes based on screen DPI:

#### UdpConfigDialog
- Base size: 350x250
- Scaled by DPI factor
- Responsive to different screen densities

#### OutputConfigDialog
- Base size: 450x350
- DPI-aware sizing

#### DSPSettingsDialog
- Base size: 550x450
- Scales with screen DPI

### 4. DPI Scaling Improvements (main.cpp)
- Added `Qt::HighDpiScaleFactorRoundingPolicy::PassThrough` (Qt 5.14+)
- Better fractional DPI scaling (125%, 150%, 175%, etc.)
- More accurate representation on high-DPI displays

## Technical Details

### DPI Scale Factor Calculation
```cpp
qreal dpiScale = screen->logicalDotsPerInch() / 96.0;
```
- 96 DPI = 1.0x (100% scaling)
- 120 DPI = 1.25x (125% scaling)
- 144 DPI = 1.5x (150% scaling)

### Example Size Calculation
```cpp
int ppiMinWidth = static_cast<int>(250 * dpiScale);
int ppiMinHeight = static_cast<int>(200 * dpiScale);
```

### Proportional Layout Sizing (Updated to use stretch factors)
```cpp
// Old approach (problematic):
// int totalTopWidth = screenGeometry.width() - 400;
// int ppiWidth = static_cast<int>(totalTopWidth * 0.67);
// topHorizontalSplitter->setSizes({ppiWidth, tableWidth});

// New approach (better):
topHorizontalSplitter->setStretchFactor(0, 2);  // PPI gets 2x space
topHorizontalSplitter->setStretchFactor(1, 1);  // Track Table gets 1x space
```

## Benefits
1. **Cross-platform compatibility** - Works correctly on different screen sizes and DPI settings
2. **Automatic scaling** - UI adapts to display characteristics without manual intervention
3. **No overlap** - Proper spacing maintained regardless of screen configuration
4. **Better UX** - Consistent appearance across different systems
5. **Future-proof** - Handles new display technologies and resolutions

## Testing Recommendations
Test on systems with:
- Different screen resolutions (1920x1080, 2560x1440, 3840x2160, etc.)
- Various DPI settings (100%, 125%, 150%, 175%, 200%)
- Multiple monitor setups
- Different operating systems (Windows, Linux with different DEs)

## Files Modified
1. MainWindow.cpp - Main layout and widget sizing
2. PPIWidget.cpp - PPI display widget
3. FFTWidget.cpp - FFT display widget
4. TimeSeriesPlotsWidget.cpp - Time series plot widgets
5. SpeedMeasurementWidget.cpp - Speed measurement widgets
6. dialogs.cpp - Configuration dialogs
7. main.cpp - DPI scaling policy

## Backward Compatibility
All changes maintain backward compatibility:
- Minimum sizes ensure usability on small screens
- Fallback values used if screen information unavailable
- Works with Qt 5.12+ (with enhanced features in Qt 5.14+)
