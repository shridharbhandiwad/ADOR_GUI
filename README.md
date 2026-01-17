# ADOR GUI

A modern, elegant Qt QML application for ADOR device control and monitoring.

## Features

- **Light Theme Design**: Clean, professional appearance with a carefully crafted color palette
- **2x2 Dialog Layout**: Four dedicated panels for complete device management
- **Modern UI Components**: Custom-styled buttons, inputs, switches, and status indicators
- **Material Design**: Built on Qt Quick Controls 2 with Material theme

## Dialog Panels

| Panel | Description |
|-------|-------------|
| **Configuration** | System settings, connection parameters, and device setup |
| **Monitoring** | Real-time metrics, statistics, and live data visualization |
| **Control** | Device commands, operation modes, and manual controls |
| **Diagnostics** | System health, troubleshooting, and event logs |

## Project Structure

```
ADOR_GUI/
├── CMakeLists.txt          # Qt6 CMake build configuration
├── src/
│   └── main.cpp            # Application entry point
├── qml/
│   ├── Main.qml            # Main window with 2x2 grid layout
│   ├── style/
│   │   ├── Theme.qml       # Light theme design tokens
│   │   └── qmldir          # Style module definition
│   ├── components/
│   │   ├── DialogCard.qml       # Collapsible card container
│   │   ├── ModernButton.qml     # Styled button variants
│   │   ├── ModernTextField.qml  # Input with label & validation
│   │   ├── ModernComboBox.qml   # Dropdown selector
│   │   ├── ModernSwitch.qml     # Toggle switch
│   │   ├── StatusIndicator.qml  # Status display with pulse
│   │   └── qmldir               # Components module
│   └── dialogs/
│       ├── ConfigurationDialog.qml
│       ├── MonitoringDialog.qml
│       ├── ControlDialog.qml
│       ├── DiagnosticsDialog.qml
│       └── qmldir               # Dialogs module
└── resources/
    └── resources.qrc       # Qt resource collection
```

## Requirements

- Qt 6.5 or higher
- CMake 3.16+
- C++17 compatible compiler

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Running

```bash
./ADOR_GUI
```

## Design System

### Color Palette (Light Theme)

| Color | Hex | Usage |
|-------|-----|-------|
| Primary | `#2563EB` | Main actions, Configuration panel |
| Success | `#10B981` | Monitoring panel, positive states |
| Accent | `#8B5CF6` | Control panel, secondary actions |
| Warning | `#F59E0B` | Diagnostics panel, alerts |
| Background | `#F8FAFC` | Main background |
| Surface | `#FFFFFF` | Cards and panels |

### Typography

- **Font Family**: Segoe UI, Roboto, system sans-serif
- **Sizes**: 10px (XS) to 32px (Display)
- **Weights**: Light (300) to Bold (700)

### Spacing

Based on 8px grid system with values from 2px to 48px.

## UI Components

### ModernButton
Variants: `primary`, `secondary`, `outline`, `ghost`, `success`, `danger`
Sizes: `small`, `medium`, `large`

### ModernTextField
Features: Label, placeholder, icon prefix, suffix, helper text, error state

### ModernSwitch
Features: Label, description, custom accent color

### StatusIndicator
States: `idle`, `active`, `success`, `warning`, `error`, `loading`
Modes: Compact and full-width

### DialogCard
Features: Collapsible content, accent color header, scroll support

## License

Proprietary - ADOR Systems
