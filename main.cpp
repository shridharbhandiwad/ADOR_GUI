#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QStyleFactory>
#include <QDir>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>
#include <QIcon>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    // Enable high DPI scaling for better cross-platform support
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    // Use scaling based on physical DPI (better for different screens)
    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    #endif
    
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("RadarGUI");
    app.setApplicationVersion("2.0.0");
    app.setOrganizationName("Zoppler Systems");
    app.setOrganizationDomain("zoppler.systems");
    
    // Set application icon
    app.setWindowIcon(QIcon(":/resources/zoppler_logo.png"));
    
    // Set modern font
    QFont defaultFont("Segoe UI", 12);
    defaultFont.setStyleHint(QFont::SansSerif);
    app.setFont(defaultFont);
    
    // Set Fusion style for widgets (if used)
    if (QStyleFactory::keys().contains("Fusion")) {
        app.setStyle("Fusion");
    }
    
    // Set modern monochrome light theme palette for Qt Widgets components
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(250, 250, 250));        // #fafafa - monochrome background
    lightPalette.setColor(QPalette::WindowText, QColor(10, 10, 10));       // #0a0a0a - monochrome text primary
    lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));          // #ffffff - card background
    lightPalette.setColor(QPalette::AlternateBase, QColor(245, 245, 245)); // #f5f5f5 - hover background
    lightPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 255));
    lightPalette.setColor(QPalette::ToolTipText, QColor(10, 10, 10));      // #0a0a0a
    lightPalette.setColor(QPalette::Text, QColor(10, 10, 10));             // #0a0a0a - monochrome text
    lightPalette.setColor(QPalette::Button, QColor(255, 255, 255));
    lightPalette.setColor(QPalette::ButtonText, QColor(10, 10, 10));       // #0a0a0a
    lightPalette.setColor(QPalette::BrightText, QColor(26, 26, 26));       // #1a1a1a - monochrome primary
    lightPalette.setColor(QPalette::Link, QColor(26, 26, 26));             // #1a1a1a - monochrome primary
    lightPalette.setColor(QPalette::Highlight, QColor(26, 26, 26));        // #1a1a1a - monochrome primary
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    lightPalette.setColor(QPalette::PlaceholderText, QColor(163, 163, 163)); // #a3a3a3 - monochrome muted
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
