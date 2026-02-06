#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QUdpSocket>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <random>
#include <QMap>
#include <QDateTime>
#include <QVector>

#include "PPIWidget.h"
#include "FFTWidget.h"
#include "SpeedMeasurementWidget.h"
#include "TimeSeriesPlotsWidget.h"
#include "LoggingWidget.h"
#include "DataStructures.h"
#include <QTabWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateDisplay();
    void readPendingDatagrams();
    void onSimulateDataToggled();
    void onOpenLoggingWindow();

    // Radar parameter slots
    void onRangeChanged(int range);
    void onMinRangeChanged(const QString& text);
    void onChirpChanged(const QString& text);
    void onBandwidthChanged(const QString& text);
    void onMinSpeedChanged(const QString& text);
    void onMaxSpeedChanged(const QString& text);
    void onChirpsPerFrameChanged(const QString& text);
    void onSamplesPerChirpChanged(const QString& text);
    void onApplySettings();
    void onSaveSettings();
    void onLoadFromFile();
    void onSaveToFile();
    void refreshTrackTable();  // Auto-refresh track table (sync with current frame data)

    // DSP parameter slots
    void onRangeAvgEdited();
    void onMinRangeEdited();
    void onMaxRangeEdited();
    void onMinSpeedEdited();
    void onMaxSpeedEdited();
    void onMinAngleEdited();
    void onMaxAngleEdited();
    void onRangeThresholdEdited();
    void onSpeedThresholdEdited();
    void onNumTracksEdited();
    void onMedianFilterEdited();
    void onMtiLengthEdited();

private:
    void setupUI();
    void createMenuBar();
    void setupNetworking();
    void setupTimer();
    void updateTrackTable();
    void applyFrameTargets();  // Apply collected frame targets as current targets (ephemeral sync)
    void generateSimulatedTargetData();
    void generateSimulatedADCData();
    void loadSettings();
    QString getSettingsFilePath() const;

    // Binary UDP data parsing - NEW
    void parseBinaryRawData(const QByteArray& datagram);
    void processRawDataFrame(const RawDataHeader_t* header,
                            const float* sample_data,
                            uint32_t total_samples);
    void parseBinaryTargetData(const QByteArray& datagram);  // NEW for target data

    // Text-based parsing (legacy/for track data)
    void parseADCMessage(const QString& message);
    void parseTrackMessage(const QString& message);
    
    // Track data logging
    QString createTimestampedFilename();
    void logTrackDataToFile(const TargetTrack& track);

    // UI Components
    PPIWidget* m_ppiWidget;
    FFTWidget* m_fftWidget;
    SpeedMeasurementWidget* m_speedMeasurementWidget;
    TimeSeriesPlotsWidget* m_timeSeriesPlotsWidget;
    LoggingWidget* m_loggingWidget;
    QWidget* m_loggingWindow;  // Separate window for logging
    QTabWidget* m_mainTabWidget;
    QTableWidget* m_trackTable;
    QSplitter* m_mainSplitter;
    QSplitter* m_rightSplitter;

    // Control widgets
    QSpinBox* m_rangeSpinBox;
    QPushButton* m_applyButton;
    QPushButton* m_saveSettingsButton;
    QPushButton* m_loadFromFileButton;
    QPushButton* m_saveToFileButton;
    QLabel* m_statusLabel;
    QLabel* m_frameCountLabel;

    // Radar parameter widgets
    QLineEdit* m_chirpLineEdit;
    QLineEdit* m_bandwidthLineEdit;
    QLineEdit* m_minRangeLineEdit;
    QLineEdit* m_minSpeedLineEdit;
    QLineEdit* m_maxSpeedLineEdit;
    QLineEdit* m_chirpsPerFrameLineEdit;
    QLineEdit* m_samplesPerChirpLineEdit;

    // DSP Setting Widgets
    QLineEdit* m_rangeAvgEdit;
    QLineEdit* m_minRangeEdit;
    QLineEdit* m_maxRangeEdit;
    QLineEdit* m_minSpeedEdit;
    QLineEdit* m_maxSpeedEdit;
    QLineEdit* m_minAngleEdit;
    QLineEdit* m_maxAngleEdit;
    QLineEdit* m_rangeThresholdEdit;
    QLineEdit* m_speedThresholdEdit;
    QLineEdit* m_numTracksEdit;
    QLineEdit* m_medianFilterEdit;
    QLineEdit* m_mtiLengthEdit;
    
    // DSP Settings panel group boxes (for theme-aware styling)
    QGroupBox* m_dspSettingsGroup;
    QGroupBox* m_dspLeftGroup;
    QGroupBox* m_dspRightGroup;
    QVector<QLabel*> m_dspLabels;  // Store labels for theme updates

    // Network
    QUdpSocket* m_udpSocket;
    static constexpr quint16 UDP_PORT = 5000;

    // Timer
    QTimer* m_updateTimer;
    static constexpr int UPDATE_INTERVAL_MS = 50;

    // Track table auto-refresh timer (for periodic UI updates)
    QTimer* m_trackRefreshTimer;
    static constexpr int TRACK_REFRESH_INTERVAL_MS = 500;  // Faster refresh for ephemeral sync
    static constexpr int TRACK_TABLE_MINIMUM_ROWS = 10;    // Minimum rows to display in track table
    
    // Frame-based track collection for ephemeral synchronization
    // Tracks are only shown when present in the current frame
    std::vector<TargetTrack> m_frameTargets;  // Tracks being collected for current frame
    uint8_t m_expectedNumTargets;              // Number of targets expected in current frame
    uint8_t m_receivedTargetCount;             // Number of targets received so far in current frame

    // Data
    TargetTrackData m_currentTargets;
    RawADCFrameTest m_currentADCFrame;

    // DSP Settings State
    DSP_Settings_t m_dsp;

    // Simulation
    bool m_simulationEnabled;
    std::mt19937 m_randomEngine;
    std::uniform_real_distribution<float> m_rangeDist;
    std::uniform_real_distribution<float> m_azimuthDist;
    std::uniform_real_distribution<float> m_speedDist;
    std::uniform_real_distribution<float> m_levelDist;
    std::uniform_int_distribution<int> m_numTargetsDist;

    // Statistics
    uint64_t m_frameCount;
    uint64_t m_targetCount;
    
    // Track data logging
    QFile* m_trackDataFile;
    QString m_currentLogFilename;

    bool m_isDarkTheme;
    void applyTheme(bool isDark);
    void applyDspSettingsTheme(bool isDark);  // Apply theme to DSP Settings panel
    QString getLightThemeStyleSheet() const;
    QString getDarkThemeStyleSheet() const;
};

#endif // MAINWINDOW_H
