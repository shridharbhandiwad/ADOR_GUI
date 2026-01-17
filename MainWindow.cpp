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
#include <QDebug>
#include <cmath>
#include <cstring>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ppiWidget(nullptr)
    , m_fftWidget(nullptr)
    , m_trackTable(nullptr)
    , m_udpSocket(nullptr)
    , m_updateTimer(nullptr)
    , m_simulationEnabled(false)
    , m_randomEngine(std::random_device{}())
    , m_rangeDist(100.0f, 500.0f)
    , m_azimuthDist(-90.0f, 90.0f)
    , m_speedDist(-50.0f, 50.0f)
    , m_levelDist(10.0f, 100.0f)
    , m_numTargetsDist(3, 8)
    , m_frameCount(0)
    , m_targetCount(0)
{
    setupUI();
    setupNetworking();
    setupTimer();
}

MainWindow::~MainWindow()
{
    if (m_updateTimer) {
        m_updateTimer->stop();
    }
}

void MainWindow::setupUI()
{
    setWindowTitle("Radar Data Visualization - Binary UDP Ready");
    setMinimumSize(1200, 800);

    // Apply blue theme stylesheet
    QString blueTheme = R"(
        QMainWindow {
            background-color: #1e3a5f;
            color: #ffffff;
        }
        QWidget {
            background-color: #1e3a5f;
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QGroupBox {
            font-weight: bold;
            font-size: 12px;
            color: #87ceeb;
            border: 2px solid #4682b4;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            background-color: #2c5282;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 10px 0 10px;
            background-color: #2c5282;
            color: #87ceeb;
        }
        QLabel {
            color: #b3d9ff;
            font-weight: normal;
            background-color: transparent;
        }
        QLineEdit {
            background-color: #ffffff;
            border: 2px solid #4682b4;
            border-radius: 4px;
            padding: 4px;
            color: #000000;
            font-weight: bold;
            selection-background-color: #87ceeb;
        }
        QLineEdit:focus {
            border: 2px solid #87ceeb;
            background-color: #f0f8ff;
        }
        QSpinBox {
            background-color: #ffffff;
            border: 2px solid #4682b4;
            border-radius: 4px;
            padding: 4px;
            color: #000000;
            font-weight: bold;
            selection-background-color: #87ceeb;
        }
        QSpinBox:focus {
            border: 2px solid #87ceeb;
            background-color: #f0f8ff;
        }
        QPushButton {
            background-color: #4682b4;
            border: 2px solid #5a9fd4;
            border-radius: 6px;
            color: #ffffff;
            font-weight: bold;
            padding: 6px 12px;
            min-height: 20px;
        }
        QPushButton:hover {
            background-color: #5a9fd4;
            border: 2px solid #87ceeb;
        }
        QPushButton:pressed {
            background-color: #2e5984;
            border: 2px solid #4682b4;
        }
        QPushButton:checked {
            background-color: #1e3a5f;
            border: 2px solid #87ceeb;
            color: #87ceeb;
        }
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #f0f8ff;
            color: #000000;
            gridline-color: #4682b4;
            border: 2px solid #4682b4;
            border-radius: 4px;
            selection-background-color: #87ceeb;
        }
        QTableWidget::item {
            padding: 4px;
            border-bottom: 1px solid #b3d9ff;
        }
        QTableWidget::item:selected {
            background-color: #4682b4;
            color: #ffffff;
        }
        QHeaderView::section {
            background-color: #4682b4;
            color: #ffffff;
            font-weight: bold;
            padding: 6px;
            border: 1px solid #5a9fd4;
        }
        QStatusBar {
            background-color: #2c5282;
            color: #b3d9ff;
            border-top: 1px solid #4682b4;
        }
        QSplitter::handle {
            background-color: #4682b4;
            border: 1px solid #5a9fd4;
        }
        QSplitter::handle:horizontal {
            width: 4px;
        }
        QSplitter::handle:vertical {
            height: 4px;
        }
    )";

    this->setStyleSheet(blueTheme);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    m_mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);

    // Left side: PPI Display
    QGroupBox* ppiGroup = new QGroupBox("PPI Display (Target Tracks)");
    QVBoxLayout* ppiLayout = new QVBoxLayout(ppiGroup);

    m_ppiWidget = new PPIWidget();
    ppiLayout->addWidget(m_ppiWidget);

    // DSP Settings Panel
    QGroupBox* settingsGroup = new QGroupBox("DSP Settings", this);
    QGridLayout* settingsLayout = new QGridLayout(settingsGroup);

    int row = 0;
    auto addRow = [&](const QString& label, QLineEdit*& edit, const QString& def) {
        QLabel* l = new QLabel(label, this);
        edit = new QLineEdit(def, this);
        edit->setMaximumWidth(80);
        settingsLayout->addWidget(l, row, 0);
        settingsLayout->addWidget(edit, row, 1);
        row++;
    };

    addRow("Range Avg",        m_rangeAvgEdit,        "1");
    addRow("Min Range (cm)",   m_minRangeEdit,        "0");
    addRow("Max Range (cm)",   m_maxRangeEdit,        "5000");
    addRow("Min Speed (km/h)", m_minSpeedEdit,        "-100");
    addRow("Max Speed (km/h)", m_maxSpeedEdit,        "100");
    addRow("Min Angle (deg)",  m_minAngleEdit,        "-60");
    addRow("Max Angle (deg)",  m_maxAngleEdit,        "60");
    addRow("Range Thresh",     m_rangeThresholdEdit,  "0");
    addRow("Speed Thresh",     m_speedThresholdEdit,  "0");
    addRow("Num Tracks",       m_numTracksEdit,       "50");
    addRow("Median Filter",    m_medianFilterEdit,    "1");
    addRow("MTI Length",       m_mtiLengthEdit,       "2");

    m_applyButton = new QPushButton("Apply", this);
    m_resetButton = new QPushButton("Reset", this);
    settingsLayout->addWidget(m_applyButton, row, 0);
    settingsLayout->addWidget(m_resetButton, row, 1);

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

    settingsLayout->setColumnMinimumWidth(0, 140);
    settingsLayout->setColumnMinimumWidth(1, 80);
    settingsLayout->setSpacing(5);
    settingsLayout->setContentsMargins(10, 10, 10, 10);

    ppiLayout->addWidget(settingsGroup);
    m_mainSplitter->addWidget(ppiGroup);

    // Right side
    m_rightSplitter = new QSplitter(Qt::Vertical);

    // FFT Display
    QGroupBox* fftGroup = new QGroupBox("FFT Spectrum (Raw ADC Data)");
    QVBoxLayout* fftLayout = new QVBoxLayout(fftGroup);
    m_fftWidget = new FFTWidget();
    m_fftWidget->setRadarParameters(100000.0f, 0.001f, 50000000.0f, 24000000000.0f);
    m_fftWidget->setMaxRange(50.0f);
    fftLayout->addWidget(m_fftWidget);
    m_rightSplitter->addWidget(fftGroup);

    // Track Table
    QGroupBox* tableGroup = new QGroupBox("Target Track Table");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    m_trackTable = new QTableWidget();
    m_trackTable->setColumnCount(4);
    QStringList headers;
    headers << "ID" << "Range (m)" << "Azimuth (°)" << "Radial Speed (m/s)";
    m_trackTable->setHorizontalHeaderLabels(headers);
    m_trackTable->horizontalHeader()->setStretchLastSection(true);
    m_trackTable->setAlternatingRowColors(true);
    m_trackTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLayout->addWidget(m_trackTable);
    m_rightSplitter->addWidget(tableGroup);

    m_mainSplitter->addWidget(m_rightSplitter);
    m_mainSplitter->setSizes({600, 600});
    m_rightSplitter->setSizes({400, 400});

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Control panel
    QHBoxLayout* controlLayout = new QHBoxLayout();
    m_simulateButton = new QPushButton("Enable Simulation");
    m_simulateButton->setCheckable(true);
    m_simulateButton->setChecked(true);
    connect(m_simulateButton, &QPushButton::toggled, this, &MainWindow::onSimulateDataToggled);
    controlLayout->addWidget(m_simulateButton);
    controlLayout->addStretch();
    m_frameCountLabel = new QLabel("Frames: 0");
    controlLayout->addWidget(m_frameCountLabel);
    m_statusLabel = new QLabel("Status: Simulation Active");
    controlLayout->addWidget(m_statusLabel);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(m_mainSplitter);

    statusBar()->showMessage("Radar Visualization Ready - UDP port 5000 (Binary & Text mode)");
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
    m_frameCountLabel->setText(QString("Frames: %1").arg(m_frameCount));

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
        m_simulateButton->setChecked(false);
        m_simulateButton->setText("Enable Simulation");
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
        m_simulateButton->setChecked(false);
        m_simulateButton->setText("Enable Simulation");
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

    for (int i = 0; i < tokens.size(); ++i) {
        const QString& token = tokens[i];

        if (token == "NumTargets:" && i + 1 < tokens.size()) {
            tokens[++i].toInt();
        } else if (token == "TgtId:" && i + 1 < tokens.size()) {
            if (parsedTargets > 0) {
                m_currentTargets.targets.push_back(target);
                target = TargetTrack();
            }
            target.target_id = tokens[++i].toInt();
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

    if (parsedTargets > 0) {
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

//==============================================================================
// SIMULATION CODE
//==============================================================================
void MainWindow::onSimulateDataToggled()
{
    m_simulationEnabled = m_simulateButton->isChecked();
    if (m_simulationEnabled) {
        m_simulateButton->setText("Disable Simulation");
        m_statusLabel->setText("Status: Simulation Active");
    } else {
        m_simulateButton->setText("Enable Simulation");
        m_statusLabel->setText("Status: Waiting for Data");
    }
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

    QByteArray datagram(reinterpret_cast<const char*>(&msg), sizeof(dsp_message_t));
    QUdpSocket udp;
    qint64 bytesSent = udp.writeDatagram(datagram, QHostAddress("127.0.0.1"), 4000);

    if (bytesSent == sizeof(dsp_message_t)) {
        m_statusLabel->setText("DSP settings sent successfully");
        qDebug() << "DSP settings sent successfully "<<bytesSent<<" bytes";
        qDebug() << "MaxRange "<<msg.settings.max_range_cm ;
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
    m_minSpeedEdit->setText("-100");
    m_maxSpeedEdit->setText("100");
    m_minAngleEdit->setText("-60");
    m_maxAngleEdit->setText("60");
    m_rangeThresholdEdit->setText("0");
    m_speedThresholdEdit->setText("0");
    m_numTracksEdit->setText("50");
    m_medianFilterEdit->setText("1");
    m_mtiLengthEdit->setText("2");
    m_speedDist = std::uniform_real_distribution<float>(-50.0f, 50.0f);
    QMessageBox::information(this, "Settings Reset", "All settings reset to default.");
}

void MainWindow::updateTrackTable()
{
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
    if (ok) m_dsp.min_range_cm = std::max(0, v);
}

void MainWindow::onMaxRangeEdited()
{
    bool ok;
    int v = m_maxRangeEdit->text().toInt(&ok);
    if (!ok) return;
    m_dsp.max_range_cm = 10;// std::max(v, static_cast<int>(m_dsp.min_range_cm) + 1);
    m_ppiWidget->setMaxRange(m_dsp.max_range_cm);
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
    if (ok) m_dsp.max_speed_kmh = std::max(static_cast<unsigned short>(v),
                                           static_cast<unsigned short>(m_dsp.min_speed_kmh));
}

void MainWindow::onMinAngleEdited()
{
    bool ok;
    int v = m_minAngleEdit->text().toInt(&ok);
    if (ok) m_dsp.min_angle_degree = v;
}

void MainWindow::onMaxAngleEdited()
{
    bool ok;
    int v = m_maxAngleEdit->text().toInt(&ok);
    if (ok) m_dsp.max_angle_degree = v;
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
