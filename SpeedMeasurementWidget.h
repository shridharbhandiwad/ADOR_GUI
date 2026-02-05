#ifndef SPEEDMEASUREMENTWIDGET_H
#define SPEEDMEASUREMENTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFrame>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include "DataStructures.h"

// Theme definition structure for consistent theming
struct SpeedWidgetTheme {
    // Background colors
    QColor backgroundColor;
    QColor cardBackground;
    
    // Primary colors
    QColor primaryColor;
    QColor primaryHover;
    QColor accentColor;
    
    // Text colors
    QColor textPrimary;
    QColor textSecondary;
    QColor textMuted;
    
    // Gauge colors
    QColor gaugeBackground;
    QColor gaugeInnerRing;
    QColor gaugeArcColor;
    QColor gaugeGlowColor;
    QColor needleColor;
    QColor needleGlow;
    QColor centerCapColor;
    
    // Speed zones
    QColor normalZone;
    QColor warningZone;
    QColor dangerZone;
    
    // Border colors
    QColor borderColor;
    QColor borderFocus;
    
    // Shadow
    QColor shadowColor;
    
    // Button colors
    QColor buttonBackground;
    QColor buttonHover;
    QColor buttonText;
};

class SpeedometerGauge : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float animatedSpeed READ getAnimatedSpeed WRITE setAnimatedSpeed)
    Q_PROPERTY(float glowIntensity READ getGlowIntensity WRITE setGlowIntensity)

public:
    explicit SpeedometerGauge(QWidget *parent = nullptr);
    
    void setSpeed(float speed);
    float getSpeed() const { return m_currentSpeed; }
    float getAnimatedSpeed() const { return m_animatedSpeed; }
    void setAnimatedSpeed(float speed);
    float getGlowIntensity() const { return m_glowIntensity; }
    void setGlowIntensity(float intensity);
    void setMaxSpeed(float maxSpeed);
    float getMaxSpeed() const { return m_maxSpeed; }
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }
    void setTheme(const SpeedWidgetTheme& theme);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawBackground(QPainter& painter);
    void drawOuterGlow(QPainter& painter);
    void drawGaugeArc(QPainter& painter);
    void drawProgressArc(QPainter& painter);
    void drawTickMarks(QPainter& painter);
    void drawSpeedLabels(QPainter& painter);
    void drawNeedle(QPainter& painter);
    void drawCenterHub(QPainter& painter);
    void drawDigitalDisplay(QPainter& painter);
    void drawSpeedZones(QPainter& painter);
    
    void updateThemeColors();
    
    float m_currentSpeed;
    float m_maxSpeed;
    float m_animatedSpeed;
    float m_glowIntensity;
    bool m_isDarkTheme;
    
    SpeedWidgetTheme m_theme;
    
    QRect m_gaugeRect;
    QPointF m_center;
    float m_radius;
    
    QTimer* m_animationTimer;
    QPropertyAnimation* m_glowAnimation;
    
    // Constants for gauge geometry
    static constexpr float START_ANGLE = 225.0f;
    static constexpr float SWEEP_ANGLE = -270.0f;
    static constexpr float WARNING_THRESHOLD = 0.7f;  // 70% of max speed
    static constexpr float DANGER_THRESHOLD = 0.85f;  // 85% of max speed
};

// Modern styled card widget for sections
class SpeedCard : public QFrame
{
    Q_OBJECT

public:
    explicit SpeedCard(QWidget *parent = nullptr);
    void setDarkTheme(bool isDark);
    void setTheme(const SpeedWidgetTheme& theme);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isDarkTheme;
    SpeedWidgetTheme m_theme;
};

// Digital display widget with animated numbers
class DigitalSpeedDisplay : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float displayValue READ getDisplayValue WRITE setDisplayValue)

public:
    explicit DigitalSpeedDisplay(QWidget *parent = nullptr);
    
    void setValue(float value);
    float getDisplayValue() const { return m_displayValue; }
    void setDisplayValue(float value);
    void setUnit(const QString& unit);
    void setDarkTheme(bool isDark);
    void setTheme(const SpeedWidgetTheme& theme);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float m_targetValue;
    float m_displayValue;
    QString m_unit;
    bool m_isDarkTheme;
    SpeedWidgetTheme m_theme;
    QPropertyAnimation* m_valueAnimation;
};

// Modern styled button
class ModernSpeedButton : public QPushButton
{
    Q_OBJECT

public:
    explicit ModernSpeedButton(const QString& text, QWidget *parent = nullptr);
    void setDarkTheme(bool isDark);
    void setTheme(const SpeedWidgetTheme& theme);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_isDarkTheme;
    bool m_hovered;
    bool m_pressed;
    SpeedWidgetTheme m_theme;
};

class SpeedMeasurementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedMeasurementWidget(QWidget *parent = nullptr);
    ~SpeedMeasurementWidget();
    
    void updateSpeed(float speed);
    void updateFromTargets(const TargetTrackData& targets);
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }

public slots:
    void onResetTopSpeed();

private:
    void setupUI();
    void updateTopSpeed(float speed);
    void applyTheme();
    SpeedWidgetTheme createLightTheme() const;
    SpeedWidgetTheme createDarkTheme() const;
    
    // UI Components
    SpeedometerGauge* m_speedometer;
    QLabel* m_actualSpeedLabel;
    DigitalSpeedDisplay* m_topSpeedDisplay;
    QLabel* m_topSpeedLabel;
    SpeedCard* m_topSpeedCard;
    SpeedCard* m_outputCard;
    ModernSpeedButton* m_resetButton;
    QLabel* m_statusLabel;
    
    // Data
    float m_topSpeed;
    bool m_isDarkTheme;
    SpeedWidgetTheme m_currentTheme;
};

#endif // SPEEDMEASUREMENTWIDGET_H
