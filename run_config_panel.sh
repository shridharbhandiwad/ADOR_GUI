#!/bin/bash
# Quick script to run the Configuration Panel with Color Theme

# Find the executable
if [ -f "./debug/RadarVisualization" ]; then
    EXEC="./debug/RadarVisualization"
elif [ -f "./release/RadarVisualization" ]; then
    EXEC="./release/RadarVisualization"
elif [ -f "./RadarVisualization" ]; then
    EXEC="./RadarVisualization"
elif [ -f "./debug/RadarVisualization.exe" ]; then
    EXEC="./debug/RadarVisualization.exe"
elif [ -f "./release/RadarVisualization.exe" ]; then
    EXEC="./release/RadarVisualization.exe"
elif [ -f "./RadarVisualization.exe" ]; then
    EXEC="./RadarVisualization.exe"
else
    echo "Error: RadarVisualization executable not found!"
    echo "Please build the application first using:"
    echo "  ./build_qmake.sh"
    echo "  or"
    echo "  qmake && make"
    exit 1
fi

echo "Running Configuration Panel with Color Theme..."
echo "Executable: $EXEC"
echo ""

# Run with --config flag to show the color-themed QML UI
$EXEC --config
