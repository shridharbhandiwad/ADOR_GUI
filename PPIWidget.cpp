//#include "PPIWidget.h"
//#include <QPaintEvent>
//#include <QResizeEvent>
//#include <QFont>
//#include <QFontMetrics>
//#include <cmath>
//#include <QtMath>


//PPIWidget::PPIWidget(QWidget *parent)
//    : QWidget(parent)
//    , m_maxRange(50.0f) // 500 default
//    , m_plotRadius(0)
//{
//    setMinimumSize(400, 200);
//    setBackgroundRole(QPalette::Base);
//    setAutoFillBackground(true);
//}

//void PPIWidget::updateTargets(const TargetTrackData& trackData)
//{
//    m_currentTargets = trackData;
//    update();
//}

//void PPIWidget::setMaxRange(float range)
//{
//    if (range > 0) {
//        m_maxRange = range;
//        update();
//    }
//}

//void PPIWidget::resizeEvent(QResizeEvent *event)
//{
//    QWidget::resizeEvent(event);

//    // Calculate plot area (semi-circle)
//    int margin = 40;
//    int availableWidth = width() - 2 * margin;
//    int availableHeight = height() - 2 * margin;

//    // For semi-circle, height should be at least half of width
//    int diameter = std::min(availableWidth, availableHeight * 2);
//    m_plotRadius = diameter / 2.0f;

//    m_plotRect = QRect(
//        (width() - diameter) / 2,
//        height() - margin - m_plotRadius,
//        diameter,
//        m_plotRadius
//    );

//    m_center = QPointF(width() / 2.0f, height() - margin);
//}

//void PPIWidget::paintEvent(QPaintEvent *event)
//{
//    Q_UNUSED(event)

//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);

//    drawBackground(painter);
//    drawRangeRings(painter);
//    drawAzimuthLines(painter);
//    drawTargets(painter);
//    drawLabels(painter);
//}

//void PPIWidget::drawBackground(QPainter& painter)
//{
//    // Fill background
//    painter.fillRect(rect(), QColor(20, 20, 20));

//    // Draw semi-circular plot area
//    painter.setBrush(QColor(0, 0, 0));
//    painter.setPen(QPen(QColor(100, 100, 100), 2));

//    QRectF ellipseRect(m_center.x() - m_plotRadius, m_center.y() - m_plotRadius,
//                       2 * m_plotRadius, 2 * m_plotRadius);
//    painter.drawChord(ellipseRect, 0, 180 * 16); // Draw upper semi-circle
//}

//void PPIWidget::drawRangeRings(QPainter& painter)
//{
//    painter.setPen(QPen(QColor(100, 100, 100), 1));

//    for (int i = 1; i <= NUM_RANGE_RINGS; ++i) {
//        float radius = (float(i) / NUM_RANGE_RINGS) * m_plotRadius;
//        QRectF ellipseRect(m_center.x() - radius, m_center.y() - radius,
//                           2 * radius, 2 * radius);
//        painter.drawChord(ellipseRect, 0, 180 * 16);
//    }
//}

//void PPIWidget::drawAzimuthLines(QPainter& painter)
//{
//    painter.setPen(QPen(QColor(100, 100, 100), 1));

//    for (int i = 0; i < NUM_AZIMUTH_LINES; ++i) {
//        float azimuth = MIN_AZIMUTH + (float(i) / (NUM_AZIMUTH_LINES - 1)) * (MAX_AZIMUTH - MIN_AZIMUTH);
//        float radians = qDegreesToRadians(90.0f - azimuth); // Convert to math coordinates

//        QPointF endPoint(
//            m_center.x() + m_plotRadius * std::cos(radians),
//            m_center.y() - m_plotRadius * std::sin(radians)
//        );

//        painter.drawLine(m_center, endPoint);
//    }
//}

//void PPIWidget::drawTargets(QPainter& painter)
//{
//    for (const auto& target : m_currentTargets.targets) {
//        // Skip targets outside our azimuth range
//        if (target.azimuth < MIN_AZIMUTH || target.azimuth > MAX_AZIMUTH) {
//            continue;
//        }

//        // Skip targets outside our range
//        if (target.radius > m_maxRange) {
//            continue;
//        }

//        QPointF targetPos = polarToCartesian(target.radius, target.azimuth);
//        QColor targetColor = getTargetColor(target.radial_speed);

//        // Draw target as a circle
//        painter.setBrush(targetColor);
//        painter.setPen(QPen(targetColor.lighter(), 2));

//        float targetSize = 5.0f;// 6 + target.level * 0.1f; // Size based on level
//        painter.drawEllipse(targetPos, targetSize, targetSize);

//        // Draw target ID just above the red dot
//        painter.setPen(QPen(Qt::white, 1));
//        painter.setFont(QFont("Arial", 8));
//        QPointF textPos = targetPos + QPointF(0, -targetSize - 5);
//        painter.drawText(textPos, QString::number(target.target_id));
//    }
//}

//void PPIWidget::drawLabels(QPainter& painter)
//{
//    painter.setPen(QPen(Qt::white, 1));
//    painter.setFont(QFont("Arial", 10));

//    // Range labels
//    for (int i = 1; i <= NUM_RANGE_RINGS; ++i) {
//        float range = (float(i) / NUM_RANGE_RINGS) * m_maxRange;
//        float radius = (float(i) / NUM_RANGE_RINGS) * m_plotRadius;

//        QPointF labelPos(m_center.x() + radius * 0.707f, m_center.y() - radius * 0.707f);
//        QString rangeText = QString("%1").arg(range, 0, 'f', 1) + "m";
//        painter.drawText(labelPos, rangeText);
//    }

//    // Azimuth labels
//    QFont azFont("Arial", 12, QFont::Bold);
//    painter.setFont(azFont);

//    for (int i = 0; i < NUM_AZIMUTH_LINES; i += 2) { // Label every other line
//        float azimuth = MIN_AZIMUTH + (float(i) / (NUM_AZIMUTH_LINES - 1)) * (MAX_AZIMUTH - MIN_AZIMUTH);
//        float radians = qDegreesToRadians(90.0f - azimuth);

//        QPointF labelPos(
//            m_center.x() + (m_plotRadius + 20) * std::cos(radians),
//            m_center.y() - (m_plotRadius + 20) * std::sin(radians)
//        );

//        QString azText = QString("%1°").arg(azimuth, 0, 'f', 0);
//        QFontMetrics fm(azFont);
//        QRect textRect = fm.boundingRect(azText);
//        labelPos -= QPointF(textRect.width() / 2, -textRect.height() / 2);

//        painter.drawText(labelPos, azText);
//    }

//    // Title
//    painter.setFont(QFont("Arial", 14, QFont::Bold));
//    painter.drawText(QPointF(10, 25), "PPI Display - Target Tracks");
//}

//QColor PPIWidget::getTargetColor(float radialSpeed) const
//{
//    // Color coding based on radial speed
//    // Red for approaching (positive speed)
//    // Blue for receding (negative speed)
//    // Green for stationary

//    if (std::abs(radialSpeed) < 1.0f) {
//        return QColor(0, 255, 0); // Green for stationary
//    } else if (radialSpeed > 0) {
//        // Approaching - red (intensity based on speed)
//        int intensity = std::min(255, static_cast<int>(50 + std::abs(radialSpeed) * 10));
//        return QColor(intensity, 0, 0);
//    } else {
//        // Receding - blue (intensity based on speed)
//        int intensity = std::min(255, static_cast<int>(50 + std::abs(radialSpeed) * 10));
//        return QColor(0, 0, intensity);
//    }
//}

//QPointF PPIWidget::polarToCartesian(float range, float azimuth) const
//{
//    // Normalize range to plot radius
//    float normalizedRange = (range / m_maxRange) * m_plotRadius;

//    // Convert azimuth to radians (0° is up/north, positive is clockwise)
//    float radians = qDegreesToRadians(90.0f - azimuth);

//    return QPointF(
//        m_center.x() + normalizedRange * std::cos(radians),
//        m_center.y() - normalizedRange * std::sin(radians)
//    );
//}



// PPIWidget.cpp - Enhanced with FoV ±20° highlighting

#include "PPIWidget.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFont>
#include <QFontMetrics>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFrame>
#include <QSettings>
#include <cmath>
#include <QtMath>

PPIWidget::PPIWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxRange(50.0f)
    , m_minRange(0.0f)
    , m_plotRadius(0)
    , m_fovAngle(20.0f)  // ±20 degrees FoV (default)
    , m_minAngle(0.0f) // Default min angle
    , m_maxAngle(0.0f)  // Default max angle
    , m_hoveredTrackIndex(-1)
    , m_isDarkTheme(false) // Default to light theme
    , m_settingsPanel(nullptr)
    , m_settingsBtn(nullptr)
    , m_minRangeSpinBox(nullptr)
    , m_maxRangeSpinBox(nullptr)
    , m_minAngleSpinBox(nullptr)
    , m_maxAngleSpinBox(nullptr)
{
    // Minimum size is now set by parent widget based on screen DPI
    // No hard-coded minimum size to allow responsive layout
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);  // Enable mouse tracking for hover detection
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Setup settings panel
    setupSettingsPanel();
    
    // Load saved settings
    loadSettings();
}

void PPIWidget::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        applyTheme();  // Update settings panel theme
        update();  // Trigger repaint with new theme colors
    }
}

// Theme-aware color helper methods
QColor PPIWidget::getBackgroundColor() const
{
    return m_isDarkTheme ? QColor(30, 41, 59) : QColor(248, 250, 252);  // #1e293b / #f8fafc
}

QColor PPIWidget::getPlotBackgroundColor() const
{
    return m_isDarkTheme ? QColor(15, 23, 42) : QColor(255, 255, 255);  // #0f172a / white
}

QColor PPIWidget::getBorderColor() const
{
    return m_isDarkTheme ? QColor(51, 65, 85) : QColor(226, 232, 240);  // #334155 / #e2e8f0
}

QColor PPIWidget::getGridColor() const
{
    return m_isDarkTheme ? QColor(71, 85, 105, 180) : QColor(203, 213, 225, 180);  // #475569 / #cbd5e1
}

QColor PPIWidget::getFovColor() const
{
    return m_isDarkTheme ? QColor(250, 250, 250, 40) : QColor(26, 26, 26, 30);  // Monochrome with transparency
}

QColor PPIWidget::getFovBorderColor() const
{
    return m_isDarkTheme ? QColor(250, 250, 250) : QColor(26, 26, 26);  // #fafafa / #1a1a1a - Monochrome
}

QColor PPIWidget::getTextColor() const
{
    return m_isDarkTheme ? QColor(226, 232, 240) : QColor(30, 41, 59);  // #e2e8f0 / #1e293b
}

QColor PPIWidget::getSecondaryTextColor() const
{
    return m_isDarkTheme ? QColor(148, 163, 184) : QColor(100, 116, 139);  // #94a3b8 / #64748b
}

QColor PPIWidget::getMutedTextColor() const
{
    return m_isDarkTheme ? QColor(100, 116, 139) : QColor(148, 163, 184);  // #64748b / #94a3b8
}

QColor PPIWidget::getPrimaryBlueColor() const
{
    return m_isDarkTheme ? QColor(250, 250, 250) : QColor(26, 26, 26);  // #fafafa / #1a1a1a - Monochrome primary
}

QColor PPIWidget::getSuccessGreenColor() const
{
    return m_isDarkTheme ? QColor(163, 163, 163) : QColor(82, 82, 82);  // #a3a3a3 / #525252 - Monochrome secondary
}

QColor PPIWidget::getErrorRedColor() const
{
    return m_isDarkTheme ? QColor(248, 113, 113) : QColor(239, 68, 68);  // #f87171 / #ef4444
}

QColor PPIWidget::getTooltipBgColor() const
{
    return m_isDarkTheme ? QColor(15, 23, 42, 220) : QColor(30, 41, 59, 200);  // #0f172a / #1e293b with transparency
}

QColor PPIWidget::getTooltipTextColor() const
{
    return QColor(248, 250, 252);  // #f8fafc - Light text for both themes (tooltip bg is always dark)
}

void PPIWidget::updateTargets(const TargetTrackData& trackData)
{
    m_currentTargets = trackData;
    update();
}

void PPIWidget::setMaxRange(float range)
{
    if (range > 0) {
        m_maxRange = range;
        update();
    }
}

void PPIWidget::setMinRange(float range)
{
    if (range >= 0) {
        m_minRange = range;
        update();
    }
}

void PPIWidget::setFoVAngle(float angle)
{
    if (angle > 0 && angle <= 90) {
        m_fovAngle = angle;
        update();
    }
}

void PPIWidget::setMinAngle(float angle)
{
    if (angle >= -90.0f && angle <= m_maxAngle) {
        m_minAngle = angle;
        // Update fovAngle to reflect the new angle range
        m_fovAngle = std::max(std::abs(m_minAngle), std::abs(m_maxAngle));
        update();
    }
}

void PPIWidget::setMaxAngle(float angle)
{
    if (angle <= 90.0f && angle >= m_minAngle) {
        m_maxAngle = angle;
        // Update fovAngle to reflect the new angle range
        m_fovAngle = std::max(std::abs(m_minAngle), std::abs(m_maxAngle));
        update();
    }
}

void PPIWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Calculate plot area (semi-circle) - reduced margins for less empty space
    int marginH = 20;  // Horizontal margin (reduced from 40)
    int marginV = 30;  // Vertical margin (for labels at top)
    int marginBottom = 50;  // Bottom margin for legend
    
    int availableWidth = width() - 2 * marginH;
    int availableHeight = height() - marginV - marginBottom;

    // For semi-circle, height should be at least half of width
    int diameter = std::min(availableWidth, availableHeight * 2);
    m_plotRadius = diameter / 2.0f;

    m_plotRect = QRect(
        (width() - diameter) / 2,
        height() - marginBottom - m_plotRadius,
        diameter,
        m_plotRadius
    );

    m_center = QPointF(width() / 2.0f, height() - marginBottom);
}

void PPIWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawFoVHighlight(painter);  // Draw FoV before other elements
    drawRangeRings(painter);
    drawAzimuthLines(painter);
    drawFoVBoundaries(painter); // Draw FoV boundaries on top
    drawTargets(painter);
    drawLabels(painter);
    drawHoverTooltip(painter);  // Draw tooltip on top of everything
}

void PPIWidget::mouseMoveEvent(QMouseEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPointF mousePos = event->position();
#else
    QPointF mousePos = event->localPos();
#endif
    int trackIndex = findTrackAtPosition(mousePos);
    
    if (trackIndex != m_hoveredTrackIndex) {
        m_hoveredTrackIndex = trackIndex;
        m_hoverPosition = mousePos;
        update();  // Trigger repaint to show/hide tooltip
    } else if (trackIndex >= 0) {
        // Update tooltip position even if same track
        m_hoverPosition = mousePos;
        update();
    }
    
    QWidget::mouseMoveEvent(event);
}

void PPIWidget::leaveEvent(QEvent *event)
{
    if (m_hoveredTrackIndex >= 0) {
        m_hoveredTrackIndex = -1;
        update();
    }
    QWidget::leaveEvent(event);
}

int PPIWidget::findTrackAtPosition(const QPointF& pos) const
{
    const float hitRadius = 12.0f;  // Pixel radius for hit detection
    
    for (size_t i = 0; i < m_currentTargets.targets.size(); ++i) {
        const auto& target = m_currentTargets.targets[i];
        
        // Skip targets outside display range
        if (target.azimuth < MIN_AZIMUTH || target.azimuth > MAX_AZIMUTH) {
            continue;
        }
        if (target.radius > m_maxRange || target.radius < m_minRange) {
            continue;
        }
        
        QPointF targetPos = polarToCartesian(target.radius, target.azimuth);
        
        // Check if mouse is within hit radius of target
        float dx = pos.x() - targetPos.x();
        float dy = pos.y() - targetPos.y();
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance <= hitRadius) {
            return static_cast<int>(i);
        }
    }
    
    return -1;  // No track found
}

void PPIWidget::drawHoverTooltip(QPainter& painter)
{
    if (m_hoveredTrackIndex < 0 || 
        static_cast<size_t>(m_hoveredTrackIndex) >= m_currentTargets.targets.size()) {
        return;
    }
    
    const auto& target = m_currentTargets.targets[m_hoveredTrackIndex];
    
    // Build tooltip text
    QString tooltipText = QString("ID: %1\nRange: %2 m\nAzimuth: %3°\nSpeed: %4 m/s")
        .arg(target.target_id)
        .arg(target.radius, 0, 'f', 2)
        .arg(target.azimuth, 0, 'f', 1)
        .arg(target.radial_speed, 0, 'f', 2);
    
    painter.save();
    
    // Set up font for measurement
    QFont tooltipFont("Segoe UI", 12);
    painter.setFont(tooltipFont);
    QFontMetrics fm(tooltipFont);
    
    // Calculate tooltip dimensions
    QStringList lines = tooltipText.split('\n');
    int maxWidth = 0;
    int totalHeight = 0;
    for (const QString& line : lines) {
        maxWidth = std::max(maxWidth, fm.horizontalAdvance(line));
        totalHeight += fm.height();
    }
    
    int padding = 10;
    int tooltipWidth = maxWidth + 2 * padding;
    int tooltipHeight = totalHeight + 2 * padding;
    
    // Calculate position (offset from target to avoid obscuring it)
    QPointF targetPos = polarToCartesian(target.radius, target.azimuth);
    float offsetX = 15;
    float offsetY = -tooltipHeight - 10;
    
    // Adjust position to keep tooltip within widget bounds
    float tooltipX = targetPos.x() + offsetX;
    float tooltipY = targetPos.y() + offsetY;
    
    // Keep within horizontal bounds
    if (tooltipX + tooltipWidth > width()) {
        tooltipX = targetPos.x() - tooltipWidth - offsetX;
    }
    if (tooltipX < 0) {
        tooltipX = 5;
    }
    
    // Keep within vertical bounds
    if (tooltipY < 0) {
        tooltipY = targetPos.y() + 15;
    }
    if (tooltipY + tooltipHeight > height()) {
        tooltipY = height() - tooltipHeight - 5;
    }
    
    QRectF tooltipRect(tooltipX, tooltipY, tooltipWidth, tooltipHeight);
    
    // Draw semi-transparent background (theme-aware)
    QColor bgColor = getTooltipBgColor();
    painter.setBrush(QBrush(bgColor));
    QColor borderColor = getPrimaryBlueColor();
    borderColor.setAlpha(180);
    painter.setPen(QPen(borderColor, 1));
    painter.drawRoundedRect(tooltipRect, 6, 6);
    
    // Draw text
    painter.setPen(QPen(getTooltipTextColor()));
    
    float textY = tooltipY + padding + fm.ascent();
    for (const QString& line : lines) {
        painter.drawText(QPointF(tooltipX + padding, textY), line);
        textY += fm.height();
    }
    
    // Draw small triangle pointer towards target
    QPointF trianglePoints[3];
    float triSize = 8;
    
    // Position triangle on the side closest to the target
    if (targetPos.x() > tooltipRect.center().x()) {
        // Triangle on right side
        trianglePoints[0] = QPointF(tooltipRect.right(), tooltipRect.center().y() - triSize);
        trianglePoints[1] = QPointF(tooltipRect.right(), tooltipRect.center().y() + triSize);
        trianglePoints[2] = QPointF(tooltipRect.right() + triSize, tooltipRect.center().y());
    } else {
        // Triangle on left side
        trianglePoints[0] = QPointF(tooltipRect.left(), tooltipRect.center().y() - triSize);
        trianglePoints[1] = QPointF(tooltipRect.left(), tooltipRect.center().y() + triSize);
        trianglePoints[2] = QPointF(tooltipRect.left() - triSize, tooltipRect.center().y());
    }
    
    painter.setBrush(QBrush(bgColor));
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(trianglePoints, 3);
    
    painter.restore();
}

void PPIWidget::drawBackground(QPainter& painter)
{
    // Fill background with elegant gradient (theme-aware)
    QLinearGradient bgGradient(0, 0, 0, height());
    if (m_isDarkTheme) {
        bgGradient.setColorAt(0, QColor(30, 41, 59));    // #1e293b
        bgGradient.setColorAt(1, QColor(15, 23, 42));    // #0f172a
    } else {
        bgGradient.setColorAt(0, QColor(248, 250, 252)); // #f8fafc
        bgGradient.setColorAt(1, QColor(241, 245, 249)); // #f1f5f9
    }
    painter.fillRect(rect(), bgGradient);

    // Draw semi-circular plot area with premium radar-style gradient
    QRadialGradient radarGradient(m_center, m_plotRadius);
    if (m_isDarkTheme) {
        radarGradient.setColorAt(0, QColor(30, 41, 59, 255));    // Center
        radarGradient.setColorAt(0.3, QColor(22, 33, 50, 255));
        radarGradient.setColorAt(0.7, QColor(15, 23, 42, 255));
        radarGradient.setColorAt(1.0, QColor(10, 15, 30, 255));    // Edge - deeper blue
    } else {
        radarGradient.setColorAt(0, QColor(255, 255, 255, 255)); // Pure white center
        radarGradient.setColorAt(0.4, QColor(250, 252, 255, 255));
        radarGradient.setColorAt(0.8, QColor(241, 245, 249, 255));
        radarGradient.setColorAt(1.0, QColor(226, 232, 240, 255)); // Soft gray edge
    }
    
    painter.setBrush(radarGradient);
    
    // Premium border with subtle glow effect
    QColor borderColor = getBorderColor();
    QPen borderPen(borderColor, 2);
    painter.setPen(borderPen);

    QRectF ellipseRect(m_center.x() - m_plotRadius, m_center.y() - m_plotRadius,
                       2 * m_plotRadius, 2 * m_plotRadius);
    
    // Draw subtle outer glow (only in dark theme)
    if (m_isDarkTheme) {
        QColor glowColor(59, 130, 246, 30);
        painter.setPen(QPen(glowColor, 6));
        painter.setBrush(Qt::NoBrush);
        painter.drawChord(ellipseRect, 0, 180 * 16);
    }
    
    // Draw main radar circle
    painter.setBrush(radarGradient);
    painter.setPen(borderPen);
    painter.drawChord(ellipseRect, 0, 180 * 16);
}

void PPIWidget::drawFoVHighlight(QPainter& painter)
{
    // Create a highlighted sector for the Field of View (using min/max angles)
    painter.save();

    // Set up semi-transparent blue fill for active FoV area (theme-aware)
    painter.setBrush(QBrush(getFovColor()));
    painter.setPen(Qt::NoPen);

    // Calculate the angular span in Qt's coordinate system
    // Qt angles: 0° = 3 o'clock, 90° = 12 o'clock (up), positive = counter-clockwise
    // Our radar: 0° = 12 o'clock (north), positive = clockwise
    // Conversion: qt_angle = 90° - radar_angle

    float startAngleRadar = m_minAngle;  // Use configured min angle
    float endAngleRadar = m_maxAngle;    // Use configured max angle

    // Convert to Qt angles (in 16ths of a degree)
    int startAngleQt = static_cast<int>((90.0f - endAngleRadar) * 16);   // Start from max radar angle
    int spanAngleQt = static_cast<int>((endAngleRadar - startAngleRadar) * 16); // Span from min to max

    // Draw the FoV sector
    QRectF ellipseRect(m_center.x() - m_plotRadius, m_center.y() - m_plotRadius,
                       2 * m_plotRadius, 2 * m_plotRadius);

    // Only draw the upper half (semi-circle) within FoV
    if (startAngleQt >= 0 && startAngleQt <= 180 * 16) {
        painter.drawPie(ellipseRect, startAngleQt, spanAngleQt);
    }

    painter.restore();
}

void PPIWidget::drawFoVBoundaries(QPainter& painter)
{
    // Draw FoV boundary lines using configured min/max angles
    painter.save();

    // Use theme-aware FoV boundary color
    painter.setPen(QPen(getFovBorderColor(), 2, Qt::SolidLine));

    // Draw left boundary (min angle)
    float leftAngle = m_minAngle;
    float leftRadians = qDegreesToRadians(90.0f - leftAngle);
    QPointF leftEndPoint(
        m_center.x() + m_plotRadius * std::cos(leftRadians),
        m_center.y() - m_plotRadius * std::sin(leftRadians)
    );
    painter.drawLine(m_center, leftEndPoint);

    // Draw right boundary (max angle)
    float rightAngle = m_maxAngle;
    float rightRadians = qDegreesToRadians(90.0f - rightAngle);
    QPointF rightEndPoint(
        m_center.x() + m_plotRadius * std::cos(rightRadians),
        m_center.y() - m_plotRadius * std::sin(rightRadians)
    );
    painter.drawLine(m_center, rightEndPoint);

    // Draw FoV arc at the perimeter
    QRectF ellipseRect(m_center.x() - m_plotRadius, m_center.y() - m_plotRadius,
                       2 * m_plotRadius, 2 * m_plotRadius);
    int startAngleQt = static_cast<int>((90.0f - rightAngle) * 16);
    int spanAngleQt = static_cast<int>((rightAngle - leftAngle) * 16);
    painter.drawArc(ellipseRect, startAngleQt, spanAngleQt);

    painter.restore();
}

void PPIWidget::drawRangeRings(QPainter& painter)
{
    painter.setPen(QPen(getGridColor(), 1)); // Theme-aware grid color

    for (int i = 1; i <= NUM_RANGE_RINGS; ++i) {
        float radius = (float(i) / NUM_RANGE_RINGS) * m_plotRadius;
        QRectF ellipseRect(m_center.x() - radius, m_center.y() - radius,
                           2 * radius, 2 * radius);
        painter.drawChord(ellipseRect, 0, 180 * 16);
    }
}

void PPIWidget::drawAzimuthLines(QPainter& painter)
{
    for (int i = 0; i < NUM_AZIMUTH_LINES; ++i) {
        float azimuth = MIN_AZIMUTH + (float(i) / (NUM_AZIMUTH_LINES - 1)) * (MAX_AZIMUTH - MIN_AZIMUTH);
        float radians = qDegreesToRadians(90.0f - azimuth);

        QPointF endPoint(
            m_center.x() + m_plotRadius * std::cos(radians),
            m_center.y() - m_plotRadius * std::sin(radians)
        );

        // Draw different styles for lines within and outside FoV (using configured min/max angles)
        if (azimuth >= m_minAngle && azimuth <= m_maxAngle) {
            // Lines within FoV - theme-aware primary color
            QColor fovLineColor = getPrimaryBlueColor();
            fovLineColor.setAlpha(120);
            painter.setPen(QPen(fovLineColor, 1));
        } else {
            // Lines outside FoV - theme-aware grid color (more muted)
            QColor outsideFovColor = getGridColor();
            outsideFovColor.setAlpha(100);
            painter.setPen(QPen(outsideFovColor, 1));
        }

        painter.drawLine(m_center, endPoint);
    }
}

void PPIWidget::drawTargets(QPainter& painter)
{
    for (const auto& target : m_currentTargets.targets) {
        // Skip targets outside our azimuth range
        if (target.azimuth < MIN_AZIMUTH || target.azimuth > MAX_AZIMUTH) {
            continue;
        }

        // Skip targets outside our range (both min and max)
        if (target.radius > m_maxRange || target.radius < m_minRange) {
            continue;
        }

        QPointF targetPos = polarToCartesian(target.radius, target.azimuth);
        QColor targetColor = getTargetColor(target.radial_speed);

        // Check if target is within configured FoV (using min/max angles)
        bool inFoV = (target.azimuth >= m_minAngle && target.azimuth <= m_maxAngle);
        inFoV = true;

        // Draw target as a circle
        painter.setBrush(targetColor);
        if (inFoV) {
            // Targets within FoV - brighter border
            painter.setPen(QPen(targetColor.lighter(), 3));
        } else {
            // Targets outside FoV - dimmer appearance
            QColor dimmedColor = targetColor;
            dimmedColor.setAlpha(100);
            painter.setBrush(dimmedColor);
            painter.setPen(QPen(dimmedColor.lighter(), 2));
        }

        float targetSize = inFoV ? 10.0f : 8.0f; // Larger targets in FoV
        painter.drawEllipse(targetPos, targetSize, targetSize);

        // Draw target ID (theme-aware text color)
        QColor idTextColor = m_isDarkTheme ? QColor(226, 232, 240) : Qt::white;
        painter.setPen(QPen(idTextColor, 1));
        painter.setFont(QFont("Arial", inFoV ? 11 : 9)); // Larger text in FoV
        QPointF textPos = targetPos + QPointF(0, -targetSize - 5);
        painter.drawText(textPos, QString::number(target.target_id));
    }
}

void PPIWidget::drawLabels(QPainter& painter)
{
    // Premium range labels with enhanced styling
    QFont rangeFont("Segoe UI", 11, QFont::Medium);
    painter.setFont(rangeFont);
    painter.setPen(QPen(getSecondaryTextColor(), 1));

    for (int i = 1; i <= NUM_RANGE_RINGS; ++i) {
        float range = (float(i) / NUM_RANGE_RINGS) * m_maxRange;
        float radius = (float(i) / NUM_RANGE_RINGS) * m_plotRadius;

        QPointF labelPos(m_center.x() + radius * 0.707f, m_center.y() - radius * 0.707f);
        QString rangeText = QString("%1m").arg(range, 0, 'f', 0);
        
        // Draw subtle background for range labels
        QFontMetrics fm(rangeFont);
        QRect textRect = fm.boundingRect(rangeText);
        QRectF bgRect(labelPos.x() - 2, labelPos.y() - textRect.height() + 2, 
                      textRect.width() + 4, textRect.height() + 2);
        
        QColor labelBg = m_isDarkTheme ? QColor(15, 23, 42, 180) : QColor(255, 255, 255, 200);
        painter.fillRect(bgRect, labelBg);
        painter.drawText(labelPos, rangeText);
    }

    // Azimuth labels with premium styling
    QFont azFont("Segoe UI", 12, QFont::DemiBold);
    painter.setFont(azFont);

    for (int i = 0; i < NUM_AZIMUTH_LINES; i += 2) {
        float azimuth = MIN_AZIMUTH + (float(i) / (NUM_AZIMUTH_LINES - 1)) * (MAX_AZIMUTH - MIN_AZIMUTH);
        float radians = qDegreesToRadians(90.0f - azimuth);

        QPointF labelPos(
            m_center.x() + (m_plotRadius + 22) * std::cos(radians),
            m_center.y() - (m_plotRadius + 22) * std::sin(radians)
        );

        QString azText = QString("%1°").arg(azimuth, 0, 'f', 0);
        QFontMetrics fm(azFont);
        QRect textRect = fm.boundingRect(azText);
        labelPos -= QPointF(textRect.width() / 2, -textRect.height() / 2);

        // Highlight FoV boundary labels with accent color
        if (std::abs(azimuth - m_minAngle) < 1.0f || std::abs(azimuth - m_maxAngle) < 1.0f) {
            painter.setPen(QPen(getPrimaryBlueColor(), 1));
        } else {
            painter.setPen(QPen(getSecondaryTextColor(), 1));
        }

        painter.drawText(labelPos, azText);
    }

    // FoV Legend at bottom with premium styling - centered to avoid angle label overlap
    painter.save();
    
    // Calculate legend dimensions and center position
    QString legendText = "● Blue: Active FoV   ● Bright: In FoV   ● Dim: Outside FoV";
    QFont legendFont("Segoe UI", 9);
    painter.setFont(legendFont);
    QFontMetrics legendFm(legendFont);
    int legendTextWidth = legendFm.horizontalAdvance(legendText);
    int legendWidth = legendTextWidth + 16;  // Add padding
    int legendHeight = 28;
    int legendX = (width() - legendWidth) / 2;  // Center horizontally
    int legendY = height() - legendHeight - 6;  // Position at bottom with small margin
    
    QRectF legendBg(legendX, legendY, legendWidth, legendHeight);
    QLinearGradient legendGrad(legendBg.topLeft(), legendBg.bottomRight());
    if (m_isDarkTheme) {
        legendGrad.setColorAt(0, QColor(15, 23, 42, 200));
        legendGrad.setColorAt(1, QColor(30, 41, 59, 180));
    } else {
        legendGrad.setColorAt(0, QColor(255, 255, 255, 220));
        legendGrad.setColorAt(1, QColor(248, 250, 252, 200));
    }
    painter.setBrush(legendGrad);
    painter.setPen(QPen(getBorderColor(), 1));
    painter.drawRoundedRect(legendBg, 6, 6);
    
    painter.setPen(QPen(getMutedTextColor(), 1));
    // Center text within the legend box
    int textX = legendX + (legendWidth - legendTextWidth) / 2;
    int textY = legendY + (legendHeight + legendFm.ascent() - legendFm.descent()) / 2;
    painter.drawText(QPointF(textX, textY), legendText);
    painter.restore();
    
    // Target Colors Legend - top left corner
    painter.save();
    
    QFont targetLegendFont("Segoe UI", 9);
    painter.setFont(targetLegendFont);
    QFontMetrics targetFm(targetLegendFont);
    
    // Define legend items with colors and text
    struct LegendItem {
        QColor color;
        QString label;
    };
    
    std::vector<LegendItem> targetLegendItems = {
        { getErrorRedColor(), "Approaching" },
        { getSuccessGreenColor(), "Stationary" },
        { getPrimaryBlueColor(), "Receding" }
    };
    
    // Calculate dimensions
    int itemSpacing = 8;
    int dotSize = 10;
    int dotTextSpacing = 5;
    int targetLegendPadding = 10;
    int totalItemsWidth = 0;
    
    for (const auto& item : targetLegendItems) {
        totalItemsWidth += dotSize + dotTextSpacing + targetFm.horizontalAdvance(item.label) + itemSpacing;
    }
    totalItemsWidth -= itemSpacing;  // Remove last spacing
    
    int targetLegendWidth = totalItemsWidth + 2 * targetLegendPadding;
    int targetLegendHeight = 28;
    int targetLegendX = 10;  // Left margin
    int targetLegendY = 10;  // Top margin
    
    // Draw background
    QRectF targetLegendBg(targetLegendX, targetLegendY, targetLegendWidth, targetLegendHeight);
    QLinearGradient targetLegendGrad(targetLegendBg.topLeft(), targetLegendBg.bottomRight());
    if (m_isDarkTheme) {
        targetLegendGrad.setColorAt(0, QColor(15, 23, 42, 220));
        targetLegendGrad.setColorAt(1, QColor(30, 41, 59, 200));
    } else {
        targetLegendGrad.setColorAt(0, QColor(255, 255, 255, 240));
        targetLegendGrad.setColorAt(1, QColor(248, 250, 252, 220));
    }
    painter.setBrush(targetLegendGrad);
    painter.setPen(QPen(getBorderColor(), 1));
    painter.drawRoundedRect(targetLegendBg, 6, 6);
    
    // Draw title "Target Speed:" before items
    QString titleText = "Target Speed:";
    int titleWidth = targetFm.horizontalAdvance(titleText);
    
    // Recalculate with title
    targetLegendWidth = titleWidth + 10 + totalItemsWidth + 2 * targetLegendPadding;
    targetLegendBg = QRectF(targetLegendX, targetLegendY, targetLegendWidth, targetLegendHeight);
    
    // Redraw background with correct width
    if (m_isDarkTheme) {
        targetLegendGrad.setColorAt(0, QColor(15, 23, 42, 220));
        targetLegendGrad.setColorAt(1, QColor(30, 41, 59, 200));
    } else {
        targetLegendGrad.setColorAt(0, QColor(255, 255, 255, 240));
        targetLegendGrad.setColorAt(1, QColor(248, 250, 252, 220));
    }
    targetLegendGrad.setStart(targetLegendBg.topLeft());
    targetLegendGrad.setFinalStop(targetLegendBg.bottomRight());
    painter.setBrush(targetLegendGrad);
    painter.drawRoundedRect(targetLegendBg, 6, 6);
    
    // Draw title
    painter.setPen(QPen(getTextColor(), 1));
    int currentX = targetLegendX + targetLegendPadding;
    int centerY = targetLegendY + targetLegendHeight / 2;
    painter.drawText(QPointF(currentX, centerY + targetFm.ascent() / 2 - 1), titleText);
    currentX += titleWidth + 10;
    
    // Draw legend items
    for (const auto& item : targetLegendItems) {
        // Draw colored dot
        painter.setBrush(item.color);
        painter.setPen(QPen(item.color.darker(120), 1));
        painter.drawEllipse(QPointF(currentX + dotSize / 2, centerY), dotSize / 2.0, dotSize / 2.0);
        currentX += dotSize + dotTextSpacing;
        
        // Draw label
        painter.setPen(QPen(getMutedTextColor(), 1));
        painter.drawText(QPointF(currentX, centerY + targetFm.ascent() / 2 - 1), item.label);
        currentX += targetFm.horizontalAdvance(item.label) + itemSpacing;
    }
    
    painter.restore();
}

QColor PPIWidget::getTargetColor(float radialSpeed) const
{
    // Modern color coding based on radial speed (theme-aware)
    if (std::abs(radialSpeed) < 1.0f) {
        return getSuccessGreenColor(); // Green for stationary
    } else if (radialSpeed > 0) {
        // Approaching - warm red/orange
        return getErrorRedColor(); // Red for approaching
    } else {
        // Receding - cool blue
        return getPrimaryBlueColor(); // Blue for receding
    }
}

QPointF PPIWidget::polarToCartesian(float range, float azimuth) const
{
    // Normalize range to plot radius
    float normalizedRange = (range / m_maxRange) * m_plotRadius;

    // Convert azimuth to radians (0° is up/north, positive is clockwise)
    float radians = qDegreesToRadians(90.0f - azimuth);

    return QPointF(
        m_center.x() + normalizedRange * std::cos(radians),
        m_center.y() - normalizedRange * std::sin(radians)
    );
}

void PPIWidget::setupSettingsPanel()
{
    m_settingsPanel = new QWidget(this);
    m_settingsPanel->setVisible(false);  // Hidden by default
    
    QVBoxLayout* settingsLayout = new QVBoxLayout(m_settingsPanel);
    settingsLayout->setContentsMargins(10, 10, 10, 10);
    settingsLayout->setSpacing(10);
    
    // Create a frame for better visual grouping
    QFrame* settingsFrame = new QFrame(this);
    settingsFrame->setFrameShape(QFrame::StyledPanel);
    QGridLayout* gridLayout = new QGridLayout(settingsFrame);
    gridLayout->setSpacing(10);
    
    int row = 0;
    
    // Range settings
    QLabel* rangeLabel = new QLabel("<b>Range Settings</b>", this);
    gridLayout->addWidget(rangeLabel, row, 0, 1, 4);
    row++;
    
    QLabel* minRangeLabel = new QLabel("Min Range:", this);
    m_minRangeSpinBox = new QDoubleSpinBox(this);
    m_minRangeSpinBox->setRange(0, 1000);
    m_minRangeSpinBox->setValue(m_minRange);
    m_minRangeSpinBox->setSuffix(" m");
    m_minRangeSpinBox->setDecimals(1);
    connect(m_minRangeSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &PPIWidget::onMinRangeChanged);
    gridLayout->addWidget(minRangeLabel, row, 0);
    gridLayout->addWidget(m_minRangeSpinBox, row, 1);
    
    QLabel* maxRangeLabel = new QLabel("Max Range:", this);
    m_maxRangeSpinBox = new QDoubleSpinBox(this);
    m_maxRangeSpinBox->setRange(1, 1000);
    m_maxRangeSpinBox->setValue(m_maxRange);
    m_maxRangeSpinBox->setSuffix(" m");
    m_maxRangeSpinBox->setDecimals(1);
    connect(m_maxRangeSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &PPIWidget::onMaxRangeChanged);
    gridLayout->addWidget(maxRangeLabel, row, 2);
    gridLayout->addWidget(m_maxRangeSpinBox, row, 3);
    row++;
    
    // Separator
    QFrame* line1 = new QFrame(this);
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(line1, row, 0, 1, 4);
    row++;
    
    // Angle settings
    QLabel* angleLabel = new QLabel("<b>Angle Settings</b>", this);
    gridLayout->addWidget(angleLabel, row, 0, 1, 4);
    row++;
    
    QLabel* minAngleLabel = new QLabel("Min Angle:", this);
    m_minAngleSpinBox = new QDoubleSpinBox(this);
    m_minAngleSpinBox->setRange(-90, 90);
    m_minAngleSpinBox->setValue(m_minAngle);
    m_minAngleSpinBox->setSuffix(" °");
    m_minAngleSpinBox->setDecimals(1);
    connect(m_minAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &PPIWidget::onMinAngleChanged);
    gridLayout->addWidget(minAngleLabel, row, 0);
    gridLayout->addWidget(m_minAngleSpinBox, row, 1);
    
    QLabel* maxAngleLabel = new QLabel("Max Angle:", this);
    m_maxAngleSpinBox = new QDoubleSpinBox(this);
    m_maxAngleSpinBox->setRange(-90, 90);
    m_maxAngleSpinBox->setValue(m_maxAngle);
    m_maxAngleSpinBox->setSuffix(" °");
    m_maxAngleSpinBox->setDecimals(1);
    connect(m_maxAngleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &PPIWidget::onMaxAngleChanged);
    gridLayout->addWidget(maxAngleLabel, row, 2);
    gridLayout->addWidget(m_maxAngleSpinBox, row, 3);
    row++;
    
    settingsLayout->addWidget(settingsFrame);
    
    // Add info label
    QLabel* infoLabel = new QLabel(
        "<i>Tip: Hover over targets for details</i>", this);
    infoLabel->setWordWrap(true);
    settingsLayout->addWidget(infoLabel);
    
    applyTheme();
}

void PPIWidget::applyTheme()
{
    if (!m_settingsPanel) return;
    
    QString settingsPanelStyle;
    if (m_isDarkTheme) {
        settingsPanelStyle = R"(
            QWidget {
                background-color: #1e293b;
                color: #e2e8f0;
            }
            QFrame {
                background-color: #0f172a;
                border: 1px solid #334155;
                border-radius: 8px;
            }
            QLabel {
                color: #e2e8f0;
            }
            QDoubleSpinBox {
                background-color: #0f172a;
                border: 1px solid #475569;
                border-radius: 6px;
                padding: 5px;
                color: #e2e8f0;
            }
            QDoubleSpinBox:hover {
                border-color: #64748b;
            }
            QDoubleSpinBox:focus {
                border-color: #94a3b8;
            }
        )";
    } else {
        settingsPanelStyle = R"(
            QWidget {
                background-color: #f8fafc;
                color: #1e293b;
            }
            QFrame {
                background-color: #ffffff;
                border: 1px solid #e2e8f0;
                border-radius: 8px;
            }
            QLabel {
                color: #1e293b;
            }
            QDoubleSpinBox {
                background-color: #ffffff;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 5px;
                color: #1e293b;
            }
            QDoubleSpinBox:hover {
                border-color: #94a3b8;
            }
            QDoubleSpinBox:focus {
                border-color: #64748b;
            }
        )";
    }
    
    m_settingsPanel->setStyleSheet(settingsPanelStyle);
}

void PPIWidget::saveSettings()
{
    QSettings settings;
    settings.beginGroup("PPIWidget");
    settings.setValue("minRange", m_minRange);
    settings.setValue("maxRange", m_maxRange);
    settings.setValue("minAngle", m_minAngle);
    settings.setValue("maxAngle", m_maxAngle);
    settings.endGroup();
}

void PPIWidget::loadSettings()
{
    QSettings settings;
    settings.beginGroup("PPIWidget");
    
    if (settings.contains("minRange")) {
        setMinRange(settings.value("minRange", 0.0f).toFloat());
    }
    if (settings.contains("maxRange")) {
        setMaxRange(settings.value("maxRange", 50.0f).toFloat());
    }
    if (settings.contains("minAngle")) {
        setMinAngle(settings.value("minAngle", -60.0f).toFloat());
    }
    if (settings.contains("maxAngle")) {
        setMaxAngle(settings.value("maxAngle", 60.0f).toFloat());
    }
    
    settings.endGroup();
    
    // Update spinboxes to reflect loaded values
    if (m_minRangeSpinBox) m_minRangeSpinBox->setValue(m_minRange);
    if (m_maxRangeSpinBox) m_maxRangeSpinBox->setValue(m_maxRange);
    if (m_minAngleSpinBox) m_minAngleSpinBox->setValue(m_minAngle);
    if (m_maxAngleSpinBox) m_maxAngleSpinBox->setValue(m_maxAngle);
}

void PPIWidget::onSettingsToggled()
{
    if (m_settingsPanel) {
        m_settingsPanel->setVisible(!m_settingsPanel->isVisible());
    }
}

void PPIWidget::onMinRangeChanged(double value)
{
    if (value < m_maxRangeSpinBox->value()) {
        setMinRange(value);
        saveSettings();
    }
}

void PPIWidget::onMaxRangeChanged(double value)
{
    if (value > m_minRangeSpinBox->value()) {
        setMaxRange(value);
        saveSettings();
    }
}

void PPIWidget::onMinAngleChanged(double value)
{
    if (value < m_maxAngleSpinBox->value()) {
        setMinAngle(value);
        saveSettings();
    }
}

void PPIWidget::onMaxAngleChanged(double value)
{
    if (value > m_minAngleSpinBox->value()) {
        setMaxAngle(value);
        saveSettings();
    }
}
