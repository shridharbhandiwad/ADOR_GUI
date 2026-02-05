#pragma once

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <vector>
#include "DataStructures.h"

class PPIWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PPIWidget(QWidget *parent = nullptr);

    void updateTargets(const TargetTrackData& trackData);
    void setMaxRange(float range);
    void setMinRange(float range);
    void setFoVAngle(float angle);  // NEW: Set Field of View angle
    void setMinAngle(float angle);  // NEW: Set minimum display angle
    void setMaxAngle(float angle);  // NEW: Set maximum display angle
    void setDarkTheme(bool isDark); // NEW: Set dark/light theme

    float getMaxRange() const { return m_maxRange; }
    float getMinRange() const { return m_minRange; }
    float getFoVAngle() const { return m_fovAngle; }  // NEW: Get FoV angle
    float getMinAngle() const { return m_minAngle; }  // NEW: Get min angle
    float getMaxAngle() const { return m_maxAngle; }  // NEW: Get max angle
    bool isDarkTheme() const { return m_isDarkTheme; } // NEW: Get current theme
    
    // Settings persistence
    void saveSettings();
    void loadSettings();
    
    // Get settings panel widget
    QWidget* getSettingsPanel() { return m_settingsPanel; }

public slots:
    void onSettingsToggled();
    void onMinRangeChanged(double value);
    void onMaxRangeChanged(double value);
    void onMinAngleChanged(double value);
    void onMaxAngleChanged(double value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    // Drawing functions
    void drawBackground(QPainter& painter);
    void drawFoVHighlight(QPainter& painter);   // NEW: Draw FoV sector highlight
    void drawFoVBoundaries(QPainter& painter);  // NEW: Draw FoV boundary lines
    void drawRangeRings(QPainter& painter);
    void drawAzimuthLines(QPainter& painter);
    void drawTargets(QPainter& painter);
    void drawLabels(QPainter& painter);
    void drawHoverTooltip(QPainter& painter);   // Draw transparent hover tooltip

    // Utility functions
    QColor getTargetColor(float radialSpeed) const;
    QPointF polarToCartesian(float range, float azimuth) const;
    int findTrackAtPosition(const QPointF& pos) const;  // Find track under mouse position
    
    // Settings panel setup
    void setupSettingsPanel();
    void applyTheme();

    // Data members
    TargetTrackData m_currentTargets;

    // Display parameters
    float m_maxRange;           // Maximum range to display (meters)
    float m_minRange;           // Minimum range to display (meters)
    float m_fovAngle;           // NEW: Field of View half-angle (e.g., 20° for ±20°)
    float m_minAngle;           // NEW: Minimum angle for display filtering (degrees)
    float m_maxAngle;           // NEW: Maximum angle for display filtering (degrees)
    float m_plotRadius;         // Radius of the plot area in pixels
    QPointF m_center;           // Center point of the plot
    QRect m_plotRect;           // Bounding rectangle of the plot

    // Hover tracking
    int m_hoveredTrackIndex;    // Index of currently hovered track (-1 if none)
    QPointF m_hoverPosition;    // Current mouse position for tooltip placement

    // Theme support
    bool m_isDarkTheme;         // Current theme state
    
    // Settings panel
    QWidget* m_settingsPanel;
    QPushButton* m_settingsBtn;
    QDoubleSpinBox* m_minRangeSpinBox;
    QDoubleSpinBox* m_maxRangeSpinBox;
    QDoubleSpinBox* m_minAngleSpinBox;
    QDoubleSpinBox* m_maxAngleSpinBox;
    
    // Theme-aware color helper methods
    QColor getBackgroundColor() const;
    QColor getPlotBackgroundColor() const;
    QColor getBorderColor() const;
    QColor getGridColor() const;
    QColor getFovColor() const;
    QColor getFovBorderColor() const;
    QColor getTextColor() const;
    QColor getSecondaryTextColor() const;
    QColor getMutedTextColor() const;
    QColor getPrimaryBlueColor() const;
    QColor getSuccessGreenColor() const;
    QColor getErrorRedColor() const;
    QColor getTooltipBgColor() const;
    QColor getTooltipTextColor() const;

    // Constants for radar display
    static const int NUM_RANGE_RINGS = 5;      // Number of concentric range rings
    static const int NUM_AZIMUTH_LINES = 19;   // Number of radial azimuth lines

    // Coordinate system constants (full display range)
    static constexpr float MIN_AZIMUTH = -90.0f;    // Minimum azimuth angle for display
    static constexpr float MAX_AZIMUTH = 90.0f;     // Maximum azimuth angle for display
};


