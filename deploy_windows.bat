@echo off
setlocal enabledelayedexpansion

REM ============================================================================
REM Radar Visualization - Windows Deployment Script
REM Creates a standalone executable package with all Qt dependencies
REM ============================================================================

echo.
echo ============================================================================
echo    RADAR VISUALIZATION - WINDOWS DEPLOYMENT PACKAGE BUILDER
echo ============================================================================
echo.

REM Configuration
set APP_NAME=RadarVisualization
set VERSION=1.0.0
set DEPLOY_DIR=deploy\%APP_NAME%
set ZIP_NAME=%APP_NAME%_Windows_v%VERSION%.zip

REM ============================================================================
REM Step 1: Find Qt Installation
REM ============================================================================
echo [1/6] Searching for Qt installation...

set QMAKE_CMD=
set QT_BIN_DIR=

REM Check if qmake is in PATH
where qmake >nul 2>&1
if %errorlevel% equ 0 (
    set QMAKE_CMD=qmake
    for /f "tokens=*" %%i in ('where qmake') do (
        set "QMAKE_PATH=%%i"
        for %%F in ("!QMAKE_PATH!") do set "QT_BIN_DIR=%%~dpF"
        goto :found_qt
    )
)

REM Check common Qt installation paths (Qt 6.x and 5.x)
set QT_SEARCH_PATHS=^
C:\Qt\6.8.0\msvc2022_64\bin;^
C:\Qt\6.7.3\msvc2022_64\bin;^
C:\Qt\6.7.2\msvc2022_64\bin;^
C:\Qt\6.7.1\msvc2022_64\bin;^
C:\Qt\6.7.0\msvc2022_64\bin;^
C:\Qt\6.6.3\msvc2022_64\bin;^
C:\Qt\6.6.2\msvc2022_64\bin;^
C:\Qt\6.6.1\msvc2022_64\bin;^
C:\Qt\6.6.0\msvc2022_64\bin;^
C:\Qt\6.5.3\msvc2019_64\bin;^
C:\Qt\6.5.2\msvc2019_64\bin;^
C:\Qt\6.5.1\msvc2019_64\bin;^
C:\Qt\6.5.0\msvc2019_64\bin;^
C:\Qt\5.15.2\msvc2019_64\bin;^
C:\Qt\5.15.2\msvc2017_64\bin;^
C:\Qt\6.8.0\mingw_64\bin;^
C:\Qt\6.7.0\mingw_64\bin;^
C:\Qt\6.6.0\mingw_64\bin;^
C:\Qt\5.15.2\mingw81_64\bin

for %%P in (%QT_SEARCH_PATHS%) do (
    if exist "%%P\qmake.exe" (
        set "QMAKE_CMD=%%P\qmake.exe"
        set "QT_BIN_DIR=%%P\"
        echo    Found Qt at: %%P
        goto :found_qt
    )
)

REM Qt not found
echo.
echo ERROR: Qt installation not found!
echo.
echo Please install Qt and either:
echo   1. Add Qt bin directory to your PATH environment variable
echo   2. Or set QT_DIR environment variable to Qt installation path
echo.
echo Example Qt paths:
echo   C:\Qt\6.7.0\msvc2022_64\bin
echo   C:\Qt\5.15.2\msvc2019_64\bin
echo.
echo Download Qt from: https://www.qt.io/download
echo.
pause
exit /b 1

:found_qt
echo    Using qmake: %QMAKE_CMD%

REM Get Qt version
for /f "tokens=*" %%i in ('"%QMAKE_CMD%" -query QT_VERSION 2^>nul') do set QT_VERSION=%%i
echo    Qt Version: %QT_VERSION%

REM Set windeployqt path
set WINDEPLOYQT=%QT_BIN_DIR%windeployqt.exe
if not exist "%WINDEPLOYQT%" (
    set WINDEPLOYQT=%QT_BIN_DIR%windeployqt6.exe
)
if not exist "%WINDEPLOYQT%" (
    echo ERROR: windeployqt not found at %QT_BIN_DIR%
    pause
    exit /b 1
)
echo    Using windeployqt: %WINDEPLOYQT%
echo.

REM ============================================================================
REM Step 2: Check for Build Tools
REM ============================================================================
echo [2/6] Checking build tools...

set MAKE_CMD=
where nmake >nul 2>&1
if %errorlevel% equ 0 (
    set MAKE_CMD=nmake
    echo    Using: nmake (Visual Studio)
    goto :found_make
)

where mingw32-make >nul 2>&1
if %errorlevel% equ 0 (
    set MAKE_CMD=mingw32-make
    echo    Using: mingw32-make (MinGW)
    goto :found_make
)

where make >nul 2>&1
if %errorlevel% equ 0 (
    set MAKE_CMD=make
    echo    Using: make
    goto :found_make
)

echo ERROR: No build tool found (nmake, mingw32-make, or make)
echo.
echo For Visual Studio: Run this script from "Developer Command Prompt for VS"
echo For MinGW: Ensure mingw32-make is in your PATH
echo.
pause
exit /b 1

:found_make
echo.

REM ============================================================================
REM Step 3: Clean and Build Release
REM ============================================================================
echo [3/6] Building Release version...

REM Clean previous builds
if exist release rmdir /s /q release 2>nul
if exist debug rmdir /s /q debug 2>nul
if exist deploy rmdir /s /q deploy 2>nul
del /q *.o moc_* ui_* qrc_* Makefile Makefile.* 2>nul

REM Run qmake for release build
echo    Running qmake...
"%QMAKE_CMD%" RadarVisualization.pro -spec win32-msvc "CONFIG+=release" "CONFIG+=qtquickcompiler"
if %errorlevel% neq 0 (
    REM Try without msvc spec for MinGW
    "%QMAKE_CMD%" RadarVisualization.pro "CONFIG+=release" "CONFIG+=qtquickcompiler"
    if %errorlevel% neq 0 (
        echo ERROR: qmake failed!
        pause
        exit /b 1
    )
)

REM Build
echo    Compiling (this may take a few minutes)...
%MAKE_CMD% release
if %errorlevel% neq 0 (
    %MAKE_CMD%
    if %errorlevel% neq 0 (
        echo ERROR: Build failed!
        pause
        exit /b 1
    )
)

REM Find the executable
set EXE_PATH=
if exist "release\%APP_NAME%.exe" (
    set EXE_PATH=release\%APP_NAME%.exe
) else if exist "%APP_NAME%.exe" (
    set EXE_PATH=%APP_NAME%.exe
) else (
    echo ERROR: Executable not found after build!
    pause
    exit /b 1
)

echo    Build successful: %EXE_PATH%
echo.

REM ============================================================================
REM Step 4: Create Deployment Directory
REM ============================================================================
echo [4/6] Creating deployment package...

REM Create deploy directory
mkdir "%DEPLOY_DIR%" 2>nul

REM Copy executable
copy "%EXE_PATH%" "%DEPLOY_DIR%\" >nul
echo    Copied executable

REM Copy customer README if exists
if exist "README_CUSTOMER.txt" (
    copy "README_CUSTOMER.txt" "%DEPLOY_DIR%\README.txt" >nul
    echo    Copied README for customer
)

REM ============================================================================
REM Step 5: Run windeployqt to collect Qt dependencies
REM ============================================================================
echo [5/6] Collecting Qt dependencies with windeployqt...

REM Run windeployqt with QML support
"%WINDEPLOYQT%" --release --qmldir . --no-translations --no-system-d3d-compiler --no-opengl-sw "%DEPLOY_DIR%\%APP_NAME%.exe"

if %errorlevel% neq 0 (
    echo    Warning: windeployqt reported issues, trying with minimal options...
    "%WINDEPLOYQT%" "%DEPLOY_DIR%\%APP_NAME%.exe"
)

echo    Qt dependencies collected
echo.

REM ============================================================================
REM Step 6: Create ZIP Package
REM ============================================================================
echo [6/6] Creating ZIP package...

REM Check for PowerShell (available on all modern Windows)
where powershell >nul 2>&1
if %errorlevel% equ 0 (
    REM Remove existing ZIP if present
    if exist "%ZIP_NAME%" del /q "%ZIP_NAME%"
    
    REM Create ZIP using PowerShell
    powershell -Command "Compress-Archive -Path 'deploy\%APP_NAME%\*' -DestinationPath '%ZIP_NAME%' -Force"
    
    if exist "%ZIP_NAME%" (
        echo    ZIP package created: %ZIP_NAME%
    ) else (
        echo    Warning: Could not create ZIP automatically
        echo    Please manually ZIP the contents of: deploy\%APP_NAME%
    )
) else (
    echo    PowerShell not found - please manually ZIP: deploy\%APP_NAME%
)

REM ============================================================================
REM Done!
REM ============================================================================
echo.
echo ============================================================================
echo    DEPLOYMENT COMPLETE!
echo ============================================================================
echo.
echo Package Contents:
echo    Folder: deploy\%APP_NAME%\
if exist "%ZIP_NAME%" echo    ZIP:    %ZIP_NAME%
echo.
echo The customer can:
echo    1. Extract the ZIP file (if created)
echo    2. Run %APP_NAME%.exe directly - no Qt installation needed!
echo.
echo Package includes:
echo    - %APP_NAME%.exe (main application)
echo    - Qt runtime libraries (DLLs)
echo    - Qt plugins (platforms, imageformats, etc.)
echo    - QML modules (QtQuick, QtQuick.Controls, etc.)
echo.
echo ----------------------------------------------------------------------------
echo    Ready for distribution!
echo ----------------------------------------------------------------------------
echo.

REM List package contents
echo Package contents:
dir /b "%DEPLOY_DIR%"
echo.

REM Show package size
for %%A in ("%DEPLOY_DIR%") do (
    echo Folder location: %%~fA
)
if exist "%ZIP_NAME%" (
    for %%A in ("%ZIP_NAME%") do (
        echo ZIP file size: %%~zA bytes
    )
)

echo.
pause
