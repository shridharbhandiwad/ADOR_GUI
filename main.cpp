#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QStyleFactory>
#include <QDir>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    // Enable high DPI scaling
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("ZopplerSystems RadarGUI");
    app.setApplicationVersion("2.0.0");
    app.setOrganizationName("Radar Systems");
    app.setOrganizationDomain("radar.systems");
    
    // Set modern font
    QFont defaultFont("Segoe UI", 10);
    defaultFont.setStyleHint(QFont::SansSerif);
    app.setFont(defaultFont);
    
    // Set Fusion style for widgets (if used)
    if (QStyleFactory::keys().contains("Fusion")) {
        app.setStyle("Fusion");
    }
    
    // Set modern light theme palette for Qt Widgets components
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(248, 250, 252));        // #f8fafc
    lightPalette.setColor(QPalette::WindowText, QColor(30, 41, 59));       // #1e293b
    lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));          // #ffffff
    lightPalette.setColor(QPalette::AlternateBase, QColor(241, 245, 249)); // #f1f5f9
    lightPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));
    lightPalette.setColor(QPalette::ToolTipText, QColor(30, 41, 59));
    lightPalette.setColor(QPalette::Text, QColor(30, 41, 59));             // #1e293b
    lightPalette.setColor(QPalette::Button, QColor(255, 255, 255));
    lightPalette.setColor(QPalette::ButtonText, QColor(30, 41, 59));
    lightPalette.setColor(QPalette::BrightText, QColor(59, 130, 246));     // #3b82f6
    lightPalette.setColor(QPalette::Link, QColor(59, 130, 246));           // #3b82f6
    lightPalette.setColor(QPalette::Highlight, QColor(59, 130, 246));      // #3b82f6
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    lightPalette.setColor(QPalette::PlaceholderText, QColor(148, 163, 184)); // #94a3b8
    app.setPalette(lightPalette);
    
    // Set Qt Quick Controls 2 style
    QQuickStyle::setStyle("Basic");
    
    // Check for command line arguments
    bool showConfig = false;
    bool showVisualization = true;
    
    for (int i = 1; i < argc; i++) {
        QString arg = QString::fromLocal8Bit(argv[i]);
        if (arg == "--config" || arg == "-c") {
            showConfig = true;
            showVisualization = false;
        } else if (arg == "--visualization" || arg == "-v") {
            showVisualization = true;
            showConfig = false;
        } else if (arg == "--both" || arg == "-b") {
            showConfig = true;
            showVisualization = true;
        } else if (arg == "--help" || arg == "-h") {
            qDebug() << "Radar Data Visualization Application";
            qDebug() << "Usage:";
            qDebug() << "  --config, -c         Show only configuration panel (QML)";
            qDebug() << "  --visualization, -v  Show only visualization window (default)";
            qDebug() << "  --both, -b           Show both windows";
            qDebug() << "  --help, -h           Show this help";
            return 0;
        }
    }
    
    MainWindow* visualizationWindow = nullptr;
    QQmlApplicationEngine* qmlEngine = nullptr;
    
    // Create visualization window
    if (showVisualization) {
        visualizationWindow = new MainWindow();
        visualizationWindow->show();
    }
    
    // Create QML configuration panel
    if (showConfig) {
        qmlEngine = new QQmlApplicationEngine();
        
        // Add import paths
        qmlEngine->addImportPath("qrc:/");
        qmlEngine->addImportPath("qrc:/qml");
        
        // Load main QML file
        const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
        
        QObject::connect(qmlEngine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                qWarning() << "Failed to load QML configuration panel";
            } else {
                qDebug() << "QML configuration panel loaded successfully";
            }
        }, Qt::QueuedConnection);
        
        qmlEngine->load(url);
    }
    
    int result = app.exec();
    
    // Cleanup
    if (visualizationWindow) {
        delete visualizationWindow;
    }
    if (qmlEngine) {
        delete qmlEngine;
    }
    
    return result;
}
