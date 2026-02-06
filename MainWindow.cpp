#include "MainWindow.h"
#include "SpeedMeasurementWidget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QNetworkDatagram>
#include <QHeaderView>
#include <QMessageBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QRegularExpression>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTextStream>
#include <QFileInfo>
#include <QScreen>
#include <QJsonDocument>
#include <QJsonObject>
#include <cmath>
#include <cstring>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ppiWidget(nullptr)
    , m_fftWidget(nullptr)
    , m_speedMeasurementWidget(nullptr)
    , m_timeSeriesPlotsWidget(nullptr)
    , m_loggingWidget(nullptr)
    , m_mainTabWidget(nullptr)
    , m_trackTable(nullptr)
    , m_udpSocket(nullptr)
    , m_updateTimer(nullptr)
    , m_trackRefreshTimer(nullptr)
    , m_saveSettingsButton(nullptr)
    , m_loadFromFileButton(nullptr)
    , m_saveToFileButton(nullptr)
    , m_simulationEnabled(false)  // Simulation disabled by default
    , m_randomEngine(std::random_device{}())
    , m_rangeDist(100.0f, 500.0f)
    , m_azimuthDist(-90.0f, 90.0f)
    , m_speedDist(-50.0f, 50.0f)
    , m_levelDist(10.0f, 100.0f)
    , m_numTargetsDist(3, 8)
    , m_frameCount(0)
    , m_targetCount(0)
    , m_trackDataFile(nullptr)
    , m_dsp{}  // Zero-initialize the DSP settings struct
    , m_isDarkTheme(false)
    , m_expectedNumTargets(0)
    , m_receivedTargetCount(0)
{
    // Initialize DSP settings with default values matching UI defaults
    m_dsp.range_mvg_avg_length = 1;
    m_dsp.min_range_cm = 0;
    m_dsp.max_range_cm = 5000;
    m_dsp.min_speed_kmh = 0;   // Signed type now supports negative values
    m_dsp.max_speed_kmh = 100;
    m_dsp.min_angle_degree = 0; // Signed type now supports negative values
    m_dsp.max_angle_degree = 0;
    m_dsp.range_threshold = 0;
    m_dsp.speed_threshold = 0;
    m_dsp.enable_tracking = 1;
    m_dsp.num_of_tracks = 50;
    m_dsp.median_filter_length = 1;
    m_dsp.enable_mti_filter = 1;
    m_dsp.mti_filter_length = 2;

    setupUI();
    loadSettings();  // Load saved settings on startup
    setupNetworking();
    setupTimer();
}

MainWindow::~MainWindow()
{
    if (m_updateTimer) {
        m_updateTimer->stop();
    }
    if (m_trackRefreshTimer) {
        m_trackRefreshTimer->stop();
    }
    
    // Close track data file if open
    if (m_trackDataFile) {
        m_trackDataFile->close();
        delete m_trackDataFile;
        m_trackDataFile = nullptr;
    }
}

void MainWindow::setupUI()
{
    setWindowTitle("ZopplerSystems RadarGUI");
    
    // Get screen geometry and DPI for responsive sizing
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry;
    qreal dpiScale = 1.0;
    
    if (screen) {
        screenGeometry = screen->availableGeometry();
        // Calculate DPI scale factor (baseline: 96 DPI)
        dpiScale = screen->logicalDotsPerInch() / 96.0;
        qDebug() << "Screen DPI:" << screen->logicalDotsPerInch() << "Scale factor:" << dpiScale;
    } else {
        // Fallback if no screen detected
        screenGeometry = QRect(0, 0, 1920, 1080);
    }
    
    // Set responsive minimum size based on screen size (60% of available width, 70% of height)
    int minWidth = qMax(1000, static_cast<int>(screenGeometry.width() * 0.6));
    int minHeight = qMax(700, static_cast<int>(screenGeometry.height() * 0.7));
    setMinimumSize(minWidth, minHeight);
    
    // Maximize window to screen size on startup
    setGeometry(screenGeometry);
    showMaximized();

    // Apply premium engineering dashboard theme
    QString lightTheme = R"(
        /* ========== MAIN WINDOW - MONOCHROME LIGHT ========== */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #fafafa, stop:1 #f5f5f5);
            color: #0a0a0a;
        }
        
        /* ========== BASE WIDGET STYLING ========== */
        QWidget {
            background-color: transparent;
            color: #0a0a0a;
            font-family: 'Segoe UI', 'Inter', 'SF Pro Display', -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 15px;
        }
        
        /* ========== CARD-STYLE GROUP BOXES ========== */
        QGroupBox {
            font-weight: 600;
            font-size: 16px;
            color: #0a0a0a;
            border: 1px solid #e5e5e5;
            border-radius: 12px;
            margin-top: 14px;
            padding: 16px 12px 12px 12px;
            background-color: #ffffff;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            top: 2px;
            padding: 3px 12px;
            background: #1a1a1a;
            color: #ffffff;
            font-weight: 700;
            font-size: 14px;
            border-radius: 6px;
            letter-spacing: 0.3px;
        }
        
        /* ========== LABELS ========== */
        QLabel {
            color: #525252;
            font-weight: 500;
            background-color: transparent;
            letter-spacing: 0.1px;
        }
        
        /* ========== INPUT FIELDS ========== */
        QLineEdit {
            background-color: #fafafa;
            border: 2px solid #e5e5e5;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0a0a0a;
            font-weight: 500;
            font-size: 15px;
            selection-background-color: #1a1a1a;
            selection-color: #ffffff;
        }
        
        QLineEdit:hover {
            border-color: #a3a3a3;
            background-color: #ffffff;
        }
        
        QLineEdit:focus {
            border: 2px solid #1a1a1a;
            background-color: #ffffff;
            outline: none;
        }
        
        QLineEdit:disabled {
            background-color: #f5f5f5;
            color: #a3a3a3;
            border-color: #e5e5e5;
        }
        
        /* ========== SPIN BOXES ========== */
        QSpinBox, QDoubleSpinBox {
            background-color: #fafafa;
            border: 2px solid #e5e5e5;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0a0a0a;
            font-weight: 500;
            selection-background-color: #1a1a1a;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #a3a3a3;
            background-color: #ffffff;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #1a1a1a;
            background-color: #ffffff;
        }
        
        QSpinBox::up-button, QSpinBox::down-button,
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            background-color: transparent;
            border: none;
            width: 24px;
            border-radius: 4px;
        }
        
        QSpinBox::up-button:hover, QSpinBox::down-button:hover,
        QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: rgba(26, 26, 26, 0.1);
        }
        
        /* ========== BUTTONS - PRIMARY ========== */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a1a, stop:1 #0a0a0a);
            border: none;
            border-radius: 10px;
            color: #ffffff;
            font-weight: 600;
            font-size: 15px;
            padding: 12px 24px;
            min-height: 20px;
            letter-spacing: 0.3px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #333333, stop:1 #1a1a1a);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a0a0a, stop:1 #000000);
        }
        
        QPushButton:checked {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a0a0a, stop:1 #000000);
            color: #ffffff;
        }
        
        QPushButton:disabled {
            background: #d4d4d4;
            color: #a3a3a3;
        }
        
        /* ========== DATA TABLE ========== */
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #fafafa;
            color: #0a0a0a;
            gridline-color: transparent;
            border: 1px solid #e5e5e5;
            border-radius: 12px;
            selection-background-color: rgba(26, 26, 26, 0.1);
            selection-color: #0a0a0a;
            outline: none;
        }
        
        QTableWidget::item {
            padding: 12px 16px;
            border-bottom: 1px solid #f5f5f5;
        }
        
        QTableWidget::item:selected {
            background-color: rgba(26, 26, 26, 0.1);
            color: #0a0a0a;
        }
        
        QTableWidget::item:hover {
            background-color: rgba(26, 26, 26, 0.05);
        }
        
        QHeaderView {
            background-color: transparent;
        }
        
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #fafafa, stop:1 #f5f5f5);
            color: #525252;
            font-weight: 700;
            font-size: 13px;
            padding: 14px 16px;
            border: none;
            border-bottom: 2px solid #e5e5e5;
            text-transform: uppercase;
            letter-spacing: 0.8px;
        }
        
        QHeaderView::section:first {
            border-top-left-radius: 12px;
        }
        
        QHeaderView::section:last {
            border-top-right-radius: 12px;
        }
        
        /* ========== SPLITTER HANDLES ========== */
        QSplitter::handle {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #e5e5e5, stop:0.5 #d4d4d4, stop:1 #e5e5e5);
            border-radius: 3px;
        }
        
        QSplitter::handle:horizontal {
            width: 6px;
            margin: 4px 2px;
        }
        
        QSplitter::handle:vertical {
            height: 6px;
            margin: 2px 4px;
        }
        
        QSplitter::handle:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #a3a3a3, stop:0.5 #525252, stop:1 #a3a3a3);
        }
        
        /* ========== SCROLLBARS ========== */
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border-radius: 6px;
            margin: 4px 2px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #d4d4d4;
            border-radius: 5px;
            min-height: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #a3a3a3;
        }
        
        QScrollBar::handle:vertical:pressed {
            background-color: #525252;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            height: 0px;
            background: transparent;
        }
        
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border-radius: 6px;
            margin: 2px 4px;
        }
        
        QScrollBar::handle:horizontal {
            background-color: #d4d4d4;
            border-radius: 5px;
            min-width: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: #a3a3a3;
        }
        
        QScrollBar::handle:horizontal:pressed {
            background-color: #525252;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            width: 0px;
            background: transparent;
        }
        
        /* ========== TOOLTIPS ========== */
        QToolTip {
            background-color: #1a1a1a;
            color: #fafafa;
            border: 1px solid #404040;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 14px;
            font-weight: 500;
        }
        
        /* ========== CORNER WIDGET ========== */
        QAbstractScrollArea::corner {
            background-color: transparent;
        }
    )";

    this->setStyleSheet(lightTheme);

    // Create Menu Bar
    createMenuBar();

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create main horizontal layout: DSP Settings (left) | Right side (PPI, Track Table, FFT)
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // ========== RIGHT SIDE: PPI (center-top), Track Table (right-top), FFT (bottom) ==========
    // Create a vertical splitter for right side: top row and bottom FFT
    QSplitter* rightVerticalSplitter = new QSplitter(Qt::Vertical);
    rightVerticalSplitter->setHandleWidth(6);
    
    // Top row: PPI (left) and Track Table (right)
    QSplitter* topHorizontalSplitter = new QSplitter(Qt::Horizontal);
    topHorizontalSplitter->setHandleWidth(6);

    // ========== CENTER-TOP: PPI Display ==========
    QGroupBox* ppiGroup = new QGroupBox("PPI");
    QVBoxLayout* ppiLayout = new QVBoxLayout(ppiGroup);
    ppiLayout->setSpacing(4);
    ppiLayout->setContentsMargins(4, 12, 4, 4);  // Reduced margins

    m_ppiWidget = new PPIWidget();
    // Use responsive minimum size based on screen DPI and size
    int ppiMinWidth = static_cast<int>(250 * dpiScale);
    int ppiMinHeight = static_cast<int>(200 * dpiScale);
    m_ppiWidget->setMinimumSize(ppiMinWidth, ppiMinHeight);
    ppiLayout->addWidget(m_ppiWidget, 1);  // Stretch factor 1 to take available space

    topHorizontalSplitter->addWidget(ppiGroup);

    // ========== RIGHT-TOP: Track Table ==========
    QGroupBox* tableGroup = new QGroupBox("Track Table");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->setContentsMargins(4, 12, 4, 4);  // Reduced margins
    m_trackTable = new QTableWidget();
    m_trackTable->setColumnCount(4);
    QStringList headers;
    headers << "ID" << "Range (m)" << "Azimuth (°)" << "Radial Speed (m/s)";
    m_trackTable->setHorizontalHeaderLabels(headers);
    m_trackTable->horizontalHeader()->setStretchLastSection(true);
    m_trackTable->verticalHeader()->setVisible(false);  // Hide row numbers to remove black column on left
    m_trackTable->setAlternatingRowColors(true);
    m_trackTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Initialize track table with minimum rows (empty)
    m_trackTable->setRowCount(TRACK_TABLE_MINIMUM_ROWS);
    for (int i = 0; i < TRACK_TABLE_MINIMUM_ROWS; ++i) {
        m_trackTable->setItem(i, 0, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 1, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 2, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 3, new QTableWidgetItem(""));
    }
    
    tableLayout->addWidget(m_trackTable, 1);

    topHorizontalSplitter->addWidget(tableGroup);
    
    // Set initial sizes for PPI (larger) and Track Table (smaller) - proportional to screen width
    int totalTopWidth = screenGeometry.width() - 400;  // Subtract DSP panel width
    int ppiWidth = static_cast<int>(totalTopWidth * 0.67);  // 67% for PPI
    int tableWidth = static_cast<int>(totalTopWidth * 0.33);  // 33% for Track Table
    topHorizontalSplitter->setSizes({ppiWidth, tableWidth});

    rightVerticalSplitter->addWidget(topHorizontalSplitter);

    // ========== BOTTOM: FFT Display ==========
    QGroupBox* fftGroup = new QGroupBox("FFT");
    QVBoxLayout* fftLayout = new QVBoxLayout(fftGroup);
    fftLayout->setContentsMargins(4, 12, 4, 4);  // Reduced margins
    m_fftWidget = new FFTWidget();
    m_fftWidget->setRadarParameters(100000.0f, 0.001f, 50000000.0f, 24000000000.0f);
    m_fftWidget->setMaxRange(50.0f);
    // Use responsive minimum size based on screen DPI and size
    int fftMinWidth = static_cast<int>(250 * dpiScale);
    int fftMinHeight = static_cast<int>(120 * dpiScale);
    m_fftWidget->setMinimumSize(fftMinWidth, fftMinHeight);
    fftLayout->addWidget(m_fftWidget);
    
    rightVerticalSplitter->addWidget(fftGroup);
    
    // Set initial sizes for top row (larger) and FFT - proportional to screen height
    int totalHeight = screenGeometry.height() - 100;  // Subtract menu and margins
    int topRowHeight = static_cast<int>(totalHeight * 0.60);  // 60% for top row (PPI + Track Table)
    int fftHeight = static_cast<int>(totalHeight * 0.40);     // 40% for FFT
    rightVerticalSplitter->setSizes({topRowHeight, fftHeight});

    // ========== LEFT: DSP Settings Panel (Vertical Layout) ==========
    m_dspSettingsGroup = new QGroupBox("DSP Settings", this);
    // Styling will be applied dynamically based on theme
    
    QVBoxLayout* settingsMainLayout = new QVBoxLayout(m_dspSettingsGroup);
    settingsMainLayout->setSpacing(8);
    settingsMainLayout->setContentsMargins(12, 20, 12, 12);
    
    // Single vertical container for settings
    QWidget* settingsContainer = new QWidget(this);
    QVBoxLayout* settingsVLayout = new QVBoxLayout(settingsContainer);
    settingsVLayout->setSpacing(8);
    settingsVLayout->setContentsMargins(0, 0, 0, 0);
    
    // Range & Speed section
    m_dspLeftGroup = new QGroupBox("Range && Speed", this);
    QGridLayout* leftLayout = new QGridLayout(m_dspLeftGroup);
    leftLayout->setSpacing(6);
    leftLayout->setContentsMargins(10, 16, 10, 10);
    
    // Filter & Tracking section
    m_dspRightGroup = new QGroupBox("Filter && Tracking", this);
    QGridLayout* rightLayout = new QGridLayout(m_dspRightGroup);
    rightLayout->setSpacing(6);
    rightLayout->setContentsMargins(10, 16, 10, 10);

    // Clear labels vector before adding new ones
    m_dspLabels.clear();
    
    // Compact field helper for narrower panel - responsive sizing
    auto addField = [&](QGridLayout* layout, int row, const QString& label, QLineEdit*& edit, const QString& def) {
        QLabel* l = new QLabel(label, this);
        m_dspLabels.append(l);  // Store label for theme updates
        edit = new QLineEdit(def, this);
        // Use responsive sizes based on DPI
        int editMinWidth = static_cast<int>(60 * dpiScale);
        int editMaxWidth = static_cast<int>(90 * dpiScale);
        int editMinHeight = static_cast<int>(26 * dpiScale);
        edit->setMinimumWidth(editMinWidth);
        edit->setMaximumWidth(editMaxWidth);
        edit->setMinimumHeight(editMinHeight);
        layout->addWidget(l, row, 0);
        layout->addWidget(edit, row, 1);
    };

    // Range & Speed fields
    addField(leftLayout, 0, "Range Avg",        m_rangeAvgEdit,        "1");
    addField(leftLayout, 1, "Min Range (cm)",   m_minRangeEdit,        "0");
    addField(leftLayout, 2, "Max Range (cm)",   m_maxRangeEdit,        "5000");
    addField(leftLayout, 3, "Min Speed (km/h)", m_minSpeedEdit,        "-100");
    addField(leftLayout, 4, "Max Speed (km/h)", m_maxSpeedEdit,        "100");
    addField(leftLayout, 5, "Range Thresh",     m_rangeThresholdEdit,  "0");
    
    // Filter & Tracking fields
    addField(rightLayout, 0, "Min Angle (deg)",  m_minAngleEdit,        "0");
    addField(rightLayout, 1, "Max Angle (deg)",  m_maxAngleEdit,        "0");
    addField(rightLayout, 2, "Speed Thresh",     m_speedThresholdEdit,  "0");
    addField(rightLayout, 3, "Num Tracks",       m_numTracksEdit,       "50");
    addField(rightLayout, 4, "Median Filter",    m_medianFilterEdit,    "1");
    addField(rightLayout, 5, "MTI Length",       m_mtiLengthEdit,       "2");

    // Set column widths for narrower panel - responsive based on DPI
    int labelColWidth = static_cast<int>(90 * dpiScale);
    int editColWidth = static_cast<int>(60 * dpiScale);
    leftLayout->setColumnMinimumWidth(0, labelColWidth);
    leftLayout->setColumnMinimumWidth(1, editColWidth);
    rightLayout->setColumnMinimumWidth(0, labelColWidth);
    rightLayout->setColumnMinimumWidth(1, editColWidth);
    
    // Stack the two sections vertically
    settingsVLayout->addWidget(m_dspLeftGroup);
    settingsVLayout->addWidget(m_dspRightGroup);
    settingsVLayout->addStretch();
    
    settingsMainLayout->addWidget(settingsContainer, 1);
    
    // Create buttons layout - Compact vertical layout for narrow panel
    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setStyleSheet("background-color: transparent;");
    QGridLayout* buttonLayout = new QGridLayout(buttonContainer);
    buttonLayout->setContentsMargins(0, 8, 0, 0);
    buttonLayout->setSpacing(8);
    
    // Apply button - Premium blue gradient with icon hint
    m_applyButton = new QPushButton("Apply", this);
    m_applyButton->setMinimumHeight(34);
    m_applyButton->setCursor(Qt::PointingHandCursor);
    m_applyButton->setStyleSheet(R"(
        QPushButton {
            font-size: 13px;
            font-weight: 600;
            padding: 6px 12px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a1a, stop:1 #0a0a0a);
            color: white;
            border: none;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #333333, stop:1 #1a1a1a);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a0a0a, stop:1 #000000);
        }
    )");
    
    // Save Settings button - Monochrome solid style
    m_saveSettingsButton = new QPushButton("Save", this);
    m_saveSettingsButton->setMinimumHeight(34);
    m_saveSettingsButton->setCursor(Qt::PointingHandCursor);
    m_saveSettingsButton->setStyleSheet(R"(
        QPushButton {
            font-size: 13px;
            font-weight: 600;
            padding: 6px 12px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a1a, stop:1 #0a0a0a);
            color: white;
            border: none;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #333333, stop:1 #1a1a1a);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a0a0a, stop:1 #000000);
        }
    )");
    
    // Load From File button - Monochrome outlined style
    m_loadFromFileButton = new QPushButton("Load From File", this);
    m_loadFromFileButton->setMinimumHeight(34);
    m_loadFromFileButton->setCursor(Qt::PointingHandCursor);
    m_loadFromFileButton->setStyleSheet(R"(
        QPushButton {
            font-size: 13px;
            font-weight: 600;
            padding: 6px 12px;
            background-color: transparent;
            color: #1a1a1a;
            border: 2px solid #1a1a1a;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background-color: rgba(26, 26, 26, 0.08);
            border-color: #333333;
            color: #0a0a0a;
        }
        QPushButton:pressed {
            background-color: rgba(26, 26, 26, 0.15);
            border-color: #0a0a0a;
        }
    )");
    
    // Save to File button - Monochrome outlined style
    m_saveToFileButton = new QPushButton("Save to File", this);
    m_saveToFileButton->setMinimumHeight(34);
    m_saveToFileButton->setCursor(Qt::PointingHandCursor);
    m_saveToFileButton->setStyleSheet(R"(
        QPushButton {
            font-size: 13px;
            font-weight: 600;
            padding: 6px 12px;
            background-color: transparent;
            color: #1a1a1a;
            border: 2px solid #1a1a1a;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background-color: rgba(26, 26, 26, 0.08);
            border-color: #333333;
            color: #0a0a0a;
        }
        QPushButton:pressed {
            background-color: rgba(26, 26, 26, 0.15);
            border-color: #0a0a0a;
        }
    )");
    
    // Arrange buttons in a 2x2 grid for compact display
    buttonLayout->addWidget(m_applyButton, 0, 0);
    buttonLayout->addWidget(m_saveSettingsButton, 0, 1);
    buttonLayout->addWidget(m_loadFromFileButton, 1, 0);
    buttonLayout->addWidget(m_saveToFileButton, 1, 1);

    settingsMainLayout->addWidget(buttonContainer, 0);

    // Connect DSP settings signals
    connect(m_rangeAvgEdit,        &QLineEdit::editingFinished, this, &MainWindow::onRangeAvgEdited);
    connect(m_minRangeEdit,        &QLineEdit::editingFinished, this, &MainWindow::onMinRangeEdited);
    connect(m_maxRangeEdit,        &QLineEdit::editingFinished, this, &MainWindow::onMaxRangeEdited);
    connect(m_minSpeedEdit,        &QLineEdit::editingFinished, this, &MainWindow::onMinSpeedEdited);
    connect(m_maxSpeedEdit,        &QLineEdit::editingFinished, this, &MainWindow::onMaxSpeedEdited);
    connect(m_minAngleEdit,        &QLineEdit::editingFinished, this, &MainWindow::onMinAngleEdited);
    connect(m_maxAngleEdit,        &QLineEdit::editingFinished, this, &MainWindow::onMaxAngleEdited);
    connect(m_rangeThresholdEdit,  &QLineEdit::editingFinished, this, &MainWindow::onRangeThresholdEdited);
    connect(m_speedThresholdEdit,  &QLineEdit::editingFinished, this, &MainWindow::onSpeedThresholdEdited);
    connect(m_numTracksEdit,       &QLineEdit::editingFinished, this, &MainWindow::onNumTracksEdited);
    connect(m_medianFilterEdit,    &QLineEdit::editingFinished, this, &MainWindow::onMedianFilterEdited);
    connect(m_mtiLengthEdit,       &QLineEdit::editingFinished, this, &MainWindow::onMtiLengthEdited);
    connect(m_applyButton,         &QPushButton::clicked,       this, &MainWindow::onApplySettings);
    connect(m_saveSettingsButton,  &QPushButton::clicked,       this, &MainWindow::onSaveSettings);
    connect(m_loadFromFileButton,  &QPushButton::clicked,       this, &MainWindow::onLoadFromFile);
    connect(m_saveToFileButton,    &QPushButton::clicked,       this, &MainWindow::onSaveToFile);

    // ========== ASSEMBLE MAIN LAYOUT ==========
    // Add DSP Settings on the left (responsive width based on screen size)
    int dspMinWidth = qMax(280, static_cast<int>(280 * dpiScale));
    int dspMaxWidth = qMax(350, static_cast<int>(screenGeometry.width() * 0.20));  // Max 20% of screen width
    m_dspSettingsGroup->setMinimumWidth(dspMinWidth);
    m_dspSettingsGroup->setMaximumWidth(dspMaxWidth);
    mainLayout->addWidget(m_dspSettingsGroup, 0);
    
    // ========== CREATE TAB WIDGET FOR MAIN CONTENT ==========
    m_mainTabWidget = new QTabWidget(this);
    m_mainTabWidget->setStyleSheet(R"(
        QTabWidget::pane {
            border: 1px solid #e5e5e5;
            border-radius: 8px;
            background-color: #ffffff;
            padding: 4px;
        }
        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #fafafa, stop:1 #f5f5f5);
            border: 1px solid #e5e5e5;
            border-bottom: none;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            padding: 10px 20px;
            margin-right: 2px;
            font-weight: 500;
            font-size: 14px;
            color: #525252;
        }
        QTabBar::tab:selected {
            background: #ffffff;
            color: #0a0a0a;
            font-weight: 600;
            border-bottom: 2px solid #1a1a1a;
        }
        QTabBar::tab:hover:!selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff, stop:1 #fafafa);
            color: #1a1a1a;
        }
    )");
    
    // Create a container widget for the radar visualization (existing content)
    QWidget* radarTab = new QWidget();
    QVBoxLayout* radarLayout = new QVBoxLayout(radarTab);
    radarLayout->setContentsMargins(0, 0, 0, 0);
    radarLayout->addWidget(rightVerticalSplitter);
    
    // Add tabs
    m_mainTabWidget->addTab(radarTab, "Detection");
    
    // Create TimeSeries Plots tab (before Speed Measurement)
    m_timeSeriesPlotsWidget = new TimeSeriesPlotsWidget(this);
    m_mainTabWidget->addTab(m_timeSeriesPlotsWidget, "TimeSeries Plots");
    
    // Create Speed Measurement tab
    m_speedMeasurementWidget = new SpeedMeasurementWidget(this);
    m_mainTabWidget->addTab(m_speedMeasurementWidget, "Speed Measurement");
    
    // Create Logging widget (will be shown in separate window, not as tab)
    m_loggingWidget = nullptr;
    m_loggingWindow = nullptr;
    
    // Add the tab widget to the main layout with stretch
    mainLayout->addWidget(m_mainTabWidget, 1);

    // Store splitters for compatibility (if needed elsewhere)
    m_mainSplitter = nullptr;
    m_rightSplitter = rightVerticalSplitter;
    
    // Keep labels as hidden placeholders for status updates (not displayed)
    m_frameCountLabel = new QLabel("Frames: 0");
    m_statusLabel = new QLabel("Status: Ready");
    m_frameCountLabel->hide();
    m_statusLabel->hide();
    
    // Remove the status bar from the main window
    setStatusBar(nullptr);
}

void MainWindow::setupNetworking()
{
    m_udpSocket = new QUdpSocket(this);
    if (!m_udpSocket->bind(QHostAddress::Any, UDP_PORT)) {
        QMessageBox::warning(this, "Network Error",
                           QString("Failed to bind to UDP port %1").arg(UDP_PORT));
        m_statusLabel->setText("Status: Network Error - Simulation Only");
    } else {
        connect(m_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams);
        m_statusLabel->setText("Status: UDP Listening (Binary & Text)");
    }
}

void MainWindow::setupTimer()
{
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateDisplay);
    m_updateTimer->start(UPDATE_INTERVAL_MS);
    // Setup track table auto-refresh timer (2 seconds)
    m_trackRefreshTimer = new QTimer(this);
    connect(m_trackRefreshTimer, &QTimer::timeout,
            this, &MainWindow::refreshTrackTable);
    m_trackRefreshTimer->start(TRACK_REFRESH_INTERVAL_MS);
}

void MainWindow::updateDisplay()
{
    if (m_simulationEnabled) {
        generateSimulatedTargetData();
        generateSimulatedADCData();
    }

    m_ppiWidget->updateTargets(m_currentTargets);
    m_fftWidget->updateData(m_currentADCFrame);
    m_fftWidget->updateTargets(m_currentTargets);
    
    // Update speed measurement widget with target data
    if (m_speedMeasurementWidget) {
        m_speedMeasurementWidget->updateFromTargets(m_currentTargets);
    }
    
    // Update time series plots widget with target data
    if (m_timeSeriesPlotsWidget) {
        m_timeSeriesPlotsWidget->updateFromTargets(m_currentTargets);
    }
    
    // Update logging widget with target data
    if (m_loggingWidget) {
        m_loggingWidget->updateFromTargets(m_currentTargets);
    }
    
    updateTrackTable();

    m_frameCount++;
    //m_frameCountLabel->setText(QString("Frames: %1").arg(m_frameCount));

    if (m_simulationEnabled) {
        m_statusLabel->setText(QString("Status: Simulation Active - %1 targets")
                              .arg(m_currentTargets.numTracks));
    }
}

void MainWindow::readPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        qDebug() << "========================================";
        qDebug() << "Received datagram of size:" << datagram.size() << "bytes";

        // Check if it's a binary packet (minimum 4 bytes for message_type)
        if (datagram.size() >= 4) {
            // Peek at first 4 bytes to check message type
            const uint32_t* msg_type_ptr = reinterpret_cast<const uint32_t*>(datagram.constData());
            uint32_t msg_type = *msg_type_ptr;

            if (msg_type == 0x01) {
                // Binary raw ADC data packet
                qDebug() << "Detected binary RAW DATA packet (0x01)";
                parseBinaryRawData(datagram);
                continue;
            } else if (msg_type == 0x02) {
                // Binary target data packet
                qDebug() << "Detected binary TARGET DATA packet (0x02)";
                parseBinaryTargetData(datagram);
                continue;
            }
        }

        // Try text-based parsing
        QString msg = QString::fromUtf8(datagram);
        if (msg.contains("NumTargets:")) {
            parseTrackMessage(msg);
        }
        if (msg.contains("ADC:")) {
            parseADCMessage(msg);
        }
    }
}

//==============================================================================
// BINARY UDP PARSING - NEW CODE
//==============================================================================
void MainWindow::parseBinaryRawData(const QByteArray& datagram)
{
    if (datagram.size() < static_cast<int>(sizeof(RawDataHeader_t))) {
        qWarning() << "Datagram too small for binary header";
        return;
    }

    const RawDataHeader_t* header = reinterpret_cast<const RawDataHeader_t*>(datagram.constData());

    if (header->message_type != 0x01) {
        qWarning() << "Unknown binary message type:" << QString::number(header->message_type, 16);
        return;
    }

    qDebug() << "=== Binary Raw Data Frame ===";
    qDebug() << "Message Type:    0x" << QString::number(header->message_type, 16);
    qDebug() << "Frame Number:   " << header->frame_number;
    qDebug() << "Num Chirps:     " << header->num_chirps;
    qDebug() << "Num RX Antennas:" << header->num_rx_antennas;
    qDebug() << "Samples/Chirp:  " << header->num_samples_per_chirp;
    qDebug() << "RX Mask:         0x" << QString::number(header->rx_mask, 16);
    qDebug() << "ADC Resolution: " << header->adc_resolution << "bits";
    qDebug() << "Interleaved RX: " << header->interleaved_rx;
    qDebug() << "Data Format:    " << header->data_format
             << (header->data_format == 1 ? " (Complex I/Q)" : " (Real)");

    uint32_t total_samples = header->num_samples_per_chirp * header->num_chirps * header->num_rx_antennas;
    uint32_t expected_data_size = total_samples * sizeof(float);
    uint32_t expected_total_size = sizeof(RawDataHeader_t) + expected_data_size;

    qDebug() << "Total samples:  " << total_samples;
    qDebug() << "Expected size:  " << expected_total_size << "bytes";
    qDebug() << "Actual size:    " << datagram.size() << "bytes";

    if (datagram.size() < static_cast<int>(expected_total_size)) {
        qWarning() << "Datagram size mismatch. Expected:" << expected_total_size
                   << "Got:" << datagram.size();
        return;
    }

    const float* sample_data = reinterpret_cast<const float*>(
        datagram.constData() + sizeof(RawDataHeader_t)
    );

    processRawDataFrame(header, sample_data, total_samples);

    if (m_simulationEnabled) {
        m_simulationEnabled = false;
    }

    m_statusLabel->setText(QString("Binary Data - Frame %1, %2 samples")
                          .arg(header->frame_number)
                          .arg(m_currentADCFrame.complex_data.size()));
}

void MainWindow::processRawDataFrame(const RawDataHeader_t* header,
                                     const float* sample_data,
                                     uint32_t total_samples)
{
    m_currentADCFrame.complex_data.clear();

    uint32_t samples_per_chirp = header->num_samples_per_chirp;
    bool is_complex = (header->data_format == 1);

    if (is_complex) {
        // Data format: I0, Q0, I1, Q1, I2, Q2, ...
        uint32_t num_complex_samples = samples_per_chirp / 2;
        m_currentADCFrame.complex_data.reserve(num_complex_samples);
        m_currentADCFrame.num_samples_per_chirp = num_complex_samples;

        for (uint32_t i = 0; i < num_complex_samples && (i * 2 + 1) < total_samples; ++i) {
            ComplexSample sample;
            sample.I = sample_data[i * 2];
            sample.Q = sample_data[i * 2 + 1];
            m_currentADCFrame.complex_data.push_back(sample);
        }
    } else {
        // Real-only data
        m_currentADCFrame.complex_data.reserve(samples_per_chirp);
        m_currentADCFrame.num_samples_per_chirp = samples_per_chirp;

        for (uint32_t i = 0; i < samples_per_chirp && i < total_samples; ++i) {
            ComplexSample sample;
            sample.I = sample_data[i];
            sample.Q = 0.0f;
            m_currentADCFrame.complex_data.push_back(sample);
        }
    }

    m_currentADCFrame.computeMagnitudes();

    qDebug() << "Processed" << m_currentADCFrame.complex_data.size() << "complex samples";
    if (!m_currentADCFrame.complex_data.empty()) {
        qDebug() << "First sample: I=" << m_currentADCFrame.complex_data[0].I
                 << "Q=" << m_currentADCFrame.complex_data[0].Q
                 << "Mag=" << m_currentADCFrame.complex_data[0].magnitude();
    }
}

void MainWindow::parseBinaryTargetData(const QByteArray& datagram)
{
    if (datagram.size() < static_cast<int>(sizeof(TargetDataPacket_t))) {
        qWarning() << "Datagram too small for binary target packet";
        return;
    }

    const TargetDataPacket_t* packet = reinterpret_cast<const TargetDataPacket_t*>(datagram.constData());

    // Validate message type
    if (packet->message_type != 0x02) {
        qWarning() << "Unknown target message type:" << QString::number(packet->message_type, 16);
        return;
    }

    qDebug() << "=== Binary Target Data Packet ===";
    qDebug() << "Message Type:    0x" << QString::number(packet->message_type, 16);
    qDebug() << "Num Targets:    " << packet->num_targets;
    qDebug() << "Target ID:      " << packet->target_id;
    qDebug() << "Level:          " << packet->level << "dB";
    qDebug() << "Radius:         " << packet->radius << "m";
    qDebug() << "Azimuth:        " << packet->azimuth << "°";
    qDebug() << "Elevation:      " << packet->elevation << "°";
    qDebug() << "Radial Speed:   " << packet->radial_speed << "m/s";
    qDebug() << "Azimuth Speed:  " << packet->azimuth_speed << "°/s";
    qDebug() << "Elevation Speed:" << packet->elevation_speed << "°/s";

    // EPHEMERAL SYNCHRONIZATION: Frame-based track collection
    // Tracks are only shown when present in the current frame
    
    // Check if this is the start of a new frame (num_targets changed or we've received all expected)
    if (packet->num_targets != m_expectedNumTargets || m_receivedTargetCount >= m_expectedNumTargets) {
        // New frame starting - clear the frame buffer
        m_frameTargets.clear();
        m_expectedNumTargets = packet->num_targets;
        m_receivedTargetCount = 0;
        qDebug() << "Starting new frame - expecting" << m_expectedNumTargets << "targets";
    }

    // Handle special case: num_targets is 0 (no targets in this frame)
    if (packet->num_targets == 0) {
        // Clear all targets - no tracks in this frame
        m_currentTargets.targets.clear();
        m_currentTargets.numTracks = 0;
        qDebug() << "No targets in frame - clearing all tracks";
        m_statusLabel->setText("Binary Target Data - 0 targets (frame empty)");
        return;
    }

    // Create target from packet data
    TargetTrack new_target;
    new_target.target_id = packet->target_id;
    new_target.level = packet->level;
    new_target.radius = packet->radius;
    new_target.azimuth = packet->azimuth;
    new_target.elevation = packet->elevation;
    new_target.radial_speed = packet->radial_speed;
    new_target.azimuth_speed = packet->azimuth_speed;
    new_target.elevation_speed = packet->elevation_speed;
    new_target.lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    
    // Log track data to file
    logTrackDataToFile(new_target);

    // Add to frame buffer (check for duplicates in current frame)
    bool found = false;
    for (auto& target : m_frameTargets) {
        if (target.target_id == packet->target_id) {
            // Update existing target in frame buffer
            target = new_target;
            found = true;
            qDebug() << "Updated target ID:" << packet->target_id << "in frame buffer";
            break;
        }
    }

    if (!found) {
        m_frameTargets.push_back(new_target);
        m_receivedTargetCount++;
        qDebug() << "Added target ID:" << packet->target_id << "to frame buffer (" 
                 << m_receivedTargetCount << "/" << m_expectedNumTargets << ")";
    }

    // Check if we've received all targets for this frame
    if (m_receivedTargetCount >= m_expectedNumTargets) {
        // Apply the frame - replace current targets entirely (ephemeral sync)
        applyFrameTargets();
    }

    // Disable simulation when receiving real data
    if (m_simulationEnabled) {
        m_simulationEnabled = false;
    }

    m_statusLabel->setText(QString("Binary Target Data - %1/%2 targets, ID: %3")
                          .arg(m_receivedTargetCount)
                          .arg(m_expectedNumTargets)
                          .arg(packet->target_id));
}

//==============================================================================
// TEXT-BASED PARSING - EXISTING CODE (kept for backward compatibility)
//==============================================================================
void MainWindow::parseTrackMessage(const QString& message)
{
    qDebug() << "Parsing text track message";
    QStringList tokens = message.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
    m_currentTargets.targets.clear();
    m_currentTargets.numTracks = 0;

    TargetTrack target;
    int parsedTargets = 0;
     bool currentTargetValid = true;

    for (int i = 0; i < tokens.size(); ++i) {
        const QString& token = tokens[i];

        if (token == "NumTargets:" && i + 1 < tokens.size()) {
            tokens[++i].toInt();
        } else if (token == "TgtId:" && i + 1 < tokens.size()) {
            if (parsedTargets > 0 && currentTargetValid) {
                // Set timestamp for the completed target before adding
                target.lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
                m_currentTargets.targets.push_back(target);
                // Log the completed target
                logTrackDataToFile(target);
            }
            target = TargetTrack();
            target.target_id = tokens[++i].toInt();
            // Invalidate track data if track id > 50
            if (target.target_id > 50) {
                qDebug() << "Invalidating track with ID:" << target.target_id << "(ID > 50)";
                currentTargetValid = false;
            } else {
                currentTargetValid = true;
            }
            ++parsedTargets;
        } else if (token == "Level:" && i + 1 < tokens.size()) {
            target.level = tokens[++i].toFloat();
        } else if (token == "Range:" && i + 1 < tokens.size()) {
            target.radius = tokens[++i].toFloat() / 100.0;
        } else if (token == "Azimuth:" && i + 1 < tokens.size()) {
            target.azimuth = tokens[++i].toFloat();
        } else if (token == "Elevation:" && i + 1 < tokens.size()) {
            target.elevation = tokens[++i].toFloat();
        } else if (token == "RadialSpeed:" && i + 1 < tokens.size()) {
            target.radial_speed = tokens[++i].toFloat();
        } else if (token == "AzimuthSpeed:" && i + 1 < tokens.size()) {
            target.azimuth_speed = tokens[++i].toFloat();
        } else if (token == "ElevationSpeed:" && i + 1 < tokens.size()) {
            target.elevation_speed = tokens[++i].toFloat();
        }
    }

    // Append final target if one exists and is valid (target_id <= 50)
    if (parsedTargets > 0 && currentTargetValid) {
        // Set timestamp for the final target before adding
        target.lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
        m_currentTargets.targets.push_back(target);
        // Log the final target
        logTrackDataToFile(target);
    }

    m_currentTargets.numTracks = m_currentTargets.targets.size();
}

void MainWindow::parseADCMessage(const QString& message)
{
    QStringList tokens = message.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
    qDebug() << "Parsing text ADC message";
    RawADCFrameTest frame;
    frame.complex_data.clear();

    std::vector<float> raw_samples;
    qDebug() << "tokens.size() " << tokens.size();

    for (int i = 0; i < tokens.size(); ++i) {
        const QString& token = tokens[i];
        if (token == "MsgId:" && i + 1 < tokens.size()) {
            frame.msgId = tokens[++i].toUInt();
        } else if (token == "NumSamples:" && i + 1 < tokens.size()) {
            uint32_t totalSamples = tokens[++i].toUInt();
            frame.num_samples_per_chirp = totalSamples / 2;
        } else if (token == "ADC:" && i + 1 < tokens.size()) {
            raw_samples.push_back(tokens[++i].toFloat());
        }
    }

    frame.complex_data.reserve(raw_samples.size() / 2);
    qDebug() << "raw_samples.size() " << raw_samples.size();

    for (size_t i = 0; i < raw_samples.size(); i += 2) {
        if (i + 1 < raw_samples.size()) {
            ComplexSample complex_sample;
            complex_sample.I = raw_samples[i];
            complex_sample.Q = raw_samples[i + 1];
            frame.complex_data.push_back(complex_sample);
        }
    }

    frame.computeMagnitudes();
    m_currentADCFrame = frame;

    qDebug() << "Parsed" << frame.complex_data.size() << "complex samples";
    if (!frame.complex_data.empty()) {
        qDebug() << "First sample: I=" << frame.complex_data[0].I
                 << "Q=" << frame.complex_data[0].Q
                 << "Mag=" << frame.complex_data[0].magnitude();
    }
}

void MainWindow::applyFrameTargets()
{
    // EPHEMERAL SYNCHRONIZATION: Replace current targets with frame targets
    // This ensures only tracks present in the current frame are displayed
    // Tracks not in this frame are immediately removed from display
    
    m_currentTargets.targets = m_frameTargets;
    m_currentTargets.numTracks = static_cast<uint32_t>(m_frameTargets.size());
    
    qDebug() << "Applied frame targets - now showing" << m_currentTargets.numTracks << "tracks";
    
    // Clear the frame buffer for the next frame
    m_frameTargets.clear();
    m_receivedTargetCount = 0;
    
    // Update displays immediately for synchronization
    m_ppiWidget->updateTargets(m_currentTargets);
    m_fftWidget->updateTargets(m_currentTargets);
    updateTrackTable();
}

void MainWindow::refreshTrackTable()
{
    // EPHEMERAL SYNCHRONIZATION: Periodic refresh for UI consistency
    // Tracks are already managed frame-by-frame in parseBinaryTargetData
    // This function just ensures the UI stays in sync with current data
    
    // Keep minimum rows in track table, extend if more tracks are available
    int rowCount = std::max(static_cast<int>(m_currentTargets.numTracks), TRACK_TABLE_MINIMUM_ROWS);
    m_trackTable->setRowCount(rowCount);
    
    // Populate rows with actual track data
    for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
        const TargetTrack& target = m_currentTargets.targets[i];
        m_trackTable->setItem(i, 0, new QTableWidgetItem(QString::number(target.target_id)));
        m_trackTable->setItem(i, 1, new QTableWidgetItem(QString::number(target.radius, 'f', 2)));
        m_trackTable->setItem(i, 2, new QTableWidgetItem(QString::number(target.azimuth, 'f', 1)));
        m_trackTable->setItem(i, 3, new QTableWidgetItem(QString::number(target.radial_speed, 'f', 1)));
    }
    
    // Clear empty rows (when fewer tracks than minimum)
    for (int i = m_currentTargets.numTracks; i < rowCount; ++i) {
        m_trackTable->setItem(i, 0, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 1, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 2, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 3, new QTableWidgetItem(""));
    }
    m_trackTable->resizeColumnsToContents();

    // Sync PPI and FFT widgets with current data
    m_ppiWidget->updateTargets(m_currentTargets);
    m_fftWidget->updateTargets(m_currentTargets);

    qDebug() << "Track table refreshed. Active tracks:" << m_currentTargets.numTracks
             << ", Display rows:" << rowCount << "(ephemeral sync mode)";
}

//==============================================================================
// SIMULATION CODE
//==============================================================================
void MainWindow::onSimulateDataToggled()
{
    // Simulation toggle removed - simulation disabled by default
    m_simulationEnabled = false;
    m_statusLabel->setText("Status: Waiting for Data");
}

void MainWindow::generateSimulatedTargetData()
{
    uint32_t numTargets = m_numTargetsDist(m_randomEngine);
    m_currentTargets.resize(numTargets);

    for (uint32_t i = 0; i < numTargets; ++i) {
        TargetTrack& target = m_currentTargets.targets[i];
        target.target_id = i + 1;
        target.level = m_levelDist(m_randomEngine);
        target.radius = m_rangeDist(m_randomEngine);
        target.azimuth = m_azimuthDist(m_randomEngine);
        target.elevation = std::uniform_real_distribution<float>(-30.0f, 30.0f)(m_randomEngine);
        target.radial_speed = m_speedDist(m_randomEngine);
        target.azimuth_speed = std::uniform_real_distribution<float>(-5.0f, 5.0f)(m_randomEngine);
        target.elevation_speed = std::uniform_real_distribution<float>(-2.0f, 2.0f)(m_randomEngine);
    }
    m_targetCount += numTargets;
}

void MainWindow::generateSimulatedADCData()
{
    uint32_t numComplexSamples = 32;
    m_currentADCFrame.complex_data.resize(numComplexSamples);
    m_currentADCFrame.num_samples_per_chirp = numComplexSamples;

    float sampleRate = 100000.0f;
    float t_step = 1.0f / sampleRate;
    float freq1 = 5000.0f;
    float freq2 = 15000.0f;
    float freq3 = 25000.0f;
    std::uniform_real_distribution<float> noiseDist(-0.05f, 0.05f);

    for (uint32_t i = 0; i < numComplexSamples; ++i) {
        float t = i * t_step;
        float I_signal = 0.8f * std::cos(2.0f * M_PI * freq1 * t) +
                        0.5f * std::cos(2.0f * M_PI * freq2 * t) +
                        0.3f * std::cos(2.0f * M_PI * freq3 * t);
        float Q_signal = 0.8f * std::sin(2.0f * M_PI * freq1 * t) +
                        0.5f * std::sin(2.0f * M_PI * freq2 * t) +
                        0.3f * std::sin(2.0f * M_PI * freq3 * t);
        I_signal += noiseDist(m_randomEngine);
        Q_signal += noiseDist(m_randomEngine);
        m_currentADCFrame.complex_data[i].I = I_signal;
        m_currentADCFrame.complex_data[i].Q = Q_signal;
    }
    m_currentADCFrame.computeMagnitudes();
}

//==============================================================================
// DSP SETTINGS
//==============================================================================
void MainWindow::onApplySettings()
{
    dsp_message_t msg{};
    msg.msg_type = DSP_CMD_SET;

    msg.settings.range_mvg_avg_length = m_dsp.range_mvg_avg_length;
    msg.settings.min_range_cm         = m_dsp.min_range_cm;
    msg.settings.max_range_cm         = m_dsp.max_range_cm;
    msg.settings.min_speed_kmh        = m_dsp.min_speed_kmh;
    msg.settings.max_speed_kmh        = m_dsp.max_speed_kmh;
    msg.settings.min_angle_degree     = m_dsp.min_angle_degree;
    msg.settings.max_angle_degree     = m_dsp.max_angle_degree;
    msg.settings.range_threshold      = m_dsp.range_threshold;
    msg.settings.speed_threshold      = m_dsp.speed_threshold;
    msg.settings.enable_tracking      = m_dsp.enable_tracking;
    msg.settings.num_of_tracks        = m_dsp.num_of_tracks;
    msg.settings.median_filter_length = m_dsp.median_filter_length;
    msg.settings.enable_mti_filter    = m_dsp.enable_mti_filter;
    msg.settings.mti_filter_length    = m_dsp.mti_filter_length;

    // Apply settings to both PPI and FFT displays (convert cm to meters for range)
    float minRangeMeters = m_dsp.min_range_cm / 100.0f;
    float maxRangeMeters = m_dsp.max_range_cm / 100.0f;
    
    // Apply range settings
    m_ppiWidget->setMinRange(minRangeMeters);
    m_ppiWidget->setMaxRange(maxRangeMeters);
    m_fftWidget->setMinRange(minRangeMeters);
    m_fftWidget->setMaxRange(maxRangeMeters);
    
    // Apply angle settings
    m_ppiWidget->setMinAngle(static_cast<float>(m_dsp.min_angle_degree));
    m_ppiWidget->setMaxAngle(static_cast<float>(m_dsp.max_angle_degree));
    m_fftWidget->setMinAngle(static_cast<float>(m_dsp.min_angle_degree));
    m_fftWidget->setMaxAngle(static_cast<float>(m_dsp.max_angle_degree));

    QByteArray datagram(reinterpret_cast<const char*>(&msg), sizeof(dsp_message_t));
    QUdpSocket udp;
    qint64 bytesSent = udp.writeDatagram(datagram, QHostAddress("127.0.0.1"), 4000);

    if (bytesSent == sizeof(dsp_message_t)) {
        m_statusLabel->setText(QString("DSP settings applied - Range: %1-%2m, Angle: %3° to %4°")
                              .arg(minRangeMeters, 0, 'f', 1)
                              .arg(maxRangeMeters, 0, 'f', 1)
                              .arg(m_dsp.min_angle_degree)
                              .arg(m_dsp.max_angle_degree));
        qDebug() << "DSP settings sent successfully" << bytesSent << "bytes";
        qDebug() << "Range:" << minRangeMeters << "-" << maxRangeMeters << "m";
        qDebug() << "Angle:" << m_dsp.min_angle_degree << "-" << m_dsp.max_angle_degree << "deg";
        qDebug() << "Speed:" << m_dsp.min_speed_kmh << "-" << m_dsp.max_speed_kmh<< "kmh";
    } else {
        m_statusLabel->setText("ERROR: DSP UDP transmission failed");
        qDebug() << "DSP settings transmission failed";
    }
}

void MainWindow::updateTrackTable()
{
    // EPHEMERAL SYNCHRONIZATION: Update track table with current frame data
    // Keep minimum rows in track table, extend if more tracks are available
    int rowCount = std::max(static_cast<int>(m_currentTargets.numTracks), TRACK_TABLE_MINIMUM_ROWS);
    m_trackTable->setRowCount(rowCount);
    
    // Populate rows with actual track data
    for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
        const TargetTrack& target = m_currentTargets.targets[i];
        m_trackTable->setItem(i, 0, new QTableWidgetItem(QString::number(target.target_id)));
        m_trackTable->setItem(i, 1, new QTableWidgetItem(QString::number(target.radius, 'f', 2)));
        m_trackTable->setItem(i, 2, new QTableWidgetItem(QString::number(target.azimuth, 'f', 1)));
        m_trackTable->setItem(i, 3, new QTableWidgetItem(QString::number(target.radial_speed, 'f', 1)));
    }
    
    // Clear empty rows (when fewer tracks than minimum)
    for (int i = m_currentTargets.numTracks; i < rowCount; ++i) {
        m_trackTable->setItem(i, 0, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 1, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 2, new QTableWidgetItem(""));
        m_trackTable->setItem(i, 3, new QTableWidgetItem(""));
    }
    m_trackTable->resizeColumnsToContents();
}

//==============================================================================
// RADAR PARAMETER SLOTS (Existing functionality)
//==============================================================================
void MainWindow::onRangeChanged(int range) {
    // Keep existing implementation
}

void MainWindow::onMinRangeChanged(const QString& text) {
    // Keep existing implementation
}

void MainWindow::onChirpChanged(const QString& text) {
    // Keep existing implementation
}

void MainWindow::onBandwidthChanged(const QString& text) {
    // Keep existing implementation
}

void MainWindow::onMinSpeedChanged(const QString& text) {
    // Keep existing implementation
}

void MainWindow::onMaxSpeedChanged(const QString& text) {
    // Keep existing implementation
}

void MainWindow::onChirpsPerFrameChanged(const QString& text) {
    // Keep existing implementation
}

void MainWindow::onSamplesPerChirpChanged(const QString& text) {
    // Keep existing implementation
}

//==============================================================================
// DSP PARAMETER SLOTS
//==============================================================================
void MainWindow::onRangeAvgEdited()
{
    bool ok;
    int v = m_rangeAvgEdit->text().toInt(&ok);
    if (ok) m_dsp.range_mvg_avg_length = std::max(1, v);
}

void MainWindow::onMinRangeEdited()
{
    bool ok;
    int v = m_minRangeEdit->text().toInt(&ok);
    if (ok) {
        m_dsp.min_range_cm = std::max(0, v);
        // Apply to FFT widget (convert cm to meters)
        float minRangeMeters = m_dsp.min_range_cm / 100.0f;
        m_fftWidget->setMinRange(minRangeMeters);
        m_ppiWidget->setMinRange(minRangeMeters);
    }
}

void MainWindow::onMaxRangeEdited()
{
    bool ok;
    int v = m_maxRangeEdit->text().toInt(&ok);
    if (!ok) return;
    m_dsp.max_range_cm = std::max(v, static_cast<int>(m_dsp.min_range_cm) + 1);
    // Apply to both PPI and FFT widgets (convert cm to meters)
    float maxRangeMeters = m_dsp.max_range_cm / 100.0f;
    m_ppiWidget->setMaxRange(maxRangeMeters);
    m_fftWidget->setMaxRange(maxRangeMeters);
}

void MainWindow::onMinSpeedEdited()
{
    bool ok;
    int v = m_minSpeedEdit->text().toInt(&ok);
    if (ok) m_dsp.min_speed_kmh = v;
}

void MainWindow::onMaxSpeedEdited()
{
    bool ok;
    int v = m_maxSpeedEdit->text().toInt(&ok);
    if (ok) m_dsp.max_speed_kmh = std::max(static_cast<uint16_t>(v),
                                           m_dsp.min_speed_kmh);
}

void MainWindow::onMinAngleEdited()
{
    bool ok;
    int v = m_minAngleEdit->text().toInt(&ok);
    if (ok) {
        m_dsp.min_angle_degree = v;
        // Apply to both PPI and FFT widgets
        m_ppiWidget->setMinAngle(static_cast<float>(v));
        m_fftWidget->setMinAngle(static_cast<float>(v));
    }
}

void MainWindow::onMaxAngleEdited()
{
    bool ok;
    int v = m_maxAngleEdit->text().toInt(&ok);
    if (ok) {
        m_dsp.max_angle_degree = v;
        // Apply to both PPI and FFT widgets
        m_ppiWidget->setMaxAngle(static_cast<float>(v));
        m_fftWidget->setMaxAngle(static_cast<float>(v));
    }
}

void MainWindow::onRangeThresholdEdited()
{
    bool ok;
    int v = m_rangeThresholdEdit->text().toInt(&ok);
    if (ok) m_dsp.range_threshold = v;
}

void MainWindow::onSpeedThresholdEdited()
{
    bool ok;
    int v = m_speedThresholdEdit->text().toInt(&ok);
    if (ok) m_dsp.speed_threshold = v;
}

void MainWindow::onNumTracksEdited()
{
    bool ok;
    int v = m_numTracksEdit->text().toInt(&ok);
    if (ok) m_dsp.num_of_tracks = std::max(1, v);
}

void MainWindow::onMedianFilterEdited()
{
    bool ok;
    int v = m_medianFilterEdit->text().toInt(&ok);
    if (ok) m_dsp.median_filter_length = std::max(1, v | 1);
}

void MainWindow::onMtiLengthEdited()
{
    bool ok;
    int v = m_mtiLengthEdit->text().toInt(&ok);
    if (ok) m_dsp.mti_filter_length = std::max(1, v);
}

//==============================================================================
// MENU BAR
//==============================================================================
void MainWindow::createMenuBar()
{
    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setStyleSheet(R"(
        QMenuBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff, stop:1 #fafafa);
            border: none;
            border-bottom: 1px solid #e5e5e5;
            padding: 6px 12px;
            spacing: 6px;
            min-height: 16px;
        }
        QMenuBar::item {
            background-color: transparent;
            padding: 8px 16px;
            border-radius: 8px;
            font-size: 13px;
            font-weight: 600;
            color: #525252;
            letter-spacing: 0.2px;
        }
        QMenuBar::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(26, 26, 26, 0.1), stop:1 rgba(26, 26, 26, 0.06));
            color: #1a1a1a;
        }
        QMenuBar::item:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(26, 26, 26, 0.15), stop:1 rgba(26, 26, 26, 0.1));
            color: #0a0a0a;
        }
        QMenu {
            background-color: #ffffff;
            border: 1px solid #e5e5e5;
            border-radius: 12px;
            padding: 8px;
            margin-top: 4px;
        }
        QMenu::item {
            padding: 10px 32px 10px 16px;
            border-radius: 6px;
            font-size: 15px;
            font-weight: 500;
            color: #0a0a0a;
            margin: 2px 4px;
        }
        QMenu::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(26, 26, 26, 0.1), stop:1 rgba(26, 26, 26, 0.06));
            color: #1a1a1a;
        }
        QMenu::item:disabled {
            color: #a3a3a3;
        }
        QMenu::separator {
            height: 1px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 transparent, stop:0.1 #e5e5e5, stop:0.9 #e5e5e5, stop:1 transparent);
            margin: 8px 12px;
        }
        QMenu::icon {
            margin-left: 12px;
        }
        QMenu::indicator {
            width: 16px;
            height: 16px;
            margin-left: 8px;
        }
        QMenu::indicator:checked {
            background-color: #1a1a1a;
            border-radius: 4px;
        }
    )");
    
    // File Menu
    QMenu* fileMenu = menuBar->addMenu(tr("&File"));
    
    QAction* openAction = fileMenu->addAction(tr("&Open Configuration..."));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, [this]() {
        QString fileName = QFileDialog::getOpenFileName(this, 
            tr("Open Configuration"), "", tr("Configuration Files (*.ini);;All Files (*)"));
        if (!fileName.isEmpty()) {
            QSettings settings(fileName, QSettings::IniFormat);
            // Load settings from file
            settings.beginGroup("DSPSettings");
            if (settings.contains("rangeAvg")) m_rangeAvgEdit->setText(settings.value("rangeAvg").toString());
            if (settings.contains("minRange")) m_minRangeEdit->setText(settings.value("minRange").toString());
            if (settings.contains("maxRange")) m_maxRangeEdit->setText(settings.value("maxRange").toString());
            if (settings.contains("minSpeed")) m_minSpeedEdit->setText(settings.value("minSpeed").toString());
            if (settings.contains("maxSpeed")) m_maxSpeedEdit->setText(settings.value("maxSpeed").toString());
            if (settings.contains("minAngle")) m_minAngleEdit->setText(settings.value("minAngle").toString());
            if (settings.contains("maxAngle")) m_maxAngleEdit->setText(settings.value("maxAngle").toString());
            if (settings.contains("rangeThreshold")) m_rangeThresholdEdit->setText(settings.value("rangeThreshold").toString());
            if (settings.contains("speedThreshold")) m_speedThresholdEdit->setText(settings.value("speedThreshold").toString());
            if (settings.contains("numTracks")) m_numTracksEdit->setText(settings.value("numTracks").toString());
            if (settings.contains("medianFilter")) m_medianFilterEdit->setText(settings.value("medianFilter").toString());
            if (settings.contains("mtiLength")) m_mtiLengthEdit->setText(settings.value("mtiLength").toString());
            settings.endGroup();
            m_statusLabel->setText(QString("Status: Configuration loaded from %1").arg(QFileInfo(fileName).fileName()));
        }
    });
    
    QAction* saveAction = fileMenu->addAction(tr("&Save Configuration"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveSettings);
    
    QAction* saveAsAction = fileMenu->addAction(tr("Save Configuration &As..."));
    saveAsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));
    connect(saveAsAction, &QAction::triggered, this, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Configuration"), "", tr("Configuration Files (*.ini);;All Files (*)"));
        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".ini")) fileName += ".ini";
            QSettings settings(fileName, QSettings::IniFormat);
            settings.beginGroup("DSPSettings");
            settings.setValue("rangeAvg", m_rangeAvgEdit->text());
            settings.setValue("minRange", m_minRangeEdit->text());
            settings.setValue("maxRange", m_maxRangeEdit->text());
            settings.setValue("minSpeed", m_minSpeedEdit->text());
            settings.setValue("maxSpeed", m_maxSpeedEdit->text());
            settings.setValue("minAngle", m_minAngleEdit->text());
            settings.setValue("maxAngle", m_maxAngleEdit->text());
            settings.setValue("rangeThreshold", m_rangeThresholdEdit->text());
            settings.setValue("speedThreshold", m_speedThresholdEdit->text());
            settings.setValue("numTracks", m_numTracksEdit->text());
            settings.setValue("medianFilter", m_medianFilterEdit->text());
            settings.setValue("mtiLength", m_mtiLengthEdit->text());
            settings.endGroup();
            settings.sync();
            m_statusLabel->setText(QString("Status: Configuration saved to %1").arg(QFileInfo(fileName).fileName()));
        }
    });
    
    fileMenu->addSeparator();
    
    QAction* exportAction = fileMenu->addAction(tr("&Export Data..."));
    exportAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    connect(exportAction, &QAction::triggered, this, [this]() {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Export Data"), "", tr("CSV Files (*.csv);;All Files (*)"));
        if (!fileName.isEmpty()) {
            // Export track data to CSV
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << "ID,Range (m),Azimuth (deg),Radial Speed (m/s)\n";
                for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
                    const TargetTrack& target = m_currentTargets.targets[i];
                    out << target.target_id << "," 
                        << target.radius << "," 
                        << target.azimuth << "," 
                        << target.radial_speed << "\n";
                }
                file.close();
                m_statusLabel->setText(QString("Status: Data exported to %1").arg(QFileInfo(fileName).fileName()));
            }
        }
    });
    
    fileMenu->addSeparator();
    
    QAction* exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    // View Menu
    QMenu* viewMenu = menuBar->addMenu(tr("&View"));
    
    QAction* loggingAction = viewMenu->addAction(tr("&Logging"));
    loggingAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    connect(loggingAction, &QAction::triggered, this, &MainWindow::onOpenLoggingWindow);
    
    viewMenu->addSeparator();
    
    QAction* fullScreenAction = viewMenu->addAction(tr("&Full Screen"));
    fullScreenAction->setShortcut(QKeySequence(Qt::Key_F11));
    fullScreenAction->setCheckable(true);
    connect(fullScreenAction, &QAction::triggered, this, [this, fullScreenAction](bool checked) {
        if (checked) {
            showFullScreen();
        } else {
            showNormal();
        }
    });
    
    viewMenu->addSeparator();
    
    QAction* refreshAction = viewMenu->addAction(tr("&Refresh"));
    refreshAction->setShortcut(QKeySequence::Refresh);
    connect(refreshAction, &QAction::triggered, this, [this]() {
        m_ppiWidget->update();
        m_fftWidget->update();
        m_statusLabel->setText("Status: Display refreshed");
    });
    
    // Connection Menu
    QMenu* connectionMenu = menuBar->addMenu(tr("&Connection"));
    
    QAction* reconnectAction = connectionMenu->addAction(tr("&Reconnect UDP"));
    reconnectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
    connect(reconnectAction, &QAction::triggered, this, [this]() {
        if (m_udpSocket) {
            m_udpSocket->close();
            if (m_udpSocket->bind(QHostAddress::Any, UDP_PORT)) {
                m_statusLabel->setText("Status: UDP reconnected on port 5000");
            } else {
                m_statusLabel->setText("Status: Failed to reconnect UDP");
            }
        }
    });
    
    connectionMenu->addSeparator();
    
    QAction* networkInfoAction = connectionMenu->addAction(tr("&Network Info..."));
    connect(networkInfoAction, &QAction::triggered, this, [this]() {
        QString info = QString("UDP Port: %1\nStatus: %2")
            .arg(UDP_PORT)
            .arg(m_udpSocket && m_udpSocket->state() == QAbstractSocket::BoundState ? "Listening" : "Not Connected");
        QMessageBox::information(this, "Network Information", info);
    });
    
//    // Tools Menu
//    QMenu* toolsMenu = menuBar->addMenu(tr("&Tools"));
    
//    QAction* applyAction = toolsMenu->addAction(tr("&Apply Settings"));
//    applyAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Return));
//    connect(applyAction, &QAction::triggered, this, &MainWindow::onApplySettings);
    
//    QAction* resetAction = toolsMenu->addAction(tr("&Reset Settings"));
//    connect(resetAction, &QAction::triggered, this, &MainWindow::onResetSettings);
    
//    toolsMenu->addSeparator();
    
//    QAction* defaultsAction = toolsMenu->addAction(tr("Restore &Defaults"));
//    connect(defaultsAction, &QAction::triggered, this, &MainWindow::onDefaultSettings);
    
//    // Help Menu
//    QMenu* helpMenu = menuBar->addMenu(tr("&Help"));
    
//    QAction* shortcutsAction = helpMenu->addAction(tr("&Keyboard Shortcuts"));
//    connect(shortcutsAction, &QAction::triggered, this, [this]() {
//        QString shortcuts =
//            "Keyboard Shortcuts:\n\n"
//            "File:\n"
//            "  Ctrl+N     New Configuration\n"
//            "  Ctrl+O     Open Configuration\n"
//            "  Ctrl+S     Save Configuration\n"
//            "  Ctrl+E     Export Data\n\n"
//            "View:\n"
//            "  F11        Toggle Full Screen\n"
//            "  F5         Refresh Display\n\n"
//            "Tools:\n"
//            "  Ctrl+Enter Apply Settings\n"
//            "  Ctrl+R     Reconnect UDP";
//        QMessageBox::information(this, "Keyboard Shortcuts", shortcuts);
//    });
    
//    helpMenu->addSeparator();
    
//    QAction* aboutAction = helpMenu->addAction(tr("&About Radar Visualization"));
//    connect(aboutAction, &QAction::triggered, this, [this]() {
//        QMessageBox::about(this, "About Radar Visualization",
//            "<h3>Radar Data Visualization</h3>"
//            "<p>Version 1.0.0</p>"
//            "<p>A modern radar data visualization and configuration "
//            "application for real-time radar signal processing and display.</p>"
//            "<p>Features:</p>"
//            "<ul>"
//            "<li>PPI (Plan Position Indicator) display</li>"
//            "<li>FFT spectrum analysis</li>"
//            "<li>Real-time target tracking</li>"
//            "<li>UDP binary and text data support</li>"
//            "</ul>"
//        );
//    });
    
//    QAction* aboutQtAction = helpMenu->addAction(tr("About &Qt"));
//    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    
    // Theme Menu (NEW - Replaces Help Menu)
        QMenu* themeMenu = menuBar->addMenu(tr("&Theme"));

        // Declare actions first for cross-referencing
        QAction* lightThemeAction = new QAction(tr("☀️  &Light Theme"), this);
        QAction* darkThemeAction = new QAction(tr("🌙  &Dark Theme"), this);

        lightThemeAction->setCheckable(true);
        lightThemeAction->setChecked(!m_isDarkTheme);
        darkThemeAction->setCheckable(true);
        darkThemeAction->setChecked(m_isDarkTheme);

        themeMenu->addAction(lightThemeAction);
        themeMenu->addAction(darkThemeAction);

        connect(lightThemeAction, &QAction::triggered, this, [this, lightThemeAction, darkThemeAction]() {
            if (!lightThemeAction->isChecked()) {
                lightThemeAction->setChecked(true);
                return;
            }
            m_isDarkTheme = false;
            applyTheme(false);
            darkThemeAction->setChecked(false);

            QSettings settings(getSettingsFilePath(), QSettings::IniFormat);
            settings.setValue("UI/theme", "light");
            m_statusLabel->setText("Status: Light theme applied");
        });

        connect(darkThemeAction, &QAction::triggered, this, [this, lightThemeAction, darkThemeAction]() {
            if (!darkThemeAction->isChecked()) {
                darkThemeAction->setChecked(true);
                return;
            }
            m_isDarkTheme = true;
            applyTheme(true);
            lightThemeAction->setChecked(false);

            QSettings settings(getSettingsFilePath(), QSettings::IniFormat);
            settings.setValue("UI/theme", "dark");
            m_statusLabel->setText("Status: Dark theme applied");
        });

        themeMenu->addSeparator();

        QAction* toggleThemeAction = themeMenu->addAction(tr("&Toggle Theme"));
        toggleThemeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
        connect(toggleThemeAction, &QAction::triggered, this, [this, lightThemeAction, darkThemeAction]() {
            m_isDarkTheme = !m_isDarkTheme;
            applyTheme(m_isDarkTheme);
            lightThemeAction->setChecked(!m_isDarkTheme);
            darkThemeAction->setChecked(m_isDarkTheme);

            QSettings settings(getSettingsFilePath(), QSettings::IniFormat);
            settings.setValue("UI/theme", m_isDarkTheme ? "dark" : "light");
            m_statusLabel->setText(QString("Status: %1 theme applied")
                .arg(m_isDarkTheme ? "Dark" : "Light"));
        });

    // Design Mode Menu (NEW - beside Theme)
    QMenu* designModeMenu = menuBar->addMenu(tr("&Design Mode"));
    
    QAction* enableDesignModeAction = designModeMenu->addAction(tr("&Enable Design Mode"));
    enableDesignModeAction->setCheckable(true);
    enableDesignModeAction->setChecked(false);
    enableDesignModeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_D));
    connect(enableDesignModeAction, &QAction::triggered, this, [this](bool checked) {
        m_statusLabel->setText(QString("Status: Design Mode %1").arg(checked ? "enabled" : "disabled"));
    });
    
    designModeMenu->addSeparator();
    
    QAction* editLayoutAction = designModeMenu->addAction(tr("&Edit Layout"));
    connect(editLayoutAction, &QAction::triggered, this, [this]() {
        m_statusLabel->setText("Status: Layout editing mode");
    });
    
    QAction* customizeWidgetsAction = designModeMenu->addAction(tr("&Customize Widgets"));
    connect(customizeWidgetsAction, &QAction::triggered, this, [this]() {
        m_statusLabel->setText("Status: Widget customization");
    });
    
    QAction* resetLayoutAction = designModeMenu->addAction(tr("&Reset to Default Layout"));
    connect(resetLayoutAction, &QAction::triggered, this, [this]() {
        m_statusLabel->setText("Status: Layout reset to default");
    });

    // Add Zoppler Systems branding to the right side of the menubar
    QWidget* brandingWidget = new QWidget(menuBar);
    QHBoxLayout* brandingLayout = new QHBoxLayout(brandingWidget);
    brandingLayout->setContentsMargins(8, 0, 16, 0);
    brandingLayout->setSpacing(8);
    
    // Logo icon
    QLabel* logoLabel = new QLabel(brandingWidget);
    QPixmap logoPixmap(":/resources/zoppler_logo.png");
    logoLabel->setPixmap(logoPixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setFixedSize(36, 36);
    brandingLayout->addWidget(logoLabel);
    
    // Company name label
    QLabel* companyLabel = new QLabel("Zoppler Systems", brandingWidget);
    companyLabel->setStyleSheet(R"(
        QLabel {
            color: #475569;
            font-size: 20px;
            font-weight: 600;
            letter-spacing: 0.5px;
            background: transparent;
            padding: 0px;
        }
    )");
    brandingLayout->addWidget(companyLabel);
    
    brandingWidget->setStyleSheet("background: transparent;");
    menuBar->setCornerWidget(brandingWidget, Qt::TopRightCorner);

    setMenuBar(menuBar);
}

QString MainWindow::getLightThemeStyleSheet() const
{
    return R"(
        /* ========== MAIN WINDOW - MONOCHROME LIGHT ========== */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #fafafa, stop:1 #f5f5f5);
            color: #0a0a0a;
        }
        
        /* ========== BASE WIDGET STYLING ========== */
        QWidget {
            background-color: transparent;
            color: #0a0a0a;
            font-family: 'Segoe UI', 'Inter', 'SF Pro Display', -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 15px;
        }
        
        /* ========== CARD-STYLE GROUP BOXES ========== */
        QGroupBox {
            font-weight: 600;
            font-size: 16px;
            color: #0a0a0a;
            border: 1px solid #e5e5e5;
            border-radius: 12px;
            margin-top: 14px;
            padding: 16px 12px 12px 12px;
            background-color: #ffffff;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            top: 2px;
            padding: 3px 12px;
            background: #1a1a1a;
            color: #ffffff;
            font-weight: 700;
            font-size: 14px;
            border-radius: 6px;
            letter-spacing: 0.3px;
        }
        
        /* ========== LABELS ========== */
        QLabel {
            color: #525252;
            font-weight: 500;
            background-color: transparent;
            letter-spacing: 0.1px;
        }
        
        /* ========== INPUT FIELDS ========== */
        QLineEdit {
            background-color: #fafafa;
            border: 2px solid #e5e5e5;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0a0a0a;
            font-weight: 500;
            font-size: 15px;
            selection-background-color: #1a1a1a;
            selection-color: #ffffff;
        }
        
        QLineEdit:hover {
            border-color: #a3a3a3;
            background-color: #ffffff;
        }
        
        QLineEdit:focus {
            border: 2px solid #1a1a1a;
            background-color: #ffffff;
        }
        
        /* ========== SPIN BOXES ========== */
        QSpinBox, QDoubleSpinBox {
            background-color: #fafafa;
            border: 2px solid #e5e5e5;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0a0a0a;
            font-weight: 500;
            selection-background-color: #1a1a1a;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #a3a3a3;
            background-color: #ffffff;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #1a1a1a;
            background-color: #ffffff;
        }
        
        QSpinBox::up-button, QSpinBox::down-button,
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            background-color: transparent;
            border: none;
            width: 24px;
            border-radius: 4px;
        }
        
        QSpinBox::up-button:hover, QSpinBox::down-button:hover,
        QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: rgba(26, 26, 26, 0.1);
        }
        
        /* ========== BUTTONS ========== */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1a1a1a, stop:1 #0a0a0a);
            border: none;
            border-radius: 10px;
            color: #ffffff;
            font-weight: 600;
            font-size: 15px;
            padding: 12px 24px;
            min-height: 20px;
            letter-spacing: 0.3px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #333333, stop:1 #1a1a1a);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0a0a0a, stop:1 #000000);
        }
        
        QPushButton:disabled {
            background: #d4d4d4;
            color: #a3a3a3;
        }
        
        /* ========== DATA TABLE ========== */
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #fafafa;
            color: #0a0a0a;
            gridline-color: transparent;
            border: 1px solid #e5e5e5;
            border-radius: 12px;
            selection-background-color: rgba(26, 26, 26, 0.1);
            selection-color: #0a0a0a;
            outline: none;
        }
        
        QTableWidget::item {
            padding: 12px 16px;
            border-bottom: 1px solid #f5f5f5;
        }
        
        QTableWidget::item:selected {
            background-color: rgba(26, 26, 26, 0.1);
            color: #0a0a0a;
        }
        
        QTableWidget::item:hover {
            background-color: rgba(26, 26, 26, 0.05);
        }
        
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #fafafa, stop:1 #f5f5f5);
            color: #525252;
            font-weight: 700;
            font-size: 13px;
            padding: 14px 16px;
            border: none;
            border-bottom: 2px solid #e5e5e5;
            text-transform: uppercase;
            letter-spacing: 0.8px;
        }
        
        /* ========== SPLITTER HANDLES ========== */
        QSplitter::handle {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #e5e5e5, stop:0.5 #d4d4d4, stop:1 #e5e5e5);
            border-radius: 3px;
        }
        
        QSplitter::handle:horizontal {
            width: 6px;
            margin: 4px 2px;
        }
        
        QSplitter::handle:vertical {
            height: 6px;
            margin: 2px 4px;
        }
        
        QSplitter::handle:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #a3a3a3, stop:0.5 #525252, stop:1 #a3a3a3);
        }
        
        /* ========== SCROLLBARS ========== */
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border-radius: 6px;
            margin: 4px 2px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #d4d4d4;
            border-radius: 5px;
            min-height: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #a3a3a3;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            height: 0px;
            background: transparent;
        }
        
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border-radius: 6px;
            margin: 2px 4px;
        }
        
        QScrollBar::handle:horizontal {
            background-color: #d4d4d4;
            border-radius: 5px;
            min-width: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: #a3a3a3;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            width: 0px;
            background: transparent;
        }
        
        /* ========== TOOLTIPS ========== */
        QToolTip {
            background-color: #1a1a1a;
            color: #fafafa;
            border: 1px solid #404040;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 14px;
            font-weight: 500;
        }
    )";
}

QString MainWindow::getDarkThemeStyleSheet() const
{
    return R"(
        /* ========== MAIN WINDOW - MONOCHROME DARK ========== */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #171717, stop:1 #0a0a0a);
            color: #fafafa;
        }
        
        /* ========== BASE WIDGET STYLING ========== */
        QWidget {
            background-color: transparent;
            color: #fafafa;
            font-family: 'Segoe UI', 'Inter', 'SF Pro Display', -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 15px;
        }
        
        /* ========== CARD-STYLE GROUP BOXES ========== */
        QGroupBox {
            font-weight: 600;
            font-size: 16px;
            color: #fafafa;
            border: 1px solid #262626;
            border-radius: 12px;
            margin-top: 14px;
            padding: 16px 12px 12px 12px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #171717, stop:1 #0a0a0a);
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            top: 2px;
            padding: 3px 12px;
            background: #fafafa;
            color: #0a0a0a;
            font-weight: 700;
            font-size: 14px;
            border-radius: 6px;
            letter-spacing: 0.3px;
        }
        
        /* ========== LABELS ========== */
        QLabel {
            color: #a3a3a3;
            font-weight: 500;
            background-color: transparent;
            letter-spacing: 0.1px;
        }
        
        /* ========== INPUT FIELDS ========== */
        QLineEdit {
            background-color: #0a0a0a;
            border: 2px solid #262626;
            border-radius: 10px;
            padding: 10px 14px;
            color: #fafafa;
            font-weight: 500;
            font-size: 15px;
            selection-background-color: #fafafa;
            selection-color: #0a0a0a;
        }
        
        QLineEdit:hover {
            border-color: #404040;
            background-color: #171717;
        }
        
        QLineEdit:focus {
            border: 2px solid #fafafa;
            background-color: #171717;
        }
        
        /* ========== SPIN BOXES ========== */
        QSpinBox, QDoubleSpinBox {
            background-color: #0a0a0a;
            border: 2px solid #262626;
            border-radius: 10px;
            padding: 10px 14px;
            color: #fafafa;
            font-weight: 500;
            selection-background-color: #fafafa;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #404040;
            background-color: #171717;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #fafafa;
            background-color: #171717;
        }
        
        QSpinBox::up-button, QSpinBox::down-button,
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            background-color: transparent;
            border: none;
            width: 24px;
            border-radius: 4px;
        }
        
        QSpinBox::up-button:hover, QSpinBox::down-button:hover,
        QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: rgba(250, 250, 250, 0.15);
        }
        
        /* ========== BUTTONS ========== */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #fafafa, stop:1 #e5e5e5);
            border: none;
            border-radius: 10px;
            color: #0a0a0a;
            font-weight: 600;
            font-size: 15px;
            padding: 12px 24px;
            min-height: 20px;
            letter-spacing: 0.3px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff, stop:1 #fafafa);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #e5e5e5, stop:1 #d4d4d4);
        }
        
        QPushButton:disabled {
            background: #262626;
            color: #525252;
        }
        
        /* ========== DATA TABLE ========== */
        QTableWidget {
            background-color: #0a0a0a;
            alternate-background-color: #171717;
            color: #fafafa;
            gridline-color: transparent;
            border: 1px solid #262626;
            border-radius: 12px;
            selection-background-color: rgba(250, 250, 250, 0.15);
            selection-color: #fafafa;
            outline: none;
        }
        
        QTableWidget::item {
            padding: 12px 16px;
            border-bottom: 1px solid #171717;
        }
        
        QTableWidget::item:selected {
            background-color: rgba(250, 250, 250, 0.15);
            color: #fafafa;
        }
        
        QTableWidget::item:hover {
            background-color: rgba(250, 250, 250, 0.08);
        }
        
        QHeaderView {
            background-color: transparent;
        }
        
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #171717, stop:1 #0a0a0a);
            color: #a3a3a3;
            font-weight: 700;
            font-size: 13px;
            padding: 14px 16px;
            border: none;
            border-bottom: 2px solid #262626;
            text-transform: uppercase;
            letter-spacing: 0.8px;
        }
        
        /* ========== SPLITTER HANDLES ========== */
        QSplitter::handle {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #262626, stop:0.5 #404040, stop:1 #262626);
            border-radius: 3px;
        }
        
        QSplitter::handle:horizontal {
            width: 6px;
            margin: 4px 2px;
        }
        
        QSplitter::handle:vertical {
            height: 6px;
            margin: 2px 4px;
        }
        
        QSplitter::handle:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #525252, stop:0.5 #a3a3a3, stop:1 #525252);
        }
        
        /* ========== SCROLLBARS ========== */
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border-radius: 6px;
            margin: 4px 2px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #404040;
            border-radius: 5px;
            min-height: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #525252;
        }
        
        QScrollBar::handle:vertical:pressed {
            background-color: #a3a3a3;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            height: 0px;
            background: transparent;
        }
        
        QScrollBar:horizontal {
            background-color: transparent;
            height: 12px;
            border-radius: 6px;
            margin: 2px 4px;
        }
        
        QScrollBar::handle:horizontal {
            background-color: #404040;
            border-radius: 5px;
            min-width: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: #525252;
        }
        
        QScrollBar::handle:horizontal:pressed {
            background-color: #a3a3a3;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            width: 0px;
            background: transparent;
        }
        
        /* ========== TOOLTIPS ========== */
        QToolTip {
            background-color: #171717;
            color: #fafafa;
            border: 1px solid #404040;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 14px;
            font-weight: 500;
        }
    )";
}

void MainWindow::applyTheme(bool isDark)
{
    if (isDark) {
        this->setStyleSheet(getDarkThemeStyleSheet());
    } else {
        this->setStyleSheet(getLightThemeStyleSheet());
    }

    // Propagate theme to PPI and FFT widgets for their custom drawing
    if (m_ppiWidget) {
        m_ppiWidget->setDarkTheme(isDark);
    }
    if (m_fftWidget) {
        m_fftWidget->setDarkTheme(isDark);
    }
    if (m_speedMeasurementWidget) {
        m_speedMeasurementWidget->setDarkTheme(isDark);
    }
    if (m_timeSeriesPlotsWidget) {
        m_timeSeriesPlotsWidget->setDarkTheme(isDark);
    }
    if (m_loggingWidget) {
        m_loggingWidget->setDarkTheme(isDark);
    }
    
    // Apply theme to DSP Settings panel
    applyDspSettingsTheme(isDark);

    // Force update of all widgets
    update();
    if (m_trackTable) m_trackTable->update();
}

void MainWindow::applyDspSettingsTheme(bool isDark)
{
    // ============================================
    // CLASSIC MONOCHROME PALETTE
    // Light Theme: Dark charcoal (#1a1a1a) accents on light background
    // Dark Theme: White/light grey (#fafafa) accents on dark background
    // ============================================
    
    if (isDark) {
        // Dark theme styles for DSP Settings panel - Monochrome design
        if (m_dspSettingsGroup) {
            m_dspSettingsGroup->setStyleSheet(R"(
                QGroupBox {
                    font-size: 17px;
                    font-weight: 700;
                    padding: 24px 16px 16px 16px;
                    margin-top: 16px;
                    background-color: #171717;
                    border: 1px solid #262626;
                    border-radius: 16px;
                }
                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top left;
                    left: 16px;
                    top: 2px;
                    padding: 6px 16px;
                    background-color: #fafafa;
                    color: #0a0a0a;
                    font-weight: 700;
                    font-size: 15px;
                    border-radius: 8px;
                    letter-spacing: 0.3px;
                }
            )");
        }
        
        if (m_dspLeftGroup) {
            m_dspLeftGroup->setStyleSheet(R"(
                QGroupBox {
                    font-size: 14px;
                    font-weight: 600;
                    padding: 16px 12px 12px 12px;
                    margin-top: 14px;
                    background-color: #0a0a0a;
                    border: 1px solid #262626;
                    border-radius: 12px;
                }
                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top left;
                    left: 12px;
                    top: 2px;
                    padding: 4px 12px;
                    background-color: #fafafa;
                    color: #0a0a0a;
                    font-weight: 600;
                    font-size: 13px;
                    border-radius: 6px;
                    letter-spacing: 0.5px;
                }
            )");
        }
        
        if (m_dspRightGroup) {
            m_dspRightGroup->setStyleSheet(R"(
                QGroupBox {
                    font-size: 14px;
                    font-weight: 600;
                    padding: 16px 12px 12px 12px;
                    margin-top: 14px;
                    background-color: #0a0a0a;
                    border: 1px solid #262626;
                    border-radius: 12px;
                }
                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top left;
                    left: 12px;
                    top: 2px;
                    padding: 4px 12px;
                    background-color: #a3a3a3;
                    color: #0a0a0a;
                    font-weight: 600;
                    font-size: 13px;
                    border-radius: 6px;
                    letter-spacing: 0.5px;
                }
            )");
        }
        
        // Apply dark theme to labels - Monochrome
        const QString darkLabelStyle = R"(
            font-size: 14px;
            font-weight: 600;
            color: #a3a3a3;
            background-color: transparent;
            letter-spacing: 0.2px;
        )";
        for (QLabel* label : m_dspLabels) {
            if (label) label->setStyleSheet(darkLabelStyle);
        }
        
        // Apply dark theme to line edits - Monochrome
        const QString darkEditStyle = R"(
            font-size: 13px;
            padding: 8px 12px;
            font-weight: 500;
            background-color: #0a0a0a;
            border: 2px solid #262626;
            border-radius: 8px;
            color: #fafafa;
        )";
        QList<QLineEdit*> lineEdits = {m_rangeAvgEdit, m_minRangeEdit, m_maxRangeEdit, 
                                       m_minSpeedEdit, m_maxSpeedEdit, m_minAngleEdit,
                                       m_maxAngleEdit, m_rangeThresholdEdit, m_speedThresholdEdit,
                                       m_numTracksEdit, m_medianFilterEdit, m_mtiLengthEdit};
        for (QLineEdit* edit : lineEdits) {
            if (edit) edit->setStyleSheet(darkEditStyle);
        }
    } else {
        // Light theme styles for DSP Settings panel - Monochrome design
        if (m_dspSettingsGroup) {
            m_dspSettingsGroup->setStyleSheet(R"(
                QGroupBox {
                    font-size: 17px;
                    font-weight: 700;
                    padding: 24px 16px 16px 16px;
                    margin-top: 16px;
                    background-color: #ffffff;
                    border: 1px solid #e5e5e5;
                    border-radius: 16px;
                }
                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top left;
                    left: 16px;
                    top: 2px;
                    padding: 6px 16px;
                    background-color: #1a1a1a;
                    color: #ffffff;
                    font-weight: 700;
                    font-size: 15px;
                    border-radius: 8px;
                    letter-spacing: 0.3px;
                }
            )");
        }
        
        if (m_dspLeftGroup) {
            m_dspLeftGroup->setStyleSheet(R"(
                QGroupBox {
                    font-size: 14px;
                    font-weight: 600;
                    padding: 16px 12px 12px 12px;
                    margin-top: 14px;
                    background-color: #fafafa;
                    border: 1px solid #e5e5e5;
                    border-radius: 12px;
                }
                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top left;
                    left: 12px;
                    top: 2px;
                    padding: 4px 12px;
                    background-color: #1a1a1a;
                    color: #ffffff;
                    font-weight: 600;
                    font-size: 13px;
                    border-radius: 6px;
                    letter-spacing: 0.5px;
                }
            )");
        }
        
        if (m_dspRightGroup) {
            m_dspRightGroup->setStyleSheet(R"(
                QGroupBox {
                    font-size: 14px;
                    font-weight: 600;
                    padding: 16px 12px 12px 12px;
                    margin-top: 14px;
                    background-color: #fafafa;
                    border: 1px solid #e5e5e5;
                    border-radius: 12px;
                }
                QGroupBox::title {
                    subcontrol-origin: margin;
                    subcontrol-position: top left;
                    left: 12px;
                    top: 2px;
                    padding: 4px 12px;
                    background-color: #525252;
                    color: #ffffff;
                    font-weight: 600;
                    font-size: 13px;
                    border-radius: 6px;
                    letter-spacing: 0.5px;
                }
            )");
        }
        
        // Apply light theme to labels - Monochrome
        const QString lightLabelStyle = R"(
            font-size: 14px;
            font-weight: 600;
            color: #525252;
            background-color: transparent;
            letter-spacing: 0.2px;
        )";
        for (QLabel* label : m_dspLabels) {
            if (label) label->setStyleSheet(lightLabelStyle);
        }
        
        // Apply light theme to line edits - Monochrome
        const QString lightEditStyle = R"(
            font-size: 13px;
            padding: 8px 12px;
            font-weight: 500;
            background-color: #ffffff;
            border: 2px solid #e5e5e5;
            border-radius: 8px;
            color: #0a0a0a;
        )";
        QList<QLineEdit*> lineEdits = {m_rangeAvgEdit, m_minRangeEdit, m_maxRangeEdit, 
                                       m_minSpeedEdit, m_maxSpeedEdit, m_minAngleEdit,
                                       m_maxAngleEdit, m_rangeThresholdEdit, m_speedThresholdEdit,
                                       m_numTracksEdit, m_medianFilterEdit, m_mtiLengthEdit};
        for (QLineEdit* edit : lineEdits) {
            if (edit) edit->setStyleSheet(lightEditStyle);
        }
    }
}

//==============================================================================
// SETTINGS PERSISTENCE
//==============================================================================
QString MainWindow::getSettingsFilePath() const
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return configPath + "/RadarVisualization.ini";
}

void MainWindow::onSaveSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);
    
    settings.beginGroup("DSPSettings");
    settings.setValue("rangeAvg", m_rangeAvgEdit->text());
    settings.setValue("minRange", m_minRangeEdit->text());
    settings.setValue("maxRange", m_maxRangeEdit->text());
    settings.setValue("minSpeed", m_minSpeedEdit->text());
    settings.setValue("maxSpeed", m_maxSpeedEdit->text());
    settings.setValue("minAngle", m_minAngleEdit->text());
    settings.setValue("maxAngle", m_maxAngleEdit->text());
    settings.setValue("rangeThreshold", m_rangeThresholdEdit->text());
    settings.setValue("speedThreshold", m_speedThresholdEdit->text());
    settings.setValue("numTracks", m_numTracksEdit->text());
    settings.setValue("medianFilter", m_medianFilterEdit->text());
    settings.setValue("mtiLength", m_mtiLengthEdit->text());
    settings.endGroup();
    
    settings.sync();
    
    QMessageBox::information(this, "Settings Saved", 
        QString("Settings have been saved to:\n%1").arg(getSettingsFilePath()));
    m_statusLabel->setText("Status: Settings saved");
}

void MainWindow::loadSettings()
{
    QString settingsPath = getSettingsFilePath();
    QSettings settings(settingsPath, QSettings::IniFormat);
    
    if (!QFile::exists(settingsPath)) {
        qDebug() << "No saved settings found, using defaults";
        // Apply default settings to displays
        m_ppiWidget->setMinRange(0.0f);
        m_ppiWidget->setMaxRange(50.0f);
        m_ppiWidget->setMinAngle(-60.0f);
        m_ppiWidget->setMaxAngle(60.0f);
        
        m_fftWidget->setMinRange(0.0f);
        m_fftWidget->setMaxRange(50.0f);
        m_fftWidget->setMinAngle(-60.0f);
        m_fftWidget->setMaxAngle(60.0f);
        return;
    }
    
    settings.beginGroup("DSPSettings");
    
    if (settings.contains("rangeAvg"))
        m_rangeAvgEdit->setText(settings.value("rangeAvg", "1").toString());
    if (settings.contains("minRange"))
        m_minRangeEdit->setText(settings.value("minRange", "0").toString());
    if (settings.contains("maxRange"))
        m_maxRangeEdit->setText(settings.value("maxRange", "5000").toString());
    if (settings.contains("minSpeed"))
        m_minSpeedEdit->setText(settings.value("minSpeed", "-100").toString());
    if (settings.contains("maxSpeed"))
        m_maxSpeedEdit->setText(settings.value("maxSpeed", "100").toString());
    if (settings.contains("minAngle"))
        m_minAngleEdit->setText(settings.value("minAngle", "0").toString());
    if (settings.contains("maxAngle"))
        m_maxAngleEdit->setText(settings.value("maxAngle", "0").toString());
    if (settings.contains("rangeThreshold"))
        m_rangeThresholdEdit->setText(settings.value("rangeThreshold", "0").toString());
    if (settings.contains("speedThreshold"))
        m_speedThresholdEdit->setText(settings.value("speedThreshold", "0").toString());
    if (settings.contains("numTracks"))
        m_numTracksEdit->setText(settings.value("numTracks", "50").toString());
    if (settings.contains("medianFilter"))
        m_medianFilterEdit->setText(settings.value("medianFilter", "1").toString());
    if (settings.contains("mtiLength"))
        m_mtiLengthEdit->setText(settings.value("mtiLength", "2").toString());
    
    settings.endGroup();
    
    // Update internal DSP settings from loaded values
    onRangeAvgEdited();
    onMinRangeEdited();
    onMaxRangeEdited();
    onMinSpeedEdited();
    onMaxSpeedEdited();
    onMinAngleEdited();
    onMaxAngleEdited();
    onRangeThresholdEdited();
    onSpeedThresholdEdited();
    onNumTracksEdited();
    onMedianFilterEdited();
    onMtiLengthEdited();
    
    // Apply loaded settings to both displays (range in meters, angles in degrees)
    float minRangeMeters = m_dsp.min_range_cm / 100.0f;
    float maxRangeMeters = m_dsp.max_range_cm / 100.0f;
    
    m_ppiWidget->setMinRange(minRangeMeters);
    m_ppiWidget->setMaxRange(maxRangeMeters);
    m_ppiWidget->setMinAngle(static_cast<float>(m_dsp.min_angle_degree));
    m_ppiWidget->setMaxAngle(static_cast<float>(m_dsp.max_angle_degree));
    
    m_fftWidget->setMinRange(minRangeMeters);
    m_fftWidget->setMaxRange(maxRangeMeters);
    m_fftWidget->setMinAngle(static_cast<float>(m_dsp.min_angle_degree));
    m_fftWidget->setMaxAngle(static_cast<float>(m_dsp.max_angle_degree));
    
    qDebug() << "Settings loaded from:" << settingsPath;
    qDebug() << "Applied to displays - Range:" << minRangeMeters << "-" << maxRangeMeters << "m";
    qDebug() << "Angle:" << m_dsp.min_angle_degree << "-" << m_dsp.max_angle_degree << "deg";

    qDebug() << "Settings loaded from:" << settingsPath;
    qDebug() << "Applied to displays - Range:" << minRangeMeters << "-" << maxRangeMeters << "m";
    qDebug() << "Angle:" << m_dsp.min_angle_degree << "-" << m_dsp.max_angle_degree << "deg";

    // Load theme preference
    QString theme = settings.value("UI/theme", "light").toString();
    m_isDarkTheme = (theme == "dark");
    applyTheme(m_isDarkTheme);

    qDebug() << "Theme:" << theme;
}

void MainWindow::onLoadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Load DSP Settings",
        QString(),
        "JSON Files (*.json);;All Files (*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Load Error", 
            QString("Could not open file for reading:\n%1").arg(fileName));
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(this, "Parse Error", 
            QString("Failed to parse JSON file:\n%1").arg(parseError.errorString()));
        return;
    }
    
    if (!doc.isObject()) {
        QMessageBox::warning(this, "Format Error", 
            "Invalid JSON format: Expected an object.");
        return;
    }
    
    QJsonObject obj = doc.object();
    
    // Load values from JSON and update UI fields
    if (obj.contains("range_avg"))
        m_rangeAvgEdit->setText(QString::number(obj["range_avg"].toInt(1)));
    if (obj.contains("min_range_cm"))
        m_minRangeEdit->setText(QString::number(obj["min_range_cm"].toInt(0)));
    if (obj.contains("max_range_cm"))
        m_maxRangeEdit->setText(QString::number(obj["max_range_cm"].toInt(5000)));
    if (obj.contains("min_speed_kmh"))
        m_minSpeedEdit->setText(QString::number(obj["min_speed_kmh"].toInt(0)));
    if (obj.contains("max_speed_kmh"))
        m_maxSpeedEdit->setText(QString::number(obj["max_speed_kmh"].toInt(100)));
    if (obj.contains("min_angle_degree"))
        m_minAngleEdit->setText(QString::number(obj["min_angle_degree"].toInt(0)));
    if (obj.contains("max_angle_degree"))
        m_maxAngleEdit->setText(QString::number(obj["max_angle_degree"].toInt(0)));
    if (obj.contains("range_threshold"))
        m_rangeThresholdEdit->setText(QString::number(obj["range_threshold"].toInt(0)));
    if (obj.contains("speed_threshold"))
        m_speedThresholdEdit->setText(QString::number(obj["speed_threshold"].toInt(0)));
    if (obj.contains("num_tracks"))
        m_numTracksEdit->setText(QString::number(obj["num_tracks"].toInt(50)));
    if (obj.contains("median_filter"))
        m_medianFilterEdit->setText(QString::number(obj["median_filter"].toInt(1)));
    if (obj.contains("mti_length"))
        m_mtiLengthEdit->setText(QString::number(obj["mti_length"].toInt(2)));
    
    // Update internal DSP settings from loaded values
    onRangeAvgEdited();
    onMinRangeEdited();
    onMaxRangeEdited();
    onMinSpeedEdited();
    onMaxSpeedEdited();
    onMinAngleEdited();
    onMaxAngleEdited();
    onRangeThresholdEdited();
    onSpeedThresholdEdited();
    onNumTracksEdited();
    onMedianFilterEdited();
    onMtiLengthEdited();
    
    // Apply loaded settings to both displays (range in meters, angles in degrees)
    float minRangeMeters = m_dsp.min_range_cm / 100.0f;
    float maxRangeMeters = m_dsp.max_range_cm / 100.0f;
    
    m_ppiWidget->setMinRange(minRangeMeters);
    m_ppiWidget->setMaxRange(maxRangeMeters);
    m_ppiWidget->setMinAngle(static_cast<float>(m_dsp.min_angle_degree));
    m_ppiWidget->setMaxAngle(static_cast<float>(m_dsp.max_angle_degree));
    
    m_fftWidget->setMinRange(minRangeMeters);
    m_fftWidget->setMaxRange(maxRangeMeters);
    m_fftWidget->setMinAngle(static_cast<float>(m_dsp.min_angle_degree));
    m_fftWidget->setMaxAngle(static_cast<float>(m_dsp.max_angle_degree));
    
    m_statusLabel->setText("Status: Settings loaded from file");
    QMessageBox::information(this, "Settings Loaded", 
        QString("DSP settings have been loaded from:\n%1").arg(fileName));
}

void MainWindow::onSaveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save DSP Settings",
        "dsp_settings.json",
        "JSON Files (*.json);;All Files (*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Ensure .json extension
    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        fileName += ".json";
    }
    
    // Build JSON object from current settings
    QJsonObject obj;
    obj["range_avg"] = m_rangeAvgEdit->text().toInt();
    obj["min_range_cm"] = m_minRangeEdit->text().toInt();
    obj["max_range_cm"] = m_maxRangeEdit->text().toInt();
    obj["min_speed_kmh"] = m_minSpeedEdit->text().toInt();
    obj["max_speed_kmh"] = m_maxSpeedEdit->text().toInt();
    obj["min_angle_degree"] = m_minAngleEdit->text().toInt();
    obj["max_angle_degree"] = m_maxAngleEdit->text().toInt();
    obj["range_threshold"] = m_rangeThresholdEdit->text().toInt();
    obj["speed_threshold"] = m_speedThresholdEdit->text().toInt();
    obj["num_tracks"] = m_numTracksEdit->text().toInt();
    obj["median_filter"] = m_medianFilterEdit->text().toInt();
    obj["mti_length"] = m_mtiLengthEdit->text().toInt();
    
    QJsonDocument doc(obj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Save Error", 
            QString("Could not open file for writing:\n%1").arg(fileName));
        return;
    }
    
    file.write(jsonData);
    file.close();
    
    m_statusLabel->setText("Status: Settings saved to file");
    QMessageBox::information(this, "Settings Saved", 
        QString("DSP settings have been saved to:\n%1").arg(fileName));
}

void MainWindow::onOpenLoggingWindow()
{
    // If window already exists, just show and raise it
    if (m_loggingWindow && m_loggingWidget) {
        m_loggingWindow->show();
        m_loggingWindow->raise();
        m_loggingWindow->activateWindow();
        return;
    }
    
    // Create new window
    m_loggingWindow = new QWidget(nullptr, Qt::Window);
    m_loggingWindow->setWindowTitle("Logging");
    m_loggingWindow->setAttribute(Qt::WA_DeleteOnClose);
    m_loggingWindow->resize(1200, 800);
    
    // Create layout for the window
    QVBoxLayout* windowLayout = new QVBoxLayout(m_loggingWindow);
    windowLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create logging widget
    m_loggingWidget = new LoggingWidget(m_loggingWindow);
    windowLayout->addWidget(m_loggingWidget);
    
    // Apply current theme to logging widget
    m_loggingWidget->setDarkTheme(m_isDarkTheme);
    
    // Connect to clean up when window is closed
    connect(m_loggingWindow, &QWidget::destroyed, this, [this]() {
        m_loggingWindow = nullptr;
        m_loggingWidget = nullptr;
    });
    
    // Show the window
    m_loggingWindow->show();
}

//==============================================================================
// TRACK DATA LOGGING
//==============================================================================
QString MainWindow::createTimestampedFilename()
{
    // Get current system time and format it
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timestamp = currentTime.toString("yyyyMMdd_HHmmss_zzz");
    QString filename = QString("D:/track_data_%1.csv").arg(timestamp);
    return filename;
}

void MainWindow::logTrackDataToFile(const TargetTrack& track)
{
    // Create new file if not exists or if file pointer is null
    if (!m_trackDataFile) {
        // Ensure D:/ directory exists and is accessible
        QDir dDrive("D:/");
        if (!dDrive.exists()) {
            qDebug() << "Warning: D:/ drive not accessible. Attempting to create directory...";
            if (!dDrive.mkpath("D:/")) {
                qDebug() << "Failed to access or create D:/ directory. Track data logging disabled.";
                return;
            }
        }
        
        m_currentLogFilename = createTimestampedFilename();
        m_trackDataFile = new QFile(m_currentLogFilename);
        
        if (!m_trackDataFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            qDebug() << "Failed to open track data file:" << m_currentLogFilename 
                     << "Error:" << m_trackDataFile->errorString();
            delete m_trackDataFile;
            m_trackDataFile = nullptr;
            return;
        }
        
        // Write CSV header
        QTextStream out(m_trackDataFile);
        out << "Timestamp,Target_ID,Range_m,Radial_Speed_m_s,Azimuth_deg,Elevation_deg,Level_dB,Azimuth_Speed_deg_s,Elevation_Speed_deg_s,System_Time\n";
        out.flush();
        
        qDebug() << "Created track data log file in D:/ drive:" << m_currentLogFilename;
    }
    
    // Write track data to file
    QTextStream out(m_trackDataFile);
    QDateTime systemTime = QDateTime::fromMSecsSinceEpoch(track.lastUpdateTime);
    out << track.lastUpdateTime << ","
        << track.target_id << ","
        << track.radius << ","
        << track.radial_speed << ","
        << track.azimuth << ","
        << track.elevation << ","
        << track.level << ","
        << track.azimuth_speed << ","
        << track.elevation_speed << ","
        << systemTime.toString("yyyy-MM-dd HH:mm:ss.zzz") << "\n";
    out.flush();
    
    // Log to console for debugging
    qDebug() << "Track data logged - ID:" << track.target_id 
             << "Range:" << track.radius << "m"
             << "Speed:" << track.radial_speed << "m/s"
             << "Azimuth:" << track.azimuth << "deg"
             << "Elevation:" << track.elevation << "deg"
             << "Level:" << track.level << "dB"
             << "Timestamp:" << track.lastUpdateTime;
}
