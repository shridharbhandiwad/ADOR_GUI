#include "SpeedMeasurementWidget.h"
#include <QPainterPath>
#include <QConicalGradient>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QtMath>
#include <QFont>
#include <QFontMetrics>

// ==================== SpeedometerGauge Implementation ====================

SpeedometerGauge::SpeedometerGauge(QWidget *parent)
    : QWidget(parent)
    , m_currentSpeed(0.0f)
    , m_maxSpeed(200.0f)
    , m_animatedSpeed(0.0f)
    , m_isDarkTheme(false)
    , m_radius(0.0f)
{
    setMinimumSize(300, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Setup animation timer for smooth needle movement
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, [this]() {
        float diff = m_currentSpeed - m_animatedSpeed;
        if (qAbs(diff) < 0.5f) {
            m_animatedSpeed = m_currentSpeed;
            m_animationTimer->stop();
        } else {
            m_animatedSpeed += diff * 0.15f;  // Smooth interpolation
        }
        update();
    });
    m_animationTimer->setInterval(16);  // ~60 FPS
}

void SpeedometerGauge::setSpeed(float speed)
{
    m_currentSpeed = qBound(0.0f, speed, m_maxSpeed);
    if (!m_animationTimer->isActive()) {
        m_animationTimer->start();
    }
}

void SpeedometerGauge::setMaxSpeed(float maxSpeed)
{
    m_maxSpeed = maxSpeed;
    update();
}

void SpeedometerGauge::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void SpeedometerGauge::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    int side = qMin(width(), height());
    m_radius = side * 0.45f;
    m_center = QPointF(width() / 2.0f, height() / 2.0f);
    m_gaugeRect = QRect(
        static_cast<int>(m_center.x() - m_radius),
        static_cast<int>(m_center.y() - m_radius),
        static_cast<int>(m_radius * 2),
        static_cast<int>(m_radius * 2)
    );
}

void SpeedometerGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    
    drawBackground(painter);
    drawOuterRing(painter);
    drawSpeedScale(painter);
    drawSpeedMarkers(painter);
    drawNeedle(painter);
    drawCenterCap(painter);
}

QColor SpeedometerGauge::getBackgroundColor() const
{
    return m_isDarkTheme ? QColor(30, 41, 59) : QColor(248, 250, 252);
}

QColor SpeedometerGauge::getOuterRingColor() const
{
    // Cyan/Teal color like in the reference image
    return QColor(0, 188, 212);  // Cyan-500
}

QColor SpeedometerGauge::getInnerBackgroundColor() const
{
    return m_isDarkTheme ? QColor(51, 65, 85) : QColor(75, 85, 99);  // Gray inner
}

QColor SpeedometerGauge::getScaleTextColor() const
{
    return m_isDarkTheme ? QColor(226, 232, 240) : QColor(15, 23, 42);
}

QColor SpeedometerGauge::getNeedleColor() const
{
    return m_isDarkTheme ? QColor(248, 250, 252) : QColor(15, 23, 42);
}

QColor SpeedometerGauge::getCenterCapColor() const
{
    return m_isDarkTheme ? QColor(71, 85, 105) : QColor(51, 65, 85);
}

QColor SpeedometerGauge::getHighSpeedZoneColor() const
{
    return QColor(239, 68, 68);  // Red for danger zone
}

void SpeedometerGauge::drawBackground(QPainter& painter)
{
    painter.fillRect(rect(), getBackgroundColor());
}

void SpeedometerGauge::drawOuterRing(QPainter& painter)
{
    painter.save();
    
    float outerRadius = m_radius;
    float ringWidth = m_radius * 0.12f;
    float innerRadius = outerRadius - ringWidth;
    
    // Draw outer cyan/teal ring
    QPen ringPen(getOuterRingColor(), ringWidth);
    ringPen.setCapStyle(Qt::FlatCap);
    painter.setPen(ringPen);
    painter.setBrush(Qt::NoBrush);
    
    QRectF outerRect(
        m_center.x() - outerRadius + ringWidth/2,
        m_center.y() - outerRadius + ringWidth/2,
        (outerRadius - ringWidth/2) * 2,
        (outerRadius - ringWidth/2) * 2
    );
    
    // Draw the arc (270 degrees from bottom-left going clockwise)
    painter.drawArc(outerRect, static_cast<int>(START_ANGLE * 16), static_cast<int>(SWEEP_ANGLE * 16));
    
    // Draw high speed danger zone (red arc for last ~25 km/h)
    float dangerStartSpeed = m_maxSpeed - 25.0f;
    float dangerAngle = START_ANGLE + (SWEEP_ANGLE * (dangerStartSpeed / m_maxSpeed));
    float dangerSweep = SWEEP_ANGLE * (25.0f / m_maxSpeed);
    
    QPen dangerPen(getHighSpeedZoneColor(), ringWidth);
    dangerPen.setCapStyle(Qt::FlatCap);
    painter.setPen(dangerPen);
    painter.drawArc(outerRect, static_cast<int>(dangerAngle * 16), static_cast<int>(dangerSweep * 16));
    
    // Draw inner gray circle
    painter.setPen(Qt::NoPen);
    painter.setBrush(getInnerBackgroundColor());
    painter.drawEllipse(m_center, innerRadius - 2, innerRadius - 2);
    
    painter.restore();
}

void SpeedometerGauge::drawSpeedScale(QPainter& painter)
{
    painter.save();
    
    QFont scaleFont("Segoe UI", static_cast<int>(m_radius * 0.08f), QFont::Bold);
    painter.setFont(scaleFont);
    painter.setPen(getScaleTextColor());
    
    // Draw speed numbers: 0, 25, 50, 75, 100, 125, 150, 175, 200
    for (int speed = 0; speed <= static_cast<int>(m_maxSpeed); speed += 25) {
        float fraction = static_cast<float>(speed) / m_maxSpeed;
        float angle = START_ANGLE + (SWEEP_ANGLE * fraction);
        float angleRad = qDegreesToRadians(angle);
        
        // Position for the number (inside the ring)
        float textRadius = m_radius * 0.70f;
        float x = m_center.x() + textRadius * qCos(angleRad);
        float y = m_center.y() - textRadius * qSin(angleRad);
        
        QString text = QString::number(speed);
        QFontMetrics fm(scaleFont);
        QRect textRect = fm.boundingRect(text);
        
        painter.drawText(
            static_cast<int>(x - textRect.width() / 2),
            static_cast<int>(y + textRect.height() / 4),
            text
        );
    }
    
    painter.restore();
}

void SpeedometerGauge::drawSpeedMarkers(QPainter& painter)
{
    painter.save();
    
    float innerRadius = m_radius * 0.85f;
    float majorTickLength = m_radius * 0.08f;
    float minorTickLength = m_radius * 0.04f;
    
    // Draw tick marks
    for (int speed = 0; speed <= static_cast<int>(m_maxSpeed); speed += 5) {
        float fraction = static_cast<float>(speed) / m_maxSpeed;
        float angle = START_ANGLE + (SWEEP_ANGLE * fraction);
        float angleRad = qDegreesToRadians(angle);
        
        bool isMajorTick = (speed % 25 == 0);
        float tickLength = isMajorTick ? majorTickLength : minorTickLength;
        float tickWidth = isMajorTick ? 2.5f : 1.5f;
        
        // Check if in danger zone
        QColor tickColor = (speed >= m_maxSpeed - 25) ? getHighSpeedZoneColor() : getScaleTextColor();
        
        QPen tickPen(tickColor, tickWidth);
        painter.setPen(tickPen);
        
        float outerX = m_center.x() + innerRadius * qCos(angleRad);
        float outerY = m_center.y() - innerRadius * qSin(angleRad);
        float innerX = m_center.x() + (innerRadius - tickLength) * qCos(angleRad);
        float innerY = m_center.y() - (innerRadius - tickLength) * qSin(angleRad);
        
        painter.drawLine(QPointF(outerX, outerY), QPointF(innerX, innerY));
    }
    
    painter.restore();
}

void SpeedometerGauge::drawNeedle(QPainter& painter)
{
    painter.save();
    
    float fraction = m_animatedSpeed / m_maxSpeed;
    float angle = START_ANGLE + (SWEEP_ANGLE * fraction);
    float angleRad = qDegreesToRadians(angle);
    
    float needleLength = m_radius * 0.55f;
    float needleTailLength = m_radius * 0.15f;
    float needleWidth = m_radius * 0.03f;
    
    painter.translate(m_center);
    painter.rotate(-angle + 90);  // Rotate coordinate system
    
    // Create needle path
    QPainterPath needlePath;
    needlePath.moveTo(0, -needleLength);  // Tip
    needlePath.lineTo(-needleWidth, 0);   // Left base
    needlePath.lineTo(0, needleTailLength);  // Tail
    needlePath.lineTo(needleWidth, 0);    // Right base
    needlePath.closeSubpath();
    
    // Draw needle shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 40));
    painter.translate(2, 2);
    painter.drawPath(needlePath);
    painter.translate(-2, -2);
    
    // Draw needle
    QLinearGradient needleGradient(0, -needleLength, 0, needleTailLength);
    needleGradient.setColorAt(0, getNeedleColor());
    needleGradient.setColorAt(0.5, getNeedleColor().darker(110));
    needleGradient.setColorAt(1, getNeedleColor().darker(130));
    
    painter.setBrush(needleGradient);
    painter.setPen(QPen(getNeedleColor().darker(150), 0.5));
    painter.drawPath(needlePath);
    
    painter.restore();
}

void SpeedometerGauge::drawCenterCap(QPainter& painter)
{
    painter.save();
    
    float capRadius = m_radius * 0.12f;
    
    // Draw cap shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 50));
    painter.drawEllipse(m_center + QPointF(2, 2), capRadius, capRadius);
    
    // Draw center cap with gradient
    QRadialGradient capGradient(m_center, capRadius);
    capGradient.setColorAt(0, getCenterCapColor().lighter(130));
    capGradient.setColorAt(0.7, getCenterCapColor());
    capGradient.setColorAt(1, getCenterCapColor().darker(120));
    
    painter.setBrush(capGradient);
    painter.setPen(QPen(getCenterCapColor().darker(150), 1));
    painter.drawEllipse(m_center, capRadius, capRadius);
    
    painter.restore();
}

// ==================== SpeedMeasurementWidget Implementation ====================

SpeedMeasurementWidget::SpeedMeasurementWidget(QWidget *parent)
    : QWidget(parent)
    , m_speedometer(nullptr)
    , m_actualSpeedLabel(nullptr)
    , m_topSpeedValue(nullptr)
    , m_topSpeedLabel(nullptr)
    , m_outputGroup(nullptr)
    , m_resetButton(nullptr)
    , m_topSpeed(0.0f)
    , m_isDarkTheme(false)
{
    setupUI();
}

SpeedMeasurementWidget::~SpeedMeasurementWidget()
{
}

void SpeedMeasurementWidget::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(40);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // ========== Left side: Speedometer with "Actual speed" label ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(15);
    
    // "Actual speed" label
    m_actualSpeedLabel = new QLabel("Actual speed");
    m_actualSpeedLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Segoe UI", 20, QFont::Bold);
    m_actualSpeedLabel->setFont(titleFont);
    leftLayout->addWidget(m_actualSpeedLabel);
    
    // Speedometer gauge
    m_speedometer = new SpeedometerGauge(this);
    m_speedometer->setMinimumSize(350, 350);
    leftLayout->addWidget(m_speedometer, 1);
    
    mainLayout->addLayout(leftLayout, 2);
    
    // ========== Right side: Top Speed display and Output section ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(30);
    
    // "Top Speed" section
    QVBoxLayout* topSpeedLayout = new QVBoxLayout();
    topSpeedLayout->setSpacing(10);
    
    m_topSpeedLabel = new QLabel("Top Speed");
    m_topSpeedLabel->setAlignment(Qt::AlignCenter);
    m_topSpeedLabel->setFont(titleFont);
    topSpeedLayout->addWidget(m_topSpeedLabel);
    
    // Top speed value display with unit
    QHBoxLayout* valueLayout = new QHBoxLayout();
    valueLayout->setAlignment(Qt::AlignCenter);
    
    m_topSpeedValue = new QLabel("0");
    QFont valueFont("Segoe UI", 72, QFont::Bold);
    m_topSpeedValue->setFont(valueFont);
    m_topSpeedValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_topSpeedValue->setMinimumWidth(200);
    m_topSpeedValue->setStyleSheet("QLabel { border: 2px solid #e2e8f0; border-radius: 8px; padding: 10px 20px; background-color: #ffffff; }");
    valueLayout->addWidget(m_topSpeedValue);
    
    QLabel* unitLabel = new QLabel("km/h");
    QFont unitFont("Segoe UI", 24, QFont::Normal);
    unitLabel->setFont(unitFont);
    unitLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    valueLayout->addWidget(unitLabel);
    
    topSpeedLayout->addLayout(valueLayout);
    rightLayout->addLayout(topSpeedLayout);
    
    rightLayout->addStretch(1);
    
    // Output 1 section with Reset button
    m_outputGroup = new QGroupBox("Output 1");
    QVBoxLayout* outputLayout = new QVBoxLayout(m_outputGroup);
    outputLayout->setAlignment(Qt::AlignCenter);
    
    m_resetButton = new QPushButton("Reset");
    m_resetButton->setMinimumSize(120, 40);
    connect(m_resetButton, &QPushButton::clicked, this, &SpeedMeasurementWidget::onResetTopSpeed);
    outputLayout->addWidget(m_resetButton, 0, Qt::AlignCenter);
    
    rightLayout->addWidget(m_outputGroup);
    rightLayout->addStretch(1);
    
    mainLayout->addLayout(rightLayout, 1);
    
    applyTheme();
}

void SpeedMeasurementWidget::updateSpeed(float speed)
{
    if (m_speedometer) {
        m_speedometer->setSpeed(speed);
    }
    updateTopSpeed(speed);
}

void SpeedMeasurementWidget::updateFromTargets(const TargetTrackData& targets)
{
    // Find the fastest target (by radial speed magnitude)
    float maxSpeed = 0.0f;
    
    for (size_t i = 0; i < targets.numTracks && i < targets.targets.size(); ++i) {
        const auto& target = targets.targets[i];
        // Convert radial speed (m/s) to km/h and use absolute value
        float speedKmh = qAbs(target.radial_speed) * 3.6f;
        if (speedKmh > maxSpeed) {
            maxSpeed = speedKmh;
        }
    }
    
    updateSpeed(maxSpeed);
}

void SpeedMeasurementWidget::updateTopSpeed(float speed)
{
    if (speed > m_topSpeed) {
        m_topSpeed = speed;
        if (m_topSpeedValue) {
            m_topSpeedValue->setText(QString::number(static_cast<int>(m_topSpeed)));
        }
    }
}

void SpeedMeasurementWidget::onResetTopSpeed()
{
    m_topSpeed = 0.0f;
    if (m_topSpeedValue) {
        m_topSpeedValue->setText("0");
    }
    if (m_speedometer) {
        m_speedometer->setSpeed(0.0f);
    }
}

void SpeedMeasurementWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    if (m_speedometer) {
        m_speedometer->setDarkTheme(isDark);
    }
    applyTheme();
}

void SpeedMeasurementWidget::applyTheme()
{
    if (m_isDarkTheme) {
        // Dark theme styling
        if (m_actualSpeedLabel) {
            m_actualSpeedLabel->setStyleSheet("QLabel { color: #f1f5f9; }");
        }
        if (m_topSpeedLabel) {
            m_topSpeedLabel->setStyleSheet("QLabel { color: #f1f5f9; }");
        }
        if (m_topSpeedValue) {
            m_topSpeedValue->setStyleSheet("QLabel { border: 2px solid #475569; border-radius: 8px; padding: 10px 20px; background-color: #1e293b; color: #f1f5f9; }");
        }
    } else {
        // Light theme styling
        if (m_actualSpeedLabel) {
            m_actualSpeedLabel->setStyleSheet("QLabel { color: #0f172a; }");
        }
        if (m_topSpeedLabel) {
            m_topSpeedLabel->setStyleSheet("QLabel { color: #0f172a; }");
        }
        if (m_topSpeedValue) {
            m_topSpeedValue->setStyleSheet("QLabel { border: 2px solid #e2e8f0; border-radius: 8px; padding: 10px 20px; background-color: #ffffff; color: #0f172a; }");
        }
    }
    update();
}
