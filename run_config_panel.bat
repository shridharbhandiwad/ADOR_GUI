@echo off
REM Quick script to run the Configuration Panel with Color Theme

REM Find the executable
if exist "debug\RadarVisualization.exe" (
    set EXEC=debug\RadarVisualization.exe
) else if exist "release\RadarVisualization.exe" (
    set EXEC=release\RadarVisualization.exe
) else if exist "RadarVisualization.exe" (
    set EXEC=RadarVisualization.exe
) else (
    echo Error: RadarVisualization.exe not found!
    echo Please build the application first using:
    echo   build_qmake.bat
    echo   or
    echo   qmake ^&^& nmake
    exit /b 1
)

echo Running Configuration Panel with Color Theme...
echo Executable: %EXEC%
echo.

REM Run with --config flag to show the color-themed QML UI
%EXEC% --config
