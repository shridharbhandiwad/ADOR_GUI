cmake_minimum_required(VERSION 3.16)

project(ADOR_GUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick QuickControls2 Widgets)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(ADOR_GUI
    src/main.cpp
)

qt_add_qml_module(ADOR_GUI
    URI ADOR
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/style/Theme.qml
        qml/style/qmldir
        qml/components/DialogCard.qml
        qml/components/ModernButton.qml
        qml/components/ModernTextField.qml
        qml/components/ModernComboBox.qml
        qml/components/ModernSwitch.qml
        qml/components/StatusIndicator.qml
        qml/dialogs/ConfigurationDialog.qml
        qml/dialogs/MonitoringDialog.qml
        qml/dialogs/ControlDialog.qml
        qml/dialogs/DiagnosticsDialog.qml
    RESOURCES
        resources/resources.qrc
)

target_link_libraries(ADOR_GUI
    PRIVATE
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Widgets
)

set_target_properties(ADOR_GUI PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

install(TARGETS ADOR_GUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
