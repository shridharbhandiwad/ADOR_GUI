cmake_minimum_required(VERSION 3.16)
project(RadarVisualization VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt
find_package(Qt6 QUIET COMPONENTS Core Widgets Network Qml Quick QuickControls2)
if (NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network Qml Quick QuickControls2)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# Enable automatic MOC, UIC, and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    main.cpp
    MainWindow.cpp
    PPIWidget.cpp
    FFTWidget.cpp
    SpeedMeasurementWidget.cpp
    TimeSeriesPlotsWidget.cpp
)

set(HEADERS
    MainWindow.h
    PPIWidget.h
    FFTWidget.h
    SpeedMeasurementWidget.h
    TimeSeriesPlotsWidget.h
    DataStructures.h
)

# Create executable
add_executable(RadarVisualization ${SOURCES} ${HEADERS})

# Link Qt libraries
if (QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(RadarVisualization Qt6::Core Qt6::Widgets Qt6::Network Qt6::Qml Qt6::Quick Qt6::QuickControls2)
else()
    target_link_libraries(RadarVisualization Qt5::Core Qt5::Widgets Qt5::Network Qt5::Qml Qt5::Quick Qt5::QuickControls2)
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(RadarVisualization PRIVATE /W4)
else()
    target_compile_options(RadarVisualization PRIVATE -Wall -Wextra -Wpedantic)
endif()