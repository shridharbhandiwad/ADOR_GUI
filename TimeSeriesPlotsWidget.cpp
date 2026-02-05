#include "TimeSeriesPlotsWidget.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainterPath>
#include <QFontMetrics>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFrame>
#include <QSettings>
#include <cmath>

// ==================== TimeSeriesPlotWidget Implementation ====================

TimeSeriesPlotWidget::TimeSeriesPlotWidget(QWidget *parent)
    : QWidget(parent)
    , m_yAxisLabel("Value")
    , m_yAxisUnit("")
    , m_minY(0.0f)
    , m_maxY(100.0f)
    , m_defaultMinY(0.0f)
    , m_defaultMaxY(100.0f)
    , m_timeWindowSeconds(60)
    , m_pointSize(4)
    , m_marginLeft(70)
    , m_marginRight(20)
    , m_marginTop(20)
    , m_marginBottom(50)
    , m_isDarkTheme(false)
    , m_isPanning(false)
    , m_zoomFactorY(1.0f)
    , m_panOffsetY(0.0f)
    , m_hoveredPointIndex(-1)
    , m_showTooltip(false)
{
    // No hard-coded minimum size to allow responsive layout
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);  // Enable mouse tracking for hover effects
}

void TimeSeriesPlotWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void TimeSeriesPlotWidget::setYAxisLabel(const QString& label)
{
    m_yAxisLabel = label;
    update();
}

void TimeSeriesPlotWidget::setYAxisUnit(const QString& unit)
{
    m_yAxisUnit = unit;
    update();
}

void TimeSeriesPlotWidget::setYAxisRange(float minY, float maxY)
{
    m_minY = minY;
    m_maxY = maxY;
    m_defaultMinY = minY;
    m_defaultMaxY = maxY;
    update();
}

void TimeSeriesPlotWidget::setPointSize(int size)
{
    m_pointSize = qMax(1, size);
    update();
}

void TimeSeriesPlotWidget::setTimeWindowSeconds(int seconds)
{
    m_timeWindowSeconds = seconds;
    update();
}

void TimeSeriesPlotWidget::clearData()
{
    m_dataPoints.clear();
    update();
}

void TimeSeriesPlotWidget::addDataPoint(qint64 timestamp, float value)
{
    m_dataPoints.append(qMakePair(timestamp, value));
    
    // Remove old data points beyond the time window
    qint64 cutoffTime = QDateTime::currentMSecsSinceEpoch() - (m_timeWindowSeconds * 1000);
    while (!m_dataPoints.isEmpty() && m_dataPoints.first().first < cutoffTime) {
        m_dataPoints.removeFirst();
    }
    
    // Limit total data points
    while (m_dataPoints.size() > MAX_DATA_POINTS) {
        m_dataPoints.removeFirst();
    }
    
    update();
}

void TimeSeriesPlotWidget::onResetZoom()
{
    // Reset zoom and pan to default view
    m_zoomFactorY = 1.0f;
    m_panOffsetY = 0.0f;
    m_minY = m_defaultMinY;
    m_maxY = m_defaultMaxY;
    update();
}

void TimeSeriesPlotWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_plotRect = QRect(m_marginLeft, m_marginTop, 
                       width() - m_marginLeft - m_marginRight,
                       height() - m_marginTop - m_marginBottom);
}

void TimeSeriesPlotWidget::paintEvent(QPaintEvent *event)
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
    drawTooltip(painter);
}

QColor TimeSeriesPlotWidget::getBackgroundColor() const
{
    return m_isDarkTheme ? QColor(30, 41, 59) : QColor(248, 250, 252);
}

QColor TimeSeriesPlotWidget::getPlotBackgroundColor() const
{
    return m_isDarkTheme ? QColor(15, 23, 42) : QColor(255, 255, 255);
}

QColor TimeSeriesPlotWidget::getBorderColor() const
{
    return m_isDarkTheme ? QColor(51, 65, 85) : QColor(226, 232, 240);
}

QColor TimeSeriesPlotWidget::getGridColor() const
{
    return m_isDarkTheme ? QColor(71, 85, 105, 100) : QColor(226, 232, 240, 150);
}

QColor TimeSeriesPlotWidget::getTextColor() const
{
    return m_isDarkTheme ? QColor(226, 232, 240) : QColor(30, 41, 59);
}

QColor TimeSeriesPlotWidget::getDataLineColor() const
{
    return m_isDarkTheme ? QColor(59, 130, 246) : QColor(37, 99, 235);  // Blue
}

QColor TimeSeriesPlotWidget::getDataFillColor() const
{
    return m_isDarkTheme ? QColor(59, 130, 246, 50) : QColor(37, 99, 235, 30);
}

void TimeSeriesPlotWidget::drawBackground(QPainter& painter)
{
    painter.fillRect(rect(), getBackgroundColor());
    
    // Draw plot area background
    painter.fillRect(m_plotRect, getPlotBackgroundColor());
    
    // Draw border
    painter.setPen(QPen(getBorderColor(), 1));
    painter.drawRect(m_plotRect);
}

void TimeSeriesPlotWidget::drawGrid(QPainter& painter)
{
    painter.setPen(QPen(getGridColor(), 1, Qt::DashLine));
    
    // Vertical grid lines (time)
    int numVerticalLines = 5;
    for (int i = 1; i < numVerticalLines; ++i) {
        int x = m_plotRect.left() + (m_plotRect.width() * i / numVerticalLines);
        painter.drawLine(x, m_plotRect.top(), x, m_plotRect.bottom());
    }
    
    // Horizontal grid lines (value)
    int numHorizontalLines = 5;
    for (int i = 1; i < numHorizontalLines; ++i) {
        int y = m_plotRect.top() + (m_plotRect.height() * i / numHorizontalLines);
        painter.drawLine(m_plotRect.left(), y, m_plotRect.right(), y);
    }
}

void TimeSeriesPlotWidget::drawAxes(QPainter& painter)
{
    painter.setPen(QPen(getTextColor(), 2));
    
    // X axis
    painter.drawLine(m_plotRect.bottomLeft(), m_plotRect.bottomRight());
    
    // Y axis
    painter.drawLine(m_plotRect.topLeft(), m_plotRect.bottomLeft());
}

void TimeSeriesPlotWidget::drawData(QPainter& painter)
{
    if (m_dataPoints.isEmpty()) return;
    
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 startTime = currentTime - (m_timeWindowSeconds * 1000);
    
    // Draw as scatter plot (point cloud) with filled circles
    painter.setPen(Qt::NoPen);
    painter.setBrush(getDataLineColor());
    
    for (int i = 0; i < m_dataPoints.size(); ++i) {
        const auto& point = m_dataPoints[i];
        
        // Calculate x position based on time
        float timeRatio = float(point.first - startTime) / float(m_timeWindowSeconds * 1000);
        if (timeRatio < 0 || timeRatio > 1) continue;
        
        int x = m_plotRect.left() + int(timeRatio * m_plotRect.width());
        
        // Calculate y position based on value
        float valueRatio = (point.second - m_minY) / (m_maxY - m_minY);
        valueRatio = qBound(0.0f, valueRatio, 1.0f);
        int y = m_plotRect.bottom() - int(valueRatio * m_plotRect.height());
        
        // Highlight hovered point
        if (i == m_hoveredPointIndex) {
            painter.setBrush(QColor(255, 200, 0));  // Yellow highlight
            painter.drawEllipse(QPoint(x, y), m_pointSize + 2, m_pointSize + 2);
            painter.setBrush(getDataLineColor());
        } else {
            // Draw filled circle
            painter.drawEllipse(QPoint(x, y), m_pointSize, m_pointSize);
        }
    }
}

void TimeSeriesPlotWidget::drawLabels(QPainter& painter)
{
    QFont labelFont("Segoe UI", 10);
    QFont axisFont("Segoe UI", 9);
    painter.setFont(labelFont);
    painter.setPen(getTextColor());
    
    // Y-axis label (rotated)
    painter.save();
    painter.translate(15, m_plotRect.center().y());
    painter.rotate(-90);
    QString yLabel = m_yAxisUnit.isEmpty() ? m_yAxisLabel : QString("%1 [%2]").arg(m_yAxisLabel, m_yAxisUnit);
    QFontMetrics fm(labelFont);
    painter.drawText(-fm.horizontalAdvance(yLabel) / 2, 0, yLabel);
    painter.restore();
    
    // X-axis label
    painter.drawText(m_plotRect.center().x() - 20, height() - 10, "Time");
    
    // Y-axis tick labels
    painter.setFont(axisFont);
    int numYTicks = 5;
    for (int i = 0; i <= numYTicks; ++i) {
        float value = m_minY + (m_maxY - m_minY) * i / numYTicks;
        int y = m_plotRect.bottom() - (m_plotRect.height() * i / numYTicks);
        QString label = QString::number(int(value));
        QFontMetrics axisfm(axisFont);
        painter.drawText(m_marginLeft - axisfm.horizontalAdvance(label) - 5, y + 4, label);
    }
    
    // X-axis tick labels (time)
    if (!m_dataPoints.isEmpty()) {
        qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
        int numXTicks = 4;
        for (int i = 0; i <= numXTicks; ++i) {
            qint64 tickTime = currentTime - m_timeWindowSeconds * 1000 + 
                              (m_timeWindowSeconds * 1000 * i / numXTicks);
            QDateTime dt = QDateTime::fromMSecsSinceEpoch(tickTime);
            QString timeStr = dt.toString("hh:mm:ss");
            int x = m_plotRect.left() + (m_plotRect.width() * i / numXTicks);
            QFontMetrics axisfm(axisFont);
            painter.drawText(x - axisfm.horizontalAdvance(timeStr) / 2, 
                           m_plotRect.bottom() + 20, timeStr);
        }
    }
}

void TimeSeriesPlotWidget::drawTooltip(QPainter& painter)
{
    if (!m_showTooltip || m_hoveredPointIndex < 0 || m_hoveredPointIndex >= m_dataPoints.size()) {
        return;
    }
    
    const auto& point = m_dataPoints[m_hoveredPointIndex];
    QDateTime dt = QDateTime::fromMSecsSinceEpoch(point.first);
    QString timeStr = dt.toString("hh:mm:ss.zzz");
    QString valueStr = QString::number(point.second, 'f', 2);
    QString tooltipText = QString("%1: %2 %3\nTime: %4")
        .arg(m_yAxisLabel)
        .arg(valueStr)
        .arg(m_yAxisUnit)
        .arg(timeStr);
    
    // Draw tooltip box
    QFont font("Segoe UI", 9);
    painter.setFont(font);
    QFontMetrics fm(font);
    
    QStringList lines = tooltipText.split('\n');
    int maxWidth = 0;
    for (const QString& line : lines) {
        maxWidth = qMax(maxWidth, fm.horizontalAdvance(line));
    }
    
    int boxWidth = maxWidth + 20;
    int boxHeight = lines.size() * fm.height() + 10;
    
    // Position tooltip near mouse, but keep it inside the widget
    int tooltipX = m_mousePos.x() + 15;
    int tooltipY = m_mousePos.y() - boxHeight - 5;
    
    if (tooltipX + boxWidth > width()) {
        tooltipX = m_mousePos.x() - boxWidth - 15;
    }
    if (tooltipY < 0) {
        tooltipY = m_mousePos.y() + 15;
    }
    
    QRect tooltipRect(tooltipX, tooltipY, boxWidth, boxHeight);
    
    // Draw tooltip background
    painter.setPen(QPen(getBorderColor(), 1));
    painter.setBrush(m_isDarkTheme ? QColor(30, 41, 59, 240) : QColor(255, 255, 255, 240));
    painter.drawRoundedRect(tooltipRect, 4, 4);
    
    // Draw tooltip text
    painter.setPen(getTextColor());
    int textY = tooltipRect.top() + fm.height();
    for (const QString& line : lines) {
        painter.drawText(tooltipRect.left() + 10, textY, line);
        textY += fm.height();
    }
}

QPointF TimeSeriesPlotWidget::dataToScreen(qint64 timestamp, float value) const
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 startTime = currentTime - (m_timeWindowSeconds * 1000);
    
    float timeRatio = float(timestamp - startTime) / float(m_timeWindowSeconds * 1000);
    float x = m_plotRect.left() + timeRatio * m_plotRect.width();
    
    float valueRatio = (value - m_minY) / (m_maxY - m_minY);
    float y = m_plotRect.bottom() - valueRatio * m_plotRect.height();
    
    return QPointF(x, y);
}

bool TimeSeriesPlotWidget::screenToData(const QPoint& screenPos, qint64& timestamp, float& value) const
{
    if (!m_plotRect.contains(screenPos)) {
        return false;
    }
    
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 startTime = currentTime - (m_timeWindowSeconds * 1000);
    
    float timeRatio = float(screenPos.x() - m_plotRect.left()) / m_plotRect.width();
    timestamp = startTime + qint64(timeRatio * m_timeWindowSeconds * 1000);
    
    float valueRatio = float(m_plotRect.bottom() - screenPos.y()) / m_plotRect.height();
    value = m_minY + valueRatio * (m_maxY - m_minY);
    
    return true;
}

int TimeSeriesPlotWidget::findNearestPoint(const QPoint& pos) const
{
    if (m_dataPoints.isEmpty() || !m_plotRect.contains(pos)) {
        return -1;
    }
    
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 startTime = currentTime - (m_timeWindowSeconds * 1000);
    
    int nearestIndex = -1;
    float minDistance = 20.0f;  // Maximum distance to consider
    
    for (int i = 0; i < m_dataPoints.size(); ++i) {
        const auto& point = m_dataPoints[i];
        QPointF screenPos = dataToScreen(point.first, point.second);
        
        float dx = screenPos.x() - pos.x();
        float dy = screenPos.y() - pos.y();
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance < minDistance) {
            minDistance = distance;
            nearestIndex = i;
        }
    }
    
    return nearestIndex;
}

void TimeSeriesPlotWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    
    if (m_isPanning) {
        // Handle panning
        int dy = event->pos().y() - m_lastMousePos.y();
        float valueRange = m_maxY - m_minY;
        float panAmount = -dy * valueRange / m_plotRect.height();
        
        m_minY += panAmount;
        m_maxY += panAmount;
        
        m_lastMousePos = event->pos();
        update();
    } else {
        // Handle hover
        int newHoveredIndex = findNearestPoint(event->pos());
        if (newHoveredIndex != m_hoveredPointIndex) {
            m_hoveredPointIndex = newHoveredIndex;
            m_showTooltip = (m_hoveredPointIndex >= 0);
            update();
        }
    }
    
    QWidget::mouseMoveEvent(event);
}

void TimeSeriesPlotWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_plotRect.contains(event->pos())) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    
    QWidget::mousePressEvent(event);
}

void TimeSeriesPlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
    
    QWidget::mouseReleaseEvent(event);
}

void TimeSeriesPlotWidget::wheelEvent(QWheelEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPoint eventPos = event->position().toPoint();
#else
    QPoint eventPos = event->pos();
#endif
    
    if (m_plotRect.contains(eventPos)) {
        // Zoom in/out
        float zoomFactor = 1.0f + (event->angleDelta().y() > 0 ? 0.1f : -0.1f);
        
        float valueRange = m_maxY - m_minY;
        float newRange = valueRange / zoomFactor;
        
        // Get mouse position in value space
        float mouseValueRatio = float(m_plotRect.bottom() - eventPos.y()) / m_plotRect.height();
        float mouseValue = m_minY + mouseValueRatio * valueRange;
        
        // Zoom around mouse position
        m_minY = mouseValue - mouseValueRatio * newRange;
        m_maxY = mouseValue + (1.0f - mouseValueRatio) * newRange;
        
        update();
        event->accept();
    } else {
        QWidget::wheelEvent(event);
    }
}

void TimeSeriesPlotWidget::leaveEvent(QEvent *event)
{
    m_hoveredPointIndex = -1;
    m_showTooltip = false;
    update();
    QWidget::leaveEvent(event);
}

// ==================== RangeVelocityPlotWidget Implementation ====================

RangeVelocityPlotWidget::RangeVelocityPlotWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxRange(100.0f)
    , m_maxVelocity(100.0f)
    , m_defaultMaxRange(100.0f)
    , m_defaultMaxVelocity(100.0f)
    , m_showHistogram(true)
    , m_pointSize(4)
    , m_marginLeft(70)
    , m_marginRight(20)
    , m_marginTop(20)
    , m_marginBottom(50)
    , m_isDarkTheme(false)
    , m_isPanning(false)
    , m_zoomFactorX(1.0f)
    , m_zoomFactorY(1.0f)
    , m_panOffsetX(0.0f)
    , m_panOffsetY(0.0f)
    , m_hoveredPointIndex(-1)
    , m_showTooltip(false)
{
    // No hard-coded minimum size to allow responsive layout
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);  // Enable mouse tracking for hover effects
    
    // Initialize histogram bins
    m_velocityHistogram.resize(HISTOGRAM_BINS);
    m_velocityHistogram.fill(0);
    m_rangeHistogram.resize(HISTOGRAM_BINS);
    m_rangeHistogram.fill(0);
}

void RangeVelocityPlotWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void RangeVelocityPlotWidget::setRangeLimit(float maxRange)
{
    m_maxRange = maxRange;
    m_defaultMaxRange = maxRange;
    update();
}

void RangeVelocityPlotWidget::setVelocityLimit(float maxVelocity)
{
    m_maxVelocity = maxVelocity;
    m_defaultMaxVelocity = maxVelocity;
    update();
}

void RangeVelocityPlotWidget::setShowHistogram(bool show)
{
    m_showHistogram = show;
    update();
}

void RangeVelocityPlotWidget::setPointSize(int size)
{
    m_pointSize = qMax(1, size);
    update();
}

void RangeVelocityPlotWidget::onResetZoom()
{
    m_zoomFactorX = 1.0f;
    m_zoomFactorY = 1.0f;
    m_panOffsetX = 0.0f;
    m_panOffsetY = 0.0f;
    m_maxRange = m_defaultMaxRange;
    m_maxVelocity = m_defaultMaxVelocity;
    update();
}

void RangeVelocityPlotWidget::clearData()
{
    m_dataPoints.clear();
    m_velocityHistogram.fill(0);
    m_rangeHistogram.fill(0);
    update();
}

void RangeVelocityPlotWidget::addDataPoint(float velocity, float range)
{
    m_dataPoints.append(qMakePair(velocity, range));
    
    // Update histograms
    if (m_showHistogram) {
        int velBin = qBound(0, int(velocity / m_maxVelocity * HISTOGRAM_BINS), HISTOGRAM_BINS - 1);
        int rangeBin = qBound(0, int(range / m_maxRange * HISTOGRAM_BINS), HISTOGRAM_BINS - 1);
        m_velocityHistogram[velBin]++;
        m_rangeHistogram[rangeBin]++;
    }
    
    // Limit data points
    while (m_dataPoints.size() > MAX_DATA_POINTS) {
        m_dataPoints.removeFirst();
    }
    
    update();
}

void RangeVelocityPlotWidget::updateFromTargets(const TargetTrackData& targets)
{
    for (size_t i = 0; i < targets.numTracks && i < targets.targets.size(); ++i) {
        const auto& target = targets.targets[i];
        float velocityKmh = qAbs(target.radial_speed) * 3.6f;  // Convert m/s to km/h
        float rangeM = target.radius;
        addDataPoint(velocityKmh, rangeM);
    }
}

void RangeVelocityPlotWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_plotRect = QRect(m_marginLeft, m_marginTop, 
                       width() - m_marginLeft - m_marginRight,
                       height() - m_marginTop - m_marginBottom);
}

void RangeVelocityPlotWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    
    drawBackground(painter);
    drawValidArea(painter);
    drawGrid(painter);
    drawAxes(painter);
    drawDataPoints(painter);
    if (m_showHistogram) {
        drawHistogram(painter);
    }
    drawLabels(painter);
    drawTooltip(painter);
}

QColor RangeVelocityPlotWidget::getBackgroundColor() const
{
    return m_isDarkTheme ? QColor(30, 41, 59) : QColor(248, 250, 252);
}

QColor RangeVelocityPlotWidget::getPlotBackgroundColor() const
{
    return m_isDarkTheme ? QColor(15, 23, 42) : QColor(255, 255, 255);
}

QColor RangeVelocityPlotWidget::getBorderColor() const
{
    return m_isDarkTheme ? QColor(51, 65, 85) : QColor(226, 232, 240);
}

QColor RangeVelocityPlotWidget::getGridColor() const
{
    return m_isDarkTheme ? QColor(71, 85, 105, 100) : QColor(226, 232, 240, 150);
}

QColor RangeVelocityPlotWidget::getTextColor() const
{
    return m_isDarkTheme ? QColor(226, 232, 240) : QColor(30, 41, 59);
}

QColor RangeVelocityPlotWidget::getValidAreaColor() const
{
    return m_isDarkTheme ? QColor(34, 197, 94, 60) : QColor(134, 239, 172, 100);  // Green with transparency
}

QColor RangeVelocityPlotWidget::getDataPointColor() const
{
    return m_isDarkTheme ? QColor(239, 68, 68) : QColor(220, 38, 38);  // Red
}

void RangeVelocityPlotWidget::drawBackground(QPainter& painter)
{
    painter.fillRect(rect(), getBackgroundColor());
    
    // Draw plot area background
    painter.fillRect(m_plotRect, getPlotBackgroundColor());
    
    // Draw border
    painter.setPen(QPen(getBorderColor(), 1));
    painter.drawRect(m_plotRect);
}

void RangeVelocityPlotWidget::drawValidArea(QPainter& painter)
{
    // Draw the valid area (green shaded region) as shown in the image
    // This represents the valid range-velocity space
    painter.fillRect(m_plotRect, getValidAreaColor());
}

void RangeVelocityPlotWidget::drawGrid(QPainter& painter)
{
    painter.setPen(QPen(getGridColor(), 1, Qt::DashLine));
    
    // Vertical grid lines (velocity)
    int numVerticalLines = 6;
    for (int i = 1; i < numVerticalLines; ++i) {
        int x = m_plotRect.left() + (m_plotRect.width() * i / numVerticalLines);
        painter.drawLine(x, m_plotRect.top(), x, m_plotRect.bottom());
    }
    
    // Horizontal grid lines (range)
    int numHorizontalLines = 6;
    for (int i = 1; i < numHorizontalLines; ++i) {
        int y = m_plotRect.top() + (m_plotRect.height() * i / numHorizontalLines);
        painter.drawLine(m_plotRect.left(), y, m_plotRect.right(), y);
    }
}

void RangeVelocityPlotWidget::drawAxes(QPainter& painter)
{
    painter.setPen(QPen(getTextColor(), 2));
    
    // X axis
    painter.drawLine(m_plotRect.bottomLeft(), m_plotRect.bottomRight());
    
    // Y axis
    painter.drawLine(m_plotRect.topLeft(), m_plotRect.bottomLeft());
}

void RangeVelocityPlotWidget::drawDataPoints(QPainter& painter)
{
    if (m_dataPoints.isEmpty()) return;
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(getDataPointColor());
    
    for (int i = 0; i < m_dataPoints.size(); ++i) {
        const auto& point = m_dataPoints[i];
        float velocity = point.first;
        float range = point.second;
        
        // Calculate position
        float xRatio = velocity / m_maxVelocity;
        float yRatio = range / m_maxRange;
        
        if (xRatio < 0 || xRatio > 1 || yRatio < 0 || yRatio > 1) continue;
        
        int x = m_plotRect.left() + int(xRatio * m_plotRect.width());
        int y = m_plotRect.bottom() - int(yRatio * m_plotRect.height());
        
        // Highlight hovered point
        if (i == m_hoveredPointIndex) {
            painter.setBrush(QColor(255, 200, 0));  // Yellow highlight
            painter.drawEllipse(QPoint(x, y), m_pointSize + 2, m_pointSize + 2);
            painter.setBrush(getDataPointColor());
        } else {
            // Draw filled circle with configurable size
            painter.drawEllipse(QPoint(x, y), m_pointSize, m_pointSize);
        }
    }
}

void RangeVelocityPlotWidget::drawHistogram(QPainter& painter)
{
    // Draw velocity histogram (along X axis at the bottom)
    int maxVelCount = *std::max_element(m_velocityHistogram.begin(), m_velocityHistogram.end());
    if (maxVelCount > 0) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(59, 130, 246, 100));  // Blue with transparency
        
        float binWidth = float(m_plotRect.width()) / HISTOGRAM_BINS;
        for (int i = 0; i < HISTOGRAM_BINS; ++i) {
            float height = float(m_velocityHistogram[i]) / maxVelCount * 30;  // Max 30 pixels
            QRectF barRect(m_plotRect.left() + i * binWidth, 
                          m_plotRect.bottom() + 25,
                          binWidth - 1, height);
            painter.drawRect(barRect);
        }
    }
    
    // Draw range histogram (along Y axis on the right)
    int maxRangeCount = *std::max_element(m_rangeHistogram.begin(), m_rangeHistogram.end());
    if (maxRangeCount > 0) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(59, 130, 246, 100));
        
        float binHeight = float(m_plotRect.height()) / HISTOGRAM_BINS;
        for (int i = 0; i < HISTOGRAM_BINS; ++i) {
            float width = float(m_rangeHistogram[i]) / maxRangeCount * 30;
            QRectF barRect(m_plotRect.right() + 5,
                          m_plotRect.bottom() - (i + 1) * binHeight,
                          width, binHeight - 1);
            painter.drawRect(barRect);
        }
    }
}

void RangeVelocityPlotWidget::drawLabels(QPainter& painter)
{
    QFont labelFont("Segoe UI", 10);
    QFont axisFont("Segoe UI", 9);
    painter.setFont(labelFont);
    painter.setPen(getTextColor());
    
    // Y-axis label (rotated)
    painter.save();
    painter.translate(15, m_plotRect.center().y());
    painter.rotate(-90);
    QString yLabel = "Range [m]";
    QFontMetrics fm(labelFont);
    painter.drawText(-fm.horizontalAdvance(yLabel) / 2, 0, yLabel);
    painter.restore();
    
    // X-axis label
    QString xLabel = "Velocity [km/h]";
    painter.drawText(m_plotRect.center().x() - fm.horizontalAdvance(xLabel) / 2, 
                    height() - 10, xLabel);
    
    // Y-axis tick labels
    painter.setFont(axisFont);
    int numYTicks = 6;
    for (int i = 0; i <= numYTicks; ++i) {
        float value = m_maxRange * i / numYTicks;
        int y = m_plotRect.bottom() - (m_plotRect.height() * i / numYTicks);
        QString label = QString::number(int(value));
        QFontMetrics axisfm(axisFont);
        painter.drawText(m_marginLeft - axisfm.horizontalAdvance(label) - 5, y + 4, label);
    }
    
    // X-axis tick labels
    int numXTicks = 6;
    for (int i = 0; i <= numXTicks; ++i) {
        float value = m_maxVelocity * i / numXTicks;
        int x = m_plotRect.left() + (m_plotRect.width() * i / numXTicks);
        QString label = QString::number(int(value));
        QFontMetrics axisfm(axisFont);
        painter.drawText(x - axisfm.horizontalAdvance(label) / 2, 
                        m_plotRect.bottom() + 15, label);
    }
}

void RangeVelocityPlotWidget::drawTooltip(QPainter& painter)
{
    if (!m_showTooltip || m_hoveredPointIndex < 0 || m_hoveredPointIndex >= m_dataPoints.size()) {
        return;
    }
    
    const auto& point = m_dataPoints[m_hoveredPointIndex];
    QString velocityStr = QString::number(point.first, 'f', 2);
    QString rangeStr = QString::number(point.second, 'f', 2);
    QString tooltipText = QString("Velocity: %1 km/h\nRange: %2 m")
        .arg(velocityStr)
        .arg(rangeStr);
    
    // Draw tooltip box
    QFont font("Segoe UI", 9);
    painter.setFont(font);
    QFontMetrics fm(font);
    
    QStringList lines = tooltipText.split('\n');
    int maxWidth = 0;
    for (const QString& line : lines) {
        maxWidth = qMax(maxWidth, fm.horizontalAdvance(line));
    }
    
    int boxWidth = maxWidth + 20;
    int boxHeight = lines.size() * fm.height() + 10;
    
    // Position tooltip near mouse, but keep it inside the widget
    int tooltipX = m_mousePos.x() + 15;
    int tooltipY = m_mousePos.y() - boxHeight - 5;
    
    if (tooltipX + boxWidth > width()) {
        tooltipX = m_mousePos.x() - boxWidth - 15;
    }
    if (tooltipY < 0) {
        tooltipY = m_mousePos.y() + 15;
    }
    
    QRect tooltipRect(tooltipX, tooltipY, boxWidth, boxHeight);
    
    // Draw tooltip background
    painter.setPen(QPen(getBorderColor(), 1));
    painter.setBrush(m_isDarkTheme ? QColor(30, 41, 59, 240) : QColor(255, 255, 255, 240));
    painter.drawRoundedRect(tooltipRect, 4, 4);
    
    // Draw tooltip text
    painter.setPen(getTextColor());
    int textY = tooltipRect.top() + fm.height();
    for (const QString& line : lines) {
        painter.drawText(tooltipRect.left() + 10, textY, line);
        textY += fm.height();
    }
}

int RangeVelocityPlotWidget::findNearestPoint(const QPoint& pos) const
{
    if (m_dataPoints.isEmpty() || !m_plotRect.contains(pos)) {
        return -1;
    }
    
    int nearestIndex = -1;
    float minDistance = 20.0f;  // Maximum distance to consider
    
    for (int i = 0; i < m_dataPoints.size(); ++i) {
        const auto& point = m_dataPoints[i];
        float velocity = point.first;
        float range = point.second;
        
        float xRatio = velocity / m_maxVelocity;
        float yRatio = range / m_maxRange;
        
        if (xRatio < 0 || xRatio > 1 || yRatio < 0 || yRatio > 1) continue;
        
        int x = m_plotRect.left() + int(xRatio * m_plotRect.width());
        int y = m_plotRect.bottom() - int(yRatio * m_plotRect.height());
        
        float dx = x - pos.x();
        float dy = y - pos.y();
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance < minDistance) {
            minDistance = distance;
            nearestIndex = i;
        }
    }
    
    return nearestIndex;
}

void RangeVelocityPlotWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    
    if (m_isPanning) {
        // Handle panning
        int dx = event->pos().x() - m_lastMousePos.x();
        int dy = event->pos().y() - m_lastMousePos.y();
        
        float velocityRange = m_maxVelocity;
        float rangeRange = m_maxRange;
        
        float panAmountX = -dx * velocityRange / m_plotRect.width();
        float panAmountY = dy * rangeRange / m_plotRect.height();
        
        m_panOffsetX += panAmountX;
        m_panOffsetY += panAmountY;
        
        m_lastMousePos = event->pos();
        update();
    } else {
        // Handle hover
        int newHoveredIndex = findNearestPoint(event->pos());
        if (newHoveredIndex != m_hoveredPointIndex) {
            m_hoveredPointIndex = newHoveredIndex;
            m_showTooltip = (m_hoveredPointIndex >= 0);
            update();
        }
    }
    
    QWidget::mouseMoveEvent(event);
}

void RangeVelocityPlotWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_plotRect.contains(event->pos())) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    
    QWidget::mousePressEvent(event);
}

void RangeVelocityPlotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
    
    QWidget::mouseReleaseEvent(event);
}

void RangeVelocityPlotWidget::wheelEvent(QWheelEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPoint eventPos = event->position().toPoint();
#else
    QPoint eventPos = event->pos();
#endif
    
    if (m_plotRect.contains(eventPos)) {
        // Zoom in/out
        float zoomFactor = 1.0f + (event->angleDelta().y() > 0 ? 0.1f : -0.1f);
        
        m_maxVelocity /= zoomFactor;
        m_maxRange /= zoomFactor;
        
        update();
        event->accept();
    } else {
        QWidget::wheelEvent(event);
    }
}

void RangeVelocityPlotWidget::leaveEvent(QEvent *event)
{
    m_hoveredPointIndex = -1;
    m_showTooltip = false;
    update();
    QWidget::leaveEvent(event);
}

// ==================== TimeSeriesPlotsWidget Implementation ====================

TimeSeriesPlotsWidget::TimeSeriesPlotsWidget(QWidget *parent)
    : QWidget(parent)
    , m_rangeVelocityPlot(nullptr)
    , m_velocityTimePlot(nullptr)
    , m_rangeTimePlot(nullptr)
    , m_settingsPanel(nullptr)
    , m_pointSizeSpinBox(nullptr)
    , m_velocityMinSpinBox(nullptr)
    , m_velocityMaxSpinBox(nullptr)
    , m_rangeMinSpinBox(nullptr)
    , m_rangeMaxSpinBox(nullptr)
    , m_timeWindowSpinBox(nullptr)
    , m_rvRangeMaxSpinBox(nullptr)
    , m_rvVelocityMaxSpinBox(nullptr)
    , m_isDarkTheme(false)
    , m_maxRange(100.0f)
    , m_maxVelocity(100.0f)
{
    setupUI();
    loadSettings();  // Load saved settings on startup
}

TimeSeriesPlotsWidget::~TimeSeriesPlotsWidget()
{
}

void TimeSeriesPlotsWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // Main content area with horizontal split
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);
    
    // Left side - Range vs Velocity plot
    QVBoxLayout* leftColumn = new QVBoxLayout();
    
    QGroupBox* leftGroup = new QGroupBox(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftGroup);
    leftLayout->setContentsMargins(5, 5, 5, 5);
    
    m_rangeVelocityPlot = new RangeVelocityPlotWidget(this);
    m_rangeVelocityPlot->setRangeLimit(m_maxRange);
    m_rangeVelocityPlot->setVelocityLimit(m_maxVelocity);
    leftLayout->addWidget(m_rangeVelocityPlot);
    
    leftColumn->addWidget(leftGroup);
    
    contentLayout->addLayout(leftColumn, 1);
    
    // Right side - Time series plots (stacked)
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(10);
    
    // Velocity vs Time plot
    QGroupBox* velocityGroup = new QGroupBox(this);
    QVBoxLayout* velocityGroupLayout = new QVBoxLayout(velocityGroup);
    velocityGroupLayout->setContentsMargins(5, 5, 5, 5);
    
    m_velocityTimePlot = new TimeSeriesPlotWidget(this);
    m_velocityTimePlot->setYAxisLabel("Velocity");
    m_velocityTimePlot->setYAxisUnit("km/h");
    m_velocityTimePlot->setYAxisRange(-40, 40);  // Default range: -40 to +40 kph
    m_velocityTimePlot->setTimeWindowSeconds(60);
    velocityGroupLayout->addWidget(m_velocityTimePlot);
    
    rightLayout->addWidget(velocityGroup, 1);
    
    // Range vs Time plot
    QGroupBox* rangeGroup = new QGroupBox(this);
    QVBoxLayout* rangeGroupLayout = new QVBoxLayout(rangeGroup);
    rangeGroupLayout->setContentsMargins(5, 5, 5, 5);
    
    m_rangeTimePlot = new TimeSeriesPlotWidget(this);
    m_rangeTimePlot->setYAxisLabel("Range");
    m_rangeTimePlot->setYAxisUnit("m");
    m_rangeTimePlot->setYAxisRange(0, 70);  // Default range: 0 to 70m
    m_rangeTimePlot->setTimeWindowSeconds(60);
    rangeGroupLayout->addWidget(m_rangeTimePlot);
    
    rightLayout->addWidget(rangeGroup, 1);
    
    contentLayout->addLayout(rightLayout, 1);
    
    mainLayout->addLayout(contentLayout, 1);
    
    // Settings panel at the bottom
    setupSettingsPanel();
    mainLayout->addWidget(m_settingsPanel);
    
    applyTheme();
}

void TimeSeriesPlotsWidget::setupSettingsPanel()
{
    m_settingsPanel = new QWidget(this);
    m_settingsPanel->setVisible(true);  // Always visible
    
    QVBoxLayout* settingsLayout = new QVBoxLayout(m_settingsPanel);
    settingsLayout->setContentsMargins(0, 5, 0, 0);
    settingsLayout->setSpacing(5);
    
    // Create a grid layout for the controls (2 rows x 3 columns)
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    
    // Row 1: Min Range, Min Velocity, Point Size
    // Min Range
    QLabel* minRangeLabel = new QLabel("Min Range", this);
    m_rangeMinSpinBox = new QDoubleSpinBox(this);
    m_rangeMinSpinBox->setRange(0, 1000);
    m_rangeMinSpinBox->setValue(0);
    m_rangeMinSpinBox->setSuffix(" m");
    m_rangeMinSpinBox->setDecimals(1);
    m_rangeMinSpinBox->setSingleStep(1.0);  // Step by 1 meter
    m_rangeMinSpinBox->setMinimumWidth(120);
    m_rangeMinSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    connect(m_rangeMinSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onRangeMinChanged);
    gridLayout->addWidget(minRangeLabel, 0, 0);
    gridLayout->addWidget(m_rangeMinSpinBox, 1, 0);
    
    // Min Velocity
    QLabel* minVelocityLabel = new QLabel("Min Velocity", this);
    m_velocityMinSpinBox = new QDoubleSpinBox(this);
    m_velocityMinSpinBox->setRange(-500, 500);
    m_velocityMinSpinBox->setValue(-40);
    m_velocityMinSpinBox->setSuffix(" km/h");
    m_velocityMinSpinBox->setDecimals(1);
    m_velocityMinSpinBox->setSingleStep(1.0);  // Step by 1 km/h
    m_velocityMinSpinBox->setMinimumWidth(120);
    m_velocityMinSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    connect(m_velocityMinSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onVelocityMinChanged);
    gridLayout->addWidget(minVelocityLabel, 0, 1);
    gridLayout->addWidget(m_velocityMinSpinBox, 1, 1);
    
    // Point Size
    QLabel* pointSizeLabel = new QLabel("Point Size", this);
    m_pointSizeSpinBox = new QSpinBox(this);
    m_pointSizeSpinBox->setRange(1, 10);
    m_pointSizeSpinBox->setValue(4);
    m_pointSizeSpinBox->setSuffix(" px");
    m_pointSizeSpinBox->setSingleStep(1);  // Step by 1 pixel
    m_pointSizeSpinBox->setMinimumWidth(120);
    m_pointSizeSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    connect(m_pointSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onPointSizeChanged);
    gridLayout->addWidget(pointSizeLabel, 0, 2);
    gridLayout->addWidget(m_pointSizeSpinBox, 1, 2);
    
    // Row 2: Max Range, Max Velocity, Time Window
    // Max Range
    QLabel* maxRangeLabel = new QLabel("Max Range", this);
    m_rangeMaxSpinBox = new QDoubleSpinBox(this);
    m_rangeMaxSpinBox->setRange(0, 1000);
    m_rangeMaxSpinBox->setValue(70);
    m_rangeMaxSpinBox->setSuffix(" m");
    m_rangeMaxSpinBox->setDecimals(1);
    m_rangeMaxSpinBox->setSingleStep(1.0);  // Step by 1 meter
    m_rangeMaxSpinBox->setMinimumWidth(120);
    m_rangeMaxSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    connect(m_rangeMaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onRangeMaxChanged);
    gridLayout->addWidget(maxRangeLabel, 2, 0);
    gridLayout->addWidget(m_rangeMaxSpinBox, 3, 0);
    
    // Max Velocity
    QLabel* maxVelocityLabel = new QLabel("Max Velocity", this);
    m_velocityMaxSpinBox = new QDoubleSpinBox(this);
    m_velocityMaxSpinBox->setRange(-500, 500);
    m_velocityMaxSpinBox->setValue(40);
    m_velocityMaxSpinBox->setSuffix(" km/h");
    m_velocityMaxSpinBox->setDecimals(1);
    m_velocityMaxSpinBox->setSingleStep(1.0);  // Step by 1 km/h
    m_velocityMaxSpinBox->setMinimumWidth(120);
    m_velocityMaxSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    connect(m_velocityMaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onVelocityMaxChanged);
    gridLayout->addWidget(maxVelocityLabel, 2, 1);
    gridLayout->addWidget(m_velocityMaxSpinBox, 3, 1);
    
    // Time Window
    QLabel* timeWindowLabel = new QLabel("Time Window", this);
    m_timeWindowSpinBox = new QSpinBox(this);
    m_timeWindowSpinBox->setRange(5, 300);
    m_timeWindowSpinBox->setValue(60);  // Default: 1 minute
    m_timeWindowSpinBox->setSuffix(" s");
    m_timeWindowSpinBox->setSingleStep(5);  // Step by 5 seconds
    m_timeWindowSpinBox->setMinimumWidth(120);
    m_timeWindowSpinBox->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    connect(m_timeWindowSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onTimeWindowChanged);
    gridLayout->addWidget(timeWindowLabel, 2, 2);
    gridLayout->addWidget(m_timeWindowSpinBox, 3, 2);
    
    // Initialize RV plot spinboxes (not visible in the new layout, but keep for compatibility)
    m_rvRangeMaxSpinBox = new QDoubleSpinBox(this);
    m_rvRangeMaxSpinBox->setRange(10, 1000);
    m_rvRangeMaxSpinBox->setValue(100);
    m_rvRangeMaxSpinBox->setVisible(false);
    connect(m_rvRangeMaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onRVRangeMaxChanged);
    
    m_rvVelocityMaxSpinBox = new QDoubleSpinBox(this);
    m_rvVelocityMaxSpinBox->setRange(10, 1000);
    m_rvVelocityMaxSpinBox->setValue(100);
    m_rvVelocityMaxSpinBox->setVisible(false);
    connect(m_rvVelocityMaxSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TimeSeriesPlotsWidget::onRVVelocityMaxChanged);
    
    // Add stretches to center the controls
    gridLayout->setColumnStretch(3, 1);
    
    settingsLayout->addLayout(gridLayout);
    
    // Add Clear, Save and Load Settings buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    
    QPushButton* clearButton = new QPushButton("Clear All Data", this);
    clearButton->setMinimumWidth(120);
    connect(clearButton, &QPushButton::clicked, this, &TimeSeriesPlotsWidget::clearAllData);
    buttonLayout->addWidget(clearButton);
    
    QPushButton* loadButton = new QPushButton("Load Settings", this);
    loadButton->setMinimumWidth(120);
    connect(loadButton, &QPushButton::clicked, this, &TimeSeriesPlotsWidget::onLoadSettings);
    buttonLayout->addWidget(loadButton);
    
    QPushButton* saveButton = new QPushButton("Save Settings", this);
    saveButton->setMinimumWidth(120);
    connect(saveButton, &QPushButton::clicked, this, &TimeSeriesPlotsWidget::onSaveSettings);
    buttonLayout->addWidget(saveButton);
    
    buttonLayout->addStretch();
    settingsLayout->addLayout(buttonLayout);
}

void TimeSeriesPlotsWidget::updateFromTargets(const TargetTrackData& targets)
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
    for (size_t i = 0; i < targets.numTracks && i < targets.targets.size(); ++i) {
        const auto& target = targets.targets[i];
        // Preserve sign for velocity (can be positive or negative)
        float velocityKmh = target.radial_speed * 3.6f;  // Convert m/s to km/h
        float velocityKmhAbs = qAbs(velocityKmh);  // Absolute value for range-velocity plot
        float rangeM = target.radius;
        
        // Update range-velocity plot (uses absolute velocity)
        if (m_rangeVelocityPlot) {
            m_rangeVelocityPlot->addDataPoint(velocityKmhAbs, rangeM);
        }
        
        // Update time series plots (velocity preserves sign)
        if (m_velocityTimePlot) {
            m_velocityTimePlot->addDataPoint(currentTime, velocityKmh);
        }
        if (m_rangeTimePlot) {
            m_rangeTimePlot->addDataPoint(currentTime, rangeM);
        }
    }
}

void TimeSeriesPlotsWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setDarkTheme(isDark);
    }
    if (m_velocityTimePlot) {
        m_velocityTimePlot->setDarkTheme(isDark);
    }
    if (m_rangeTimePlot) {
        m_rangeTimePlot->setDarkTheme(isDark);
    }
    
    applyTheme();
}

void TimeSeriesPlotsWidget::setMaxRange(float maxRange)
{
    m_maxRange = maxRange;
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setRangeLimit(maxRange);
    }
    // Keep time series plot at fixed range 0-70m
    // if (m_rangeTimePlot) {
    //     m_rangeTimePlot->setYAxisRange(0, maxRange);
    // }
}

void TimeSeriesPlotsWidget::setMaxVelocity(float maxVelocity)
{
    m_maxVelocity = maxVelocity;
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setVelocityLimit(maxVelocity);
    }
    // Keep time series plot at fixed range -40 to +40 kph
    // if (m_velocityTimePlot) {
    //     m_velocityTimePlot->setYAxisRange(0, maxVelocity);
    // }
}

void TimeSeriesPlotsWidget::onShowHistogramToggled(bool checked)
{
    // No longer used - kept for compatibility
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setShowHistogram(checked);
    }
}

void TimeSeriesPlotsWidget::onResetVelocityPlot()
{
    // No longer used - kept for compatibility
    if (m_velocityTimePlot) {
        m_velocityTimePlot->onResetZoom();
    }
}

void TimeSeriesPlotsWidget::onResetRangePlot()
{
    // No longer used - kept for compatibility
    if (m_rangeTimePlot) {
        m_rangeTimePlot->onResetZoom();
    }
}

void TimeSeriesPlotsWidget::onResetRangeVelocityPlot()
{
    // No longer used - kept for compatibility
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->onResetZoom();
    }
}

void TimeSeriesPlotsWidget::onSettingsToggled()
{
    // No longer used - settings panel is always visible in new layout
}

void TimeSeriesPlotsWidget::onPointSizeChanged(int size)
{
    if (m_velocityTimePlot) {
        m_velocityTimePlot->setPointSize(size);
    }
    if (m_rangeTimePlot) {
        m_rangeTimePlot->setPointSize(size);
    }
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setPointSize(size);
    }
}

void TimeSeriesPlotsWidget::onVelocityMinChanged(double value)
{
    if (m_velocityTimePlot && value < m_velocityMaxSpinBox->value()) {
        m_velocityTimePlot->setYAxisRange(value, m_velocityMaxSpinBox->value());
    }
}

void TimeSeriesPlotsWidget::onVelocityMaxChanged(double value)
{
    if (m_velocityTimePlot && value > m_velocityMinSpinBox->value()) {
        m_velocityTimePlot->setYAxisRange(m_velocityMinSpinBox->value(), value);
    }
}

void TimeSeriesPlotsWidget::onRangeMinChanged(double value)
{
    if (m_rangeTimePlot && value < m_rangeMaxSpinBox->value()) {
        m_rangeTimePlot->setYAxisRange(value, m_rangeMaxSpinBox->value());
    }
}

void TimeSeriesPlotsWidget::onRangeMaxChanged(double value)
{
    if (m_rangeTimePlot && value > m_rangeMinSpinBox->value()) {
        m_rangeTimePlot->setYAxisRange(m_rangeMinSpinBox->value(), value);
    }
}

void TimeSeriesPlotsWidget::onTimeWindowChanged(int seconds)
{
    if (m_velocityTimePlot) {
        m_velocityTimePlot->setTimeWindowSeconds(seconds);
    }
    if (m_rangeTimePlot) {
        m_rangeTimePlot->setTimeWindowSeconds(seconds);
    }
}

void TimeSeriesPlotsWidget::onRVRangeMaxChanged(double value)
{
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setRangeLimit(value);
    }
}

void TimeSeriesPlotsWidget::onRVVelocityMaxChanged(double value)
{
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setVelocityLimit(value);
    }
}

void TimeSeriesPlotsWidget::clearAllData()
{
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->clearData();
    }
    if (m_velocityTimePlot) {
        m_velocityTimePlot->clearData();
    }
    if (m_rangeTimePlot) {
        m_rangeTimePlot->clearData();
    }
}

void TimeSeriesPlotsWidget::applyTheme()
{
    QString textColor = m_isDarkTheme ? "#e2e8f0" : "#1e293b";
    QString bgColor = m_isDarkTheme ? "#1e293b" : "#f8fafc";
    QString borderColor = m_isDarkTheme ? "#334155" : "#e2e8f0";
    QString groupBgColor = m_isDarkTheme ? "#0f172a" : "#ffffff";
    QString inputBgColor = m_isDarkTheme ? "#0f172a" : "#ffffff";
    QString accentColor = m_isDarkTheme ? "#3b82f6" : "#2563eb";
    QString buttonBgColor = m_isDarkTheme ? "#3b82f6" : "#2563eb";
    QString buttonTextColor = "#ffffff";
    QString buttonHoverColor = m_isDarkTheme ? "#2563eb" : "#1d4ed8";
    
    // Style the settings panel
    if (m_settingsPanel) {
        QString settingsPanelStyle = QString(
            "QWidget {"
            "  background-color: %1;"
            "  color: %2;"
            "}"
            "QLabel {"
            "  color: %2;"
            "  font-weight: 500;"
            "}"
            "QSpinBox, QDoubleSpinBox {"
            "  background: %3;"
            "  border: 1px solid %4;"
            "  border-radius: 4px;"
            "  padding: 5px;"
            "  color: %2;"
            "}"
            "QSpinBox:focus, QDoubleSpinBox:focus {"
            "  border-color: %5;"
            "  border-width: 2px;"
            "}"
            "QPushButton {"
            "  background-color: %6;"
            "  color: %7;"
            "  border: none;"
            "  border-radius: 6px;"
            "  padding: 8px 16px;"
            "  font-weight: 500;"
            "}"
            "QPushButton:hover {"
            "  background-color: %8;"
            "}"
            "QPushButton:pressed {"
            "  background-color: %5;"
            "}"
        ).arg(bgColor, textColor, inputBgColor, borderColor, accentColor, 
              buttonBgColor, buttonTextColor, buttonHoverColor);
        m_settingsPanel->setStyleSheet(settingsPanelStyle);
    }
    
    // Style the group boxes
    QString groupBoxStyle = QString(
        "QGroupBox {"
        "  background-color: %1;"
        "  border: 1px solid %2;"
        "  border-radius: 8px;"
        "  margin-top: 0px;"
        "  padding: 5px;"
        "}"
    ).arg(groupBgColor, borderColor);
    
    QList<QGroupBox*> groupBoxes = findChildren<QGroupBox*>();
    for (QGroupBox* gb : groupBoxes) {
        gb->setStyleSheet(groupBoxStyle);
    }
    
    // Set widget background
    setStyleSheet(QString("QWidget { background-color: %1; }").arg(bgColor));
    
    update();
}

void TimeSeriesPlotsWidget::saveSettings()
{
    QSettings settings("Zoppler", "RadarVisualization");
    
    settings.beginGroup("TimeSeriesPlots");
    
    // Save all settings values
    settings.setValue("pointSize", m_pointSizeSpinBox->value());
    settings.setValue("velocityMin", m_velocityMinSpinBox->value());
    settings.setValue("velocityMax", m_velocityMaxSpinBox->value());
    settings.setValue("rangeMin", m_rangeMinSpinBox->value());
    settings.setValue("rangeMax", m_rangeMaxSpinBox->value());
    settings.setValue("timeWindow", m_timeWindowSpinBox->value());
    settings.setValue("rvRangeMax", m_rvRangeMaxSpinBox->value());
    settings.setValue("rvVelocityMax", m_rvVelocityMaxSpinBox->value());
    
    settings.endGroup();
}

void TimeSeriesPlotsWidget::loadSettings()
{
    QSettings settings("Zoppler", "RadarVisualization");
    
    settings.beginGroup("TimeSeriesPlots");
    
    // Load saved values, or use defaults if not found
    if (m_pointSizeSpinBox) {
        int pointSize = settings.value("pointSize", 4).toInt();
        m_pointSizeSpinBox->setValue(pointSize);
    }
    
    if (m_velocityMinSpinBox) {
        double velocityMin = settings.value("velocityMin", -40.0).toDouble();
        m_velocityMinSpinBox->setValue(velocityMin);
    }
    
    if (m_velocityMaxSpinBox) {
        double velocityMax = settings.value("velocityMax", 40.0).toDouble();
        m_velocityMaxSpinBox->setValue(velocityMax);
    }
    
    if (m_rangeMinSpinBox) {
        double rangeMin = settings.value("rangeMin", 0.0).toDouble();
        m_rangeMinSpinBox->setValue(rangeMin);
    }
    
    if (m_rangeMaxSpinBox) {
        double rangeMax = settings.value("rangeMax", 70.0).toDouble();
        m_rangeMaxSpinBox->setValue(rangeMax);
    }
    
    if (m_timeWindowSpinBox) {
        int timeWindow = settings.value("timeWindow", 60).toInt();
        m_timeWindowSpinBox->setValue(timeWindow);
    }
    
    if (m_rvRangeMaxSpinBox) {
        double rvRangeMax = settings.value("rvRangeMax", 100.0).toDouble();
        m_rvRangeMaxSpinBox->setValue(rvRangeMax);
    }
    
    if (m_rvVelocityMaxSpinBox) {
        double rvVelocityMax = settings.value("rvVelocityMax", 100.0).toDouble();
        m_rvVelocityMaxSpinBox->setValue(rvVelocityMax);
    }
    
    settings.endGroup();
}

void TimeSeriesPlotsWidget::onSaveSettings()
{
    saveSettings();
    // Could add a visual confirmation here (e.g., message box or status indication)
}

void TimeSeriesPlotsWidget::onLoadSettings()
{
    loadSettings();
    // Could add a visual confirmation here (e.g., message box or status indication)
}
