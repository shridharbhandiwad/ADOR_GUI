# Cross-Platform Layout and Sizing Fixes

## Problem
The RadarGUI application had layout and sizing issues when ported to different PCs with varying:
- Screen resolutions
- DPI settings (96, 125%, 150%, etc.)
- Display sizes

This resulted in overlapped UI elements and incorrect widget sizing as shown in the reported screenshot.

## Root Causes
1. **Hard-coded pixel sizes** - Widgets used fixed pixel values (e.g., `setMinimumSize(300, 250)`)
2. **Fixed splitter sizes** - Splitter sizes used absolute pixel values instead of proportions
3. **Non-responsive panel widths** - DSP Settings panel had fixed min/max widths
4. **No DPI awareness** - Layout calculations didn't account for screen DPI scaling

## Solutions Implemented

### 1. MainWindow.cpp
- **Responsive window sizing**: Window minimum size now based on 60% screen width, 70% screen height
- **DPI-aware calculations**: All sizes multiplied by DPI scale factor (logicalDotsPerInch / 96.0)
- **Proportional splitter sizes**: 
  - Top horizontal splitter: 67% PPI, 33% Track Table (based on available width)
  - Vertical splitter: 60% top row, 40% FFT (based on available height)
- **Responsive DSP panel**: Min width 280px, max width 20% of screen width
- **DPI-scaled widgets**: PPI and FFT widgets sized based on DPI scale
- **Responsive field sizes**: Input fields in DSP Settings panel scale with DPI

### 2. Widget Updates
All custom widgets updated to remove hard-coded minimum sizes:

#### PPIWidget.cpp
- Removed `setMinimumSize(400, 200)`
- Added `setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding)`
- Size now determined by parent layout

#### FFTWidget.cpp
- Removed `setMinimumSize(400, 200)`
- Added expanding size policy
- Responsive to layout changes

#### TimeSeriesPlotsWidget.cpp
- Updated `TimeSeriesPlotWidget` - removed hard-coded minimum size
- Updated `RangeVelocityPlotWidget` - removed hard-coded minimum size
- Both widgets now expand with available space

#### SpeedMeasurementWidget.cpp
- `SpeedometerGauge` - removed `setMinimumSize(500, 500)`
- `DigitalSpeedDisplay` - removed `setMinimumSize(180, 100)`
- Speedometer, cards, and displays now resize with layout
- `ModernSpeedButton` - uses responsive sizing with minimum height for usability

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

### Proportional Layout Sizing
```cpp
int totalTopWidth = screenGeometry.width() - 400;
int ppiWidth = static_cast<int>(totalTopWidth * 0.67);  // 67%
int tableWidth = static_cast<int>(totalTopWidth * 0.33); // 33%
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
