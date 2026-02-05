#ifndef TIMESERIESPLOTSWIDGET_H
#define TIMESERIESPLOTSWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include <QVector>
#include <QPainter>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QToolTip>
#include "DataStructures.h"

// Structure to hold time series data point
struct TimeSeriesDataPoint {
    qint64 timestamp;  // milliseconds since epoch
    float velocity;    // km/h
    float range;       // meters
};

// Base class for time series plots
class TimeSeriesPlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimeSeriesPlotWidget(QWidget *parent = nullptr);
    
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }
    void setYAxisLabel(const QString& label);
    void setYAxisUnit(const QString& unit);
    void setYAxisRange(float minY, float maxY);
    void setTimeWindowSeconds(int seconds);
    void setPointSize(int size);
    void clearData();
    
    // Add data point
    void addDataPoint(qint64 timestamp, float value);
    
    // Get current settings
    float getMinY() const { return m_minY; }
    float getMaxY() const { return m_maxY; }
    int getTimeWindowSeconds() const { return m_timeWindowSeconds; }
    int getPointSize() const { return m_pointSize; }
    
public slots:
    void onResetZoom();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void drawBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawAxes(QPainter& painter);
    void drawData(QPainter& painter);
    void drawLabels(QPainter& painter);
    void drawTooltip(QPainter& painter);
    
    // Helper functions
    QPointF dataToScreen(qint64 timestamp, float value) const;
    bool screenToData(const QPoint& screenPos, qint64& timestamp, float& value) const;
    int findNearestPoint(const QPoint& pos) const;
    
    // Theme colors
    QColor getBackgroundColor() const;
    QColor getPlotBackgroundColor() const;
    QColor getBorderColor() const;
    QColor getGridColor() const;
    QColor getTextColor() const;
    QColor getDataLineColor() const;
    QColor getDataFillColor() const;
    
    // Data storage
    QVector<QPair<qint64, float>> m_dataPoints;
    
    // Configuration
    QString m_yAxisLabel;
    QString m_yAxisUnit;
    float m_minY;
    float m_maxY;
    float m_defaultMinY;
    float m_defaultMaxY;
    int m_timeWindowSeconds;  // How many seconds of data to display
    int m_pointSize;  // Point radius in pixels
    
    // Layout
    QRect m_plotRect;
    int m_marginLeft;
    int m_marginRight;
    int m_marginTop;
    int m_marginBottom;
    
    // Theme
    bool m_isDarkTheme;
    
    // Pan and zoom state
    bool m_isPanning;
    QPoint m_lastMousePos;
    float m_zoomFactorY;
    float m_panOffsetY;
    
    // Hover state
    int m_hoveredPointIndex;
    QPoint m_mousePos;
    bool m_showTooltip;
    
    // Maximum data points to keep
    static constexpr int MAX_DATA_POINTS = 100000;
};

// Range-Velocity scatter plot widget
class RangeVelocityPlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RangeVelocityPlotWidget(QWidget *parent = nullptr);
    
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }
    void setRangeLimit(float maxRange);
    void setVelocityLimit(float maxVelocity);
    void setShowHistogram(bool show);
    void setPointSize(int size);
    void clearData();
    
    // Add data point
    void addDataPoint(float velocity, float range);
    void updateFromTargets(const TargetTrackData& targets);
    
    // Get current settings
    float getMaxRange() const { return m_maxRange; }
    float getMaxVelocity() const { return m_maxVelocity; }
    int getPointSize() const { return m_pointSize; }

public slots:
    void onResetZoom();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void drawBackground(QPainter& painter);
    void drawValidArea(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawAxes(QPainter& painter);
    void drawDataPoints(QPainter& painter);
    void drawHistogram(QPainter& painter);
    void drawLabels(QPainter& painter);
    void drawTooltip(QPainter& painter);
    
    // Helper functions
    int findNearestPoint(const QPoint& pos) const;
    
    // Theme colors
    QColor getBackgroundColor() const;
    QColor getPlotBackgroundColor() const;
    QColor getBorderColor() const;
    QColor getGridColor() const;
    QColor getTextColor() const;
    QColor getValidAreaColor() const;
    QColor getDataPointColor() const;
    
    // Data storage - stores (velocity, range) pairs
    QVector<QPair<float, float>> m_dataPoints;
    
    // Configuration
    float m_maxRange;
    float m_maxVelocity;
    float m_defaultMaxRange;
    float m_defaultMaxVelocity;
    bool m_showHistogram;
    int m_pointSize;  // Point radius in pixels
    
    // Layout
    QRect m_plotRect;
    int m_marginLeft;
    int m_marginRight;
    int m_marginTop;
    int m_marginBottom;
    
    // Theme
    bool m_isDarkTheme;
    
    // Pan and zoom state
    bool m_isPanning;
    QPoint m_lastMousePos;
    float m_zoomFactorX;
    float m_zoomFactorY;
    float m_panOffsetX;
    float m_panOffsetY;
    
    // Hover state
    int m_hoveredPointIndex;
    QPoint m_mousePos;
    bool m_showTooltip;
    
    // Histogram bins
    static constexpr int HISTOGRAM_BINS = 20;
    QVector<int> m_velocityHistogram;
    QVector<int> m_rangeHistogram;
    
    // Maximum data points to keep
    static constexpr int MAX_DATA_POINTS = 500;
};

// Main TimeSeries Plots Widget
class TimeSeriesPlotsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimeSeriesPlotsWidget(QWidget *parent = nullptr);
    ~TimeSeriesPlotsWidget();
    
    void updateFromTargets(const TargetTrackData& targets);
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }
    
    // Configuration
    void setMaxRange(float maxRange);
    void setMaxVelocity(float maxVelocity);

public slots:
    void clearAllData();
    
    // Settings slots
    void onPointSizeChanged(int size);
    void onVelocityMinChanged(double value);
    void onVelocityMaxChanged(double value);
    void onRangeMinChanged(double value);
    void onRangeMaxChanged(double value);
    void onTimeWindowChanged(int seconds);
    void onRVRangeMaxChanged(double value);
    void onRVVelocityMaxChanged(double value);
    void onSaveSettings();
    void onLoadSettings();

private slots:
    void onShowHistogramToggled(bool checked);
    void onResetVelocityPlot();
    void onResetRangePlot();
    void onResetRangeVelocityPlot();
    void onSettingsToggled();

private:
    void setupUI();
    void setupSettingsPanel();
    void applyTheme();
    void updatePlotSettings();
    void saveSettings();
    void loadSettings();
    
    // UI Components
    RangeVelocityPlotWidget* m_rangeVelocityPlot;
    TimeSeriesPlotWidget* m_velocityTimePlot;
    TimeSeriesPlotWidget* m_rangeTimePlot;
    
    // Settings panel
    QWidget* m_settingsPanel;
    QSpinBox* m_pointSizeSpinBox;
    QDoubleSpinBox* m_velocityMinSpinBox;
    QDoubleSpinBox* m_velocityMaxSpinBox;
    QDoubleSpinBox* m_rangeMinSpinBox;
    QDoubleSpinBox* m_rangeMaxSpinBox;
    QSpinBox* m_timeWindowSpinBox;
    QDoubleSpinBox* m_rvRangeMaxSpinBox;
    QDoubleSpinBox* m_rvVelocityMaxSpinBox;
    
    // Theme
    bool m_isDarkTheme;
    
    // Configuration
    float m_maxRange;
    float m_maxVelocity;
};

#endif // TIMESERIESPLOTSWIDGET_H
