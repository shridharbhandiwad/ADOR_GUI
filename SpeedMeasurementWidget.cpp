#include "SpeedMeasurementWidget.h"
#include <QPainterPath>
#include <QConicalGradient>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QtMath>
#include <QFont>
#include <QFontMetrics>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>

// ==================== SpeedometerGauge Implementation ====================

SpeedometerGauge::SpeedometerGauge(QWidget *parent)
    : QWidget(parent)
    , m_currentSpeed(0.0f)
    , m_maxSpeed(200.0f)
    , m_animatedSpeed(0.0f)
    , m_glowIntensity(0.0f)
    , m_isDarkTheme(false)
    , m_radius(0.0f)
{
    // No hard-coded minimum size to allow responsive layout
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // Setup animation timer for smooth needle movement
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, [this]() {
        float diff = m_currentSpeed - m_animatedSpeed;
        if (qAbs(diff) < 0.3f) {
            m_animatedSpeed = m_currentSpeed;
            m_animationTimer->stop();
        } else {
            m_animatedSpeed += diff * 0.12f;  // Smooth interpolation
        }
        update();
    });
    m_animationTimer->setInterval(16);  // ~60 FPS
    
    // Setup glow animation for active speed indication
    m_glowAnimation = new QPropertyAnimation(this, "glowIntensity", this);
    m_glowAnimation->setDuration(1500);
    m_glowAnimation->setStartValue(0.3f);
    m_glowAnimation->setEndValue(1.0f);
    m_glowAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_glowAnimation->setLoopCount(-1);  // Infinite loop
    
    updateThemeColors();
}

void SpeedometerGauge::setSpeed(float speed)
{
    m_currentSpeed = qBound(0.0f, speed, m_maxSpeed);
    if (!m_animationTimer->isActive()) {
        m_animationTimer->start();
    }
    
    // Start glow animation when speed is significant
    if (m_currentSpeed > 5.0f && m_glowAnimation->state() != QAbstractAnimation::Running) {
        m_glowAnimation->start();
    } else if (m_currentSpeed <= 5.0f && m_glowAnimation->state() == QAbstractAnimation::Running) {
        m_glowAnimation->stop();
        m_glowIntensity = 0.0f;
    }
}

void SpeedometerGauge::setAnimatedSpeed(float speed)
{
    m_animatedSpeed = speed;
    update();
}

void SpeedometerGauge::setGlowIntensity(float intensity)
{
    m_glowIntensity = intensity;
    update();
}

void SpeedometerGauge::setMaxSpeed(float maxSpeed)
{
    m_maxSpeed = maxSpeed;
    update();
}

void SpeedometerGauge::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    updateThemeColors();
    update();
}

void SpeedometerGauge::setTheme(const SpeedWidgetTheme& theme)
{
    m_theme = theme;
    update();
}

void SpeedometerGauge::updateThemeColors()
{
    if (m_isDarkTheme) {
        // Dark theme - classic, elegant monochrome
        m_theme.backgroundColor = QColor(18, 18, 22);      // Deep charcoal
        m_theme.cardBackground = QColor(28, 28, 34);       // Dark gray
        m_theme.gaugeBackground = QColor(28, 28, 34);      // Dark gray
        m_theme.gaugeInnerRing = QColor(45, 45, 52);       // Medium gray
        m_theme.gaugeArcColor = QColor(55, 55, 62);        // Gray track
        m_theme.gaugeGlowColor = QColor(180, 180, 190);    // Light gray
        m_theme.primaryColor = QColor(200, 200, 210);      // Silver
        m_theme.accentColor = QColor(180, 180, 190);       // Light gray accent
        m_theme.textPrimary = QColor(240, 240, 245);       // Off-white
        m_theme.textSecondary = QColor(140, 140, 150);     // Medium gray
        m_theme.needleColor = QColor(240, 240, 245);       // Off-white needle
        m_theme.needleGlow = QColor(200, 200, 210, 80);    // Silver glow
        m_theme.centerCapColor = QColor(60, 60, 68);       // Dark gray cap
        m_theme.normalZone = QColor(140, 140, 150);        // Gray (not used)
        m_theme.warningZone = QColor(160, 160, 170);       // Gray (not used)
        m_theme.dangerZone = QColor(180, 180, 190);        // Gray (not used)
        m_theme.borderColor = QColor(50, 50, 58);          // Border gray
    } else {
        // Light theme - classic, clean monochrome
        m_theme.backgroundColor = QColor(250, 250, 252);   // Off-white
        m_theme.cardBackground = QColor(255, 255, 255);    // White
        m_theme.gaugeBackground = QColor(245, 245, 248);   // Light gray
        m_theme.gaugeInnerRing = QColor(225, 225, 230);    // Silver
        m_theme.gaugeArcColor = QColor(200, 200, 210);     // Gray track
        m_theme.gaugeGlowColor = QColor(80, 80, 90);       // Dark gray
        m_theme.primaryColor = QColor(60, 60, 70);         // Charcoal
        m_theme.accentColor = QColor(80, 80, 90);          // Dark gray
        m_theme.textPrimary = QColor(35, 35, 45);          // Near black
        m_theme.textSecondary = QColor(100, 100, 110);     // Medium gray
        m_theme.needleColor = QColor(35, 35, 45);          // Near black needle
        m_theme.needleGlow = QColor(60, 60, 70, 80);       // Charcoal glow
        m_theme.centerCapColor = QColor(70, 70, 80);       // Dark gray cap
        m_theme.normalZone = QColor(100, 100, 110);        // Gray (not used)
        m_theme.warningZone = QColor(120, 120, 130);       // Gray (not used)
        m_theme.dangerZone = QColor(140, 140, 150);        // Gray (not used)
        m_theme.borderColor = QColor(220, 220, 228);       // Light border
    }
}

void SpeedometerGauge::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    int side = qMin(width(), height());
    m_radius = side * 0.42f;
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
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    drawBackground(painter);
    drawGaugeArc(painter);
    drawProgressArc(painter);
    drawTickMarks(painter);
    drawSpeedLabels(painter);
    drawNeedle(painter);
    drawCenterHub(painter);
    drawDigitalDisplay(painter);
}

void SpeedometerGauge::drawBackground(QPainter& painter)
{
    // Draw gradient background
    QRadialGradient bgGradient(m_center, m_radius * 1.2f);
    bgGradient.setColorAt(0, m_theme.cardBackground);
    bgGradient.setColorAt(1, m_theme.backgroundColor);
    painter.fillRect(rect(), bgGradient);
}

void SpeedometerGauge::drawOuterGlow(QPainter& painter)
{
    if (m_animatedSpeed < 5.0f) return;
    
    painter.save();
    
    // Determine glow color based on speed zone
    float speedRatio = m_animatedSpeed / m_maxSpeed;
    QColor glowColor;
    
    if (speedRatio >= DANGER_THRESHOLD) {
        glowColor = m_theme.dangerZone;
    } else if (speedRatio >= WARNING_THRESHOLD) {
        glowColor = m_theme.warningZone;
    } else {
        glowColor = m_theme.accentColor;
    }
    
    // Draw pulsing outer glow
    float glowRadius = m_radius * 1.08f;
    float glowWidth = m_radius * 0.08f * (0.5f + m_glowIntensity * 0.5f);
    
    QRadialGradient glowGradient(m_center, glowRadius);
    glowGradient.setColorAt(0.85f, QColor(glowColor.red(), glowColor.green(), glowColor.blue(), 
                                          static_cast<int>(80 * m_glowIntensity)));
    glowGradient.setColorAt(1.0f, QColor(glowColor.red(), glowColor.green(), glowColor.blue(), 0));
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(glowGradient);
    painter.drawEllipse(m_center, glowRadius + glowWidth, glowRadius + glowWidth);
    
    painter.restore();
}

void SpeedometerGauge::drawSpeedZones(QPainter& painter)
{
    painter.save();
    
    float outerRadius = m_radius * 0.98f;
    float zoneWidth = m_radius * 0.06f;
    
    QRectF arcRect(
        m_center.x() - outerRadius,
        m_center.y() - outerRadius,
        outerRadius * 2,
        outerRadius * 2
    );
    
    // Normal zone (0% to 70%)
    float normalAngle = SWEEP_ANGLE * WARNING_THRESHOLD;
    QPen normalPen(m_theme.normalZone, zoneWidth);
    normalPen.setCapStyle(Qt::FlatCap);
    painter.setPen(normalPen);
    painter.drawArc(arcRect, static_cast<int>(START_ANGLE * 16), static_cast<int>(normalAngle * 16));
    
    // Warning zone (70% to 85%)
    float warningStartAngle = START_ANGLE + normalAngle;
    float warningAngle = SWEEP_ANGLE * (DANGER_THRESHOLD - WARNING_THRESHOLD);
    QPen warningPen(m_theme.warningZone, zoneWidth);
    warningPen.setCapStyle(Qt::FlatCap);
    painter.setPen(warningPen);
    painter.drawArc(arcRect, static_cast<int>(warningStartAngle * 16), static_cast<int>(warningAngle * 16));
    
    // Danger zone (85% to 100%)
    float dangerStartAngle = warningStartAngle + warningAngle;
    float dangerAngle = SWEEP_ANGLE * (1.0f - DANGER_THRESHOLD);
    QPen dangerPen(m_theme.dangerZone, zoneWidth);
    dangerPen.setCapStyle(Qt::FlatCap);
    painter.setPen(dangerPen);
    painter.drawArc(arcRect, static_cast<int>(dangerStartAngle * 16), static_cast<int>(dangerAngle * 16));
    
    painter.restore();
}

void SpeedometerGauge::drawGaugeArc(QPainter& painter)
{
    painter.save();
    
    float outerRadius = m_radius * 0.88f;
    float arcWidth = m_radius * 0.12f;
    
    QRectF arcRect(
        m_center.x() - outerRadius + arcWidth/2,
        m_center.y() - outerRadius + arcWidth/2,
        (outerRadius - arcWidth/2) * 2,
        (outerRadius - arcWidth/2) * 2
    );
    
    // Draw background arc track
    QPen trackPen(m_theme.gaugeArcColor, arcWidth);
    trackPen.setCapStyle(Qt::RoundCap);
    painter.setPen(trackPen);
    painter.drawArc(arcRect, static_cast<int>(START_ANGLE * 16), static_cast<int>(SWEEP_ANGLE * 16));
    
    painter.restore();
}

void SpeedometerGauge::drawProgressArc(QPainter& painter)
{
    if (m_animatedSpeed < 0.5f) return;
    
    painter.save();
    
    float outerRadius = m_radius * 0.88f;
    float arcWidth = m_radius * 0.10f;
    
    QRectF arcRect(
        m_center.x() - outerRadius + arcWidth/2,
        m_center.y() - outerRadius + arcWidth/2,
        (outerRadius - arcWidth/2) * 2,
        (outerRadius - arcWidth/2) * 2
    );
    
    // Calculate progress angle
    float speedRatio = m_animatedSpeed / m_maxSpeed;
    float progressAngle = SWEEP_ANGLE * speedRatio;
    
    // Classic single color for progress - use primary/accent color
    QColor progressColor = m_theme.primaryColor;
    
    // Draw main progress arc - clean, no glow
    QPen progressPen(progressColor, arcWidth);
    progressPen.setCapStyle(Qt::RoundCap);
    painter.setPen(progressPen);
    painter.drawArc(arcRect, static_cast<int>(START_ANGLE * 16), static_cast<int>(progressAngle * 16));
    
    painter.restore();
}

void SpeedometerGauge::drawTickMarks(QPainter& painter)
{
    painter.save();
    
    float innerRadius = m_radius * 0.70f;
    float majorTickLength = m_radius * 0.10f;
    float minorTickLength = m_radius * 0.05f;
    
    for (int speed = 0; speed <= static_cast<int>(m_maxSpeed); speed += 10) {
        float fraction = static_cast<float>(speed) / m_maxSpeed;
        float angle = START_ANGLE + (SWEEP_ANGLE * fraction);
        float angleRad = qDegreesToRadians(angle);
        
        bool isMajorTick = (speed % 20 == 0);
        float tickLength = isMajorTick ? majorTickLength : minorTickLength;
        float tickWidth = isMajorTick ? 2.0f : 1.0f;
        
        // Classic single color for all tick marks
        QColor tickColor = m_theme.textSecondary;
        
        QPen tickPen(tickColor, tickWidth, Qt::SolidLine, Qt::RoundCap);
        painter.setPen(tickPen);
        
        float outerX = m_center.x() + innerRadius * qCos(angleRad);
        float outerY = m_center.y() - innerRadius * qSin(angleRad);
        float innerX = m_center.x() + (innerRadius - tickLength) * qCos(angleRad);
        float innerY = m_center.y() - (innerRadius - tickLength) * qSin(angleRad);
        
        painter.drawLine(QPointF(outerX, outerY), QPointF(innerX, innerY));
    }
    
    painter.restore();
}

void SpeedometerGauge::drawSpeedLabels(QPainter& painter)
{
    painter.save();
    
    int fontSize = static_cast<int>(m_radius * 0.085f);
    QFont labelFont("Segoe UI", fontSize, QFont::DemiBold);
    painter.setFont(labelFont);
    
    for (int speed = 0; speed <= static_cast<int>(m_maxSpeed); speed += 40) {
        float fraction = static_cast<float>(speed) / m_maxSpeed;
        float angle = START_ANGLE + (SWEEP_ANGLE * fraction);
        float angleRad = qDegreesToRadians(angle);
        
        float textRadius = m_radius * 0.52f;
        float x = m_center.x() + textRadius * qCos(angleRad);
        float y = m_center.y() - textRadius * qSin(angleRad);
        
        // Classic single color for all labels
        QColor textColor = m_theme.textPrimary;
        
        painter.setPen(textColor);
        
        QString text = QString::number(speed);
        QFontMetrics fm(labelFont);
        QRect textRect = fm.boundingRect(text);
        
        painter.drawText(
            static_cast<int>(x - textRect.width() / 2),
            static_cast<int>(y + textRect.height() / 4),
            text
        );
    }
    
    painter.restore();
}

void SpeedometerGauge::drawNeedle(QPainter& painter)
{
    painter.save();
    
    float fraction = m_animatedSpeed / m_maxSpeed;
    float angle = START_ANGLE + (SWEEP_ANGLE * fraction);
    
    float needleLength = m_radius * 0.55f;
    float needleTailLength = m_radius * 0.10f;
    float needleWidth = m_radius * 0.018f;  // Thinner needle for classic look
    
    painter.translate(m_center);
    painter.rotate(-angle + 90);
    
    // Create sleek needle path - more elegant shape
    QPainterPath needlePath;
    needlePath.moveTo(0, -needleLength);           // Sharp tip
    needlePath.lineTo(-needleWidth * 0.8f, -needleLength * 0.15f);  // Left shoulder
    needlePath.lineTo(-needleWidth, 0);            // Left base
    needlePath.lineTo(0, needleTailLength);        // Tail
    needlePath.lineTo(needleWidth, 0);             // Right base
    needlePath.lineTo(needleWidth * 0.8f, -needleLength * 0.15f);   // Right shoulder
    needlePath.closeSubpath();
    
    // Draw subtle needle shadow
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 25));
    painter.translate(2, 2);
    painter.drawPath(needlePath);
    painter.translate(-2, -2);
    
    // Draw needle with gradient - classic single color scheme
    QLinearGradient needleGradient(0, -needleLength, 0, needleTailLength);
    
    // Classic single needle color
    QColor needleColor = m_theme.needleColor;
    
    needleGradient.setColorAt(0, needleColor);
    needleGradient.setColorAt(0.5, needleColor.darker(110));
    needleGradient.setColorAt(1, needleColor.darker(125));
    
    painter.setBrush(needleGradient);
    painter.setPen(QPen(needleColor.darker(140), 0.5));
    painter.drawPath(needlePath);
    
    painter.restore();
}

void SpeedometerGauge::drawCenterHub(QPainter& painter)
{
    painter.save();
    
    // Thin center hub for classic aesthetic
    float hubRadius = m_radius * 0.06f;
    float innerHubRadius = m_radius * 0.035f;
    
    // Draw outer hub ring
    QRadialGradient hubGradient(m_center, hubRadius);
    hubGradient.setColorAt(0, m_theme.centerCapColor.lighter(130));
    hubGradient.setColorAt(0.5, m_theme.centerCapColor);
    hubGradient.setColorAt(1, m_theme.centerCapColor.darker(140));
    
    painter.setPen(Qt::NoPen);
    
    // Subtle shadow
    painter.setBrush(QColor(0, 0, 0, 30));
    painter.drawEllipse(m_center + QPointF(1, 1), hubRadius, hubRadius);
    
    // Hub
    painter.setBrush(hubGradient);
    painter.drawEllipse(m_center, hubRadius, hubRadius);
    
    // Small inner accent
    QRadialGradient innerGradient(m_center - QPointF(hubRadius * 0.2f, hubRadius * 0.2f), innerHubRadius);
    innerGradient.setColorAt(0, m_theme.textPrimary.lighter(120));
    innerGradient.setColorAt(0.6, m_theme.textPrimary);
    innerGradient.setColorAt(1, m_theme.textPrimary.darker(110));
    
    painter.setBrush(innerGradient);
    painter.drawEllipse(m_center, innerHubRadius, innerHubRadius);
    
    painter.restore();
}

void SpeedometerGauge::drawDigitalDisplay(QPainter& painter)
{
    painter.save();
    
    // Draw speed value in the bottom center of gauge
    float displayY = m_center.y() + m_radius * 0.25f;
    
    // Speed value
    int fontSize = static_cast<int>(m_radius * 0.22f);
    QFont speedFont("Segoe UI", fontSize, QFont::Bold);
    painter.setFont(speedFont);
    
    QString speedText = QString::number(static_cast<int>(m_animatedSpeed));
    QFontMetrics fm(speedFont);
    QRect speedRect = fm.boundingRect(speedText);
    
    // Classic single color for speed display
    QColor speedColor = m_theme.textPrimary;
    
    painter.setPen(speedColor);
    painter.drawText(
        static_cast<int>(m_center.x() - speedRect.width() / 2),
        static_cast<int>(displayY + speedRect.height() / 3),
        speedText
    );
    
    // Unit label
    int unitFontSize = static_cast<int>(m_radius * 0.08f);
    QFont unitFont("Segoe UI", unitFontSize, QFont::Medium);
    painter.setFont(unitFont);
    painter.setPen(m_theme.textSecondary);
    
    QString unitText = "km/h";
    QFontMetrics unitFm(unitFont);
    QRect unitRect = unitFm.boundingRect(unitText);
    
    painter.drawText(
        static_cast<int>(m_center.x() - unitRect.width() / 2),
        static_cast<int>(displayY + speedRect.height() / 3 + unitRect.height() + 5),
        unitText
    );
    
    painter.restore();
}

// ==================== SpeedCard Implementation ====================

SpeedCard::SpeedCard(QWidget *parent)
    : QFrame(parent)
    , m_isDarkTheme(false)
{
    setFrameShape(QFrame::NoFrame);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);
}

void SpeedCard::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void SpeedCard::setTheme(const SpeedWidgetTheme& theme)
{
    m_theme = theme;
    update();
}

void SpeedCard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // Draw shadow
    QColor shadowColor = m_isDarkTheme ? QColor(0, 0, 0, 60) : QColor(0, 0, 0, 20);
    painter.setPen(Qt::NoPen);
    painter.setBrush(shadowColor);
    painter.drawRoundedRect(rect().adjusted(4, 4, 0, 0), 16, 16);
    
    // Draw card background
    QColor bgColor = m_isDarkTheme ? QColor(30, 41, 59) : QColor(255, 255, 255);
    QColor borderColor = m_isDarkTheme ? QColor(51, 65, 85) : QColor(226, 232, 240);
    
    painter.setBrush(bgColor);
    painter.setPen(QPen(borderColor, 1));
    painter.drawRoundedRect(rect().adjusted(0, 0, -4, -4), 16, 16);
}

// ==================== DigitalSpeedDisplay Implementation ====================

DigitalSpeedDisplay::DigitalSpeedDisplay(QWidget *parent)
    : QWidget(parent)
    , m_targetValue(0.0f)
    , m_displayValue(0.0f)
    , m_unit("km/h")
    , m_isDarkTheme(false)
{
    // No hard-coded minimum size to allow responsive layout
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    m_valueAnimation = new QPropertyAnimation(this, "displayValue", this);
    m_valueAnimation->setDuration(300);
    m_valueAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void DigitalSpeedDisplay::setValue(float value)
{
    m_targetValue = value;
    m_valueAnimation->stop();
    m_valueAnimation->setStartValue(m_displayValue);
    m_valueAnimation->setEndValue(value);
    m_valueAnimation->start();
}

void DigitalSpeedDisplay::setDisplayValue(float value)
{
    m_displayValue = value;
    update();
}

void DigitalSpeedDisplay::setUnit(const QString& unit)
{
    m_unit = unit;
    update();
}

void DigitalSpeedDisplay::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void DigitalSpeedDisplay::setTheme(const SpeedWidgetTheme& theme)
{
    m_theme = theme;
    update();
}

void DigitalSpeedDisplay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    
    // Draw background with subtle gradient
    QLinearGradient bgGradient(0, 0, 0, height());
    QColor bgColor = m_isDarkTheme ? QColor(15, 23, 42) : QColor(248, 250, 252);
    bgGradient.setColorAt(0, bgColor.lighter(105));
    bgGradient.setColorAt(1, bgColor);
    
    QColor borderColor = m_isDarkTheme ? QColor(51, 65, 85) : QColor(226, 232, 240);
    
    painter.setPen(QPen(borderColor, 2));
    painter.setBrush(bgGradient);
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 12, 12);
    
    // Draw value
    QColor textColor = m_isDarkTheme ? QColor(241, 245, 249) : QColor(30, 41, 59);
    QColor accentColor = m_isDarkTheme ? QColor(34, 211, 238) : QColor(6, 182, 212);
    
    int fontSize = qMin(width(), height()) / 3;
    QFont valueFont("Segoe UI", fontSize, QFont::Bold);
    painter.setFont(valueFont);
    painter.setPen(textColor);
    
    QString valueText = QString::number(static_cast<int>(m_displayValue));
    QFontMetrics fm(valueFont);
    QRect valueRect = fm.boundingRect(valueText);
    
    int totalWidth = valueRect.width() + 10;
    
    // Draw unit
    int unitFontSize = fontSize / 3;
    QFont unitFont("Segoe UI", unitFontSize, QFont::Medium);
    QFontMetrics unitFm(unitFont);
    QRect unitRect = unitFm.boundingRect(m_unit);
    totalWidth += unitRect.width();
    
    int startX = (width() - totalWidth) / 2;
    
    painter.drawText(
        startX,
        height() / 2 + valueRect.height() / 3,
        valueText
    );
    
    painter.setFont(unitFont);
    painter.setPen(accentColor);
    painter.drawText(
        startX + valueRect.width() + 8,
        height() / 2 + valueRect.height() / 3 - (valueRect.height() - unitRect.height()) / 2,
        m_unit
    );
}

// ==================== ModernSpeedButton Implementation ====================

ModernSpeedButton::ModernSpeedButton(const QString& text, QWidget *parent)
    : QPushButton(text, parent)
    , m_isDarkTheme(false)
    , m_hovered(false)
    , m_pressed(false)
{
    // Use responsive minimum size
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setMinimumHeight(40);  // Keep minimum height for usability
    setCursor(Qt::PointingHandCursor);
    setFocusPolicy(Qt::StrongFocus);
}

void ModernSpeedButton::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    update();
}

void ModernSpeedButton::setTheme(const SpeedWidgetTheme& theme)
{
    m_theme = theme;
    update();
}

void ModernSpeedButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    
    QColor primaryColor = m_isDarkTheme ? QColor(96, 165, 250) : QColor(59, 130, 246);
    QColor hoverColor = m_isDarkTheme ? QColor(59, 130, 246) : QColor(37, 99, 235);
    QColor pressedColor = m_isDarkTheme ? QColor(37, 99, 235) : QColor(29, 78, 216);
    
    QColor bgColor = primaryColor;
    if (m_pressed) {
        bgColor = pressedColor;
    } else if (m_hovered) {
        bgColor = hoverColor;
    }
    
    // Draw shadow
    if (!m_pressed) {
        QColor shadowColor(bgColor.red(), bgColor.green(), bgColor.blue(), 80);
        painter.setPen(Qt::NoPen);
        painter.setBrush(shadowColor);
        painter.drawRoundedRect(rect().adjusted(2, 3, -2, 0), 12, 12);
    }
    
    // Draw button background with gradient
    QLinearGradient bgGradient(0, 0, 0, height());
    bgGradient.setColorAt(0, bgColor);
    bgGradient.setColorAt(1, bgColor.darker(110));
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgGradient);
    
    QRect btnRect = m_pressed ? rect().adjusted(1, 2, -1, -1) : rect().adjusted(0, 0, -2, -3);
    painter.drawRoundedRect(btnRect, 12, 12);
    
    // Draw text
    painter.setPen(Qt::white);
    QFont btnFont("Segoe UI", 13, QFont::DemiBold);
    painter.setFont(btnFont);
    painter.drawText(btnRect, Qt::AlignCenter, text());
}

void ModernSpeedButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_hovered = true;
    update();
}

void ModernSpeedButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    m_hovered = false;
    update();
}

void ModernSpeedButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    m_pressed = true;
    update();
}

void ModernSpeedButton::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
    m_pressed = false;
    update();
}

// ==================== SpeedMeasurementWidget Implementation ====================

SpeedMeasurementWidget::SpeedMeasurementWidget(QWidget *parent)
    : QWidget(parent)
    , m_speedometer(nullptr)
    , m_actualSpeedLabel(nullptr)
    , m_topSpeedDisplay(nullptr)
    , m_topSpeedLabel(nullptr)
    , m_topSpeedCard(nullptr)
    , m_outputCard(nullptr)
    , m_resetButton(nullptr)
    , m_statusLabel(nullptr)
    , m_topSpeed(0.0f)
    , m_isDarkTheme(false)
{
    m_currentTheme = createLightTheme();
    setupUI();
}

SpeedMeasurementWidget::~SpeedMeasurementWidget()
{
}

SpeedWidgetTheme SpeedMeasurementWidget::createLightTheme() const
{
    SpeedWidgetTheme theme;
    
    // Background - classic off-white
    theme.backgroundColor = QColor(250, 250, 252);    // Off-white
    theme.cardBackground = QColor(255, 255, 255);     // White
    
    // Primary - classic monochrome
    theme.primaryColor = QColor(60, 60, 70);          // Charcoal
    theme.primaryHover = QColor(45, 45, 55);          // Darker charcoal
    theme.accentColor = QColor(80, 80, 90);           // Dark gray
    
    // Text
    theme.textPrimary = QColor(35, 35, 45);           // Near black
    theme.textSecondary = QColor(100, 100, 110);      // Medium gray
    theme.textMuted = QColor(140, 140, 150);          // Light gray
    
    // Gauge
    theme.gaugeBackground = QColor(245, 245, 248);    // Light gray
    theme.gaugeInnerRing = QColor(225, 225, 230);     // Silver
    theme.gaugeArcColor = QColor(200, 200, 210);      // Gray track
    theme.gaugeGlowColor = QColor(80, 80, 90);        // Dark gray
    theme.needleColor = QColor(35, 35, 45);           // Near black
    theme.needleGlow = QColor(60, 60, 70, 80);        // Charcoal alpha
    theme.centerCapColor = QColor(70, 70, 80);        // Dark gray
    
    // Zones - monochrome (not used in classic mode)
    theme.normalZone = QColor(100, 100, 110);
    theme.warningZone = QColor(120, 120, 130);
    theme.dangerZone = QColor(140, 140, 150);
    
    // Border
    theme.borderColor = QColor(220, 220, 228);        // Light border
    theme.borderFocus = QColor(80, 80, 90);           // Focus gray
    
    // Shadow
    theme.shadowColor = QColor(0, 0, 0, 15);
    
    // Button - elegant dark
    theme.buttonBackground = QColor(55, 55, 65);      // Charcoal button
    theme.buttonHover = QColor(40, 40, 50);           // Darker on hover
    theme.buttonText = QColor(255, 255, 255);         // White text
    
    return theme;
}

SpeedWidgetTheme SpeedMeasurementWidget::createDarkTheme() const
{
    SpeedWidgetTheme theme;
    
    // Background - classic dark
    theme.backgroundColor = QColor(18, 18, 22);       // Deep charcoal
    theme.cardBackground = QColor(28, 28, 34);        // Dark gray
    
    // Primary - elegant silver
    theme.primaryColor = QColor(200, 200, 210);       // Silver
    theme.primaryHover = QColor(220, 220, 230);       // Lighter silver
    theme.accentColor = QColor(180, 180, 190);        // Light gray
    
    // Text
    theme.textPrimary = QColor(240, 240, 245);        // Off-white
    theme.textSecondary = QColor(140, 140, 150);      // Medium gray
    theme.textMuted = QColor(100, 100, 110);          // Darker gray
    
    // Gauge
    theme.gaugeBackground = QColor(28, 28, 34);       // Dark gray
    theme.gaugeInnerRing = QColor(45, 45, 52);        // Medium gray
    theme.gaugeArcColor = QColor(55, 55, 62);         // Gray track
    theme.gaugeGlowColor = QColor(180, 180, 190);     // Light gray
    theme.needleColor = QColor(240, 240, 245);        // Off-white
    theme.needleGlow = QColor(200, 200, 210, 80);     // Silver alpha
    theme.centerCapColor = QColor(60, 60, 68);        // Dark gray
    
    // Zones - monochrome (not used in classic mode)
    theme.normalZone = QColor(140, 140, 150);
    theme.warningZone = QColor(160, 160, 170);
    theme.dangerZone = QColor(180, 180, 190);
    
    // Border
    theme.borderColor = QColor(50, 50, 58);           // Border gray
    theme.borderFocus = QColor(180, 180, 190);        // Light focus
    
    // Shadow
    theme.shadowColor = QColor(0, 0, 0, 50);
    
    // Button - elegant silver
    theme.buttonBackground = QColor(180, 180, 190);   // Silver button
    theme.buttonHover = QColor(200, 200, 210);        // Lighter on hover
    theme.buttonText = QColor(25, 25, 30);            // Dark text
    
    return theme;
}

void SpeedMeasurementWidget::setupUI()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(32);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    
    // ========== Left side: Speedometer with title ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(16);
    leftLayout->setAlignment(Qt::AlignTop);
    
    // Title section with icon-like element
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignCenter);
    titleLayout->setSpacing(12);
    
    // Speed icon indicator
    QLabel* iconLabel = new QLabel("\xE2\x9A\xA1");  // Lightning bolt emoji as icon
    iconLabel->setStyleSheet("font-size: 24px;");
    titleLayout->addWidget(iconLabel);
    
    m_actualSpeedLabel = new QLabel("Live Speed");
    QFont titleFont("Segoe UI", 22, QFont::Bold);
    m_actualSpeedLabel->setFont(titleFont);
    titleLayout->addWidget(m_actualSpeedLabel);
    
    leftLayout->addLayout(titleLayout);
    
    // Speedometer gauge - larger size for better visibility (responsive)
    m_speedometer = new SpeedometerGauge(this);
    // Size will be determined by layout based on available space
    leftLayout->addWidget(m_speedometer, 1, Qt::AlignCenter);
    
    // Status label below speedometer
    m_statusLabel = new QLabel("Waiting for data...");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    QFont statusFont("Segoe UI", 13, QFont::Normal);
    m_statusLabel->setFont(statusFont);
    leftLayout->addWidget(m_statusLabel);
    
    mainLayout->addLayout(leftLayout, 3);
    
    // ========== Right side: Top Speed and Controls ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(24);
    rightLayout->setAlignment(Qt::AlignTop);
    
    // Top Speed Card (responsive size)
    m_topSpeedCard = new SpeedCard(this);
    // Size will be determined by layout based on available space
    
    QVBoxLayout* topSpeedCardLayout = new QVBoxLayout(m_topSpeedCard);
    topSpeedCardLayout->setSpacing(16);
    topSpeedCardLayout->setContentsMargins(24, 24, 24, 24);
    
    // Card header with icon
    QHBoxLayout* topSpeedHeaderLayout = new QHBoxLayout();
    topSpeedHeaderLayout->setSpacing(12);
    
    QLabel* trophyIcon = new QLabel("\xF0\x9F\x8F\x86");  // Trophy emoji
    trophyIcon->setStyleSheet("font-size: 28px;");
    topSpeedHeaderLayout->addWidget(trophyIcon);
    
    m_topSpeedLabel = new QLabel("Top Speed");
    QFont topSpeedTitleFont("Segoe UI", 18, QFont::DemiBold);
    m_topSpeedLabel->setFont(topSpeedTitleFont);
    topSpeedHeaderLayout->addWidget(m_topSpeedLabel);
    topSpeedHeaderLayout->addStretch();
    
    topSpeedCardLayout->addLayout(topSpeedHeaderLayout);
    
    // Separator line
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFixedHeight(1);
    topSpeedCardLayout->addWidget(separator);
    
    // Digital display for top speed (responsive size)
    m_topSpeedDisplay = new DigitalSpeedDisplay(this);
    // Size will be determined by layout based on available space
    m_topSpeedDisplay->setValue(0);
    topSpeedCardLayout->addWidget(m_topSpeedDisplay, 0, Qt::AlignCenter);
    
    topSpeedCardLayout->addStretch();
    
    rightLayout->addWidget(m_topSpeedCard);
    
    // Output/Controls Card (responsive size)
    m_outputCard = new SpeedCard(this);
    // Size will be determined by layout based on available space
    
    QVBoxLayout* outputCardLayout = new QVBoxLayout(m_outputCard);
    outputCardLayout->setSpacing(20);
    outputCardLayout->setContentsMargins(24, 24, 24, 24);
    
    // Card header
    QHBoxLayout* outputHeaderLayout = new QHBoxLayout();
    outputHeaderLayout->setSpacing(12);
    
    QLabel* controlIcon = new QLabel("\xE2\x9A\x99\xEF\xB8\x8F");  // Gear emoji
    controlIcon->setStyleSheet("font-size: 24px;");
    outputHeaderLayout->addWidget(controlIcon);
    
    QLabel* outputLabel = new QLabel("Controls");
    QFont outputTitleFont("Segoe UI", 18, QFont::DemiBold);
    outputLabel->setFont(outputTitleFont);
    outputHeaderLayout->addWidget(outputLabel);
    outputHeaderLayout->addStretch();
    
    outputCardLayout->addLayout(outputHeaderLayout);
    
    // Separator
    QFrame* separator2 = new QFrame();
    separator2->setFrameShape(QFrame::HLine);
    separator2->setFixedHeight(1);
    outputCardLayout->addWidget(separator2);
    
    // Reset button
    m_resetButton = new ModernSpeedButton("Reset Top Speed", this);
    connect(m_resetButton, &QPushButton::clicked, this, &SpeedMeasurementWidget::onResetTopSpeed);
    outputCardLayout->addWidget(m_resetButton, 0, Qt::AlignCenter);
    
    outputCardLayout->addStretch();
    
    rightLayout->addWidget(m_outputCard);
    rightLayout->addStretch(1);
    
    mainLayout->addLayout(rightLayout, 2);
    
    applyTheme();
}

void SpeedMeasurementWidget::updateSpeed(float speed)
{
    if (m_speedometer) {
        m_speedometer->setSpeed(speed);
    }
    
    // Update status label
    if (m_statusLabel) {
        if (speed > 0.5f) {
            m_statusLabel->setText(QString("Tracking active - %1 km/h").arg(QString::number(speed, 'f', 1)));
        } else {
            m_statusLabel->setText("No movement detected");
        }
    }
    
    updateTopSpeed(speed);
}

void SpeedMeasurementWidget::updateFromTargets(const TargetTrackData& targets)
{
    float maxSpeed = 0.0f;
    
    for (size_t i = 0; i < targets.numTracks && i < targets.targets.size(); ++i) {
        const auto& target = targets.targets[i];
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
        if (m_topSpeedDisplay) {
            m_topSpeedDisplay->setValue(m_topSpeed);
        }
    }
}

void SpeedMeasurementWidget::onResetTopSpeed()
{
    m_topSpeed = 0.0f;
    if (m_topSpeedDisplay) {
        m_topSpeedDisplay->setValue(0);
    }
    if (m_speedometer) {
        m_speedometer->setSpeed(0.0f);
    }
    if (m_statusLabel) {
        m_statusLabel->setText("Top speed reset");
    }
}

void SpeedMeasurementWidget::setDarkTheme(bool isDark)
{
    m_isDarkTheme = isDark;
    m_currentTheme = isDark ? createDarkTheme() : createLightTheme();
    
    if (m_speedometer) {
        m_speedometer->setDarkTheme(isDark);
        m_speedometer->setTheme(m_currentTheme);
    }
    if (m_topSpeedCard) {
        m_topSpeedCard->setDarkTheme(isDark);
        m_topSpeedCard->setTheme(m_currentTheme);
    }
    if (m_outputCard) {
        m_outputCard->setDarkTheme(isDark);
        m_outputCard->setTheme(m_currentTheme);
    }
    if (m_topSpeedDisplay) {
        m_topSpeedDisplay->setDarkTheme(isDark);
        m_topSpeedDisplay->setTheme(m_currentTheme);
    }
    if (m_resetButton) {
        m_resetButton->setDarkTheme(isDark);
        m_resetButton->setTheme(m_currentTheme);
    }
    
    applyTheme();
}

void SpeedMeasurementWidget::applyTheme()
{
    QString textColor = m_isDarkTheme ? "#f1f5f9" : "#1e293b";
    QString secondaryColor = m_isDarkTheme ? "#94a3b8" : "#64748b";
    QString mutedColor = m_isDarkTheme ? "#64748b" : "#94a3b8";
    QString accentColor = m_isDarkTheme ? "#22d3ee" : "#06b6d4";
    QString separatorColor = m_isDarkTheme ? "#334155" : "#e2e8f0";
    
    if (m_actualSpeedLabel) {
        m_actualSpeedLabel->setStyleSheet(QString("QLabel { color: %1; background: transparent; }").arg(textColor));
    }
    
    if (m_topSpeedLabel) {
        m_topSpeedLabel->setStyleSheet(QString("QLabel { color: %1; background: transparent; }").arg(textColor));
    }
    
    if (m_statusLabel) {
        m_statusLabel->setStyleSheet(QString("QLabel { color: %1; background: transparent; font-style: italic; }").arg(mutedColor));
    }
    
    // Style separators
    QString separatorStyle = QString("QFrame { background-color: %1; border: none; }").arg(separatorColor);
    
    // Find and style all separators
    QList<QFrame*> frames = findChildren<QFrame*>();
    for (QFrame* frame : frames) {
        if (frame->frameShape() == QFrame::HLine) {
            frame->setStyleSheet(separatorStyle);
        }
    }
    
    // Style labels in cards
    QList<QLabel*> labels = findChildren<QLabel*>();
    for (QLabel* label : labels) {
        if (label != m_actualSpeedLabel && label != m_topSpeedLabel && label != m_statusLabel) {
            // Check if it's an icon label (contains emoji)
            QString text = label->text();
            if (text.contains("\xE2") || text.contains("\xF0")) {
                // Emoji icon - keep default
                label->setStyleSheet("QLabel { background: transparent; }");
            } else if (label->font().weight() >= QFont::DemiBold) {
                // Title labels
                label->setStyleSheet(QString("QLabel { color: %1; background: transparent; }").arg(textColor));
            } else {
                // Regular labels
                label->setStyleSheet(QString("QLabel { color: %1; background: transparent; }").arg(secondaryColor));
            }
        }
    }
    
    update();
}
