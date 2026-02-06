#ifndef LOGGINGWIDGET_H
#define LOGGINGWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include <QDateTime>
#include <QPainter>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QSplitter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QToolTip>
#include "DataStructures.h"

// Structure to hold logged track data point
struct LoggedTrackDataPoint {
    qint64 timestamp;       // milliseconds since epoch
    uint32_t target_id;     // track ID
    float level;            // dB
    float range;            // meters
    float azimuth;          // degrees
    float radial_speed;     // m/s
    float azimuth_speed;    // deg/s
    float computed_range_rate;  // computed range rate (m/s)
    
    LoggedTrackDataPoint() 
        : timestamp(0), target_id(0), level(0), range(0), 
          azimuth(0), radial_speed(0), azimuth_speed(0), computed_range_rate(0) {}
};

// Structure to hold logged data per track
struct TrackLogData {
    uint32_t track_id;
    QVector<LoggedTrackDataPoint> dataPoints;
    qint64 firstSeen;
    qint64 lastSeen;
    
    TrackLogData() : track_id(0), firstSeen(0), lastSeen(0) {}
};

// Plot widget base class for logging plots
class LoggingPlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoggingPlotWidget(const QString& title, const QString& xLabel, 
                                const QString& yLabel, QWidget *parent = nullptr);
    
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }
    void clearData();
    void setData(const QVector<QPair<qint64, float>>& data, const QString& label = "");
    void setMultiTrackData(const QMap<uint32_t, QVector<QPair<qint64, float>>>& data);
    void setYAxisRange(float minY, float maxY);
    void setAutoScale(bool enabled);

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
    void drawLegend(QPainter& painter);
    void drawTooltip(QPainter& painter);
    
    QPointF dataToScreen(qint64 timestamp, float value) const;
    QColor getColorForTrackId(uint32_t trackId) const;
    
    QString m_title;
    QString m_xLabel;
    QString m_yLabel;
    
    // Data storage - maps track ID to data points
    QMap<uint32_t, QVector<QPair<qint64, float>>> m_multiTrackData;
    
    // Configuration
    float m_minY;
    float m_maxY;
    float m_defaultMinY;
    float m_defaultMaxY;
    bool m_autoScale;
    
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
    uint32_t m_hoveredTrackId;
};

// Range-Velocity scatter plot for logging
class LoggingRVPlotWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoggingRVPlotWidget(QWidget *parent = nullptr);
    
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }
    void clearData();
    void setData(const QVector<QPair<float, float>>& data);
    void setMultiTrackData(const QMap<uint32_t, QVector<QPair<float, float>>>& data);
    void setRangeLimit(float maxRange);
    void setVelocityLimit(float maxVelocity);

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
    void drawDataPoints(QPainter& painter);
    void drawLabels(QPainter& painter);
    void drawLegend(QPainter& painter);
    void drawTooltip(QPainter& painter);
    
    QColor getColorForTrackId(uint32_t trackId) const;
    
    // Data storage - maps track ID to (velocity, range) pairs
    QMap<uint32_t, QVector<QPair<float, float>>> m_multiTrackData;
    
    // Configuration
    float m_minRange;
    float m_maxRange;
    float m_minVelocity;
    float m_maxVelocity;
    float m_defaultMinRange;
    float m_defaultMaxRange;
    float m_defaultMinVelocity;
    float m_defaultMaxVelocity;
    int m_pointSize;
    
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
    uint32_t m_hoveredTrackId;
};

// Main Logging Widget
class LoggingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoggingWidget(QWidget *parent = nullptr);
    ~LoggingWidget();
    
    // Update methods
    void updateFromTargets(const TargetTrackData& targets);
    void setDarkTheme(bool isDark);
    bool isDarkTheme() const { return m_isDarkTheme; }
    
    // Logging control
    void startLogging();
    void stopLogging();
    void clearLogs();
    bool isLogging() const { return m_isLogging; }

public slots:
    // Control slots
    void onStartLogging();
    void onStopLogging();
    void onClearLogs();
    void onExportLogs();
    void onImportLogs();
    
    // Filter slots
    void onTrackFilterChanged(int index);
    void onRefreshTrackList();
    
    // Algorithm slots
    void onComputeRangeRate();
    void onAlgorithmWindowChanged(int value);
    void onSmoothingWindowChanged(int value);
    void onCustomAlgorithmClicked();
    
    // Visualization slots
    void onPlotRangeVsTime();
    void onPlotSpeedVsTime();
    void onPlotRV();
    void onUpdatePlots();
    
    // Window management
    void onDetachWindow();

private slots:
    void onRefreshPlotsTimer();

private:
    void setupUI();
    void setupControlPanel();
    void setupFilterPanel();
    void setupAlgorithmPanel();
    void setupVisualizationPanel();
    void setupPlotArea();
    void setupDataTable();
    void applyTheme();
    
    // Logging functions
    void logTrackData(const TargetTrackData& targets);
    
    // Data processing
    void computeRangeRateForTrack(uint32_t trackId);
    float computeRangeRateAtIndex(const QVector<LoggedTrackDataPoint>& points, 
                                    int index, int window, int smoothing);
    
    // Visualization helpers
    void updatePlotsForSelectedTrack();
    void updateDataTable();
    QVector<uint32_t> getSelectedTrackIds() const;
    
    // Export/Import helpers
    void exportToCSV(const QString& filename);
    void importFromCSV(const QString& filename);
    
    // UI Components
    QLabel* m_statusLabel;
    QPushButton* m_startButton;
    QPushButton* m_stopButton;
    QPushButton* m_clearButton;
    QPushButton* m_exportButton;
    QPushButton* m_importButton;
    QPushButton* m_detachButton;
    
    // Filter controls
    QComboBox* m_trackFilterCombo;
    QPushButton* m_refreshTracksButton;
    QCheckBox* m_showAllTracksCheckBox;
    
    // Algorithm controls
    QSpinBox* m_algorithmWindowSpinBox;
    QSpinBox* m_smoothingWindowSpinBox;
    QPushButton* m_computeRangeRateButton;
    QPushButton* m_customAlgorithmButton;
    QTextEdit* m_algorithmOutputText;
    
    // Visualization controls
    QPushButton* m_plotRangeTimeButton;
    QPushButton* m_plotSpeedTimeButton;
    QPushButton* m_plotRVButton;
    QCheckBox* m_autoUpdatePlotsCheckBox;
    
    // Plot widgets
    LoggingPlotWidget* m_rangeTimePlot;
    LoggingPlotWidget* m_speedTimePlot;
    LoggingRVPlotWidget* m_rvPlot;
    
    // Data table
    QTableWidget* m_dataTable;
    
    // Data storage
    QMap<uint32_t, TrackLogData> m_trackLogs;  // Maps track ID to log data
    QVector<uint32_t> m_activeTrackIds;        // List of active track IDs
    
    // State
    bool m_isLogging;
    bool m_isDarkTheme;
    qint64 m_loggingStartTime;
    int m_totalDataPoints;
    bool m_isDetached;
    QDialog* m_detachedWindow;
    
    // Timers
    QTimer* m_plotRefreshTimer;
    
    // Settings
    int m_algorithmWindow;      // Number of detections to associate (window)
    int m_smoothingWindow;      // Number of previous cycles for smoothing
    
    // Maximum data points per track
    static constexpr int MAX_DATA_POINTS_PER_TRACK = 50000;
};

#endif // LOGGINGWIDGET_H
