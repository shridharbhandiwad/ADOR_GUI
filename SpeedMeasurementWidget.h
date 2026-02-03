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
#include "DataStructures.h"

class SpeedometerGauge : public QWidget
{
    Q_OBJECT

public:
    explicit SpeedometerGauge(QWidget *parent = nullptr);
    
    void setSpeed(float speed);
    float getSpeed() const { return m_currentSpeed; }
    void setMaxSpeed(float maxSpeed);
    float getMaxSpeed() const { return m_maxSpeed; }
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawBackground(QPainter& painter);
    void drawOuterRing(QPainter& painter);
    void drawSpeedScale(QPainter& painter);
    void drawNeedle(QPainter& painter);
    void drawCenterCap(QPainter& painter);
    void drawSpeedMarkers(QPainter& painter);
    
    // Theme-aware color methods
    QColor getBackgroundColor() const;
    QColor getOuterRingColor() const;
    QColor getInnerBackgroundColor() const;
    QColor getScaleTextColor() const;
    QColor getNeedleColor() const;
    QColor getCenterCapColor() const;
    QColor getHighSpeedZoneColor() const;
    
    float m_currentSpeed;      // Current speed in km/h
    float m_maxSpeed;          // Maximum speed on scale
    float m_animatedSpeed;     // For smooth animation
    bool m_isDarkTheme;
    
    QRect m_gaugeRect;
    QPointF m_center;
    float m_radius;
    
    // Animation timer
    QTimer* m_animationTimer;
    
    // Constants
    static constexpr float START_ANGLE = 225.0f;   // Starting angle (bottom-left)
    static constexpr float SWEEP_ANGLE = -270.0f;  // Sweep clockwise (270 degrees total)
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
    
    // UI Components
    SpeedometerGauge* m_speedometer;
    QLabel* m_actualSpeedLabel;
    QLabel* m_topSpeedValue;
    QLabel* m_topSpeedLabel;
    QGroupBox* m_outputGroup;
    QPushButton* m_resetButton;
    
    // Data
    float m_topSpeed;
    bool m_isDarkTheme;
    
    // Theme-aware styling
    void applyTheme();
};

#endif // SPEEDMEASUREMENTWIDGET_H
