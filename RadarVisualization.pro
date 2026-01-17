# Qt Project File for Radar Data Visualization
# Updated for binary UDP support

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    PPIWidget.cpp \
    FFTWidget.cpp

HEADERS += \
    DataStructures.h \
    MainWindow.h \
    PPIWidget.h \
    FFTWidget.h

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
