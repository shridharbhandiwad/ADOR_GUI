# Windows Deployment Guide - Radar Visualization

This guide explains how to create a standalone Windows executable package that can be distributed to customers without requiring Qt installation on their machines.

## Quick Start

### Option 1: Using Batch Script (Recommended)
```batch
deploy_windows.bat
```

### Option 2: Using PowerShell
```powershell
.\Deploy-Windows.ps1
```

### Option 3: Using PowerShell with Custom Qt Path
```powershell
.\Deploy-Windows.ps1 -QtPath "C:\Qt\6.7.0\msvc2022_64"
```

## Prerequisites

### 1. Qt Installation
- Download and install Qt from https://www.qt.io/download
- Required Qt modules:
  - Qt Core
  - Qt GUI
  - Qt Widgets
  - Qt Network
  - Qt Quick/QML
  - Qt Quick Controls 2

### 2. Build Tools (one of the following)

#### Visual Studio (Recommended for Windows)
- Install Visual Studio 2019 or 2022
- Include "Desktop development with C++" workload
- Run deployment from "Developer Command Prompt for VS" or "Developer PowerShell for VS"

#### MinGW
- Usually included with Qt installation
- Ensure `mingw32-make` is in your PATH

## Deployment Process

The deployment scripts perform these steps:

1. **Find Qt** - Automatically locates Qt installation
2. **Find Build Tools** - Locates nmake, jom, or mingw32-make
3. **Build Release** - Compiles the application in Release mode
4. **Create Deploy Folder** - Creates `deploy\RadarVisualization\`
5. **Run windeployqt** - Collects all required Qt DLLs and plugins
6. **Create ZIP** - Packages everything into a distributable ZIP file

## Output

After successful deployment, you'll have:

```
deploy/
└── RadarVisualization/
    ├── RadarVisualization.exe      # Main application
    ├── Qt6Core.dll                  # Qt runtime libraries
    ├── Qt6Gui.dll
    ├── Qt6Widgets.dll
    ├── Qt6Network.dll
    ├── Qt6Quick.dll
    ├── Qt6Qml.dll
    ├── ...                          # Other Qt DLLs
    ├── platforms/                   # Platform plugins
    │   └── qwindows.dll
    ├── imageformats/                # Image format plugins
    ├── QtQuick/                     # QML modules
    └── ...

RadarVisualization_Windows_v1.0.0.zip   # Ready-to-distribute package
```

## Customer Instructions

Share the ZIP file with customers along with these simple instructions:

1. Extract the ZIP file to any location
2. Double-click `RadarVisualization.exe` to run
3. No installation required!

## Troubleshooting

### Qt Not Found
- Ensure Qt is installed
- Add Qt bin directory to PATH: `C:\Qt\6.7.0\msvc2022_64\bin`
- Or specify path: `.\Deploy-Windows.ps1 -QtPath "C:\Qt\6.7.0\msvc2022_64"`

### Build Tools Not Found
- Run from "Developer Command Prompt for VS"
- Or ensure MinGW is in your PATH

### Missing DLLs on Customer Machine
If the customer reports missing DLL errors:
1. Ensure you're using Release build (not Debug)
2. Check that windeployqt completed successfully
3. Install Visual C++ Redistributable on customer machine:
   - https://aka.ms/vs/17/release/vc_redist.x64.exe

### Application Crashes
1. Try running from command prompt to see error messages
2. Ensure all QML files are properly included in resources
3. Check for missing Qt plugins

## Advanced Options

### PowerShell Script Options
```powershell
# Skip build (use existing executable)
.\Deploy-Windows.ps1 -SkipBuild

# Skip ZIP creation
.\Deploy-Windows.ps1 -SkipZip

# Specify version number
.\Deploy-Windows.ps1 -Version "2.0.0"

# Combine options
.\Deploy-Windows.ps1 -QtPath "C:\Qt\6.7.0\msvc2022_64" -Version "1.5.0"
```

### Manual Deployment

If scripts don't work, manual deployment:

1. **Build Release**
   ```batch
   qmake RadarVisualization.pro CONFIG+=release
   nmake release
   ```

2. **Create deploy folder**
   ```batch
   mkdir deploy\RadarVisualization
   copy release\RadarVisualization.exe deploy\RadarVisualization\
   ```

3. **Run windeployqt**
   ```batch
   windeployqt --release --qmldir . deploy\RadarVisualization\RadarVisualization.exe
   ```

4. **Create ZIP**
   - Use Windows Explorer or 7-Zip to create ZIP from deploy folder

## Size Optimization

The default deployment includes all potentially needed libraries. To reduce size:

1. Use `--no-translations` (already enabled)
2. Use `--no-opengl-sw` for machines with proper GPU drivers (already enabled)
3. Remove unused QML modules from the deploy folder
4. Consider using UPX to compress executables (optional)

## Version History

- v1.0.0 - Initial release with full Qt Quick/QML support
