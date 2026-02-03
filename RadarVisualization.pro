# Qt Project File for Radar Data Visualization
# Updated for Qt QML UI with modern light theme

QT += core gui network widgets quick quickcontrols2 qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 qmltypes

# QML module configuration
QML_IMPORT_NAME = RadarApp
QML_IMPORT_MAJOR_VERSION = 1

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    PPIWidget.cpp \
    FFTWidget.cpp \
    SpeedMeasurementWidget.cpp \
    dialogs.cpp \
    udphandler.cpp

HEADERS += \
    DataStructures.h \
    MainWindow.h \
    PPIWidget.h \
    FFTWidget.h \
    SpeedMeasurementWidget.h \
    dialogs.h \
    udphandler.h \
    structures.h

RESOURCES += \
    qml.qrc

# QML files
QML_FILES += \
    qml/main.qml \
    qml/MainView.qml \
    qml/components/UdpConfigPanel.qml \
    qml/components/OutputConfigPanel.qml \
    qml/components/AngleCorrectionPanel.qml \
    qml/components/AmplificationPanel.qml \
    qml/components/StyleManager.qml \
    qml/components/ModernButton.qml \
    qml/components/ModernTextField.qml \
    qml/components/ModernSpinBox.qml \
    qml/components/ModernComboBox.qml \
    qml/components/ModernSlider.qml \
    qml/components/ModernSwitch.qml \
    qml/components/PanelCard.qml \
    qml/components/SectionHeader.qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Enable debugging symbols
QMAKE_CXXFLAGS += -g

# Optimize for release
CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -O2
}
