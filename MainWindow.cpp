#include "MainWindow.h"
#include <QApplication>
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
#include <QStatusBar>
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
#include <cmath>
#include <cstring>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ppiWidget(nullptr)
    , m_fftWidget(nullptr)
    , m_trackTable(nullptr)
    , m_udpSocket(nullptr)
    , m_updateTimer(nullptr)
    , m_trackRefreshTimer(nullptr)
    , m_saveSettingsButton(nullptr)
    , m_defaultSettingsButton(nullptr)
    , m_clearTracksButton(nullptr)
    , m_simulationEnabled(false)  // Simulation disabled by default
    , m_randomEngine(std::random_device{}())
    , m_rangeDist(100.0f, 500.0f)
    , m_azimuthDist(-90.0f, 90.0f)
    , m_speedDist(-50.0f, 50.0f)
    , m_levelDist(10.0f, 100.0f)
    , m_numTargetsDist(3, 8)
    , m_frameCount(0)
    , m_targetCount(0)
    , m_dsp{}  // Zero-initialize the DSP settings struct
    , m_isDarkTheme(false)
{
    // Initialize DSP settings with default values matching UI defaults
    m_dsp.range_mvg_avg_length = 1;
    m_dsp.min_range_cm = 0;
    m_dsp.max_range_cm = 5000;
    m_dsp.min_speed_kmh = 0;   // Signed type now supports negative values
    m_dsp.max_speed_kmh = 100;
    m_dsp.min_angle_degree = 0; // Signed type now supports negative values
    m_dsp.max_angle_degree = 60;
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
}

void MainWindow::setupUI()
{
    setWindowTitle("ZopplerSystems RadarGUI");
    setMinimumSize(1200, 800);
    
    // Maximize window to screen size on startup
    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        QRect screenGeometry = screen->availableGeometry();
        setGeometry(screenGeometry);
    }
    showMaximized();

    // Apply premium engineering dashboard theme
    QString lightTheme = R"(
        /* ========== MAIN WINDOW ========== */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #f8fafc, stop:1 #f1f5f9);
            color: #1e293b;
        }
        
        /* ========== BASE WIDGET STYLING ========== */
        QWidget {
            background-color: transparent;
            color: #1e293b;
            font-family: 'Segoe UI', 'Inter', 'SF Pro Display', -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 13px;
        }
        
        /* ========== CARD-STYLE GROUP BOXES ========== */
        QGroupBox {
            font-weight: 600;
            font-size: 14px;
            color: #0f172a;
            border: none;
            border-radius: 16px;
            margin-top: 20px;
            padding: 24px 20px 20px 20px;
            background-color: #ffffff;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 20px;
            top: 4px;
            padding: 4px 16px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3b82f6, stop:1 #6366f1);
            color: #ffffff;
            font-weight: 700;
            font-size: 13px;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        
        /* ========== LABELS ========== */
        QLabel {
            color: #475569;
            font-weight: 500;
            background-color: transparent;
            letter-spacing: 0.1px;
        }
        
        /* ========== INPUT FIELDS ========== */
        QLineEdit {
            background-color: #f8fafc;
            border: 2px solid #e2e8f0;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0f172a;
            font-weight: 500;
            font-size: 13px;
            selection-background-color: #3b82f6;
            selection-color: #ffffff;
        }
        
        QLineEdit:hover {
            border-color: #94a3b8;
            background-color: #ffffff;
        }
        
        QLineEdit:focus {
            border: 2px solid #3b82f6;
            background-color: #ffffff;
            outline: none;
        }
        
        QLineEdit:disabled {
            background-color: #f1f5f9;
            color: #94a3b8;
            border-color: #e2e8f0;
        }
        
        /* ========== SPIN BOXES ========== */
        QSpinBox, QDoubleSpinBox {
            background-color: #f8fafc;
            border: 2px solid #e2e8f0;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0f172a;
            font-weight: 500;
            selection-background-color: #3b82f6;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #94a3b8;
            background-color: #ffffff;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #3b82f6;
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
            background-color: rgba(59, 130, 246, 0.15);
        }
        
        /* ========== BUTTONS - PRIMARY ========== */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3b82f6, stop:1 #2563eb);
            border: none;
            border-radius: 10px;
            color: #ffffff;
            font-weight: 600;
            font-size: 13px;
            padding: 12px 24px;
            min-height: 20px;
            letter-spacing: 0.3px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2563eb, stop:1 #1d4ed8);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1d4ed8, stop:1 #1e40af);
        }
        
        QPushButton:checked {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e40af, stop:1 #1e3a8a);
            color: #ffffff;
        }
        
        QPushButton:disabled {
            background: #e2e8f0;
            color: #94a3b8;
        }
        
        /* ========== DATA TABLE ========== */
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #f8fafc;
            color: #0f172a;
            gridline-color: transparent;
            border: none;
            border-radius: 12px;
            selection-background-color: rgba(59, 130, 246, 0.12);
            selection-color: #0f172a;
            outline: none;
        }
        
        QTableWidget::item {
            padding: 12px 16px;
            border-bottom: 1px solid #f1f5f9;
        }
        
        QTableWidget::item:selected {
            background-color: rgba(59, 130, 246, 0.12);
            color: #0f172a;
        }
        
        QTableWidget::item:hover {
            background-color: rgba(59, 130, 246, 0.06);
        }
        
        QHeaderView {
            background-color: transparent;
        }
        
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #f8fafc, stop:1 #f1f5f9);
            color: #64748b;
            font-weight: 700;
            font-size: 11px;
            padding: 14px 16px;
            border: none;
            border-bottom: 2px solid #e2e8f0;
            text-transform: uppercase;
            letter-spacing: 0.8px;
        }
        
        QHeaderView::section:first {
            border-top-left-radius: 12px;
        }
        
        QHeaderView::section:last {
            border-top-right-radius: 12px;
        }
        
        /* ========== STATUS BAR ========== */
        QStatusBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff, stop:1 #f8fafc);
            color: #64748b;
            border-top: 1px solid #e2e8f0;
            padding: 10px 20px;
            font-size: 12px;
        }
        
        QStatusBar::item {
            border: none;
        }
        
        /* ========== SPLITTER HANDLES ========== */
        QSplitter::handle {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #e2e8f0, stop:0.5 #cbd5e1, stop:1 #e2e8f0);
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
                stop:0 #93c5fd, stop:0.5 #3b82f6, stop:1 #93c5fd);
        }
        
        /* ========== SCROLLBARS ========== */
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border-radius: 6px;
            margin: 4px 2px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #cbd5e1;
            border-radius: 5px;
            min-height: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #94a3b8;
        }
        
        QScrollBar::handle:vertical:pressed {
            background-color: #64748b;
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
            background-color: #cbd5e1;
            border-radius: 5px;
            min-width: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: #94a3b8;
        }
        
        QScrollBar::handle:horizontal:pressed {
            background-color: #64748b;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            width: 0px;
            background: transparent;
        }
        
        /* ========== TOOLTIPS ========== */
        QToolTip {
            background-color: #0f172a;
            color: #f8fafc;
            border: 1px solid #334155;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 12px;
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

    // Create a 2x2 grid layout for equal-sized quadrants
    QGridLayout* gridLayout = new QGridLayout(centralWidget);
    gridLayout->setContentsMargins(8, 8, 8, 8);
    gridLayout->setSpacing(8);
    
    // Set equal row and column stretch factors for equal sizing
    gridLayout->setRowStretch(0, 1);
    gridLayout->setRowStretch(1, 1);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);

    // ========== TOP-LEFT: PPI Display ==========
    QGroupBox* ppiGroup = new QGroupBox("PPI Display (Target Tracks)");
    QVBoxLayout* ppiLayout = new QVBoxLayout(ppiGroup);
    ppiLayout->setSpacing(8);
    ppiLayout->setContentsMargins(8, 16, 8, 8);

    m_ppiWidget = new PPIWidget();
    m_ppiWidget->setMinimumSize(300, 300);
    ppiLayout->addWidget(m_ppiWidget, 1);  // Stretch factor 1 to take available space

    gridLayout->addWidget(ppiGroup, 0, 0);

    // ========== TOP-RIGHT: FFT Display ==========
    QGroupBox* fftGroup = new QGroupBox("FFT Spectrum (Raw ADC Data)");
    QVBoxLayout* fftLayout = new QVBoxLayout(fftGroup);
    fftLayout->setContentsMargins(8, 16, 8, 8);
    m_fftWidget = new FFTWidget();
    m_fftWidget->setRadarParameters(100000.0f, 0.001f, 50000000.0f, 24000000000.0f);
    m_fftWidget->setMaxRange(50.0f);
    m_fftWidget->setMinimumSize(300, 300);
    fftLayout->addWidget(m_fftWidget);
    
    gridLayout->addWidget(fftGroup, 0, 1);

    // ========== BOTTOM-LEFT: DSP Settings Panel ==========
    QGroupBox* settingsGroup = new QGroupBox("DSP Settings", this);
    settingsGroup->setStyleSheet(R"(
        QGroupBox {
            font-size: 15px;
            font-weight: 700;
            padding: 24px 16px 16px 16px;
            margin-top: 16px;
            background-color: #ffffff;
            border: none;
            border-radius: 16px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 16px;
            top: 2px;
            padding: 6px 16px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3b82f6, stop:1 #6366f1);
            color: #ffffff;
            font-weight: 700;
            font-size: 13px;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
    )");
    
    QVBoxLayout* settingsMainLayout = new QVBoxLayout(settingsGroup);
    settingsMainLayout->setSpacing(12);
    settingsMainLayout->setContentsMargins(12, 20, 12, 12);
    
    // Container for the two columns
    QWidget* columnsContainer = new QWidget(this);
    QHBoxLayout* columnsLayout = new QHBoxLayout(columnsContainer);
    columnsLayout->setSpacing(24);
    columnsLayout->setContentsMargins(0, 0, 0, 0);
    
    // Left column - Range & Speed settings with modern card style
    QGroupBox* leftGroup = new QGroupBox("Range && Speed", this);
    leftGroup->setStyleSheet(R"(
        QGroupBox {
            font-size: 12px;
            font-weight: 600;
            padding: 16px 12px 12px 12px;
            margin-top: 14px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f8fafc, stop:1 #f1f5f9);
            border: 1px solid #e2e8f0;
            border-radius: 12px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            top: 2px;
            padding: 4px 12px;
            background-color: #0ea5e9;
            color: #ffffff;
            font-weight: 600;
            font-size: 11px;
            border-radius: 6px;
            letter-spacing: 0.5px;
        }
    )");
    QGridLayout* leftLayout = new QGridLayout(leftGroup);
    leftLayout->setSpacing(10);
    leftLayout->setContentsMargins(12, 20, 12, 12);
    
    // Right column - Filter & Tracking settings with modern card style
    QGroupBox* rightGroup = new QGroupBox("Filter && Tracking", this);
    rightGroup->setStyleSheet(R"(
        QGroupBox {
            font-size: 12px;
            font-weight: 600;
            padding: 16px 12px 12px 12px;
            margin-top: 14px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f8fafc, stop:1 #f1f5f9);
            border: 1px solid #e2e8f0;
            border-radius: 12px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 12px;
            top: 2px;
            padding: 4px 12px;
            background-color: #8b5cf6;
            color: #ffffff;
            font-weight: 600;
            font-size: 11px;
            border-radius: 6px;
            letter-spacing: 0.5px;
        }
    )");
    QGridLayout* rightLayout = new QGridLayout(rightGroup);
    rightLayout->setSpacing(10);
    rightLayout->setContentsMargins(12, 20, 12, 12);

    // Enhanced styling for labels and input fields
    const QString labelStyle = R"(
        font-size: 12px;
        font-weight: 600;
        color: #475569;
        background-color: transparent;
        letter-spacing: 0.2px;
    )";
    const QString editStyle = R"(
        font-size: 13px;
        padding: 8px 12px;
        font-weight: 500;
        background-color: #ffffff;
        border: 2px solid #e2e8f0;
        border-radius: 8px;
        color: #0f172a;
    )";
    
    // Increased width from 80 to 100 to prevent text clamping
    auto addField = [&](QGridLayout* layout, int row, const QString& label, QLineEdit*& edit, const QString& def) {
        QLabel* l = new QLabel(label, this);
        l->setStyleSheet(labelStyle);
        edit = new QLineEdit(def, this);
        edit->setMinimumWidth(100);
        edit->setMaximumWidth(120);
        edit->setMinimumHeight(32);
        edit->setStyleSheet(editStyle);
        layout->addWidget(l, row, 0);
        layout->addWidget(edit, row, 1);
    };

    // Left column fields - Range & Speed
    addField(leftLayout, 0, "Range Avg",        m_rangeAvgEdit,        "1");
    addField(leftLayout, 1, "Min Range (cm)",   m_minRangeEdit,        "0");
    addField(leftLayout, 2, "Max Range (cm)",   m_maxRangeEdit,        "5000");
    addField(leftLayout, 3, "Min Speed (km/h)", m_minSpeedEdit,        "-100");
    addField(leftLayout, 4, "Max Speed (km/h)", m_maxSpeedEdit,        "100");
    addField(leftLayout, 5, "Range Thresh",     m_rangeThresholdEdit,  "0");
    
    // Right column fields - Filter & Tracking
    addField(rightLayout, 0, "Min Angle (deg)",  m_minAngleEdit,        "-60");
    addField(rightLayout, 1, "Max Angle (deg)",  m_maxAngleEdit,        "60");
    addField(rightLayout, 2, "Speed Thresh",     m_speedThresholdEdit,  "0");
    addField(rightLayout, 3, "Num Tracks",       m_numTracksEdit,       "50");
    addField(rightLayout, 4, "Median Filter",    m_medianFilterEdit,    "1");
    addField(rightLayout, 5, "MTI Length",       m_mtiLengthEdit,       "2");

    // Set column widths
    leftLayout->setColumnMinimumWidth(0, 120);
    leftLayout->setColumnMinimumWidth(1, 100);
    rightLayout->setColumnMinimumWidth(0, 120);
    rightLayout->setColumnMinimumWidth(1, 100);
    
    columnsLayout->addWidget(leftGroup, 1);
    columnsLayout->addWidget(rightGroup, 1);
    
    settingsMainLayout->addWidget(columnsContainer, 1);
    
    // Create buttons layout - Premium button bar
    QWidget* buttonContainer = new QWidget(this);
    buttonContainer->setStyleSheet("background-color: transparent;");
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->setContentsMargins(0, 8, 0, 0);
    buttonLayout->setSpacing(12);
    
    // Apply button - Premium blue gradient with icon hint
    m_applyButton = new QPushButton("  Apply Settings", this);
    m_applyButton->setMinimumHeight(38);
    m_applyButton->setMinimumWidth(120);
    m_applyButton->setCursor(Qt::PointingHandCursor);
    m_applyButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            padding: 8px 16px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3b82f6, stop:1 #2563eb);
            color: white;
            border: none;
            border-radius: 10px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2563eb, stop:1 #1d4ed8);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1d4ed8, stop:1 #1e40af);
        }
    )");
    
    // Reset button - Premium red gradient
    m_resetButton = new QPushButton("  Reset", this);
    m_resetButton->setMinimumHeight(38);
    m_resetButton->setMinimumWidth(90);
    m_resetButton->setCursor(Qt::PointingHandCursor);
    m_resetButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            padding: 8px 16px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ef4444, stop:1 #dc2626);
            color: white;
            border: none;
            border-radius: 10px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #dc2626, stop:1 #b91c1c);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #b91c1c, stop:1 #991b1b);
        }
    )");
    
    // Save Settings button - Premium green gradient
    m_saveSettingsButton = new QPushButton("  Save Settings", this);
    m_saveSettingsButton->setMinimumHeight(38);
    m_saveSettingsButton->setMinimumWidth(120);
    m_saveSettingsButton->setCursor(Qt::PointingHandCursor);
    m_saveSettingsButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            padding: 8px 16px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #10b981, stop:1 #059669);
            color: white;
            border: none;
            border-radius: 10px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #059669, stop:1 #047857);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #047857, stop:1 #065f46);
        }
    )");
    
    // Default Settings button - Premium outlined amber style
    m_defaultSettingsButton = new QPushButton("  Restore Defaults", this);
    m_defaultSettingsButton->setMinimumHeight(38);
    m_defaultSettingsButton->setMinimumWidth(130);
    m_defaultSettingsButton->setCursor(Qt::PointingHandCursor);
    m_defaultSettingsButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            padding: 8px 16px;
            background-color: transparent;
            color: #d97706;
            border: 2px solid #f59e0b;
            border-radius: 10px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background-color: rgba(245, 158, 11, 0.1);
            border-color: #d97706;
            color: #b45309;
        }
        QPushButton:pressed {
            background-color: rgba(245, 158, 11, 0.2);
            border-color: #b45309;
        }
    )");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_applyButton);
    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addWidget(m_saveSettingsButton);
    buttonLayout->addWidget(m_defaultSettingsButton);
    buttonLayout->addStretch();

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
    connect(m_resetButton,         &QPushButton::clicked,       this, &MainWindow::onResetSettings);
    connect(m_saveSettingsButton,  &QPushButton::clicked,       this, &MainWindow::onSaveSettings);
    connect(m_defaultSettingsButton, &QPushButton::clicked,     this, &MainWindow::onDefaultSettings);

    gridLayout->addWidget(settingsGroup, 1, 0);

    // ========== BOTTOM-RIGHT: Track Table ==========
    QGroupBox* tableGroup = new QGroupBox("Target Track Table");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    tableLayout->setContentsMargins(8, 16, 8, 8);
    m_trackTable = new QTableWidget();
    m_trackTable->setColumnCount(4);
    QStringList headers;
    headers << "ID" << "Range (m)" << "Azimuth (°)" << "Radial Speed (m/s)";
    m_trackTable->setHorizontalHeaderLabels(headers);
    m_trackTable->horizontalHeader()->setStretchLastSection(true);
    m_trackTable->setAlternatingRowColors(true);
    m_trackTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLayout->addWidget(m_trackTable, 1);
    
    // Clear Tracks button with premium styling
    m_clearTracksButton = new QPushButton("  Clear All Tracks", this);
    m_clearTracksButton->setMinimumHeight(38);
    m_clearTracksButton->setCursor(Qt::PointingHandCursor);
    m_clearTracksButton->setStyleSheet(R"(
        QPushButton {
            font-size: 12px;
            font-weight: 600;
            padding: 8px 16px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #f87171, stop:1 #ef4444);
            color: white;
            border: none;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ef4444, stop:1 #dc2626);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #dc2626, stop:1 #b91c1c);
        }
    )");
    connect(m_clearTracksButton, &QPushButton::clicked, this, &MainWindow::onClearTracks);
    tableLayout->addWidget(m_clearTracksButton, 0);
    
    gridLayout->addWidget(tableGroup, 1, 1);

    // Initialize unused splitters to nullptr (no longer needed but kept for compatibility)
    m_mainSplitter = nullptr;
    m_rightSplitter = nullptr;
    
    // Frame count and status labels - will be updated and shown in status bar
    m_frameCountLabel = new QLabel("Frames: 0");
    m_statusLabel = new QLabel("Status: Ready");

    // Create a premium status bar with visual indicators and badges
    QWidget* statusWidget = new QWidget(this);
    statusWidget->setStyleSheet("background-color: transparent;");
    QHBoxLayout* statusLayout = new QHBoxLayout(statusWidget);
    statusLayout->setContentsMargins(8, 0, 8, 0);
    statusLayout->setSpacing(16);
    
    // Application branding badge
    QLabel* brandLabel = new QLabel("RADAR GUI", this);
    brandLabel->setStyleSheet(R"(
        QLabel {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3b82f6, stop:1 #6366f1);
            color: #ffffff;
            font-size: 10px;
            font-weight: 700;
            padding: 4px 10px;
            border-radius: 4px;
            letter-spacing: 1px;
        }
    )");
    statusLayout->addWidget(brandLabel);
    
    // Separator
    QFrame* sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::VLine);
    sep1->setStyleSheet("background-color: #e2e8f0; max-width: 1px;");
    statusLayout->addWidget(sep1);
    
    // Connection status with indicator dot
    QLabel* connectionIndicator = new QLabel("●", this);
    connectionIndicator->setStyleSheet("color: #10b981; font-size: 10px; background-color: transparent;");
    statusLayout->addWidget(connectionIndicator);
    
    QLabel* readyLabel = new QLabel("UDP:5000 Ready", this);
    readyLabel->setStyleSheet(R"(
        QLabel {
            color: #64748b;
            font-size: 12px;
            font-weight: 500;
            background-color: transparent;
        }
    )");
    statusLayout->addWidget(readyLabel);
    
    statusLayout->addStretch();
    
    // Frame counter with badge style
    m_frameCountLabel->setStyleSheet(R"(
        QLabel {
            background-color: rgba(59, 130, 246, 0.1);
            color: #3b82f6;
            font-weight: 600;
            font-size: 11px;
            padding: 4px 12px;
            border-radius: 4px;
            border: 1px solid rgba(59, 130, 246, 0.2);
        }
    )");
    statusLayout->addWidget(m_frameCountLabel);
    
    // Separator
    QFrame* sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::VLine);
    sep2->setStyleSheet("background-color: #e2e8f0; max-width: 1px;");
    statusLayout->addWidget(sep2);
    
    // Status indicator with badge
    m_statusLabel->setStyleSheet(R"(
        QLabel {
            background-color: rgba(16, 185, 129, 0.1);
            color: #059669;
            font-weight: 600;
            font-size: 11px;
            padding: 4px 12px;
            border-radius: 4px;
            border: 1px solid rgba(16, 185, 129, 0.2);
        }
    )");
    statusLayout->addWidget(m_statusLabel);
    
    statusBar()->addPermanentWidget(statusWidget, 1);
    statusBar()->setStyleSheet(R"(
        QStatusBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff, stop:1 #f8fafc);
            border-top: 1px solid #e2e8f0;
            padding: 6px 12px;
            min-height: 36px;
        }
        QStatusBar::item {
            border: none;
        }
    )");
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

    // Find if this target already exists in our list
    bool found = false;
    for (auto& target : m_currentTargets.targets) {
        if (target.target_id == packet->target_id) {
            // Update existing target
            target.level = packet->level;
            target.radius = packet->radius;
            target.azimuth = packet->azimuth;
            target.elevation = packet->elevation;
            target.radial_speed = packet->radial_speed;
            target.azimuth_speed = packet->azimuth_speed;
            target.elevation_speed = packet->elevation_speed;
            found = true;
            qDebug() << "Updated existing target ID:" << packet->target_id;
            break;
        }
    }

    if (!found) {
        // Add new target
        TargetTrack new_target;
        new_target.target_id = packet->target_id;
        new_target.level = packet->level;
        new_target.radius = packet->radius;
        new_target.azimuth = packet->azimuth;
        new_target.elevation = packet->elevation;
        new_target.radial_speed = packet->radial_speed;
        new_target.azimuth_speed = packet->azimuth_speed;
        new_target.elevation_speed = packet->elevation_speed;

        m_currentTargets.targets.push_back(new_target);
        m_currentTargets.numTracks = m_currentTargets.targets.size();
        qDebug()<<"new_target.radius "<<new_target.radius;
        qDebug() << "Added new target ID:" << packet->target_id << "Total targets:" << m_currentTargets.numTracks;
    }

    // Disable simulation when receiving real data
    if (m_simulationEnabled) {
        m_simulationEnabled = false;
    }

    m_statusLabel->setText(QString("Binary Target Data - %1 targets, ID: %2")
                          .arg(m_currentTargets.numTracks)
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
                m_currentTargets.targets.push_back(target);
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
        m_currentTargets.targets.push_back(target);
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

void MainWindow::updateTrackTimestamps()
{
    // Record the current timestamp for each track that was just received/updated
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
        uint32_t trackId = m_currentTargets.targets[i].target_id;
        m_trackLastSeen[trackId] = currentTime;
    }
}

void MainWindow::removeStaleTracksFromData()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();

    // Find and remove stale tracks from m_currentTargets
    std::vector<TargetTrack> activeTargets;
    QList<uint32_t> staleTrackIds;

    for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
        uint32_t trackId = m_currentTargets.targets[i].target_id;

        if (m_trackLastSeen.contains(trackId)) {
            qint64 lastSeen = m_trackLastSeen[trackId];
            qint64 elapsed = currentTime - lastSeen;

            if (elapsed < TRACK_STALE_TIMEOUT_MS) {
                // Track is still active, keep it
                activeTargets.push_back(m_currentTargets.targets[i]);
            } else {
                // Track is stale, mark for removal from timestamp map
                staleTrackIds.append(trackId);
                qDebug() << "Removing stale track ID:" << trackId << "(not seen for" << elapsed << "ms)";
            }
        } else {
            // Track has no timestamp (shouldn't happen normally), remove it
            qDebug() << "Removing track ID:" << trackId << "(no timestamp)";
        }
    }

    // Remove stale tracks from the timestamp map
    for (uint32_t trackId : staleTrackIds) {
        m_trackLastSeen.remove(trackId);
    }

    // Also clean up any orphaned entries in the timestamp map
    // (tracks that are in the map but not in current targets)
    QList<uint32_t> orphanedIds;
    for (auto it = m_trackLastSeen.constBegin(); it != m_trackLastSeen.constEnd(); ++it) {
        qint64 elapsed = currentTime - it.value();
        if (elapsed >= TRACK_STALE_TIMEOUT_MS) {
            orphanedIds.append(it.key());
        }
    }
    for (uint32_t trackId : orphanedIds) {
        m_trackLastSeen.remove(trackId);
        qDebug() << "Cleaned up orphaned track ID:" << trackId;
    }

    // Update the target data with only active tracks
    m_currentTargets.targets = activeTargets;
    m_currentTargets.numTracks = static_cast<uint32_t>(activeTargets.size());
}

void MainWindow::refreshTrackTable()
{
    // This is called every 2 seconds to clean up stale tracks
    // Skip cleanup if simulation is enabled (simulation generates fresh targets each frame)
    if (!m_simulationEnabled) {
        removeStaleTracksFromData();

        // Refresh the table display with updated track data
        m_trackTable->setRowCount(m_currentTargets.numTracks);
        for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
            const TargetTrack& target = m_currentTargets.targets[i];
            m_trackTable->setItem(i, 0, new QTableWidgetItem(QString::number(target.target_id)));
            m_trackTable->setItem(i, 1, new QTableWidgetItem(QString::number(target.radius, 'f', 2)));
            m_trackTable->setItem(i, 2, new QTableWidgetItem(QString::number(target.azimuth, 'f', 1)));
            m_trackTable->setItem(i, 3, new QTableWidgetItem(QString::number(target.radial_speed, 'f', 1)));
        }
        m_trackTable->resizeColumnsToContents();

        // Also update the PPI and FFT widgets with the cleaned up data
        m_ppiWidget->updateTargets(m_currentTargets);
        m_fftWidget->updateTargets(m_currentTargets);
    }

    qDebug() << "Track table refreshed. Active tracks:" << m_currentTargets.numTracks
             << "Tracked IDs:" << m_trackLastSeen.keys().size();
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

void MainWindow::onResetSettings()
{
    m_rangeAvgEdit->setText("1");
    m_minRangeEdit->setText("0");
    m_maxRangeEdit->setText("5000");
    m_minSpeedEdit->setText("0");
    m_maxSpeedEdit->setText("100");
    m_minAngleEdit->setText("0");
    m_maxAngleEdit->setText("60");
    m_rangeThresholdEdit->setText("0");
    m_speedThresholdEdit->setText("0");
    m_numTracksEdit->setText("50");
    m_medianFilterEdit->setText("1");
    m_mtiLengthEdit->setText("2");
    m_speedDist = std::uniform_real_distribution<float>(-50.0f, 50.0f);
    
    // Update ALL internal DSP settings to match UI defaults
    m_dsp.range_mvg_avg_length = 1;
    m_dsp.min_range_cm = 0;
    m_dsp.max_range_cm = 5000;
    m_dsp.min_speed_kmh = 0;
    m_dsp.max_speed_kmh = 100;
    m_dsp.min_angle_degree = 0;
    m_dsp.max_angle_degree = 60;
    m_dsp.range_threshold = 0;
    m_dsp.speed_threshold = 0;
    m_dsp.num_of_tracks = 50;
    m_dsp.median_filter_length = 1;
    m_dsp.mti_filter_length = 2;
    
    // Apply to both displays (convert cm to meters)
    m_ppiWidget->setMinRange(0.0f);
    m_ppiWidget->setMaxRange(50.0f);  // 5000cm = 50m
    m_ppiWidget->setMinAngle(-60.0f);
    m_ppiWidget->setMaxAngle(60.0f);
    
    m_fftWidget->setMinRange(0.0f);
    m_fftWidget->setMaxRange(50.0f);
    m_fftWidget->setMinAngle(-60.0f);
    m_fftWidget->setMaxAngle(60.0f);
    
    QMessageBox::information(this, "Settings Reset", "All settings reset to default.");
}

void MainWindow::updateTrackTable()
{
    // Update timestamps for all current tracks
    updateTrackTimestamps();
    m_trackTable->setRowCount(m_currentTargets.numTracks);
    for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
        const TargetTrack& target = m_currentTargets.targets[i];
        m_trackTable->setItem(i, 0, new QTableWidgetItem(QString::number(target.target_id)));
        m_trackTable->setItem(i, 1, new QTableWidgetItem(QString::number(target.radius, 'f', 2)));
        m_trackTable->setItem(i, 2, new QTableWidgetItem(QString::number(target.azimuth, 'f', 1)));
        m_trackTable->setItem(i, 3, new QTableWidgetItem(QString::number(target.radial_speed, 'f', 1)));
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
                stop:0 #ffffff, stop:1 #f8fafc);
            border: none;
            border-bottom: 1px solid #e2e8f0;
            padding: 6px 12px;
            spacing: 6px;
            min-height: 32px;
        }
        QMenuBar::item {
            background-color: transparent;
            padding: 8px 16px;
            border-radius: 8px;
            font-size: 13px;
            font-weight: 600;
            color: #475569;
            letter-spacing: 0.2px;
        }
        QMenuBar::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(59, 130, 246, 0.12), stop:1 rgba(59, 130, 246, 0.08));
            color: #3b82f6;
        }
        QMenuBar::item:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(59, 130, 246, 0.18), stop:1 rgba(59, 130, 246, 0.12));
            color: #2563eb;
        }
        QMenu {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 12px;
            padding: 8px;
            margin-top: 4px;
        }
        QMenu::item {
            padding: 10px 32px 10px 16px;
            border-radius: 6px;
            font-size: 13px;
            font-weight: 500;
            color: #374151;
            margin: 2px 4px;
        }
        QMenu::item:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(59, 130, 246, 0.12), stop:1 rgba(59, 130, 246, 0.08));
            color: #3b82f6;
        }
        QMenu::item:disabled {
            color: #94a3b8;
        }
        QMenu::separator {
            height: 1px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 transparent, stop:0.1 #e2e8f0, stop:0.9 #e2e8f0, stop:1 transparent);
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
            background-color: #3b82f6;
            border-radius: 4px;
        }
    )");
    
    // File Menu
    QMenu* fileMenu = menuBar->addMenu(tr("&File"));
    
    QAction* newAction = fileMenu->addAction(tr("&New Configuration"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, [this]() {
        onResetSettings();
        m_statusLabel->setText("Status: New configuration created");
    });
    
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
    
    QAction* clearDisplayAction = viewMenu->addAction(tr("&Clear Display"));
    connect(clearDisplayAction, &QAction::triggered, this, &MainWindow::onClearTracks);
    
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

    setMenuBar(menuBar);
}

QString MainWindow::getLightThemeStyleSheet() const
{
    return R"(
        /* ========== MAIN WINDOW ========== */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #f8fafc, stop:1 #f1f5f9);
            color: #1e293b;
        }
        
        /* ========== BASE WIDGET STYLING ========== */
        QWidget {
            background-color: transparent;
            color: #1e293b;
            font-family: 'Segoe UI', 'Inter', 'SF Pro Display', -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 13px;
        }
        
        /* ========== CARD-STYLE GROUP BOXES ========== */
        QGroupBox {
            font-weight: 600;
            font-size: 14px;
            color: #0f172a;
            border: none;
            border-radius: 16px;
            margin-top: 20px;
            padding: 24px 20px 20px 20px;
            background-color: #ffffff;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 20px;
            top: 4px;
            padding: 4px 16px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3b82f6, stop:1 #6366f1);
            color: #ffffff;
            font-weight: 700;
            font-size: 13px;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        
        /* ========== LABELS ========== */
        QLabel {
            color: #475569;
            font-weight: 500;
            background-color: transparent;
            letter-spacing: 0.1px;
        }
        
        /* ========== INPUT FIELDS ========== */
        QLineEdit {
            background-color: #f8fafc;
            border: 2px solid #e2e8f0;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0f172a;
            font-weight: 500;
            font-size: 13px;
            selection-background-color: #3b82f6;
            selection-color: #ffffff;
        }
        
        QLineEdit:hover {
            border-color: #94a3b8;
            background-color: #ffffff;
        }
        
        QLineEdit:focus {
            border: 2px solid #3b82f6;
            background-color: #ffffff;
        }
        
        /* ========== SPIN BOXES ========== */
        QSpinBox, QDoubleSpinBox {
            background-color: #f8fafc;
            border: 2px solid #e2e8f0;
            border-radius: 10px;
            padding: 10px 14px;
            color: #0f172a;
            font-weight: 500;
            selection-background-color: #3b82f6;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #94a3b8;
            background-color: #ffffff;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #3b82f6;
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
            background-color: rgba(59, 130, 246, 0.15);
        }
        
        /* ========== BUTTONS ========== */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3b82f6, stop:1 #2563eb);
            border: none;
            border-radius: 10px;
            color: #ffffff;
            font-weight: 600;
            font-size: 13px;
            padding: 12px 24px;
            min-height: 20px;
            letter-spacing: 0.3px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2563eb, stop:1 #1d4ed8);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1d4ed8, stop:1 #1e40af);
        }
        
        QPushButton:disabled {
            background: #e2e8f0;
            color: #94a3b8;
        }
        
        /* ========== DATA TABLE ========== */
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #f8fafc;
            color: #0f172a;
            gridline-color: transparent;
            border: none;
            border-radius: 12px;
            selection-background-color: rgba(59, 130, 246, 0.12);
            selection-color: #0f172a;
            outline: none;
        }
        
        QTableWidget::item {
            padding: 12px 16px;
            border-bottom: 1px solid #f1f5f9;
        }
        
        QTableWidget::item:selected {
            background-color: rgba(59, 130, 246, 0.12);
            color: #0f172a;
        }
        
        QTableWidget::item:hover {
            background-color: rgba(59, 130, 246, 0.06);
        }
        
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #f8fafc, stop:1 #f1f5f9);
            color: #64748b;
            font-weight: 700;
            font-size: 11px;
            padding: 14px 16px;
            border: none;
            border-bottom: 2px solid #e2e8f0;
            text-transform: uppercase;
            letter-spacing: 0.8px;
        }
        
        /* ========== STATUS BAR ========== */
        QStatusBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff, stop:1 #f8fafc);
            color: #64748b;
            border-top: 1px solid #e2e8f0;
            padding: 10px 20px;
            font-size: 12px;
        }
        
        /* ========== SPLITTER HANDLES ========== */
        QSplitter::handle {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #e2e8f0, stop:0.5 #cbd5e1, stop:1 #e2e8f0);
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
                stop:0 #93c5fd, stop:0.5 #3b82f6, stop:1 #93c5fd);
        }
        
        /* ========== SCROLLBARS ========== */
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border-radius: 6px;
            margin: 4px 2px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #cbd5e1;
            border-radius: 5px;
            min-height: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #94a3b8;
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
            background-color: #cbd5e1;
            border-radius: 5px;
            min-width: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: #94a3b8;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            width: 0px;
            background: transparent;
        }
        
        /* ========== TOOLTIPS ========== */
        QToolTip {
            background-color: #0f172a;
            color: #f8fafc;
            border: 1px solid #334155;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 12px;
            font-weight: 500;
        }
    )";
}

QString MainWindow::getDarkThemeStyleSheet() const
{
    return R"(
        /* ========== MAIN WINDOW - DARK ========== */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e293b, stop:1 #0f172a);
            color: #e2e8f0;
        }
        
        /* ========== BASE WIDGET STYLING ========== */
        QWidget {
            background-color: transparent;
            color: #e2e8f0;
            font-family: 'Segoe UI', 'Inter', 'SF Pro Display', -apple-system, BlinkMacSystemFont, sans-serif;
            font-size: 13px;
        }
        
        /* ========== CARD-STYLE GROUP BOXES ========== */
        QGroupBox {
            font-weight: 600;
            font-size: 14px;
            color: #f1f5f9;
            border: 1px solid #334155;
            border-radius: 16px;
            margin-top: 20px;
            padding: 24px 20px 20px 20px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e293b, stop:1 #0f172a);
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 20px;
            top: 4px;
            padding: 4px 16px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #3b82f6, stop:1 #8b5cf6);
            color: #ffffff;
            font-weight: 700;
            font-size: 13px;
            border-radius: 8px;
            letter-spacing: 0.3px;
        }
        
        /* ========== LABELS ========== */
        QLabel {
            color: #94a3b8;
            font-weight: 500;
            background-color: transparent;
            letter-spacing: 0.1px;
        }
        
        /* ========== INPUT FIELDS ========== */
        QLineEdit {
            background-color: #0f172a;
            border: 2px solid #334155;
            border-radius: 10px;
            padding: 10px 14px;
            color: #f1f5f9;
            font-weight: 500;
            font-size: 13px;
            selection-background-color: #3b82f6;
            selection-color: #ffffff;
        }
        
        QLineEdit:hover {
            border-color: #475569;
            background-color: #1e293b;
        }
        
        QLineEdit:focus {
            border: 2px solid #3b82f6;
            background-color: #1e293b;
        }
        
        /* ========== SPIN BOXES ========== */
        QSpinBox, QDoubleSpinBox {
            background-color: #0f172a;
            border: 2px solid #334155;
            border-radius: 10px;
            padding: 10px 14px;
            color: #f1f5f9;
            font-weight: 500;
            selection-background-color: #3b82f6;
        }
        
        QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #475569;
            background-color: #1e293b;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #3b82f6;
            background-color: #1e293b;
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
            background-color: rgba(59, 130, 246, 0.25);
        }
        
        /* ========== BUTTONS ========== */
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3b82f6, stop:1 #2563eb);
            border: none;
            border-radius: 10px;
            color: #ffffff;
            font-weight: 600;
            font-size: 13px;
            padding: 12px 24px;
            min-height: 20px;
            letter-spacing: 0.3px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #60a5fa, stop:1 #3b82f6);
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2563eb, stop:1 #1d4ed8);
        }
        
        QPushButton:disabled {
            background: #334155;
            color: #64748b;
        }
        
        /* ========== DATA TABLE ========== */
        QTableWidget {
            background-color: #0f172a;
            alternate-background-color: #1e293b;
            color: #f1f5f9;
            gridline-color: transparent;
            border: 1px solid #334155;
            border-radius: 12px;
            selection-background-color: rgba(59, 130, 246, 0.2);
            selection-color: #f1f5f9;
            outline: none;
        }
        
        QTableWidget::item {
            padding: 12px 16px;
            border-bottom: 1px solid #1e293b;
        }
        
        QTableWidget::item:selected {
            background-color: rgba(59, 130, 246, 0.2);
            color: #f1f5f9;
        }
        
        QTableWidget::item:hover {
            background-color: rgba(59, 130, 246, 0.1);
        }
        
        QHeaderView {
            background-color: transparent;
        }
        
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e293b, stop:1 #0f172a);
            color: #94a3b8;
            font-weight: 700;
            font-size: 11px;
            padding: 14px 16px;
            border: none;
            border-bottom: 2px solid #334155;
            text-transform: uppercase;
            letter-spacing: 0.8px;
        }
        
        /* ========== STATUS BAR ========== */
        QStatusBar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #1e293b, stop:1 #0f172a);
            color: #94a3b8;
            border-top: 1px solid #334155;
            padding: 10px 20px;
            font-size: 12px;
        }
        
        QStatusBar::item {
            border: none;
        }
        
        /* ========== SPLITTER HANDLES ========== */
        QSplitter::handle {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #334155, stop:0.5 #475569, stop:1 #334155);
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
                stop:0 #60a5fa, stop:0.5 #3b82f6, stop:1 #60a5fa);
        }
        
        /* ========== SCROLLBARS ========== */
        QScrollBar:vertical {
            background-color: transparent;
            width: 12px;
            border-radius: 6px;
            margin: 4px 2px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #475569;
            border-radius: 5px;
            min-height: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #64748b;
        }
        
        QScrollBar::handle:vertical:pressed {
            background-color: #94a3b8;
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
            background-color: #475569;
            border-radius: 5px;
            min-width: 40px;
            margin: 2px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: #64748b;
        }
        
        QScrollBar::handle:horizontal:pressed {
            background-color: #94a3b8;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal,
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            width: 0px;
            background: transparent;
        }
        
        /* ========== TOOLTIPS ========== */
        QToolTip {
            background-color: #1e293b;
            color: #f8fafc;
            border: 1px solid #475569;
            border-radius: 8px;
            padding: 10px 14px;
            font-size: 12px;
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

    // Force update of all widgets
    update();
    if (m_trackTable) m_trackTable->update();
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
        m_minAngleEdit->setText(settings.value("minAngle", "-60").toString());
    if (settings.contains("maxAngle"))
        m_maxAngleEdit->setText(settings.value("maxAngle", "60").toString());
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

void MainWindow::onDefaultSettings()
{
    // Set all fields to default values
    m_rangeAvgEdit->setText("1");
    m_minRangeEdit->setText("0");
    m_maxRangeEdit->setText("5000");
    m_minSpeedEdit->setText("0");
    m_maxSpeedEdit->setText("100");
    m_minAngleEdit->setText("0");
    m_maxAngleEdit->setText("60");
    m_rangeThresholdEdit->setText("0");
    m_speedThresholdEdit->setText("0");
    m_numTracksEdit->setText("50");
    m_medianFilterEdit->setText("1");
    m_mtiLengthEdit->setText("2");
    
    // Update internal DSP settings
    m_dsp.range_mvg_avg_length = 1;
    m_dsp.min_range_cm = 0;
    m_dsp.max_range_cm = 5000;
    m_dsp.min_speed_kmh = 0;
    m_dsp.max_speed_kmh = 100;
    m_dsp.min_angle_degree = 0;
    m_dsp.max_angle_degree = 60;
    m_dsp.range_threshold = 0;
    m_dsp.speed_threshold = 0;
    m_dsp.num_of_tracks = 50;
    m_dsp.median_filter_length = 1;
    m_dsp.mti_filter_length = 2;
    
    m_speedDist = std::uniform_real_distribution<float>(-50.0f, 50.0f);
    
    // Apply to both PPI and FFT displays (convert cm to meters)
    m_ppiWidget->setMinRange(0.0f);
    m_ppiWidget->setMaxRange(50.0f);  // 5000cm = 50m
    m_ppiWidget->setMinAngle(-60.0f);
    m_ppiWidget->setMaxAngle(60.0f);
    
    m_fftWidget->setMinRange(0.0f);
    m_fftWidget->setMaxRange(50.0f);
    m_fftWidget->setMinAngle(-60.0f);
    m_fftWidget->setMaxAngle(60.0f);
    
    m_statusLabel->setText("Status: Default settings restored");
    QMessageBox::information(this, "Default Settings", "All settings restored to factory defaults.");
}

void MainWindow::onClearTracks()
{
    // Clear the internal target data
    m_currentTargets.targets.clear();
    m_currentTargets.numTracks = 0;
    
    // Clear the PPI widget display
    m_ppiWidget->clearTracks();
    
    // Clear the track table
    m_trackTable->setRowCount(0);
    
    // Reset target count statistics
    m_targetCount = 0;
    
    // Update status
    m_statusLabel->setText("Status: Track data cleared");
    
    qDebug() << "Track data cleared from PPI and table";
}
