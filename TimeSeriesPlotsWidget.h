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
#include <QLineEdit>
#include <QGroupBox>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QToolTip>
#include <QMap>
#include <QPropertyAnimation>
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
    ~TimeSeriesPlotWidget();
    
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
    
    // Clean up old data points outside the time window
    void cleanupOldData();
    
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
    
    // Cleanup timer
    QTimer* m_cleanupTimer;
    
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
    void setMinRangeLimit(float minRange);
    void setMinVelocityLimit(float minVelocity);
    void setShowHistogram(bool show);
    void setPointSize(int size);
    void clearData();
    
    // Add data point
    void addDataPoint(float velocity, float range);
    void updateFromTargets(const TargetTrackData& targets);
    
    // Get current settings
    float getMaxRange() const { return m_maxRange; }
    float getMaxVelocity() const { return m_maxVelocity; }
    float getMinRange() const { return m_minRange; }
    float getMinVelocity() const { return m_minVelocity; }
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
    float m_minRange;
    float m_maxRange;
    float m_minVelocity;
    float m_maxVelocity;
    float m_defaultMinRange;
    float m_defaultMaxRange;
    float m_defaultMinVelocity;
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

// Digital display widget with animated numbers (similar to Top Speed style)
class DigitalRangeRateDisplay : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(float displayValue READ getDisplayValue WRITE setDisplayValue)

public:
    explicit DigitalRangeRateDisplay(QWidget *parent = nullptr);
    
    void setValue(float value);
    float getDisplayValue() const { return m_displayValue; }
    void setDisplayValue(float value);
    void setUnit(const QString& unit);
    void setDarkTheme(bool isDark);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float m_targetValue;
    float m_displayValue;
    QString m_unit;
    bool m_isDarkTheme;
    QPropertyAnimation* m_valueAnimation;
};

// Structure to store previous track data for range rate calculation
struct TrackHistory {
    qint64 timestamp;
    float range;
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
    
    // Track filter getters (for use by MainWindow to filter PPI and Track Table)
    float getFilterMinRange() const { return m_filterMinRange; }
    float getFilterMinVelocity() const { return m_filterMinVelocity; }
    bool getFilterReceding() const { return m_filterReceding; }
    bool getFilterApproaching() const { return m_filterApproaching; }
    
    // Static helper: check if a track passes given filter criteria
    static bool trackPassesFilters(const TargetTrack& track,
                                   float filterMinRange,
                                   float filterMinVelocityKph,
                                   bool filterReceding,
                                   bool filterApproaching);

signals:
    // Emitted when any track filter parameter changes
    void trackFiltersChanged();

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
    
    // Filter control slots
    void onFilterMinRangeChanged(double value);
    void onFilterMinVelocityChanged(double value);
    void onFilterMovingAvgChanged(int value);
    void onFilterDirectionChanged();

private slots:
    void onShowHistogramToggled(bool checked);
    void onResetVelocityPlot();
    void onResetRangePlot();
    void onResetRangeVelocityPlot();
    void onSettingsToggled();

private:
    void setupUI();
    void setupSettingsPanel();
    void setupFilterControls();
    void applyTheme();
    void updatePlotSettings();
    void saveSettings();
    void loadSettings();
    bool passesFilters(const TargetTrack& track, float velocityKmh) const;
    float applyMovingAverage(uint32_t trackId, float value);
    float calculateRangeRate(uint32_t trackId, float currentRange, qint64 timestamp);
    
    // UI Components
    RangeVelocityPlotWidget* m_rangeVelocityPlot;
    TimeSeriesPlotWidget* m_velocityTimePlot;
    TimeSeriesPlotWidget* m_rangeTimePlot;
    TimeSeriesPlotWidget* m_rangeRatePlot;  // New: Range Rate plot
    
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
    
    // Filter controls panel
    QWidget* m_filterPanel;
    QDoubleSpinBox* m_filterMinRangeSpinBox;
    QDoubleSpinBox* m_filterMinVelocitySpinBox;
    QSpinBox* m_filterMovingAvgSpinBox;
    QCheckBox* m_filterRecedingCheckBox;
    QCheckBox* m_filterApproachingCheckBox;
    
    // Range Rate Moving Average Display
    QLabel* m_rangeRateLabel;
    DigitalRangeRateDisplay* m_rangeRateDisplay;
    
    // Theme
    bool m_isDarkTheme;
    
    // Configuration
    float m_maxRange;
    float m_maxVelocity;
    
    // Track filtering parameters
    float m_filterMinRange;
    float m_filterMinVelocity;
    int m_filterMovingAvgSize;
    bool m_filterReceding;
    bool m_filterApproaching;
    
    // Track history for range rate calculation and moving average
    QMap<uint32_t, QVector<TrackHistory>> m_trackHistory;
    QMap<uint32_t, QVector<float>> m_movingAvgBuffer;
    
    // Range rate moving average calculation
    float m_previousRangeRate;
    float m_currentRangeRate;
    float m_rangeRateMovingAvg;
    
    // Track last data received time to detect when no data is coming in
    qint64 m_lastDataReceivedTime;
    
    // Cleanup timer to periodically remove old data
    QTimer* m_cleanupTimer;
    
    // Trackdata filter logging
    void logFilteredTrackData(const TargetTrack& target, float velocityKmh, float avgRangeM, qint64 currentTime);
    QString createFilterLogFilename();
};

#endif // TIMESERIESPLOTSWIDGET_H
