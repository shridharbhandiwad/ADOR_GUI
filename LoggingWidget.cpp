#include "LoggingWidget.h"
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QtMath>
#include <QFont>
#include <QFontMetrics>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDialog>
#include <QHeaderView>
#include <algorithm>

// ==================== LoggingPlotWidget Implementation ====================

LoggingPlotWidget::LoggingPlotWidget(const QString& title, const QString& xLabel, 
                                       const QString& yLabel, QWidget *parent)
    : QWidget(parent)
    , m_title(title)
    , m_xLabel(xLabel)
    , m_yLabel(yLabel)
    , m_minY(0.0f)
    , m_maxY(100.0f)
    , m_defaultMinY(0.0f)
    , m_defaultMaxY(100.0f)
    , m_autoScale(true)
    , m_marginLeft(80)
    , m_marginRight(30)
    , m_marginTop(60)
    , m_marginBottom(60)
    , m_isDarkTheme(false)
    , m_isPanning(false)
    , m_zoomFactorY(1.0f)
    , m_panOffsetY(0.0f)
    , m_hoveredPointIndex(-1)
    , m_showTooltip(false)
    , m_hoveredTrackId(0)
{
    setMinimumSize(400, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

void LoggingPlotWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void LoggingPlotWidget::clearData()
{
    m_multiTrackData.clear();
    update();
}

void LoggingPlotWidget::setData(const QVector<QPair<qint64, float>>& data, const QString& label)
{
    m_multiTrackData.clear();
    m_multiTrackData[0] = data;  // Use track ID 0 for single track
    update();
}

void LoggingPlotWidget::setMultiTrackData(const QMap<uint32_t, QVector<QPair<qint64, float>>>& data)
{
    m_multiTrackData = data;
    update();
}

void LoggingPlotWidget::setYAxisRange(float minY, float maxY)
{
    m_minY = minY;
    m_maxY = maxY;
    m_defaultMinY = minY;
    m_defaultMaxY = maxY;
    m_autoScale = false;
    update();
}

void LoggingPlotWidget::setAutoScale(bool enabled)
{
    m_autoScale = enabled;
    update();
}

void LoggingPlotWidget::onResetZoom()
{
    m_zoomFactorY = 1.0f;
    m_panOffsetY = 0.0f;
    m_minY = m_defaultMinY;
    m_maxY = m_defaultMaxY;
    update();
}

void LoggingPlotWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_plotRect = QRect(m_marginLeft, m_marginTop, 
                       width() - m_marginLeft - m_marginRight,
                       height() - m_marginTop - m_marginBottom);
}

void LoggingPlotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    
    drawBackground(painter);
    drawGrid(painter);
    drawAxes(painter);
    drawData(painter);
    drawLabels(painter);
    drawLegend(painter);
    if (m_showTooltip) {
        drawTooltip(painter);
    }
}

void LoggingPlotWidget::drawBackground(QPainter& painter)
{
    // Draw widget background
    QColor bgColor = m_isDarkTheme ? QColor(18, 18, 22) : QColor(250, 250, 252);
    painter.fillRect(rect(), bgColor);
    
    // Draw plot area background
    QColor plotBgColor = m_isDarkTheme ? QColor(28, 28, 34) : QColor(255, 255, 255);
    painter.fillRect(m_plotRect, plotBgColor);
    
    // Draw border
    QColor borderColor = m_isDarkTheme ? QColor(50, 50, 58) : QColor(220, 220, 228);
    painter.setPen(QPen(borderColor, 2));
    painter.drawRect(m_plotRect);
}

void LoggingPlotWidget::drawGrid(QPainter& painter)
{
    QColor gridColor = m_isDarkTheme ? QColor(45, 45, 52) : QColor(235, 235, 240);
    painter.setPen(QPen(gridColor, 1, Qt::DotLine));
    
    // Horizontal grid lines (Y axis)
    int numYLines = 8;
    for (int i = 0; i <= numYLines; ++i) {
        float y = m_plotRect.top() + (m_plotRect.height() * i / numYLines);
        painter.drawLine(m_plotRect.left(), y, m_plotRect.right(), y);
    }
    
    // Vertical grid lines (time axis)
    int numXLines = 10;
    for (int i = 0; i <= numXLines; ++i) {
        float x = m_plotRect.left() + (m_plotRect.width() * i / numXLines);
        painter.drawLine(x, m_plotRect.top(), x, m_plotRect.bottom());
    }
}

void LoggingPlotWidget::drawAxes(QPainter& painter)
{
    QColor axisColor = m_isDarkTheme ? QColor(140, 140, 150) : QColor(100, 100, 110);
    painter.setPen(QPen(axisColor, 2));
    
    // Draw X and Y axes
    painter.drawLine(m_plotRect.bottomLeft(), m_plotRect.bottomRight());
    painter.drawLine(m_plotRect.bottomLeft(), m_plotRect.topLeft());
}

void LoggingPlotWidget::drawData(QPainter& painter)
{
    if (m_multiTrackData.isEmpty()) return;
    
    // Auto-scale if enabled
    if (m_autoScale) {
        float minVal = 1e9f;
        float maxVal = -1e9f;
        for (auto it = m_multiTrackData.begin(); it != m_multiTrackData.end(); ++it) {
            for (const auto& point : it.value()) {
                minVal = qMin(minVal, point.second);
                maxVal = qMax(maxVal, point.second);
            }
        }
        if (minVal < maxVal) {
            float margin = (maxVal - minVal) * 0.1f;
            m_minY = minVal - margin;
            m_maxY = maxVal + margin;
        }
    }
    
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // Draw each track's data
    int trackIndex = 0;
    for (auto it = m_multiTrackData.begin(); it != m_multiTrackData.end(); ++it) {
        uint32_t trackId = it.key();
        const QVector<QPair<qint64, float>>& data = it.value();
        
        if (data.isEmpty()) continue;
        
        QColor lineColor = getColorForTrackId(trackId);
        
        // Draw line connecting points
        QPainterPath path;
        bool first = true;
        for (const auto& point : data) {
            QPointF screenPos = dataToScreen(point.first, point.second);
            if (first) {
                path.moveTo(screenPos);
                first = false;
            } else {
                path.lineTo(screenPos);
            }
        }
        
        painter.setPen(QPen(lineColor, 2));
        painter.drawPath(path);
        
        // Draw points
        for (const auto& point : data) {
            QPointF screenPos = dataToScreen(point.first, point.second);
            painter.setBrush(lineColor);
            painter.drawEllipse(screenPos, 3, 3);
        }
        
        trackIndex++;
    }
}

void LoggingPlotWidget::drawLabels(QPainter& painter)
{
    QColor textColor = m_isDarkTheme ? QColor(200, 200, 210) : QColor(60, 60, 70);
    painter.setPen(textColor);
    
    QFont labelFont("Segoe UI", 11, QFont::Normal);
    painter.setFont(labelFont);
    
    // Draw Y axis labels
    int numYLabels = 8;
    for (int i = 0; i <= numYLabels; ++i) {
        float y = m_plotRect.top() + (m_plotRect.height() * i / numYLabels);
        float value = m_maxY - ((m_maxY - m_minY) * i / numYLabels);
        QString label = QString::number(value, 'f', 1);
        
        QFontMetrics fm(labelFont);
        QRect textRect = fm.boundingRect(label);
        painter.drawText(m_marginLeft - textRect.width() - 10, y + textRect.height() / 3, label);
    }
    
    // Draw title
    QFont titleFont("Segoe UI", 14, QFont::Bold);
    painter.setFont(titleFont);
    QFontMetrics titleFm(titleFont);
    QRect titleRect = titleFm.boundingRect(m_title);
    painter.drawText((width() - titleRect.width()) / 2, 30, m_title);
    
    // Draw Y axis label
    painter.setFont(labelFont);
    painter.save();
    painter.translate(20, height() / 2);
    painter.rotate(-90);
    QFontMetrics fm(labelFont);
    QRect yLabelRect = fm.boundingRect(m_yLabel);
    painter.drawText(-yLabelRect.width() / 2, 0, m_yLabel);
    painter.restore();
    
    // Draw X axis label
    painter.drawText((width() - fm.boundingRect(m_xLabel).width()) / 2, 
                     height() - 15, m_xLabel);
}

void LoggingPlotWidget::drawLegend(QPainter& painter)
{
    if (m_multiTrackData.isEmpty() || m_multiTrackData.size() <= 1) return;
    
    QColor legendBg = m_isDarkTheme ? QColor(28, 28, 34, 230) : QColor(255, 255, 255, 230);
    QColor legendBorder = m_isDarkTheme ? QColor(50, 50, 58) : QColor(220, 220, 228);
    QColor textColor = m_isDarkTheme ? QColor(200, 200, 210) : QColor(60, 60, 70);
    
    int legendX = m_plotRect.right() - 150;
    int legendY = m_plotRect.top() + 10;
    int legendWidth = 140;
    int legendHeight = 25 + m_multiTrackData.size() * 25;
    
    // Draw legend background
    painter.setBrush(legendBg);
    painter.setPen(QPen(legendBorder, 1));
    painter.drawRoundedRect(legendX, legendY, legendWidth, legendHeight, 8, 8);
    
    // Draw legend items
    QFont legendFont("Segoe UI", 10, QFont::Normal);
    painter.setFont(legendFont);
    painter.setPen(textColor);
    
    int yOffset = legendY + 20;
    for (auto it = m_multiTrackData.begin(); it != m_multiTrackData.end(); ++it) {
        uint32_t trackId = it.key();
        QColor lineColor = getColorForTrackId(trackId);
        
        // Draw color square
        painter.setBrush(lineColor);
        painter.drawRect(legendX + 10, yOffset - 8, 15, 15);
        
        // Draw track label
        painter.setPen(textColor);
        QString label = QString("Track %1").arg(trackId);
        painter.drawText(legendX + 35, yOffset + 3, label);
        
        yOffset += 25;
    }
}

void LoggingPlotWidget::drawTooltip(QPainter& painter)
{
    // Tooltip implementation
    QString tooltipText = QString("Track %1").arg(m_hoveredTrackId);
    
    QFont tooltipFont("Segoe UI", 10, QFont::Normal);
    painter.setFont(tooltipFont);
    QFontMetrics fm(tooltipFont);
    QRect textRect = fm.boundingRect(tooltipText);
    
    int padding = 8;
    QRect tooltipRect(m_mousePos.x() + 15, m_mousePos.y() - textRect.height() - padding * 2,
                      textRect.width() + padding * 2, textRect.height() + padding * 2);
    
    QColor tooltipBg = m_isDarkTheme ? QColor(28, 28, 34, 240) : QColor(255, 255, 255, 240);
    QColor tooltipBorder = m_isDarkTheme ? QColor(80, 80, 90) : QColor(180, 180, 190);
    QColor textColor = m_isDarkTheme ? QColor(240, 240, 245) : QColor(35, 35, 45);
    
    painter.setBrush(tooltipBg);
    painter.setPen(QPen(tooltipBorder, 2));
    painter.drawRoundedRect(tooltipRect, 6, 6);
    
    painter.setPen(textColor);
    painter.drawText(tooltipRect, Qt::AlignCenter, tooltipText);
}

QPointF LoggingPlotWidget::dataToScreen(qint64 timestamp, float value) const
{
    if (m_multiTrackData.isEmpty()) return QPointF();
    
    // Find min and max timestamps
    qint64 minTime = LLONG_MAX;
    qint64 maxTime = LLONG_MIN;
    for (auto it = m_multiTrackData.begin(); it != m_multiTrackData.end(); ++it) {
        for (const auto& point : it.value()) {
            minTime = qMin(minTime, point.first);
            maxTime = qMax(maxTime, point.first);
        }
    }
    
    if (maxTime <= minTime) maxTime = minTime + 1000;
    
    float xRatio = static_cast<float>(timestamp - minTime) / (maxTime - minTime);
    float yRatio = (m_maxY - value) / (m_maxY - m_minY);
    
    float x = m_plotRect.left() + xRatio * m_plotRect.width();
    float y = m_plotRect.top() + yRatio * m_plotRect.height();
    
    return QPointF(x, y);
}

QColor LoggingPlotWidget::getColorForTrackId(uint32_t trackId) const
{
    // Generate distinct colors for different tracks
    QVector<QColor> colors;
    if (m_isDarkTheme) {
        colors = {
            QColor(96, 165, 250),   // Blue
            QColor(34, 197, 94),    // Green
            QColor(251, 191, 36),   // Yellow
            QColor(248, 113, 113),  // Red
            QColor(168, 85, 247),   // Purple
            QColor(236, 72, 153),   // Pink
            QColor(20, 184, 166),   // Teal
            QColor(251, 146, 60)    // Orange
        };
    } else {
        colors = {
            QColor(37, 99, 235),    // Blue
            QColor(22, 163, 74),    // Green
            QColor(202, 138, 4),    // Yellow
            QColor(220, 38, 38),    // Red
            QColor(126, 34, 206),   // Purple
            QColor(190, 24, 93),    // Pink
            QColor(13, 148, 136),   // Teal
            QColor(234, 88, 12)     // Orange
        };
    }
    
    return colors[trackId % colors.size()];
}

void LoggingPlotWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_panOffsetY += delta.y() / m_plotRect.height();
        m_lastMousePos = event->pos();
        update();
    } else {
        // Check for hover
        m_showTooltip = m_plotRect.contains(event->pos());
        update();
    }
}

void LoggingPlotWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_plotRect.contains(event->pos())) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
    }
}

void LoggingPlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = false;
    }
}

void LoggingPlotWidget::wheelEvent(QWheelEvent *event)
{
    if (m_plotRect.contains(event->pos())) {
        float zoomDelta = event->angleDelta().y() > 0 ? 0.9f : 1.1f;
        m_zoomFactorY *= zoomDelta;
        
        float range = m_maxY - m_minY;
        float center = (m_maxY + m_minY) / 2.0f;
        float newRange = range * zoomDelta;
        m_minY = center - newRange / 2.0f;
        m_maxY = center + newRange / 2.0f;
        
        update();
    }
}

void LoggingPlotWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_showTooltip = false;
    update();
}

// ==================== LoggingRVPlotWidget Implementation ====================

LoggingRVPlotWidget::LoggingRVPlotWidget(QWidget *parent)
    : QWidget(parent)
    , m_minRange(0.0f)
    , m_maxRange(500.0f)
    , m_minVelocity(-100.0f)
    , m_maxVelocity(100.0f)
    , m_defaultMinRange(0.0f)
    , m_defaultMaxRange(500.0f)
    , m_defaultMinVelocity(-100.0f)
    , m_defaultMaxVelocity(100.0f)
    , m_pointSize(4)
    , m_marginLeft(80)
    , m_marginRight(30)
    , m_marginTop(60)
    , m_marginBottom(60)
    , m_isDarkTheme(false)
    , m_isPanning(false)
    , m_zoomFactorX(1.0f)
    , m_zoomFactorY(1.0f)
    , m_panOffsetX(0.0f)
    , m_panOffsetY(0.0f)
    , m_hoveredPointIndex(-1)
    , m_showTooltip(false)
    , m_hoveredTrackId(0)
{
    setMinimumSize(400, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);
}

void LoggingRVPlotWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void LoggingRVPlotWidget::clearData()
{
    m_multiTrackData.clear();
    update();
}

void LoggingRVPlotWidget::setData(const QVector<QPair<float, float>>& data)
{
    m_multiTrackData.clear();
    m_multiTrackData[0] = data;
    update();
}

void LoggingRVPlotWidget::setMultiTrackData(const QMap<uint32_t, QVector<QPair<float, float>>>& data)
{
    m_multiTrackData = data;
    update();
}

void LoggingRVPlotWidget::setRangeLimit(float maxRange)
{
    m_maxRange = maxRange;
    m_defaultMaxRange = maxRange;
    update();
}

void LoggingRVPlotWidget::setVelocityLimit(float maxVelocity)
{
    m_maxVelocity = maxVelocity;
    m_minVelocity = -maxVelocity;
    m_defaultMaxVelocity = maxVelocity;
    m_defaultMinVelocity = -maxVelocity;
    update();
}

void LoggingRVPlotWidget::onResetZoom()
{
    m_zoomFactorX = 1.0f;
    m_zoomFactorY = 1.0f;
    m_panOffsetX = 0.0f;
    m_panOffsetY = 0.0f;
    m_minRange = m_defaultMinRange;
    m_maxRange = m_defaultMaxRange;
    m_minVelocity = m_defaultMinVelocity;
    m_maxVelocity = m_defaultMaxVelocity;
    update();
}

void LoggingRVPlotWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_plotRect = QRect(m_marginLeft, m_marginTop, 
                       width() - m_marginLeft - m_marginRight,
                       height() - m_marginTop - m_marginBottom);
}

void LoggingRVPlotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    
    drawBackground(painter);
    drawGrid(painter);
    drawAxes(painter);
    drawDataPoints(painter);
    drawLabels(painter);
    drawLegend(painter);
    if (m_showTooltip) {
        drawTooltip(painter);
    }
}

void LoggingRVPlotWidget::drawBackground(QPainter& painter)
{
    QColor bgColor = m_isDarkTheme ? QColor(18, 18, 22) : QColor(250, 250, 252);
    painter.fillRect(rect(), bgColor);
    
    QColor plotBgColor = m_isDarkTheme ? QColor(28, 28, 34) : QColor(255, 255, 255);
    painter.fillRect(m_plotRect, plotBgColor);
    
    QColor borderColor = m_isDarkTheme ? QColor(50, 50, 58) : QColor(220, 220, 228);
    painter.setPen(QPen(borderColor, 2));
    painter.drawRect(m_plotRect);
}

void LoggingRVPlotWidget::drawGrid(QPainter& painter)
{
    QColor gridColor = m_isDarkTheme ? QColor(45, 45, 52) : QColor(235, 235, 240);
    painter.setPen(QPen(gridColor, 1, Qt::DotLine));
    
    // Vertical grid lines
    int numXLines = 10;
    for (int i = 0; i <= numXLines; ++i) {
        float x = m_plotRect.left() + (m_plotRect.width() * i / numXLines);
        painter.drawLine(x, m_plotRect.top(), x, m_plotRect.bottom());
    }
    
    // Horizontal grid lines
    int numYLines = 10;
    for (int i = 0; i <= numYLines; ++i) {
        float y = m_plotRect.top() + (m_plotRect.height() * i / numYLines);
        painter.drawLine(m_plotRect.left(), y, m_plotRect.right(), y);
    }
}

void LoggingRVPlotWidget::drawAxes(QPainter& painter)
{
    QColor axisColor = m_isDarkTheme ? QColor(140, 140, 150) : QColor(100, 100, 110);
    painter.setPen(QPen(axisColor, 2));
    
    // Find zero velocity line position
    float zeroVelRatio = (0.0f - m_minVelocity) / (m_maxVelocity - m_minVelocity);
    float zeroVelY = m_plotRect.bottom() - zeroVelRatio * m_plotRect.height();
    
    // Draw X axis (at zero velocity)
    if (zeroVelY >= m_plotRect.top() && zeroVelY <= m_plotRect.bottom()) {
        painter.drawLine(m_plotRect.left(), zeroVelY, m_plotRect.right(), zeroVelY);
    }
    
    // Draw Y axis (at zero range - left edge)
    painter.drawLine(m_plotRect.bottomLeft(), m_plotRect.topLeft());
}

void LoggingRVPlotWidget::drawDataPoints(QPainter& painter)
{
    if (m_multiTrackData.isEmpty()) return;
    
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    for (auto it = m_multiTrackData.begin(); it != m_multiTrackData.end(); ++it) {
        uint32_t trackId = it.key();
        const QVector<QPair<float, float>>& data = it.value();
        
        QColor pointColor = getColorForTrackId(trackId);
        painter.setBrush(pointColor);
        painter.setPen(Qt::NoPen);
        
        for (const auto& point : data) {
            float velocity = point.first;
            float range = point.second;
            
            // Convert to screen coordinates
            float xRatio = (range - m_minRange) / (m_maxRange - m_minRange);
            float yRatio = (velocity - m_minVelocity) / (m_maxVelocity - m_minVelocity);
            
            float x = m_plotRect.left() + xRatio * m_plotRect.width();
            float y = m_plotRect.bottom() - yRatio * m_plotRect.height();
            
            // Draw point
            if (x >= m_plotRect.left() && x <= m_plotRect.right() &&
                y >= m_plotRect.top() && y <= m_plotRect.bottom()) {
                painter.drawEllipse(QPointF(x, y), m_pointSize, m_pointSize);
            }
        }
    }
}

void LoggingRVPlotWidget::drawLabels(QPainter& painter)
{
    QColor textColor = m_isDarkTheme ? QColor(200, 200, 210) : QColor(60, 60, 70);
    painter.setPen(textColor);
    
    QFont labelFont("Segoe UI", 11, QFont::Normal);
    painter.setFont(labelFont);
    
    // Draw X axis labels (Range)
    int numXLabels = 10;
    for (int i = 0; i <= numXLabels; ++i) {
        float x = m_plotRect.left() + (m_plotRect.width() * i / numXLabels);
        float value = m_minRange + ((m_maxRange - m_minRange) * i / numXLabels);
        QString label = QString::number(value, 'f', 0);
        
        QFontMetrics fm(labelFont);
        QRect textRect = fm.boundingRect(label);
        painter.drawText(x - textRect.width() / 2, m_plotRect.bottom() + 25, label);
    }
    
    // Draw Y axis labels (Velocity)
    int numYLabels = 10;
    for (int i = 0; i <= numYLabels; ++i) {
        float y = m_plotRect.top() + (m_plotRect.height() * i / numYLabels);
        float value = m_maxVelocity - ((m_maxVelocity - m_minVelocity) * i / numYLabels);
        QString label = QString::number(value, 'f', 0);
        
        QFontMetrics fm(labelFont);
        QRect textRect = fm.boundingRect(label);
        painter.drawText(m_marginLeft - textRect.width() - 10, y + textRect.height() / 3, label);
    }
    
    // Draw title
    QFont titleFont("Segoe UI", 14, QFont::Bold);
    painter.setFont(titleFont);
    QString title = "Range-Velocity Plot";
    QFontMetrics titleFm(titleFont);
    QRect titleRect = titleFm.boundingRect(title);
    painter.drawText((width() - titleRect.width()) / 2, 30, title);
    
    // Draw axis labels
    painter.setFont(labelFont);
    QFontMetrics fm(labelFont);
    
    // X axis label
    QString xLabel = "Range (m)";
    QRect xLabelRect = fm.boundingRect(xLabel);
    painter.drawText((width() - xLabelRect.width()) / 2, height() - 15, xLabel);
    
    // Y axis label
    painter.save();
    painter.translate(20, height() / 2);
    painter.rotate(-90);
    QString yLabel = "Velocity (m/s)";
    QRect yLabelRect = fm.boundingRect(yLabel);
    painter.drawText(-yLabelRect.width() / 2, 0, yLabel);
    painter.restore();
}

void LoggingRVPlotWidget::drawLegend(QPainter& painter)
{
    if (m_multiTrackData.isEmpty() || m_multiTrackData.size() <= 1) return;
    
    QColor legendBg = m_isDarkTheme ? QColor(28, 28, 34, 230) : QColor(255, 255, 255, 230);
    QColor legendBorder = m_isDarkTheme ? QColor(50, 50, 58) : QColor(220, 220, 228);
    QColor textColor = m_isDarkTheme ? QColor(200, 200, 210) : QColor(60, 60, 70);
    
    int legendX = m_plotRect.right() - 150;
    int legendY = m_plotRect.top() + 10;
    int legendWidth = 140;
    int legendHeight = 25 + m_multiTrackData.size() * 25;
    
    painter.setBrush(legendBg);
    painter.setPen(QPen(legendBorder, 1));
    painter.drawRoundedRect(legendX, legendY, legendWidth, legendHeight, 8, 8);
    
    QFont legendFont("Segoe UI", 10, QFont::Normal);
    painter.setFont(legendFont);
    painter.setPen(textColor);
    
    int yOffset = legendY + 20;
    for (auto it = m_multiTrackData.begin(); it != m_multiTrackData.end(); ++it) {
        uint32_t trackId = it.key();
        QColor pointColor = getColorForTrackId(trackId);
        
        painter.setBrush(pointColor);
        painter.drawEllipse(legendX + 15, yOffset - 5, 10, 10);
        
        painter.setPen(textColor);
        QString label = QString("Track %1").arg(trackId);
        painter.drawText(legendX + 35, yOffset + 3, label);
        
        yOffset += 25;
    }
}

void LoggingRVPlotWidget::drawTooltip(QPainter& painter)
{
    // Similar to LoggingPlotWidget
}

QColor LoggingRVPlotWidget::getColorForTrackId(uint32_t trackId) const
{
    QVector<QColor> colors;
    if (m_isDarkTheme) {
        colors = {
            QColor(96, 165, 250), QColor(34, 197, 94), QColor(251, 191, 36),
            QColor(248, 113, 113), QColor(168, 85, 247), QColor(236, 72, 153),
            QColor(20, 184, 166), QColor(251, 146, 60)
        };
    } else {
        colors = {
            QColor(37, 99, 235), QColor(22, 163, 74), QColor(202, 138, 4),
            QColor(220, 38, 38), QColor(126, 34, 206), QColor(190, 24, 93),
            QColor(13, 148, 136), QColor(234, 88, 12)
        };
    }
    return colors[trackId % colors.size()];
}

void LoggingRVPlotWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    if (m_isPanning) {
        QPoint delta = event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();
        update();
    }
}

void LoggingRVPlotWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_plotRect.contains(event->pos())) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
    }
}

void LoggingRVPlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = false;
    }
}

void LoggingRVPlotWidget::wheelEvent(QWheelEvent *event)
{
    if (m_plotRect.contains(event->pos())) {
        float zoomDelta = event->angleDelta().y() > 0 ? 0.9f : 1.1f;
        
        float rangeCenter = (m_maxRange + m_minRange) / 2.0f;
        float rangeSpan = (m_maxRange - m_minRange) * zoomDelta;
        m_minRange = rangeCenter - rangeSpan / 2.0f;
        m_maxRange = rangeCenter + rangeSpan / 2.0f;
        
        float velCenter = (m_maxVelocity + m_minVelocity) / 2.0f;
        float velSpan = (m_maxVelocity - m_minVelocity) * zoomDelta;
        m_minVelocity = velCenter - velSpan / 2.0f;
        m_maxVelocity = velCenter + velSpan / 2.0f;
        
        update();
    }
}

void LoggingRVPlotWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_showTooltip = false;
    update();
}

// ==================== LoggingWidget Implementation ====================

LoggingWidget::LoggingWidget(QWidget *parent)
    : QWidget(parent)
    , m_statusLabel(nullptr)
    , m_startButton(nullptr)
    , m_stopButton(nullptr)
    , m_clearButton(nullptr)
    , m_exportButton(nullptr)
    , m_importButton(nullptr)
    , m_detachButton(nullptr)
    , m_trackFilterCombo(nullptr)
    , m_refreshTracksButton(nullptr)
    , m_showAllTracksCheckBox(nullptr)
    , m_algorithmWindowSpinBox(nullptr)
    , m_smoothingWindowSpinBox(nullptr)
    , m_computeRangeRateButton(nullptr)
    , m_customAlgorithmButton(nullptr)
    , m_algorithmOutputText(nullptr)
    , m_plotRangeTimeButton(nullptr)
    , m_plotSpeedTimeButton(nullptr)
    , m_plotRVButton(nullptr)
    , m_autoUpdatePlotsCheckBox(nullptr)
    , m_rangeTimePlot(nullptr)
    , m_speedTimePlot(nullptr)
    , m_rvPlot(nullptr)
    , m_dataTable(nullptr)
    , m_isLogging(false)
    , m_isDarkTheme(false)
    , m_loggingStartTime(0)
    , m_totalDataPoints(0)
    , m_plotRefreshTimer(nullptr)
    , m_algorithmWindow(5)
    , m_smoothingWindow(3)
{
    setupUI();
    
    // Setup refresh timer for auto-update
    m_plotRefreshTimer = new QTimer(this);
    m_plotRefreshTimer->setInterval(1000);  // Refresh every 1 second
    connect(m_plotRefreshTimer, &QTimer::timeout, this, &LoggingWidget::onRefreshPlotsTimer);
}

LoggingWidget::~LoggingWidget()
{
    if (m_plotRefreshTimer) {
        m_plotRefreshTimer->stop();
    }
}

void LoggingWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    
    // Create main splitter for left controls and right plots
    QSplitter* mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // ========== Left Panel: Controls ==========
    QWidget* leftPanel = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(12);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    
    setupControlPanel();
    setupFilterPanel();
    setupAlgorithmPanel();
    setupVisualizationPanel();
    
    // Add all control panels to left layout
    QWidget* controlsWidget = findChild<QWidget*>("controlsWidget");
    QWidget* filterWidget = findChild<QWidget*>("filterWidget");
    QWidget* algorithmWidget = findChild<QWidget*>("algorithmWidget");
    QWidget* visualizationWidget = findChild<QWidget*>("visualizationWidget");
    
    if (controlsWidget) leftLayout->addWidget(controlsWidget);
    if (filterWidget) leftLayout->addWidget(filterWidget);
    if (algorithmWidget) leftLayout->addWidget(algorithmWidget);
    if (visualizationWidget) leftLayout->addWidget(visualizationWidget);
    
    leftLayout->addStretch();
    
    // ========== Right Panel: Plots and Data ==========
    QWidget* rightPanel = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(8);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    
    setupPlotArea();
    setupDataTable();
    
    QWidget* plotWidget = findChild<QWidget*>("plotWidget");
    if (plotWidget) {
        rightLayout->addWidget(plotWidget, 2);
    }
    
    if (m_dataTable) {
        QGroupBox* tableGroup = new QGroupBox("Logged Data Table", this);
        QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
        tableLayout->addWidget(m_dataTable);
        rightLayout->addWidget(tableGroup, 1);
    }
    
    // Add panels to splitter
    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 3);
    
    mainLayout->addWidget(mainSplitter);
}

void LoggingWidget::setupControlPanel()
{
    QGroupBox* controlGroup = new QGroupBox("Logging Control", this);
    controlGroup->setObjectName("controlsWidget");
    QVBoxLayout* layout = new QVBoxLayout(controlGroup);
    
    // Status label
    m_statusLabel = new QLabel("Status: Not Logging", this);
    QFont statusFont = m_statusLabel->font();
    statusFont.setBold(true);
    m_statusLabel->setFont(statusFont);
    layout->addWidget(m_statusLabel);
    
    // Control buttons
    QHBoxLayout* buttonLayout1 = new QHBoxLayout();
    m_startButton = new QPushButton("Start Logging", this);
    m_stopButton = new QPushButton("Stop Logging", this);
    m_stopButton->setEnabled(false);
    buttonLayout1->addWidget(m_startButton);
    buttonLayout1->addWidget(m_stopButton);
    layout->addLayout(buttonLayout1);
    
    QHBoxLayout* buttonLayout2 = new QHBoxLayout();
    m_clearButton = new QPushButton("Clear Logs", this);
    m_detachButton = new QPushButton("Detach Window", this);
    buttonLayout2->addWidget(m_clearButton);
    buttonLayout2->addWidget(m_detachButton);
    layout->addLayout(buttonLayout2);
    
    QHBoxLayout* buttonLayout3 = new QHBoxLayout();
    m_exportButton = new QPushButton("Export CSV", this);
    m_importButton = new QPushButton("Import CSV", this);
    buttonLayout3->addWidget(m_exportButton);
    buttonLayout3->addWidget(m_importButton);
    layout->addLayout(buttonLayout3);
    
    // Connect signals
    connect(m_startButton, &QPushButton::clicked, this, &LoggingWidget::onStartLogging);
    connect(m_stopButton, &QPushButton::clicked, this, &LoggingWidget::onStopLogging);
    connect(m_clearButton, &QPushButton::clicked, this, &LoggingWidget::onClearLogs);
    connect(m_exportButton, &QPushButton::clicked, this, &LoggingWidget::onExportLogs);
    connect(m_importButton, &QPushButton::clicked, this, &LoggingWidget::onImportLogs);
    connect(m_detachButton, &QPushButton::clicked, this, &LoggingWidget::onDetachWindow);
}

void LoggingWidget::setupFilterPanel()
{
    QGroupBox* filterGroup = new QGroupBox("Track Filter", this);
    filterGroup->setObjectName("filterWidget");
    QVBoxLayout* layout = new QVBoxLayout(filterGroup);
    
    QLabel* label = new QLabel("Select Track ID:", this);
    layout->addWidget(label);
    
    QHBoxLayout* comboLayout = new QHBoxLayout();
    m_trackFilterCombo = new QComboBox(this);
    m_trackFilterCombo->addItem("All Tracks", -1);
    comboLayout->addWidget(m_trackFilterCombo, 1);
    
    m_refreshTracksButton = new QPushButton("Refresh", this);
    comboLayout->addWidget(m_refreshTracksButton);
    layout->addLayout(comboLayout);
    
    m_showAllTracksCheckBox = new QCheckBox("Show All Tracks", this);
    m_showAllTracksCheckBox->setChecked(true);
    layout->addWidget(m_showAllTracksCheckBox);
    
    connect(m_trackFilterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &LoggingWidget::onTrackFilterChanged);
    connect(m_refreshTracksButton, &QPushButton::clicked, this, &LoggingWidget::onRefreshTrackList);
    connect(m_showAllTracksCheckBox, &QCheckBox::stateChanged, this, &LoggingWidget::onUpdatePlots);
}

void LoggingWidget::setupAlgorithmPanel()
{
    QGroupBox* algorithmGroup = new QGroupBox("Range Rate Algorithm", this);
    algorithmGroup->setObjectName("algorithmWidget");
    QVBoxLayout* layout = new QVBoxLayout(algorithmGroup);
    
    // Detection window setting
    QHBoxLayout* windowLayout = new QHBoxLayout();
    QLabel* windowLabel = new QLabel("Detection Window:", this);
    m_algorithmWindowSpinBox = new QSpinBox(this);
    m_algorithmWindowSpinBox->setRange(2, 50);
    m_algorithmWindowSpinBox->setValue(5);
    m_algorithmWindowSpinBox->setToolTip("Number of detections to consider for range rate computation");
    windowLayout->addWidget(windowLabel);
    windowLayout->addWidget(m_algorithmWindowSpinBox);
    layout->addLayout(windowLayout);
    
    // Smoothing window setting
    QHBoxLayout* smoothLayout = new QHBoxLayout();
    QLabel* smoothLabel = new QLabel("Smoothing Window:", this);
    m_smoothingWindowSpinBox = new QSpinBox(this);
    m_smoothingWindowSpinBox->setRange(1, 20);
    m_smoothingWindowSpinBox->setValue(3);
    m_smoothingWindowSpinBox->setToolTip("Number of previous cycles for data smoothing");
    smoothLayout->addWidget(smoothLabel);
    smoothLayout->addWidget(m_smoothingWindowSpinBox);
    layout->addLayout(smoothLayout);
    
    // Compute button
    m_computeRangeRateButton = new QPushButton("Compute Range Rate", this);
    layout->addWidget(m_computeRangeRateButton);
    
    // Custom algorithm button
    m_customAlgorithmButton = new QPushButton("Custom Algorithm...", this);
    layout->addWidget(m_customAlgorithmButton);
    
    // Algorithm output
    QLabel* outputLabel = new QLabel("Algorithm Output:", this);
    layout->addWidget(outputLabel);
    
    m_algorithmOutputText = new QTextEdit(this);
    m_algorithmOutputText->setReadOnly(true);
    m_algorithmOutputText->setMaximumHeight(100);
    layout->addWidget(m_algorithmOutputText);
    
    connect(m_algorithmWindowSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &LoggingWidget::onAlgorithmWindowChanged);
    connect(m_smoothingWindowSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &LoggingWidget::onSmoothingWindowChanged);
    connect(m_computeRangeRateButton, &QPushButton::clicked, this, &LoggingWidget::onComputeRangeRate);
    connect(m_customAlgorithmButton, &QPushButton::clicked, this, &LoggingWidget::onCustomAlgorithmClicked);
}

void LoggingWidget::setupVisualizationPanel()
{
    QGroupBox* visualizationGroup = new QGroupBox("Visualization", this);
    visualizationGroup->setObjectName("visualizationWidget");
    QVBoxLayout* layout = new QVBoxLayout(visualizationGroup);
    
    m_plotRangeTimeButton = new QPushButton("Plot Range vs Time", this);
    layout->addWidget(m_plotRangeTimeButton);
    
    m_plotSpeedTimeButton = new QPushButton("Plot Speed vs Time", this);
    layout->addWidget(m_plotSpeedTimeButton);
    
    m_plotRVButton = new QPushButton("Plot Range-Velocity", this);
    layout->addWidget(m_plotRVButton);
    
    m_autoUpdatePlotsCheckBox = new QCheckBox("Auto-Update Plots", this);
    m_autoUpdatePlotsCheckBox->setChecked(true);
    layout->addWidget(m_autoUpdatePlotsCheckBox);
    
    connect(m_plotRangeTimeButton, &QPushButton::clicked, this, &LoggingWidget::onPlotRangeVsTime);
    connect(m_plotSpeedTimeButton, &QPushButton::clicked, this, &LoggingWidget::onPlotSpeedVsTime);
    connect(m_plotRVButton, &QPushButton::clicked, this, &LoggingWidget::onPlotRV);
    connect(m_autoUpdatePlotsCheckBox, &QCheckBox::stateChanged, this, &LoggingWidget::onUpdatePlots);
}

void LoggingWidget::setupPlotArea()
{
    QWidget* plotWidget = new QWidget(this);
    plotWidget->setObjectName("plotWidget");
    QVBoxLayout* layout = new QVBoxLayout(plotWidget);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);
    
    // Create tabbed plot area
    QTabWidget* plotTabs = new QTabWidget(this);
    
    // Range vs Time plot
    m_rangeTimePlot = new LoggingPlotWidget("Range vs Time", "Time", "Range (m)", this);
    plotTabs->addTab(m_rangeTimePlot, "Range vs Time");
    
    // Speed vs Time plot
    m_speedTimePlot = new LoggingPlotWidget("Speed vs Time", "Time", "Speed (m/s)", this);
    plotTabs->addTab(m_speedTimePlot, "Speed vs Time");
    
    // Range-Velocity plot
    m_rvPlot = new LoggingRVPlotWidget(this);
    plotTabs->addTab(m_rvPlot, "Range-Velocity");
    
    layout->addWidget(plotTabs);
}

void LoggingWidget::setupDataTable()
{
    m_dataTable = new QTableWidget(this);
    m_dataTable->setColumnCount(8);
    m_dataTable->setHorizontalHeaderLabels({
        "Timestamp", "Track ID", "Range (m)", "Speed (m/s)", 
        "Azimuth (deg)", "Level (dB)", "Range Rate (m/s)", "Duration (s)"
    });
    m_dataTable->horizontalHeader()->setStretchLastSection(true);
    m_dataTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_dataTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_dataTable->setAlternatingRowColors(true);
    m_dataTable->setMaximumHeight(200);
}

void LoggingWidget::updateFromTargets(const TargetTrackData& targets)
{
    if (!m_isLogging) return;
    
    logTrackData(targets);
    
    // Auto-update plots if enabled
    if (m_autoUpdatePlotsCheckBox && m_autoUpdatePlotsCheckBox->isChecked()) {
        if (!m_plotRefreshTimer->isActive()) {
            m_plotRefreshTimer->start();
        }
    }
    
    // Update status
    m_statusLabel->setText(QString("Status: Logging (%1 tracks, %2 data points)")
                           .arg(m_trackLogs.size())
                           .arg(m_totalDataPoints));
}

void LoggingWidget::logTrackData(const TargetTrackData& targets)
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
    for (size_t i = 0; i < targets.numTracks && i < targets.targets.size(); ++i) {
        const TargetTrack& target = targets.targets[i];
        
        LoggedTrackDataPoint dataPoint;
        dataPoint.timestamp = currentTime;
        dataPoint.target_id = target.target_id;
        dataPoint.level = target.level;
        dataPoint.range = target.radius;
        dataPoint.azimuth = target.azimuth;
        dataPoint.radial_speed = target.radial_speed;
        dataPoint.azimuth_speed = target.azimuth_speed;
        dataPoint.computed_range_rate = 0.0f;  // Will be computed later
        
        // Add to track log
        if (!m_trackLogs.contains(target.target_id)) {
            TrackLogData newTrack;
            newTrack.track_id = target.target_id;
            newTrack.firstSeen = currentTime;
            newTrack.lastSeen = currentTime;
            m_trackLogs[target.target_id] = newTrack;
            
            if (!m_activeTrackIds.contains(target.target_id)) {
                m_activeTrackIds.append(target.target_id);
            }
        }
        
        TrackLogData& trackLog = m_trackLogs[target.target_id];
        trackLog.lastSeen = currentTime;
        trackLog.dataPoints.append(dataPoint);
        
        // Limit data points per track
        if (trackLog.dataPoints.size() > MAX_DATA_POINTS_PER_TRACK) {
            trackLog.dataPoints.removeFirst();
        }
        
        m_totalDataPoints++;
    }
}

void LoggingWidget::onStartLogging()
{
    m_isLogging = true;
    m_loggingStartTime = QDateTime::currentMSecsSinceEpoch();
    
    m_startButton->setEnabled(false);
    m_stopButton->setEnabled(true);
    m_statusLabel->setText("Status: Logging...");
    
    if (m_algorithmOutputText) {
        m_algorithmOutputText->append("Logging started at " + 
                                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
}

void LoggingWidget::onStopLogging()
{
    m_isLogging = false;
    
    m_startButton->setEnabled(true);
    m_stopButton->setEnabled(false);
    m_statusLabel->setText(QString("Status: Stopped (%1 tracks, %2 data points)")
                           .arg(m_trackLogs.size())
                           .arg(m_totalDataPoints));
    
    if (m_plotRefreshTimer) {
        m_plotRefreshTimer->stop();
    }
    
    if (m_algorithmOutputText) {
        m_algorithmOutputText->append("Logging stopped at " + 
                                       QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
}

void LoggingWidget::onClearLogs()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Clear Logs", 
        "Are you sure you want to clear all logged data?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        m_trackLogs.clear();
        m_activeTrackIds.clear();
        m_totalDataPoints = 0;
        
        if (m_rangeTimePlot) m_rangeTimePlot->clearData();
        if (m_speedTimePlot) m_speedTimePlot->clearData();
        if (m_rvPlot) m_rvPlot->clearData();
        if (m_dataTable) m_dataTable->setRowCount(0);
        
        m_statusLabel->setText("Status: Cleared");
        if (m_algorithmOutputText) {
            m_algorithmOutputText->clear();
            m_algorithmOutputText->append("Logs cleared.");
        }
        
        onRefreshTrackList();
    }
}

void LoggingWidget::onExportLogs()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Export Logs", "", "CSV Files (*.csv);;All Files (*)");
    
    if (!filename.isEmpty()) {
        exportToCSV(filename);
        QMessageBox::information(this, "Export Complete", 
                                 QString("Logs exported to %1").arg(filename));
    }
}

void LoggingWidget::onImportLogs()
{
    QString filename = QFileDialog::getOpenFileName(
        this, "Import Logs", "", "CSV Files (*.csv);;All Files (*)");
    
    if (!filename.isEmpty()) {
        importFromCSV(filename);
        QMessageBox::information(this, "Import Complete", 
                                 QString("Logs imported from %1").arg(filename));
        onRefreshTrackList();
        onUpdatePlots();
    }
}

void LoggingWidget::exportToCSV(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export Error", "Could not open file for writing.");
        return;
    }
    
    QTextStream out(&file);
    
    // Write header
    out << "Timestamp,Track_ID,Range_m,Speed_m_s,Azimuth_deg,Level_dB,Range_Rate_m_s\n";
    
    // Write data for all tracks
    for (auto it = m_trackLogs.begin(); it != m_trackLogs.end(); ++it) {
        const TrackLogData& trackLog = it.value();
        for (const LoggedTrackDataPoint& point : trackLog.dataPoints) {
            out << point.timestamp << ","
                << point.target_id << ","
                << point.range << ","
                << point.radial_speed << ","
                << point.azimuth << ","
                << point.level << ","
                << point.computed_range_rate << "\n";
        }
    }
    
    file.close();
}

void LoggingWidget::importFromCSV(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Import Error", "Could not open file for reading.");
        return;
    }
    
    QTextStream in(&file);
    
    // Skip header
    QString header = in.readLine();
    
    // Clear existing data
    m_trackLogs.clear();
    m_activeTrackIds.clear();
    m_totalDataPoints = 0;
    
    // Read data
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        
        if (fields.size() >= 7) {
            LoggedTrackDataPoint point;
            point.timestamp = fields[0].toLongLong();
            point.target_id = fields[1].toUInt();
            point.range = fields[2].toFloat();
            point.radial_speed = fields[3].toFloat();
            point.azimuth = fields[4].toFloat();
            point.level = fields[5].toFloat();
            point.computed_range_rate = fields[6].toFloat();
            
            if (!m_trackLogs.contains(point.target_id)) {
                TrackLogData newTrack;
                newTrack.track_id = point.target_id;
                newTrack.firstSeen = point.timestamp;
                newTrack.lastSeen = point.timestamp;
                m_trackLogs[point.target_id] = newTrack;
                
                if (!m_activeTrackIds.contains(point.target_id)) {
                    m_activeTrackIds.append(point.target_id);
                }
            }
            
            TrackLogData& trackLog = m_trackLogs[point.target_id];
            trackLog.lastSeen = qMax(trackLog.lastSeen, point.timestamp);
            trackLog.firstSeen = qMin(trackLog.firstSeen, point.timestamp);
            trackLog.dataPoints.append(point);
            
            m_totalDataPoints++;
        }
    }
    
    file.close();
}

void LoggingWidget::onTrackFilterChanged(int index)
{
    Q_UNUSED(index)
    updatePlotsForSelectedTrack();
    updateDataTable();
}

void LoggingWidget::onRefreshTrackList()
{
    m_trackFilterCombo->clear();
    m_trackFilterCombo->addItem("All Tracks", -1);
    
    for (uint32_t trackId : m_activeTrackIds) {
        m_trackFilterCombo->addItem(QString("Track %1").arg(trackId), trackId);
    }
}

void LoggingWidget::onComputeRangeRate()
{
    m_algorithmWindow = m_algorithmWindowSpinBox->value();
    m_smoothingWindow = m_smoothingWindowSpinBox->value();
    
    if (m_algorithmOutputText) {
        m_algorithmOutputText->append(QString("Computing range rate with window=%1, smoothing=%2")
                                       .arg(m_algorithmWindow)
                                       .arg(m_smoothingWindow));
    }
    
    // Compute range rate for all tracks
    for (auto it = m_trackLogs.begin(); it != m_trackLogs.end(); ++it) {
        computeRangeRateForTrack(it.key());
    }
    
    if (m_algorithmOutputText) {
        m_algorithmOutputText->append("Range rate computation complete.");
    }
    
    // Update plots and table
    onUpdatePlots();
    updateDataTable();
}

void LoggingWidget::computeRangeRateForTrack(uint32_t trackId)
{
    if (!m_trackLogs.contains(trackId)) return;
    
    TrackLogData& trackLog = m_trackLogs[trackId];
    QVector<LoggedTrackDataPoint>& points = trackLog.dataPoints;
    
    for (int i = 0; i < points.size(); ++i) {
        float rangeRate = computeRangeRateAtIndex(points, i, m_algorithmWindow, m_smoothingWindow);
        points[i].computed_range_rate = rangeRate;
    }
}

float LoggingWidget::computeRangeRateAtIndex(const QVector<LoggedTrackDataPoint>& points, 
                                               int index, int window, int smoothing)
{
    if (points.isEmpty() || index < 0 || index >= points.size()) return 0.0f;
    
    // Simple range rate computation using finite difference
    int startIdx = qMax(0, index - window / 2);
    int endIdx = qMin(points.size() - 1, index + window / 2);
    
    if (startIdx >= endIdx) return points[index].radial_speed;
    
    // Calculate average range rate over window
    float sumRangeRate = 0.0f;
    int count = 0;
    
    for (int i = startIdx; i <= endIdx; ++i) {
        if (i > 0 && i < points.size()) {
            float dt = (points[i].timestamp - points[i-1].timestamp) / 1000.0f;  // Convert to seconds
            if (dt > 0.001f) {
                float dr = points[i].range - points[i-1].range;
                float rate = dr / dt;
                sumRangeRate += rate;
                count++;
            }
        }
    }
    
    float avgRangeRate = (count > 0) ? (sumRangeRate / count) : points[index].radial_speed;
    
    // Apply smoothing
    if (smoothing > 1 && index >= smoothing) {
        float smoothedRate = 0.0f;
        for (int i = 0; i < smoothing && (index - i) >= 0; ++i) {
            smoothedRate += avgRangeRate;
        }
        avgRangeRate = smoothedRate / smoothing;
    }
    
    return avgRangeRate;
}

void LoggingWidget::onAlgorithmWindowChanged(int value)
{
    m_algorithmWindow = value;
}

void LoggingWidget::onSmoothingWindowChanged(int value)
{
    m_smoothingWindow = value;
}

void LoggingWidget::onCustomAlgorithmClicked()
{
    QMessageBox::information(this, "Custom Algorithm", 
                             "Custom algorithm editor not implemented yet.\n"
                             "This feature will allow you to write custom range rate algorithms.");
}

void LoggingWidget::onPlotRangeVsTime()
{
    updatePlotsForSelectedTrack();
}

void LoggingWidget::onPlotSpeedVsTime()
{
    updatePlotsForSelectedTrack();
}

void LoggingWidget::onPlotRV()
{
    updatePlotsForSelectedTrack();
}

void LoggingWidget::onUpdatePlots()
{
    updatePlotsForSelectedTrack();
}

void LoggingWidget::updatePlotsForSelectedTrack()
{
    QVector<uint32_t> selectedTracks = getSelectedTrackIds();
    
    // Prepare data for plots
    QMap<uint32_t, QVector<QPair<qint64, float>>> rangeData;
    QMap<uint32_t, QVector<QPair<qint64, float>>> speedData;
    QMap<uint32_t, QVector<QPair<float, float>>> rvData;
    
    for (uint32_t trackId : selectedTracks) {
        if (m_trackLogs.contains(trackId)) {
            const TrackLogData& trackLog = m_trackLogs[trackId];
            
            for (const LoggedTrackDataPoint& point : trackLog.dataPoints) {
                rangeData[trackId].append(qMakePair(point.timestamp, point.range));
                speedData[trackId].append(qMakePair(point.timestamp, point.radial_speed));
                rvData[trackId].append(qMakePair(point.radial_speed, point.range));
            }
        }
    }
    
    // Update plots
    if (m_rangeTimePlot) {
        m_rangeTimePlot->setMultiTrackData(rangeData);
    }
    
    if (m_speedTimePlot) {
        m_speedTimePlot->setMultiTrackData(speedData);
    }
    
    if (m_rvPlot) {
        m_rvPlot->setMultiTrackData(rvData);
    }
}

void LoggingWidget::updateDataTable()
{
    if (!m_dataTable) return;
    
    QVector<uint32_t> selectedTracks = getSelectedTrackIds();
    
    m_dataTable->setRowCount(0);
    
    for (uint32_t trackId : selectedTracks) {
        if (m_trackLogs.contains(trackId)) {
            const TrackLogData& trackLog = m_trackLogs[trackId];
            
            for (const LoggedTrackDataPoint& point : trackLog.dataPoints) {
                int row = m_dataTable->rowCount();
                m_dataTable->insertRow(row);
                
                m_dataTable->setItem(row, 0, new QTableWidgetItem(
                    QDateTime::fromMSecsSinceEpoch(point.timestamp).toString("yyyy-MM-dd hh:mm:ss.zzz")));
                m_dataTable->setItem(row, 1, new QTableWidgetItem(QString::number(point.target_id)));
                m_dataTable->setItem(row, 2, new QTableWidgetItem(QString::number(point.range, 'f', 2)));
                m_dataTable->setItem(row, 3, new QTableWidgetItem(QString::number(point.radial_speed, 'f', 2)));
                m_dataTable->setItem(row, 4, new QTableWidgetItem(QString::number(point.azimuth, 'f', 2)));
                m_dataTable->setItem(row, 5, new QTableWidgetItem(QString::number(point.level, 'f', 2)));
                m_dataTable->setItem(row, 6, new QTableWidgetItem(QString::number(point.computed_range_rate, 'f', 2)));
                
                qint64 duration = (point.timestamp - trackLog.firstSeen) / 1000;
                m_dataTable->setItem(row, 7, new QTableWidgetItem(QString::number(duration)));
            }
        }
    }
    
    m_dataTable->resizeColumnsToContents();
}

QVector<uint32_t> LoggingWidget::getSelectedTrackIds() const
{
    QVector<uint32_t> selectedTracks;
    
    if (m_showAllTracksCheckBox && m_showAllTracksCheckBox->isChecked()) {
        return m_activeTrackIds;
    }
    
    if (m_trackFilterCombo) {
        int trackId = m_trackFilterCombo->currentData().toInt();
        if (trackId >= 0) {
            selectedTracks.append(static_cast<uint32_t>(trackId));
        } else {
            selectedTracks = m_activeTrackIds;
        }
    }
    
    return selectedTracks;
}

void LoggingWidget::onRefreshPlotsTimer()
{
    if (m_autoUpdatePlotsCheckBox && m_autoUpdatePlotsCheckBox->isChecked()) {
        updatePlotsForSelectedTrack();
    }
}

void LoggingWidget::onDetachWindow()
{
    // Create a new detached window
    QDialog* detachedWindow = new QDialog(nullptr);
    detachedWindow->setWindowTitle("Logging Tab - Detached");
    detachedWindow->resize(1200, 800);
    detachedWindow->setAttribute(Qt::WA_DeleteOnClose);
    
    // Clone the widget
    LoggingWidget* clonedWidget = new LoggingWidget(detachedWindow);
    clonedWidget->m_trackLogs = this->m_trackLogs;
    clonedWidget->m_activeTrackIds = this->m_activeTrackIds;
    clonedWidget->m_totalDataPoints = this->m_totalDataPoints;
    clonedWidget->setDarkTheme(this->m_isDarkTheme);
    clonedWidget->onRefreshTrackList();
    clonedWidget->updatePlotsForSelectedTrack();
    
    QVBoxLayout* layout = new QVBoxLayout(detachedWindow);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(clonedWidget);
    
    detachedWindow->show();
    
    QMessageBox::information(this, "Window Detached", 
                             "Logging tab has been detached to a separate window.");
}

void LoggingWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    
    if (m_rangeTimePlot) m_rangeTimePlot->setDarkTheme(isDark);
    if (m_speedTimePlot) m_speedTimePlot->setDarkTheme(isDark);
    if (m_rvPlot) m_rvPlot->setDarkTheme(isDark);
    
    applyTheme();
}

void LoggingWidget::applyTheme()
{
    // Theme will be applied through MainWindow's global stylesheet
    update();
}

void LoggingWidget::startLogging()
{
    onStartLogging();
}

void LoggingWidget::stopLogging()
{
    onStopLogging();
}

void LoggingWidget::clearLogs()
{
    onClearLogs();
}
