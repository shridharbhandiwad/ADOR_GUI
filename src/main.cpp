#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QFont>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // Set application properties
    app.setOrganizationName("ADOR");
    app.setOrganizationDomain("ador.com");
    app.setApplicationName("ADOR GUI");
    app.setApplicationVersion("1.0.0");
    
    // Use Material style with Light theme for modern look
    QQuickStyle::setStyle("Material");
    
    // Set default font for elegant appearance
    QFont defaultFont("Segoe UI", 10);
    defaultFont.setStyleStrategy(QFont::PreferAntialias);
    app.setFont(defaultFont);
    
    QQmlApplicationEngine engine;
    
    // Expose application version to QML
    engine.rootContext()->setContextProperty("appVersion", app.applicationVersion());
    
    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/ADOR/qml/Main.qml"));
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    
    engine.load(url);
    
    if (engine.rootObjects().isEmpty())
        return -1;
    
    return app.exec();
}
