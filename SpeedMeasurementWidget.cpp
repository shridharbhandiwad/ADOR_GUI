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
    // Use expanding size policy to adapt to available space
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
    // Fill entire background first
    painter.fillRect(rect(), m_theme.backgroundColor);
    
    // Draw premium radial gradient background
    QRadialGradient bgGradient(m_center, m_radius * 1.3f);
    bgGradient.setColorAt(0, m_theme.cardBackground);
    bgGradient.setColorAt(0.6, m_theme.cardBackground);
    bgGradient.setColorAt(1, m_theme.backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgGradient);
    painter.drawRect(rect());
    
    // Outer decorative ring for premium feel
    float outerRingRadius = m_radius * 0.96f;
    QPen outerRingPen(m_theme.gaugeArcColor, 1.5);
    painter.setPen(outerRingPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(m_center, outerRingRadius, outerRingRadius);
    
    // Inner subtle circle
    float innerRingRadius = m_radius * 0.72f;
    QPen innerRingPen(m_theme.gaugeArcColor.lighter(110), 0.8);
    painter.setPen(innerRingPen);
    painter.drawEllipse(m_center, innerRingRadius, innerRingRadius);
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
    
    int fontSize = static_cast<int>(m_radius * 0.09f);
    QFont labelFont("Segoe UI", fontSize, QFont::Bold);
    labelFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
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
    
    // Speed value - larger enterprise font
    int fontSize = static_cast<int>(m_radius * 0.26f);
    QFont speedFont("Segoe UI", fontSize, QFont::Bold);
    speedFont.setLetterSpacing(QFont::AbsoluteSpacing, 2.0);
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
    
    // Unit label - refined enterprise style
    int unitFontSize = static_cast<int>(m_radius * 0.09f);
    QFont unitFont("Segoe UI", unitFontSize, QFont::DemiBold);
    unitFont.setLetterSpacing(QFont::AbsoluteSpacing, 3.0);
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
    
    int radius = 14;
    QRect cardRect = rect().adjusted(3, 3, -3, -3);
    
    // Multi-layered shadow for depth (enterprise aesthetic)
    // Outer soft shadow
    QColor outerShadow = m_isDarkTheme ? QColor(0, 0, 0, 50) : QColor(0, 0, 0, 12);
    painter.setPen(Qt::NoPen);
    painter.setBrush(outerShadow);
    painter.drawRoundedRect(cardRect.adjusted(2, 4, 2, 4), radius, radius);
    
    // Inner tighter shadow
    QColor innerShadow = m_isDarkTheme ? QColor(0, 0, 0, 35) : QColor(0, 0, 0, 8);
    painter.setBrush(innerShadow);
    painter.drawRoundedRect(cardRect.adjusted(1, 2, 1, 2), radius, radius);
    
    // Card background with subtle gradient
    QLinearGradient bgGradient(0, cardRect.top(), 0, cardRect.bottom());
    if (m_isDarkTheme) {
        bgGradient.setColorAt(0, QColor(30, 30, 40));
        bgGradient.setColorAt(0.5, QColor(26, 26, 34));
        bgGradient.setColorAt(1, QColor(22, 22, 30));
    } else {
        bgGradient.setColorAt(0, QColor(255, 255, 255));
        bgGradient.setColorAt(0.5, QColor(252, 252, 254));
        bgGradient.setColorAt(1, QColor(248, 248, 252));
    }
    
    QColor borderColor = m_isDarkTheme ? QColor(52, 52, 64) : QColor(215, 215, 228);
    
    painter.setBrush(bgGradient);
    painter.setPen(QPen(borderColor, 1.5));
    painter.drawRoundedRect(cardRect, radius, radius);
    
    // Subtle top highlight line for glass effect
    QPainterPath topHighlight;
    topHighlight.addRoundedRect(QRectF(cardRect.left() + 10, cardRect.top(), cardRect.width() - 20, 1), 0, 0);
    QColor highlightColor = m_isDarkTheme ? QColor(255, 255, 255, 15) : QColor(255, 255, 255, 200);
    painter.setPen(Qt::NoPen);
    painter.setBrush(highlightColor);
    painter.drawPath(topHighlight);
}

// ==================== DigitalSpeedDisplay Implementation ====================

DigitalSpeedDisplay::DigitalSpeedDisplay(QWidget *parent)
    : QWidget(parent)
    , m_targetValue(0.0f)
    , m_displayValue(0.0f)
    , m_unit("km/h")
    , m_isDarkTheme(false)
{
    // Use expanding size policy to adapt to available space
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
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
    
    int radius = 10;
    QRect displayRect = rect().adjusted(2, 2, -2, -2);
    
    // Enterprise background with refined gradient
    QLinearGradient bgGradient(0, 0, 0, height());
    if (m_isDarkTheme) {
        bgGradient.setColorAt(0, QColor(20, 20, 28));
        bgGradient.setColorAt(0.5, QColor(16, 16, 24));
        bgGradient.setColorAt(1, QColor(12, 12, 20));
    } else {
        bgGradient.setColorAt(0, QColor(250, 250, 254));
        bgGradient.setColorAt(0.5, QColor(246, 246, 252));
        bgGradient.setColorAt(1, QColor(242, 242, 248));
    }
    
    QColor borderColor = m_isDarkTheme ? QColor(48, 48, 60) : QColor(210, 210, 224);
    
    // Subtle inset shadow
    QColor insetShadow = m_isDarkTheme ? QColor(0, 0, 0, 40) : QColor(0, 0, 0, 10);
    painter.setPen(Qt::NoPen);
    painter.setBrush(insetShadow);
    painter.drawRoundedRect(displayRect.adjusted(0, 0, 1, 1), radius, radius);
    
    painter.setPen(QPen(borderColor, 1.5));
    painter.setBrush(bgGradient);
    painter.drawRoundedRect(displayRect, radius, radius);
    
    // Speed value - large, bold, enterprise font
    QColor textColor = m_isDarkTheme ? QColor(235, 235, 245) : QColor(28, 28, 42);
    QColor unitColor = m_isDarkTheme ? QColor(130, 130, 145) : QColor(90, 90, 108);
    
    int fontSize = qMax(qMin(width(), height()) / 3, 20);
    QFont valueFont("Segoe UI", fontSize, QFont::Bold);
    valueFont.setLetterSpacing(QFont::AbsoluteSpacing, 2.0);
    painter.setFont(valueFont);
    painter.setPen(textColor);
    
    QString valueText = QString::number(static_cast<int>(m_displayValue));
    QFontMetrics fm(valueFont);
    QRect valueRect = fm.boundingRect(valueText);
    
    int totalWidth = valueRect.width() + 12;
    
    // Unit text
    int unitFontSize = qMax(fontSize / 3, 10);
    QFont unitFont("Segoe UI", unitFontSize, QFont::Medium);
    unitFont.setLetterSpacing(QFont::AbsoluteSpacing, 1.0);
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
    painter.setPen(unitColor);
    painter.drawText(
        startX + valueRect.width() + 10,
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
    
    // Enterprise monochrome button palette
    QColor primaryColor = m_isDarkTheme ? QColor(160, 160, 175) : QColor(50, 50, 62);
    QColor hoverColor = m_isDarkTheme ? QColor(180, 180, 195) : QColor(38, 38, 50);
    QColor pressedColor = m_isDarkTheme ? QColor(140, 140, 155) : QColor(30, 30, 42);
    QColor textCol = m_isDarkTheme ? QColor(22, 22, 30) : QColor(255, 255, 255);
    
    QColor bgColor = primaryColor;
    if (m_pressed) {
        bgColor = pressedColor;
    } else if (m_hovered) {
        bgColor = hoverColor;
    }
    
    int radius = 10;
    QRect btnRect = m_pressed ? rect().adjusted(1, 1, -1, -1) : rect().adjusted(0, 0, -2, -2);
    
    // Enterprise shadow - subtle, professional
    if (!m_pressed) {
        QColor shadowColor(0, 0, 0, m_isDarkTheme ? 60 : 30);
        painter.setPen(Qt::NoPen);
        painter.setBrush(shadowColor);
        painter.drawRoundedRect(btnRect.adjusted(1, 3, 1, 3), radius, radius);
    }
    
    // Button background with subtle gradient
    QLinearGradient bgGradient(0, btnRect.top(), 0, btnRect.bottom());
    bgGradient.setColorAt(0, bgColor.lighter(108));
    bgGradient.setColorAt(0.45, bgColor);
    bgGradient.setColorAt(1, bgColor.darker(108));
    
    painter.setPen(QPen(bgColor.darker(120), 1.0));
    painter.setBrush(bgGradient);
    painter.drawRoundedRect(btnRect, radius, radius);
    
    // Top highlight for a subtle inset/glass effect
    if (!m_pressed) {
        QLinearGradient highlightGrad(0, btnRect.top(), 0, btnRect.top() + btnRect.height() / 2);
        highlightGrad.setColorAt(0, QColor(255, 255, 255, m_isDarkTheme ? 25 : 50));
        highlightGrad.setColorAt(1, QColor(255, 255, 255, 0));
        painter.setPen(Qt::NoPen);
        painter.setBrush(highlightGrad);
        painter.drawRoundedRect(btnRect.adjusted(1, 1, -1, -btnRect.height()/2), radius, radius);
    }
    
    // Draw text - enterprise font
    painter.setPen(textCol);
    QFont btnFont("Segoe UI", 14, QFont::DemiBold);
    btnFont.setLetterSpacing(QFont::AbsoluteSpacing, 1.5);
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
    // Use expanding size policy to adapt to available space
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(40);
    mainLayout->setContentsMargins(32, 28, 32, 28);
    
    // ========== Left side: Speedometer with title ==========
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->setSpacing(18);
    leftLayout->setAlignment(Qt::AlignTop);
    
    // Title section - clean enterprise text, no icons
    m_actualSpeedLabel = new QLabel("LIVE SPEED");
    m_actualSpeedLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(m_actualSpeedLabel);
    
    // Speedometer gauge - doubled size for enterprise visibility
    m_speedometer = new SpeedometerGauge(this);
    leftLayout->addWidget(m_speedometer, 1, Qt::AlignCenter);
    
    // Status label below speedometer
    m_statusLabel = new QLabel("Waiting for data...");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(m_statusLabel);
    
    mainLayout->addLayout(leftLayout, 3);
    
    // ========== Right side: Top Speed and Controls ==========
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(28);
    rightLayout->setAlignment(Qt::AlignTop);
    
    // Top Speed Card
    m_topSpeedCard = new SpeedCard(this);
    m_topSpeedCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QVBoxLayout* topSpeedCardLayout = new QVBoxLayout(m_topSpeedCard);
    topSpeedCardLayout->setSpacing(18);
    topSpeedCardLayout->setContentsMargins(28, 28, 28, 28);
    
    // Card header - clean text, no emoji
    m_topSpeedLabel = new QLabel("TOP SPEED");
    topSpeedCardLayout->addWidget(m_topSpeedLabel);
    
    // Separator line
    QFrame* separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFixedHeight(2);
    topSpeedCardLayout->addWidget(separator);
    
    // Digital display for top speed
    m_topSpeedDisplay = new DigitalSpeedDisplay(this);
    m_topSpeedDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_topSpeedDisplay->setValue(0);
    topSpeedCardLayout->addWidget(m_topSpeedDisplay, 0, Qt::AlignCenter);
    
    topSpeedCardLayout->addStretch();
    
    rightLayout->addWidget(m_topSpeedCard);
    
    // Controls Card
    m_outputCard = new SpeedCard(this);
    m_outputCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QVBoxLayout* outputCardLayout = new QVBoxLayout(m_outputCard);
    outputCardLayout->setSpacing(22);
    outputCardLayout->setContentsMargins(28, 28, 28, 28);
    
    // Card header - clean text, no emoji
    QLabel* outputLabel = new QLabel("CONTROLS");
    outputCardLayout->addWidget(outputLabel);
    
    // Separator
    QFrame* separator2 = new QFrame();
    separator2->setFrameShape(QFrame::HLine);
    separator2->setFixedHeight(2);
    outputCardLayout->addWidget(separator2);
    
    // Reset button
    m_resetButton = new ModernSpeedButton("Reset Top Speed", this);
    m_resetButton->setMinimumHeight(48);
    m_resetButton->setMinimumWidth(220);
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
    // Enterprise color palette
    QString bgColor = m_isDarkTheme ? "#12121a" : "#f8f9fc";
    QString cardBg = m_isDarkTheme ? "#1c1c26" : "#ffffff";
    QString textColor = m_isDarkTheme ? "#e8e8f0" : "#1a1a2e";
    QString secondaryColor = m_isDarkTheme ? "#9898a8" : "#5a5a72";
    QString mutedColor = m_isDarkTheme ? "#6a6a7e" : "#8a8aa0";
    QString accentColor = m_isDarkTheme ? "#a0a0b0" : "#3c3c4a";
    QString separatorColor = m_isDarkTheme ? "#2a2a36" : "#e0e0ea";
    QString separatorAccent = m_isDarkTheme ? "#3a3a48" : "#c8c8d8";
    QString cardBorder = m_isDarkTheme ? "#2e2e3a" : "#d8d8e4";
    QString headerUnderline = m_isDarkTheme ? "#4a4a58" : "#b0b0c0";
    
    // Apply main widget background
    setStyleSheet(QString(
        "SpeedMeasurementWidget {"
        "  background-color: %1;"
        "}"
    ).arg(bgColor));
    
    // Enterprise "LIVE SPEED" header - large, tracked, uppercase
    if (m_actualSpeedLabel) {
        m_actualSpeedLabel->setStyleSheet(QString(
            "QLabel {"
            "  color: %1;"
            "  background: transparent;"
            "  font-family: 'Segoe UI', 'Helvetica Neue', 'Arial', sans-serif;"
            "  font-size: 20px;"
            "  font-weight: 700;"
            "  letter-spacing: 6px;"
            "  padding: 8px 20px;"
            "  border-bottom: 3px solid %2;"
            "}"
        ).arg(textColor, headerUnderline));
    }
    
    // Enterprise "TOP SPEED" header
    if (m_topSpeedLabel) {
        m_topSpeedLabel->setStyleSheet(QString(
            "QLabel {"
            "  color: %1;"
            "  background: transparent;"
            "  font-family: 'Segoe UI', 'Helvetica Neue', 'Arial', sans-serif;"
            "  font-size: 18px;"
            "  font-weight: 700;"
            "  letter-spacing: 5px;"
            "  padding-bottom: 6px;"
            "}"
        ).arg(textColor));
    }
    
    // Status label - refined, subtle
    if (m_statusLabel) {
        m_statusLabel->setStyleSheet(QString(
            "QLabel {"
            "  color: %1;"
            "  background: transparent;"
            "  font-family: 'Segoe UI', 'Helvetica Neue', 'Arial', sans-serif;"
            "  font-size: 14px;"
            "  font-weight: 400;"
            "  letter-spacing: 1px;"
            "  font-style: italic;"
            "  padding: 6px 12px;"
            "}"
        ).arg(mutedColor));
    }
    
    // Separator styling - refined thin lines with subtle gradient feel
    QString separatorStyle = QString(
        "QFrame {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "    stop:0 transparent, stop:0.1 %1, stop:0.9 %1, stop:1 transparent);"
        "  border: none;"
        "  max-height: 2px;"
        "  min-height: 2px;"
        "}"
    ).arg(separatorAccent);
    
    QList<QFrame*> frames = findChildren<QFrame*>();
    for (QFrame* frame : frames) {
        if (frame->frameShape() == QFrame::HLine) {
            frame->setStyleSheet(separatorStyle);
        }
    }
    
    // Style all child labels in cards - "CONTROLS" and any others
    QList<QLabel*> labels = findChildren<QLabel*>();
    for (QLabel* label : labels) {
        if (label != m_actualSpeedLabel && label != m_topSpeedLabel && label != m_statusLabel) {
            QString text = label->text();
            // Detect section headers by uppercase text
            if (text == "CONTROLS" || text == "TOP SPEED" || text == "LIVE SPEED") {
                label->setStyleSheet(QString(
                    "QLabel {"
                    "  color: %1;"
                    "  background: transparent;"
                    "  font-family: 'Segoe UI', 'Helvetica Neue', 'Arial', sans-serif;"
                    "  font-size: 18px;"
                    "  font-weight: 700;"
                    "  letter-spacing: 5px;"
                    "  padding-bottom: 6px;"
                    "}"
                ).arg(textColor));
            } else {
                label->setStyleSheet(QString(
                    "QLabel {"
                    "  color: %1;"
                    "  background: transparent;"
                    "  font-family: 'Segoe UI', 'Helvetica Neue', 'Arial', sans-serif;"
                    "  font-size: 14px;"
                    "  font-weight: 400;"
                    "  letter-spacing: 0.5px;"
                    "}"
                ).arg(secondaryColor));
            }
        }
    }
    
    update();
}
