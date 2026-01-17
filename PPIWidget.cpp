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
#include <cmath>
#include <QtMath>

PPIWidget::PPIWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxRange(50.0f)
    , m_minRange(0.0f)
    , m_plotRadius(0)
    , m_fovAngle(20.0f)  // ±20 degrees FoV (default)
    , m_minAngle(-60.0f) // Default min angle
    , m_maxAngle(60.0f)  // Default max angle
{
    setMinimumSize(400, 200);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
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
    if (angle >= -90.0f && angle < m_maxAngle) {
        m_minAngle = angle;
        // Update fovAngle to reflect the new angle range
        m_fovAngle = std::max(std::abs(m_minAngle), std::abs(m_maxAngle));
        update();
    }
}

void PPIWidget::setMaxAngle(float angle)
{
    if (angle <= 90.0f && angle > m_minAngle) {
        m_maxAngle = angle;
        // Update fovAngle to reflect the new angle range
        m_fovAngle = std::max(std::abs(m_minAngle), std::abs(m_maxAngle));
        update();
    }
}

void PPIWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Calculate plot area (semi-circle)
    int margin = 40;
    int availableWidth = width() - 2 * margin;
    int availableHeight = height() - 2 * margin;

    // For semi-circle, height should be at least half of width
    int diameter = std::min(availableWidth, availableHeight * 2);
    m_plotRadius = diameter / 2.0f;

    m_plotRect = QRect(
        (width() - diameter) / 2,
        height() - margin - m_plotRadius,
        diameter,
        m_plotRadius
    );

    m_center = QPointF(width() / 2.0f, height() - margin);
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
}

void PPIWidget::drawBackground(QPainter& painter)
{
    // Fill background with modern light theme
    painter.fillRect(rect(), QColor(248, 250, 252)); // #f8fafc - Light gray background

    // Draw semi-circular plot area with subtle gradient
    QRadialGradient gradient(m_center, m_plotRadius);
    gradient.setColorAt(0, QColor(255, 255, 255));    // White center
    gradient.setColorAt(1, QColor(241, 245, 249));    // #f1f5f9 - Slightly darker edge
    
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(226, 232, 240), 2)); // #e2e8f0 - Border color

    QRectF ellipseRect(m_center.x() - m_plotRadius, m_center.y() - m_plotRadius,
                       2 * m_plotRadius, 2 * m_plotRadius);
    painter.drawChord(ellipseRect, 0, 180 * 16); // Draw upper semi-circle
}

void PPIWidget::drawFoVHighlight(QPainter& painter)
{
    // Create a highlighted sector for the Field of View (using min/max angles)
    painter.save();

    // Set up semi-transparent blue fill for active FoV area (light theme)
    QColor fovColor(59, 130, 246, 40); // #3b82f6 - Primary blue with transparency
    painter.setBrush(QBrush(fovColor));
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

    // Use primary blue for FoV boundaries (light theme)
    painter.setPen(QPen(QColor(59, 130, 246), 2, Qt::SolidLine)); // #3b82f6

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
    painter.setPen(QPen(QColor(203, 213, 225, 180), 1)); // #cbd5e1 - Light theme grid color

    for (int i = 1; i <= NUM_RANGE_RINGS; ++i) {
        float radius = (float(i) / NUM_RANGE_RINGS) * m_plotRadius;
        QRectF ellipseRect(m_center.x() - radius, m_center.y() - radius,
                           2 * radius, 2 * radius);
        painter.drawChord(ellipseRect, 0, 180 * 16);
    }
}

void PPIWidget::drawAzimuthLines(QPainter& painter)
{
    painter.setPen(QPen(QColor(203, 213, 225, 180), 1)); // #cbd5e1 - Light theme grid color

    for (int i = 0; i < NUM_AZIMUTH_LINES; ++i) {
        float azimuth = MIN_AZIMUTH + (float(i) / (NUM_AZIMUTH_LINES - 1)) * (MAX_AZIMUTH - MIN_AZIMUTH);
        float radians = qDegreesToRadians(90.0f - azimuth);

        QPointF endPoint(
            m_center.x() + m_plotRadius * std::cos(radians),
            m_center.y() - m_plotRadius * std::sin(radians)
        );

        // Draw different styles for lines within and outside FoV (using configured min/max angles)
        if (azimuth >= m_minAngle && azimuth <= m_maxAngle) {
            // Lines within FoV - primary blue
            painter.setPen(QPen(QColor(59, 130, 246, 120), 1)); // #3b82f6
        } else {
            // Lines outside FoV - light gray
            painter.setPen(QPen(QColor(203, 213, 225, 100), 1)); // #cbd5e1
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

        float targetSize = inFoV ? 6.0f : 4.0f; // Larger targets in FoV
        painter.drawEllipse(targetPos, targetSize, targetSize);

        // Draw target ID
        painter.setPen(QPen(Qt::white, 1));
        painter.setFont(QFont("Arial", inFoV ? 9 : 7)); // Larger text in FoV
        QPointF textPos = targetPos + QPointF(0, -targetSize - 5);
        painter.drawText(textPos, QString::number(target.target_id));
    }
}

void PPIWidget::drawLabels(QPainter& painter)
{
    painter.setPen(QPen(QColor(100, 116, 139), 1)); // #64748b - Light theme text color
    painter.setFont(QFont("Segoe UI", 10));

    // Range labels
    for (int i = 1; i <= NUM_RANGE_RINGS; ++i) {
        float range = (float(i) / NUM_RANGE_RINGS) * m_maxRange;
        float radius = (float(i) / NUM_RANGE_RINGS) * m_plotRadius;

        QPointF labelPos(m_center.x() + radius * 0.707f, m_center.y() - radius * 0.707f);
        QString rangeText = QString("%1").arg(range, 0, 'f', 1) + "m";
        painter.drawText(labelPos, rangeText);
    }

    // Azimuth labels
    QFont azFont("Segoe UI", 11, QFont::DemiBold);
    painter.setFont(azFont);
    painter.setPen(QPen(QColor(100, 116, 139), 1)); // #64748b

    for (int i = 0; i < NUM_AZIMUTH_LINES; i += 2) {
        float azimuth = MIN_AZIMUTH + (float(i) / (NUM_AZIMUTH_LINES - 1)) * (MAX_AZIMUTH - MIN_AZIMUTH);
        float radians = qDegreesToRadians(90.0f - azimuth);

        QPointF labelPos(
            m_center.x() + (m_plotRadius + 20) * std::cos(radians),
            m_center.y() - (m_plotRadius + 20) * std::sin(radians)
        );

        QString azText = QString("%1°").arg(azimuth, 0, 'f', 0);
        QFontMetrics fm(azFont);
        QRect textRect = fm.boundingRect(azText);
        labelPos -= QPointF(textRect.width() / 2, -textRect.height() / 2);

        // Highlight FoV boundary labels (using configured min/max angles)
        if (std::abs(azimuth - m_minAngle) < 1.0f || std::abs(azimuth - m_maxAngle) < 1.0f) {
            painter.setPen(QPen(QColor(59, 130, 246), 1)); // #3b82f6 - Primary blue for FoV boundaries
        } else {
            painter.setPen(QPen(QColor(100, 116, 139), 1)); // #64748b - Normal gray
        }

        painter.drawText(labelPos, azText);
    }

    // Title and FoV indicator
    painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
    painter.setPen(QPen(QColor(30, 41, 59), 1)); // #1e293b - Dark text for title
    painter.drawText(QPointF(10, 25), "PPI Display - Target Tracks");

    // FoV information (show actual min/max angles)
    painter.setFont(QFont("Segoe UI", 10, QFont::Medium));
    painter.setPen(QPen(QColor(59, 130, 246), 1)); // #3b82f6 - Primary blue for FoV info
    QString fovText = QString("FoV: %1° to %2°").arg(m_minAngle, 0, 'f', 0).arg(m_maxAngle, 0, 'f', 0);
    painter.drawText(QPointF(10, 45), fovText);

    // Range information
    painter.setPen(QPen(QColor(16, 185, 129), 1)); // #10b981 - Success green
    QString rangeText = QString("Range: %1m to %2m").arg(m_minRange, 0, 'f', 1).arg(m_maxRange, 0, 'f', 1);
    painter.drawText(QPointF(10, 65), rangeText);

    // Legend
    painter.setFont(QFont("Segoe UI", 9));
    painter.setPen(QPen(QColor(148, 163, 184), 1)); // #94a3b8 - Muted text
    painter.drawText(QPointF(10, height() - 30), "Blue area: Active Field of View");
    painter.drawText(QPointF(10, height() - 15), "Bright targets: Within FoV, Dim targets: Outside FoV");
}

QColor PPIWidget::getTargetColor(float radialSpeed) const
{
    // Modern color coding based on radial speed (light theme friendly)
    if (std::abs(radialSpeed) < 1.0f) {
        return QColor(16, 185, 129); // #10b981 - Success green for stationary
    } else if (radialSpeed > 0) {
        // Approaching - warm red/orange
        int intensity = std::min(255, static_cast<int>(150 + std::abs(radialSpeed) * 10));
        return QColor(239, 68, 68); // #ef4444 - Error red for approaching
    } else {
        // Receding - cool blue
        int intensity = std::min(255, static_cast<int>(150 + std::abs(radialSpeed) * 10));
        return QColor(59, 130, 246); // #3b82f6 - Primary blue for receding
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
