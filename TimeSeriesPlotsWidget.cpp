#include "TimeSeriesPlotsWidget.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainterPath>
#include <QFontMetrics>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFrame>
#include <cmath>

// ==================== TimeSeriesPlotWidget Implementation ====================

TimeSeriesPlotWidget::TimeSeriesPlotWidget(QWidget *parent)
    : QWidget(parent)
    , m_yAxisLabel("Value")
    , m_yAxisUnit("")
    , m_minY(0.0f)
    , m_maxY(100.0f)
    , m_timeWindowSeconds(30)
    , m_marginLeft(70)
    , m_marginRight(20)
    , m_marginTop(20)
    , m_marginBottom(50)
    , m_isDarkTheme(false)
{
    setMinimumSize(300, 150);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
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
    // Reset to default view - could implement zoom functionality later
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
    
    // Build path for data line
    QPainterPath dataPath;
    QPainterPath fillPath;
    bool firstPoint = true;
    
    for (const auto& point : m_dataPoints) {
        // Calculate x position based on time
        float timeRatio = float(point.first - startTime) / float(m_timeWindowSeconds * 1000);
        if (timeRatio < 0 || timeRatio > 1) continue;
        
        int x = m_plotRect.left() + int(timeRatio * m_plotRect.width());
        
        // Calculate y position based on value
        float valueRatio = (point.second - m_minY) / (m_maxY - m_minY);
        valueRatio = qBound(0.0f, valueRatio, 1.0f);
        int y = m_plotRect.bottom() - int(valueRatio * m_plotRect.height());
        
        if (firstPoint) {
            dataPath.moveTo(x, y);
            fillPath.moveTo(x, m_plotRect.bottom());
            fillPath.lineTo(x, y);
            firstPoint = false;
        } else {
            dataPath.lineTo(x, y);
            fillPath.lineTo(x, y);
        }
    }
    
    // Complete fill path
    if (!firstPoint && !m_dataPoints.isEmpty()) {
        fillPath.lineTo(m_plotRect.right(), m_plotRect.bottom());
        fillPath.closeSubpath();
        
        // Draw fill
        painter.fillPath(fillPath, getDataFillColor());
        
        // Draw line
        painter.setPen(QPen(getDataLineColor(), 2));
        painter.drawPath(dataPath);
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

// ==================== RangeVelocityPlotWidget Implementation ====================

RangeVelocityPlotWidget::RangeVelocityPlotWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxRange(150.0f)
    , m_maxVelocity(240.0f)
    , m_showHistogram(true)
    , m_marginLeft(70)
    , m_marginRight(20)
    , m_marginTop(20)
    , m_marginBottom(50)
    , m_isDarkTheme(false)
{
    setMinimumSize(300, 300);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    
    // Initialize histogram bins
    m_velocityHistogram.resize(HISTOGRAM_BINS, 0);
    m_rangeHistogram.resize(HISTOGRAM_BINS, 0);
}

void RangeVelocityPlotWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void RangeVelocityPlotWidget::setRangeLimit(float maxRange)
{
    m_maxRange = maxRange;
    update();
}

void RangeVelocityPlotWidget::setVelocityLimit(float maxVelocity)
{
    m_maxVelocity = maxVelocity;
    update();
}

void RangeVelocityPlotWidget::setShowHistogram(bool show)
{
    m_showHistogram = show;
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
    
    for (const auto& point : m_dataPoints) {
        float velocity = point.first;
        float range = point.second;
        
        // Calculate position
        float xRatio = velocity / m_maxVelocity;
        float yRatio = range / m_maxRange;
        
        if (xRatio < 0 || xRatio > 1 || yRatio < 0 || yRatio > 1) continue;
        
        int x = m_plotRect.left() + int(xRatio * m_plotRect.width());
        int y = m_plotRect.bottom() - int(yRatio * m_plotRect.height());
        
        // Draw point
        painter.drawEllipse(QPoint(x, y), 3, 3);
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

// ==================== TimeSeriesPlotsWidget Implementation ====================

TimeSeriesPlotsWidget::TimeSeriesPlotsWidget(QWidget *parent)
    : QWidget(parent)
    , m_showHistogramCheckbox(nullptr)
    , m_rangeVelocityPlot(nullptr)
    , m_velocityTimePlot(nullptr)
    , m_rangeTimePlot(nullptr)
    , m_resetVelocityBtn(nullptr)
    , m_resetRangeBtn(nullptr)
    , m_isDarkTheme(false)
    , m_maxRange(150.0f)
    , m_maxVelocity(240.0f)
{
    setupUI();
}

TimeSeriesPlotsWidget::~TimeSeriesPlotsWidget()
{
}

void TimeSeriesPlotsWidget::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // Top row with checkbox
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addStretch();
    m_showHistogramCheckbox = new QCheckBox("Show Histogram", this);
    m_showHistogramCheckbox->setChecked(true);
    connect(m_showHistogramCheckbox, &QCheckBox::toggled, 
            this, &TimeSeriesPlotsWidget::onShowHistogramToggled);
    topLayout->addWidget(m_showHistogramCheckbox);
    mainLayout->addLayout(topLayout);
    
    // Main content area with horizontal split
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);
    
    // Left side - Range vs Velocity plot
    QGroupBox* leftGroup = new QGroupBox(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftGroup);
    leftLayout->setContentsMargins(5, 5, 5, 5);
    
    m_rangeVelocityPlot = new RangeVelocityPlotWidget(this);
    m_rangeVelocityPlot->setRangeLimit(m_maxRange);
    m_rangeVelocityPlot->setVelocityLimit(m_maxVelocity);
    leftLayout->addWidget(m_rangeVelocityPlot);
    
    contentLayout->addWidget(leftGroup, 1);
    
    // Right side - Time series plots (stacked)
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(10);
    
    // Velocity vs Time plot with reset button
    QHBoxLayout* velocityLayout = new QHBoxLayout();
    
    QGroupBox* velocityGroup = new QGroupBox(this);
    QVBoxLayout* velocityGroupLayout = new QVBoxLayout(velocityGroup);
    velocityGroupLayout->setContentsMargins(5, 5, 5, 5);
    
    m_velocityTimePlot = new TimeSeriesPlotWidget(this);
    m_velocityTimePlot->setYAxisLabel("Velocity");
    m_velocityTimePlot->setYAxisUnit("km/h");
    m_velocityTimePlot->setYAxisRange(0, m_maxVelocity);
    m_velocityTimePlot->setTimeWindowSeconds(30);
    velocityGroupLayout->addWidget(m_velocityTimePlot);
    
    velocityLayout->addWidget(velocityGroup, 1);
    
    // Reset button for velocity plot
    m_resetVelocityBtn = new QPushButton(this);
    m_resetVelocityBtn->setFixedSize(32, 32);
    m_resetVelocityBtn->setToolTip("Reset Velocity Plot");
    m_resetVelocityBtn->setText("\xF0\x9F\x94\x8D");  // Magnifying glass emoji
    connect(m_resetVelocityBtn, &QPushButton::clicked, 
            this, &TimeSeriesPlotsWidget::onResetVelocityPlot);
    velocityLayout->addWidget(m_resetVelocityBtn, 0, Qt::AlignCenter);
    
    rightLayout->addLayout(velocityLayout, 1);
    
    // Range vs Time plot with reset button
    QHBoxLayout* rangeLayout = new QHBoxLayout();
    
    QGroupBox* rangeGroup = new QGroupBox(this);
    QVBoxLayout* rangeGroupLayout = new QVBoxLayout(rangeGroup);
    rangeGroupLayout->setContentsMargins(5, 5, 5, 5);
    
    m_rangeTimePlot = new TimeSeriesPlotWidget(this);
    m_rangeTimePlot->setYAxisLabel("Range");
    m_rangeTimePlot->setYAxisUnit("m");
    m_rangeTimePlot->setYAxisRange(0, m_maxRange);
    m_rangeTimePlot->setTimeWindowSeconds(30);
    rangeGroupLayout->addWidget(m_rangeTimePlot);
    
    rangeLayout->addWidget(rangeGroup, 1);
    
    // Reset button for range plot
    m_resetRangeBtn = new QPushButton(this);
    m_resetRangeBtn->setFixedSize(32, 32);
    m_resetRangeBtn->setToolTip("Reset Range Plot");
    m_resetRangeBtn->setText("\xF0\x9F\x94\x8D");
    connect(m_resetRangeBtn, &QPushButton::clicked, 
            this, &TimeSeriesPlotsWidget::onResetRangePlot);
    rangeLayout->addWidget(m_resetRangeBtn, 0, Qt::AlignCenter);
    
    rightLayout->addLayout(rangeLayout, 1);
    
    contentLayout->addLayout(rightLayout, 1);
    
    mainLayout->addLayout(contentLayout, 1);
    
    applyTheme();
}

void TimeSeriesPlotsWidget::updateFromTargets(const TargetTrackData& targets)
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    
    for (size_t i = 0; i < targets.numTracks && i < targets.targets.size(); ++i) {
        const auto& target = targets.targets[i];
        float velocityKmh = qAbs(target.radial_speed) * 3.6f;  // Convert m/s to km/h
        float rangeM = target.radius;
        
        // Update range-velocity plot
        if (m_rangeVelocityPlot) {
            m_rangeVelocityPlot->addDataPoint(velocityKmh, rangeM);
        }
        
        // Update time series plots
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
    if (m_rangeTimePlot) {
        m_rangeTimePlot->setYAxisRange(0, maxRange);
    }
}

void TimeSeriesPlotsWidget::setMaxVelocity(float maxVelocity)
{
    m_maxVelocity = maxVelocity;
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setVelocityLimit(maxVelocity);
    }
    if (m_velocityTimePlot) {
        m_velocityTimePlot->setYAxisRange(0, maxVelocity);
    }
}

void TimeSeriesPlotsWidget::onShowHistogramToggled(bool checked)
{
    if (m_rangeVelocityPlot) {
        m_rangeVelocityPlot->setShowHistogram(checked);
    }
}

void TimeSeriesPlotsWidget::onResetVelocityPlot()
{
    if (m_velocityTimePlot) {
        m_velocityTimePlot->clearData();
    }
}

void TimeSeriesPlotsWidget::onResetRangePlot()
{
    if (m_rangeTimePlot) {
        m_rangeTimePlot->clearData();
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
    
    // Style the checkbox
    if (m_showHistogramCheckbox) {
        m_showHistogramCheckbox->setStyleSheet(QString(
            "QCheckBox { color: %1; font-weight: 500; }"
            "QCheckBox::indicator { width: 18px; height: 18px; }"
            "QCheckBox::indicator:unchecked { border: 2px solid %2; border-radius: 3px; background: transparent; }"
            "QCheckBox::indicator:checked { border: 2px solid %3; border-radius: 3px; background: %3; }"
        ).arg(textColor, borderColor, m_isDarkTheme ? "#3b82f6" : "#2563eb"));
    }
    
    // Style the reset buttons
    QString buttonStyle = QString(
        "QPushButton {"
        "  background: %1;"
        "  border: 1px solid %2;"
        "  border-radius: 4px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background: %3;"
        "}"
    ).arg(groupBgColor, borderColor, m_isDarkTheme ? "#334155" : "#e2e8f0");
    
    if (m_resetVelocityBtn) {
        m_resetVelocityBtn->setStyleSheet(buttonStyle);
    }
    if (m_resetRangeBtn) {
        m_resetRangeBtn->setStyleSheet(buttonStyle);
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
