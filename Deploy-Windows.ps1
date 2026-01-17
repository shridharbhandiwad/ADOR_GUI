# ============================================================================
# Radar Visualization - Windows Deployment Script (PowerShell)
# Creates a standalone executable package with all Qt dependencies
# ============================================================================
#
# Usage:
#   .\Deploy-Windows.ps1
#   .\Deploy-Windows.ps1 -QtPath "C:\Qt\6.7.0\msvc2022_64"
#   .\Deploy-Windows.ps1 -SkipBuild (if already built)
#
# ============================================================================

param(
    [string]$QtPath = "",
    [switch]$SkipBuild = $false,
    [switch]$SkipZip = $false,
    [string]$Version = "1.0.0"
)

$ErrorActionPreference = "Stop"

# Configuration
$AppName = "RadarVisualization"
$DeployDir = "deploy\$AppName"
$ZipName = "${AppName}_Windows_v${Version}.zip"

Write-Host ""
Write-Host "============================================================================" -ForegroundColor Cyan
Write-Host "   RADAR VISUALIZATION - WINDOWS DEPLOYMENT PACKAGE BUILDER" -ForegroundColor Cyan
Write-Host "============================================================================" -ForegroundColor Cyan
Write-Host ""

# ============================================================================
# Function: Find Qt Installation
# ============================================================================
function Find-QtInstallation {
    param([string]$ProvidedPath)
    
    # If path provided, use it
    if ($ProvidedPath -and (Test-Path "$ProvidedPath\bin\qmake.exe")) {
        return "$ProvidedPath\bin"
    }
    
    # Check if qmake is in PATH
    $qmakeInPath = Get-Command qmake -ErrorAction SilentlyContinue
    if ($qmakeInPath) {
        return Split-Path $qmakeInPath.Source
    }
    
    # Search common Qt installation paths
    $searchPaths = @(
        "C:\Qt\6.8.0\msvc2022_64\bin",
        "C:\Qt\6.7.3\msvc2022_64\bin",
        "C:\Qt\6.7.2\msvc2022_64\bin",
        "C:\Qt\6.7.1\msvc2022_64\bin",
        "C:\Qt\6.7.0\msvc2022_64\bin",
        "C:\Qt\6.6.3\msvc2022_64\bin",
        "C:\Qt\6.6.2\msvc2022_64\bin",
        "C:\Qt\6.6.1\msvc2022_64\bin",
        "C:\Qt\6.6.0\msvc2022_64\bin",
        "C:\Qt\6.5.3\msvc2019_64\bin",
        "C:\Qt\6.5.2\msvc2019_64\bin",
        "C:\Qt\6.5.1\msvc2019_64\bin",
        "C:\Qt\6.5.0\msvc2019_64\bin",
        "C:\Qt\5.15.2\msvc2019_64\bin",
        "C:\Qt\5.15.2\msvc2017_64\bin",
        "C:\Qt\6.8.0\mingw_64\bin",
        "C:\Qt\6.7.0\mingw_64\bin",
        "C:\Qt\6.6.0\mingw_64\bin",
        "C:\Qt\5.15.2\mingw81_64\bin"
    )
    
    foreach ($path in $searchPaths) {
        if (Test-Path "$path\qmake.exe") {
            return $path
        }
    }
    
    return $null
}

# ============================================================================
# Function: Find Build Tool
# ============================================================================
function Find-BuildTool {
    # Check for nmake (Visual Studio)
    $nmake = Get-Command nmake -ErrorAction SilentlyContinue
    if ($nmake) {
        return @{ Command = "nmake"; Name = "nmake (Visual Studio)" }
    }
    
    # Check for jom (faster parallel nmake)
    $jom = Get-Command jom -ErrorAction SilentlyContinue
    if ($jom) {
        return @{ Command = "jom"; Name = "jom (Qt parallel make)" }
    }
    
    # Check for mingw32-make
    $mingw = Get-Command mingw32-make -ErrorAction SilentlyContinue
    if ($mingw) {
        return @{ Command = "mingw32-make"; Name = "mingw32-make (MinGW)" }
    }
    
    # Check for make
    $make = Get-Command make -ErrorAction SilentlyContinue
    if ($make) {
        return @{ Command = "make"; Name = "make" }
    }
    
    return $null
}

# ============================================================================
# Step 1: Find Qt Installation
# ============================================================================
Write-Host "[1/6] Searching for Qt installation..." -ForegroundColor Yellow

$QtBinDir = Find-QtInstallation -ProvidedPath $QtPath

if (-not $QtBinDir) {
    Write-Host ""
    Write-Host "ERROR: Qt installation not found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Please install Qt and either:" -ForegroundColor White
    Write-Host "  1. Add Qt bin directory to your PATH environment variable" -ForegroundColor Gray
    Write-Host "  2. Or run: .\Deploy-Windows.ps1 -QtPath 'C:\Qt\6.7.0\msvc2022_64'" -ForegroundColor Gray
    Write-Host ""
    Write-Host "Download Qt from: https://www.qt.io/download" -ForegroundColor Cyan
    exit 1
}

$QMake = Join-Path $QtBinDir "qmake.exe"
$WinDeployQt = Join-Path $QtBinDir "windeployqt.exe"
if (-not (Test-Path $WinDeployQt)) {
    $WinDeployQt = Join-Path $QtBinDir "windeployqt6.exe"
}

$QtVersion = & $QMake -query QT_VERSION 2>$null

Write-Host "   Qt Binary Dir: $QtBinDir" -ForegroundColor Green
Write-Host "   Qt Version: $QtVersion" -ForegroundColor Green
Write-Host ""

# ============================================================================
# Step 2: Find Build Tool
# ============================================================================
Write-Host "[2/6] Checking build tools..." -ForegroundColor Yellow

$BuildTool = Find-BuildTool

if (-not $BuildTool) {
    Write-Host ""
    Write-Host "ERROR: No build tool found!" -ForegroundColor Red
    Write-Host ""
    Write-Host "For Visual Studio: Run this from 'Developer PowerShell for VS'" -ForegroundColor Gray
    Write-Host "For MinGW: Ensure mingw32-make is in your PATH" -ForegroundColor Gray
    exit 1
}

Write-Host "   Using: $($BuildTool.Name)" -ForegroundColor Green
Write-Host ""

# ============================================================================
# Step 3: Build Release
# ============================================================================
if (-not $SkipBuild) {
    Write-Host "[3/6] Building Release version..." -ForegroundColor Yellow
    
    # Clean previous builds
    Write-Host "   Cleaning previous builds..." -ForegroundColor Gray
    Remove-Item -Path "release" -Recurse -Force -ErrorAction SilentlyContinue
    Remove-Item -Path "debug" -Recurse -Force -ErrorAction SilentlyContinue
    Remove-Item -Path "deploy" -Recurse -Force -ErrorAction SilentlyContinue
    Remove-Item -Path "*.o", "moc_*", "ui_*", "qrc_*" -Force -ErrorAction SilentlyContinue
    Remove-Item -Path "Makefile", "Makefile.*" -Force -ErrorAction SilentlyContinue
    
    # Run qmake
    Write-Host "   Running qmake..." -ForegroundColor Gray
    
    # Determine spec based on build tool
    $QMakeSpec = ""
    if ($BuildTool.Command -eq "nmake" -or $BuildTool.Command -eq "jom") {
        $QMakeSpec = "-spec win32-msvc"
    }
    
    $qmakeArgs = @("RadarVisualization.pro", "CONFIG+=release", "CONFIG+=qtquickcompiler")
    if ($QMakeSpec) {
        $qmakeArgs = @("RadarVisualization.pro", $QMakeSpec, "CONFIG+=release", "CONFIG+=qtquickcompiler")
    }
    
    & $QMake @qmakeArgs
    if ($LASTEXITCODE -ne 0) {
        # Try without spec
        & $QMake "RadarVisualization.pro" "CONFIG+=release"
        if ($LASTEXITCODE -ne 0) {
            Write-Host "ERROR: qmake failed!" -ForegroundColor Red
            exit 1
        }
    }
    
    # Build
    Write-Host "   Compiling (this may take a few minutes)..." -ForegroundColor Gray
    
    if ($BuildTool.Command -eq "jom") {
        & jom release
    } elseif ($BuildTool.Command -eq "nmake") {
        & nmake release
    } else {
        & $BuildTool.Command release
    }
    
    if ($LASTEXITCODE -ne 0) {
        # Try without 'release' target
        & $BuildTool.Command
        if ($LASTEXITCODE -ne 0) {
            Write-Host "ERROR: Build failed!" -ForegroundColor Red
            exit 1
        }
    }
    
    Write-Host "   Build successful!" -ForegroundColor Green
} else {
    Write-Host "[3/6] Skipping build (using existing executable)..." -ForegroundColor Yellow
}

# Find executable
$ExePath = $null
if (Test-Path "release\$AppName.exe") {
    $ExePath = "release\$AppName.exe"
} elseif (Test-Path "$AppName.exe") {
    $ExePath = "$AppName.exe"
}

if (-not $ExePath) {
    Write-Host "ERROR: Executable not found!" -ForegroundColor Red
    exit 1
}

Write-Host "   Executable: $ExePath" -ForegroundColor Green
Write-Host ""

# ============================================================================
# Step 4: Create Deployment Directory
# ============================================================================
Write-Host "[4/6] Creating deployment package..." -ForegroundColor Yellow

# Create deploy directory
New-Item -ItemType Directory -Path $DeployDir -Force | Out-Null

# Copy executable
Copy-Item $ExePath -Destination $DeployDir -Force
Write-Host "   Copied executable to $DeployDir" -ForegroundColor Green

# Copy customer README if exists
if (Test-Path "README_CUSTOMER.txt") {
    Copy-Item "README_CUSTOMER.txt" -Destination "$DeployDir\README.txt" -Force
    Write-Host "   Copied README for customer" -ForegroundColor Green
}
Write-Host ""

# ============================================================================
# Step 5: Run windeployqt
# ============================================================================
Write-Host "[5/6] Collecting Qt dependencies with windeployqt..." -ForegroundColor Yellow

$deployArgs = @(
    "--release",
    "--qmldir", ".",
    "--no-translations",
    "--no-system-d3d-compiler",
    "--no-opengl-sw",
    (Join-Path $DeployDir "$AppName.exe")
)

& $WinDeployQt @deployArgs 2>&1 | ForEach-Object { Write-Host "   $_" -ForegroundColor Gray }

if ($LASTEXITCODE -ne 0) {
    Write-Host "   Warning: windeployqt had issues, trying minimal deployment..." -ForegroundColor Yellow
    & $WinDeployQt (Join-Path $DeployDir "$AppName.exe")
}

Write-Host "   Qt dependencies collected" -ForegroundColor Green
Write-Host ""

# ============================================================================
# Step 6: Create ZIP Package
# ============================================================================
if (-not $SkipZip) {
    Write-Host "[6/6] Creating ZIP package..." -ForegroundColor Yellow
    
    # Remove existing ZIP
    Remove-Item $ZipName -Force -ErrorAction SilentlyContinue
    
    # Create ZIP
    Compress-Archive -Path "$DeployDir\*" -DestinationPath $ZipName -Force
    
    if (Test-Path $ZipName) {
        $zipSize = (Get-Item $ZipName).Length / 1MB
        Write-Host "   ZIP package created: $ZipName ($([math]::Round($zipSize, 2)) MB)" -ForegroundColor Green
    } else {
        Write-Host "   Warning: Could not create ZIP" -ForegroundColor Yellow
    }
} else {
    Write-Host "[6/6] Skipping ZIP creation..." -ForegroundColor Yellow
}

Write-Host ""

# ============================================================================
# Summary
# ============================================================================
Write-Host "============================================================================" -ForegroundColor Cyan
Write-Host "   DEPLOYMENT COMPLETE!" -ForegroundColor Cyan
Write-Host "============================================================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Package Contents:" -ForegroundColor White
Write-Host "   Folder: $DeployDir\" -ForegroundColor Gray
if (Test-Path $ZipName) {
    Write-Host "   ZIP:    $ZipName" -ForegroundColor Gray
}
Write-Host ""
Write-Host "The customer can:" -ForegroundColor White
Write-Host "   1. Extract the ZIP file" -ForegroundColor Gray
Write-Host "   2. Run $AppName.exe directly - no Qt installation needed!" -ForegroundColor Gray
Write-Host ""
Write-Host "Package includes:" -ForegroundColor White
Write-Host "   - $AppName.exe (main application)" -ForegroundColor Gray
Write-Host "   - Qt runtime libraries (DLLs)" -ForegroundColor Gray
Write-Host "   - Qt plugins (platforms, imageformats, etc.)" -ForegroundColor Gray
Write-Host "   - QML modules (QtQuick, QtQuick.Controls, etc.)" -ForegroundColor Gray
Write-Host ""
Write-Host "----------------------------------------------------------------------------" -ForegroundColor Cyan
Write-Host "   Ready for distribution!" -ForegroundColor Green
Write-Host "----------------------------------------------------------------------------" -ForegroundColor Cyan
Write-Host ""

# List package contents
Write-Host "Package contents:" -ForegroundColor White
Get-ChildItem $DeployDir | ForEach-Object { Write-Host "   $($_.Name)" -ForegroundColor Gray }
Write-Host ""

# Calculate total size
$totalSize = (Get-ChildItem $DeployDir -Recurse | Measure-Object -Property Length -Sum).Sum / 1MB
Write-Host "Total package size: $([math]::Round($totalSize, 2)) MB" -ForegroundColor White
Write-Host ""
