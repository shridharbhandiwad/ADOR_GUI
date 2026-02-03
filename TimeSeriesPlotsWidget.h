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
    void clearData();
    
    // Add data point
    void addDataPoint(qint64 timestamp, float value);
    
public slots:
    void onResetZoom();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawBackground(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawAxes(QPainter& painter);
    void drawData(QPainter& painter);
    void drawLabels(QPainter& painter);
    
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
    int m_timeWindowSeconds;  // How many seconds of data to display
    
    // Layout
    QRect m_plotRect;
    int m_marginLeft;
    int m_marginRight;
    int m_marginTop;
    int m_marginBottom;
    
    // Theme
    bool m_isDarkTheme;
    
    // Maximum data points to keep
    static constexpr int MAX_DATA_POINTS = 1000;
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
    void clearData();
    
    // Add data point
    void addDataPoint(float velocity, float range);
    void updateFromTargets(const TargetTrackData& targets);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawBackground(QPainter& painter);
    void drawValidArea(QPainter& painter);
    void drawGrid(QPainter& painter);
    void drawAxes(QPainter& painter);
    void drawDataPoints(QPainter& painter);
    void drawHistogram(QPainter& painter);
    void drawLabels(QPainter& painter);
    
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
    bool m_showHistogram;
    
    // Layout
    QRect m_plotRect;
    int m_marginLeft;
    int m_marginRight;
    int m_marginTop;
    int m_marginBottom;
    
    // Theme
    bool m_isDarkTheme;
    
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
    void onShowHistogramToggled(bool checked);
    void onResetVelocityPlot();
    void onResetRangePlot();
    void clearAllData();

private:
    void setupUI();
    void applyTheme();
    
    // UI Components
    QCheckBox* m_showHistogramCheckbox;
    RangeVelocityPlotWidget* m_rangeVelocityPlot;
    TimeSeriesPlotWidget* m_velocityTimePlot;
    TimeSeriesPlotWidget* m_rangeTimePlot;
    QPushButton* m_resetVelocityBtn;
    QPushButton* m_resetRangeBtn;
    
    // Theme
    bool m_isDarkTheme;
    
    // Configuration
    float m_maxRange;
    float m_maxVelocity;
};

#endif // TIMESERIESPLOTSWIDGET_H
