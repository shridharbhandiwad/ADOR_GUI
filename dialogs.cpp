#include "dialogs.h"
#include <QSlider>
#include <QSettings>
#include <QScreen>
#include <QGuiApplication>

// UDP Configuration Dialog Implementation
UdpConfigDialog::UdpConfigDialog(QWidget* parent)
    : QDialog(parent)
    , udpHandler(std::make_unique<UdpHandler>(this))
    , m_isDarkTheme(false)
{
    setWindowTitle("UDP Configuration");
    setModal(true);
    // Responsive minimum size based on screen
    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        qreal dpiScale = screen->logicalDotsPerInch() / 96.0;
        int minWidth = qMax(350, static_cast<int>(350 * dpiScale));
        int minHeight = qMax(250, static_cast<int>(250 * dpiScale));
        setMinimumSize(minWidth, minHeight);
    } else {
        setMinimumSize(400, 300);
    }
    
    setupUI();
    applyThemeStyleSheet();
    
    // Connect UDP handler signals
    connect(udpHandler.get(), &UdpHandler::connectionStatusChanged, 
            this, &UdpConfigDialog::onConnectionStatusChanged);
    connect(udpHandler.get(), &UdpHandler::newDetectionReceived, 
            this, &UdpConfigDialog::onNewDetectionReceived);
    connect(udpHandler.get(), &UdpHandler::errorOccurred, 
            this, &UdpConfigDialog::onErrorOccurred);
    connect(udpHandler.get(), &UdpHandler::statisticsUpdated, 
            this, &UdpConfigDialog::onStatisticsUpdated);
    
    updateUI();
}

UdpConfigDialog::~UdpConfigDialog() = default;

bool UdpConfigDialog::isConnected() const
{
    return udpHandler && udpHandler->isConnected();
}

void UdpConfigDialog::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Host address
    QHBoxLayout* hostLayout = new QHBoxLayout();
    hostLayout->addWidget(new QLabel("Host Address:"));
    hostEdit = new QLineEdit("127.0.0.1"); // Default to loopback
    hostLayout->addWidget(hostEdit);
    layout->addLayout(hostLayout);
    
    // Port
    QHBoxLayout* portLayout = new QHBoxLayout();
    portLayout->addWidget(new QLabel("Port:"));
    portSpinBox = new QSpinBox();
    portSpinBox->setRange(1, 65535);
    portSpinBox->setValue(5000); // Default to port 5000
    portLayout->addWidget(portSpinBox);
    layout->addLayout(portLayout);
    
    // Connection status
    statusLabel = new QLabel("Disconnected");
    statusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    layout->addWidget(statusLabel);
    
    // Statistics
    statisticsLabel = new QLabel("Packets: 0 received, 0 dropped, 0.0 pps");
    statisticsLabel->setStyleSheet("QLabel { color: gray; font-size: 12px; }");
    layout->addWidget(statisticsLabel);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    connectButton = new QPushButton("Connect");
    disconnectButton = new QPushButton("Disconnect");
    closeButton = new QPushButton("Close");
    
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(disconnectButton);
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);
    
    connect(connectButton, &QPushButton::clicked, this, &UdpConfigDialog::connectToHost);
    connect(disconnectButton, &QPushButton::clicked, this, &UdpConfigDialog::disconnectFromHost);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    
    disconnectButton->setEnabled(false);
}

void UdpConfigDialog::updateUI()
{
    bool connected = isConnected();
    connectButton->setEnabled(!connected);
    disconnectButton->setEnabled(connected);
    hostEdit->setEnabled(!connected);
    portSpinBox->setEnabled(!connected);
}

void UdpConfigDialog::connectToHost()
{
    QString host = hostEdit->text().trimmed();
    int port = portSpinBox->value();
    
    if (host.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid host address.");
        return;
    }
    
    if (udpHandler->connectToHost(host, port)) {
        statusLabel->setText(QString("Connected - Listening on %1:%2").arg(host).arg(port));
        statusLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
        emit connectionStatusChanged(true);
    } else {
        statusLabel->setText("Connection Failed");
        statusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    }
    
    updateUI();
}

void UdpConfigDialog::disconnectFromHost()
{
    udpHandler->disconnectFromHost();
    statusLabel->setText("Disconnected");
    statusLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    emit connectionStatusChanged(false);
    updateUI();
}

void UdpConfigDialog::onConnectionStatusChanged(bool connected)
{
    updateUI();
    emit connectionStatusChanged(connected);
}

void UdpConfigDialog::onNewDetectionReceived(const DetectionData& detection)
{
    //qDebug()<<"onNewDetectionReceived";
    emit dataReceived(detection);
}

void UdpConfigDialog::onErrorOccurred(const QString& error)
{
    QMessageBox::warning(this, "UDP Error", error);
}

void UdpConfigDialog::onStatisticsUpdated(int received, int dropped, double rate)
{
    statisticsLabel->setText(QString("Packets: %1 received, %2 dropped, %3 pps")
                            .arg(received)
                            .arg(dropped)
                            .arg(rate, 0, 'f', 1));
}

void UdpConfigDialog::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        applyThemeStyleSheet();
    }
}

void UdpConfigDialog::applyThemeStyleSheet()
{
    if (m_isDarkTheme) {
        setStyleSheet(R"(
            QDialog {
                background-color: #171717;
                color: #fafafa;
            }
            QLabel {
                color: #a3a3a3;
            }
            QLineEdit, QSpinBox {
                background-color: #0a0a0a;
                border: 1px solid #262626;
                border-radius: 4px;
                padding: 4px 8px;
                color: #fafafa;
            }
            QLineEdit:focus, QSpinBox:focus {
                border-color: #fafafa;
            }
            QPushButton {
                background-color: #262626;
                color: #fafafa;
                border: 1px solid #404040;
                border-radius: 6px;
                padding: 8px 16px;
                min-width: 80px;
            }
            QPushButton:hover {
                background-color: #404040;
            }
            QPushButton:pressed {
                background-color: #171717;
            }
        )");
    } else {
        setStyleSheet(R"(
            QDialog {
                background-color: #fafafa;
                color: #1a1a1a;
            }
            QLabel {
                color: #525252;
            }
            QLineEdit, QSpinBox {
                background-color: #ffffff;
                border: 1px solid #d4d4d4;
                border-radius: 4px;
                padding: 4px 8px;
                color: #1a1a1a;
            }
            QLineEdit:focus, QSpinBox:focus {
                border-color: #1a1a1a;
            }
            QPushButton {
                background-color: #e5e5e5;
                color: #1a1a1a;
                border: 1px solid #a3a3a3;
                border-radius: 6px;
                padding: 8px 16px;
                min-width: 80px;
            }
            QPushButton:hover {
                background-color: #d4d4d4;
            }
            QPushButton:pressed {
                background-color: #fafafa;
            }
        )");
    }
}

// Output Configuration Dialog Implementation
OutputConfigDialog::OutputConfigDialog(QWidget* parent)
    : QDialog(parent)
    , m_isDarkTheme(false)
{
    setWindowTitle("Output Configuration");
    setModal(true);
    // Responsive minimum size based on screen
    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        qreal dpiScale = screen->logicalDotsPerInch() / 96.0;
        int minWidth = qMax(450, static_cast<int>(450 * dpiScale));
        int minHeight = qMax(350, static_cast<int>(350 * dpiScale));
        setMinimumSize(minWidth, minHeight);
    } else {
        setMinimumSize(500, 400);
    }
    
    setupUI();
    applyThemeStyleSheet();
    loadSettings();
}

void OutputConfigDialog::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Create tabs for each output
    tabWidget = new QTabWidget();
    
    for (int i = 1; i <= 3; ++i) {
        QWidget* outputTab = createOutputTab(i);
        outputTabs.push_back(outputTab);
        tabWidget->addTab(outputTab, QString("Output %1").arg(i));
    }
    
    layout->addWidget(tabWidget);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Cancel");
    QPushButton* applyButton = new QPushButton("Apply");
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(applyButton);
    layout->addLayout(buttonLayout);
    
    connect(okButton, &QPushButton::clicked, [this]() {
        saveSettings();
        accept();
    });
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(applyButton, &QPushButton::clicked, this, &OutputConfigDialog::saveSettings);
}

QWidget* OutputConfigDialog::createOutputTab(int outputNum)
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Output type
    QGroupBox* typeGroup = new QGroupBox("Output Type");
    QVBoxLayout* typeLayout = new QVBoxLayout(typeGroup);
    
    QRadioButton* digitalRadio = new QRadioButton("Digital");
    QRadioButton* pwmVelRadio = new QRadioButton("PWM (Velocity)");
    QRadioButton* pwmRangeRadio = new QRadioButton("PWM (Range)");
    digitalRadio->setChecked(true);
    
    typeLayout->addWidget(digitalRadio);
    typeLayout->addWidget(pwmVelRadio);
    typeLayout->addWidget(pwmRangeRadio);
    layout->addWidget(typeGroup);
    
    // Pin configuration
    QGroupBox* pinGroup = new QGroupBox("Pin Configuration");
    QVBoxLayout* pinLayout = new QVBoxLayout(pinGroup);
    
    QComboBox* pinConfigCombo = new QComboBox();
    pinConfigCombo->addItems({
        "Active Low - Normally Open",
        "Active Low - Normally Closed",
        "Active High - Normally Open",
        "Active High - Normally Closed",
        "Totem Pole - Normally Open",
        "Totem Pole - Normally Closed"
    });
    
    pinLayout->addWidget(new QLabel("Configuration:"));
    pinLayout->addWidget(pinConfigCombo);
    layout->addWidget(pinGroup);
    
    // Timing settings
    QGroupBox* timingGroup = new QGroupBox("Timing Settings");
    QGridLayout* timingLayout = new QGridLayout(timingGroup);
    
    timingLayout->addWidget(new QLabel("Rising Delay (ms):"), 0, 0);
    QSpinBox* risingDelaySpinBox = new QSpinBox();
    risingDelaySpinBox->setRange(0, 5000);
    risingDelaySpinBox->setValue(375);
    timingLayout->addWidget(risingDelaySpinBox, 0, 1);
    
    timingLayout->addWidget(new QLabel("Falling Delay (ms):"), 1, 0);
    QSpinBox* fallingDelaySpinBox = new QSpinBox();
    fallingDelaySpinBox->setRange(0, 5000);
    fallingDelaySpinBox->setValue(75);
    timingLayout->addWidget(fallingDelaySpinBox, 1, 1);
    
    layout->addWidget(timingGroup);
    
    // Store references for later access
    tab->setProperty("digitalRadio", QVariant::fromValue(digitalRadio));
    tab->setProperty("pwmVelRadio", QVariant::fromValue(pwmVelRadio));
    tab->setProperty("pwmRangeRadio", QVariant::fromValue(pwmRangeRadio));
    tab->setProperty("pinConfigCombo", QVariant::fromValue(pinConfigCombo));
    tab->setProperty("risingDelaySpinBox", QVariant::fromValue(risingDelaySpinBox));
    tab->setProperty("fallingDelaySpinBox", QVariant::fromValue(fallingDelaySpinBox));
    
    return tab;
}

OutputConfigDialog::OutputSettings OutputConfigDialog::getOutputSettings(int outputNumber) const
{
    if (outputNumber < 1 || outputNumber > 3) {
        return OutputSettings();
    }
    
    QWidget* tab = outputTabs[outputNumber - 1];
    OutputSettings settings;
    
    QRadioButton* digitalRadio = tab->property("digitalRadio").value<QRadioButton*>();
    QRadioButton* pwmVelRadio = tab->property("pwmVelRadio").value<QRadioButton*>();
    QRadioButton* pwmRangeRadio = tab->property("pwmRangeRadio").value<QRadioButton*>();
    QComboBox* pinConfigCombo = tab->property("pinConfigCombo").value<QComboBox*>();
    QSpinBox* risingDelaySpinBox = tab->property("risingDelaySpinBox").value<QSpinBox*>();
    QSpinBox* fallingDelaySpinBox = tab->property("fallingDelaySpinBox").value<QSpinBox*>();
    
    if (digitalRadio->isChecked()) {
        settings.type = OutputSettings::DIGITAL;
    } else if (pwmVelRadio->isChecked()) {
        settings.type = OutputSettings::PWM_VELOCITY;
    } else if (pwmRangeRadio->isChecked()) {
        settings.type = OutputSettings::PWM_RANGE;
    }
    
    settings.pinConfig = static_cast<OutputSettings::PinConfiguration>(pinConfigCombo->currentIndex());
    settings.risingDelay = risingDelaySpinBox->value();
    settings.fallingDelay = fallingDelaySpinBox->value();
    
    return settings;
}

void OutputConfigDialog::setOutputSettings(int outputNumber, const OutputSettings& settings)
{
    if (outputNumber < 1 || outputNumber > 3) {
        return;
    }
    
    QWidget* tab = outputTabs[outputNumber - 1];
    
    QRadioButton* digitalRadio = tab->property("digitalRadio").value<QRadioButton*>();
    QRadioButton* pwmVelRadio = tab->property("pwmVelRadio").value<QRadioButton*>();
    QRadioButton* pwmRangeRadio = tab->property("pwmRangeRadio").value<QRadioButton*>();
    QComboBox* pinConfigCombo = tab->property("pinConfigCombo").value<QComboBox*>();
    QSpinBox* risingDelaySpinBox = tab->property("risingDelaySpinBox").value<QSpinBox*>();
    QSpinBox* fallingDelaySpinBox = tab->property("fallingDelaySpinBox").value<QSpinBox*>();
    
    switch (settings.type) {
    case OutputSettings::DIGITAL:
        digitalRadio->setChecked(true);
        break;
    case OutputSettings::PWM_VELOCITY:
        pwmVelRadio->setChecked(true);
        break;
    case OutputSettings::PWM_RANGE:
        pwmRangeRadio->setChecked(true);
        break;
    }
    
    pinConfigCombo->setCurrentIndex(settings.pinConfig);
    risingDelaySpinBox->setValue(settings.risingDelay);
    fallingDelaySpinBox->setValue(settings.fallingDelay);
}

void OutputConfigDialog::saveSettings()
{
    QSettings settings;
    
    for (int i = 1; i <= 3; ++i) {
        OutputSettings outputSettings = getOutputSettings(i);
        QString prefix = QString("Output%1/").arg(i);
        
        settings.setValue(prefix + "type", static_cast<int>(outputSettings.type));
        settings.setValue(prefix + "pinConfig", static_cast<int>(outputSettings.pinConfig));
        settings.setValue(prefix + "risingDelay", outputSettings.risingDelay);
        settings.setValue(prefix + "fallingDelay", outputSettings.fallingDelay);
    }
    
    QMessageBox::information(this, "Settings Saved", "Output configuration saved successfully!");
}

void OutputConfigDialog::loadSettings()
{
    QSettings settings;
    
    for (int i = 1; i <= 3; ++i) {
        OutputSettings outputSettings;
        QString prefix = QString("Output%1/").arg(i);
        
        outputSettings.type = static_cast<OutputSettings::OutputType>(
            settings.value(prefix + "type", static_cast<int>(OutputSettings::DIGITAL)).toInt());
        outputSettings.pinConfig = static_cast<OutputSettings::PinConfiguration>(
            settings.value(prefix + "pinConfig", static_cast<int>(OutputSettings::ACTIVE_LOW_NO)).toInt());
        outputSettings.risingDelay = settings.value(prefix + "risingDelay", 375).toInt();
        outputSettings.fallingDelay = settings.value(prefix + "fallingDelay", 75).toInt();
        
        setOutputSettings(i, outputSettings);
    }
}

void OutputConfigDialog::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        applyThemeStyleSheet();
    }
}

void OutputConfigDialog::applyThemeStyleSheet()
{
    QString styleSheet = m_isDarkTheme ? R"(
        QDialog { background-color: #171717; color: #fafafa; }
        QGroupBox { color: #fafafa; border: 1px solid #262626; border-radius: 6px; margin-top: 12px; background-color: #0a0a0a; }
        QGroupBox::title { color: #fafafa; }
        QLabel { color: #a3a3a3; }
        QRadioButton, QCheckBox { color: #a3a3a3; }
        QComboBox, QSpinBox { background-color: #0a0a0a; border: 1px solid #262626; color: #fafafa; padding: 4px; }
        QPushButton { background-color: #262626; color: #fafafa; border: 1px solid #404040; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #404040; }
        QTabWidget::pane { border: 1px solid #262626; background-color: #171717; }
        QTabBar::tab { background-color: #262626; color: #a3a3a3; border: 1px solid #404040; padding: 8px 16px; }
        QTabBar::tab:selected { background-color: #171717; color: #fafafa; }
    )" : R"(
        QDialog { background-color: #fafafa; color: #1a1a1a; }
        QGroupBox { color: #1a1a1a; border: 1px solid #d4d4d4; border-radius: 6px; margin-top: 12px; background-color: #ffffff; }
        QGroupBox::title { color: #1a1a1a; }
        QLabel { color: #525252; }
        QRadioButton, QCheckBox { color: #525252; }
        QComboBox, QSpinBox { background-color: #ffffff; border: 1px solid #d4d4d4; color: #1a1a1a; padding: 4px; }
        QPushButton { background-color: #e5e5e5; color: #1a1a1a; border: 1px solid #a3a3a3; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #d4d4d4; }
        QTabWidget::pane { border: 1px solid #d4d4d4; background-color: #fafafa; }
        QTabBar::tab { background-color: #e5e5e5; color: #525252; border: 1px solid #a3a3a3; padding: 8px 16px; }
        QTabBar::tab:selected { background-color: #fafafa; color: #1a1a1a; }
    )";
    setStyleSheet(styleSheet);
}

// Angle Correction Dialog Implementation
AngleCorrectionDialog::AngleCorrectionDialog(QWidget* parent)
    : QDialog(parent)
    , m_isDarkTheme(false)
{
    setWindowTitle("Angle Correction");
    setModal(true);
    
    setupUI();
    applyThemeStyleSheet();
    updateControls();
}

void AngleCorrectionDialog::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Correction method
    QGroupBox* methodGroup = new QGroupBox("Correction Method");
    QVBoxLayout* methodLayout = new QVBoxLayout(methodGroup);
    
    angleRadio = new QRadioButton("Correction with mounting angle");
    heightRadio = new QRadioButton("Correction with mounting height");
    angleRadio->setChecked(true);
    
    methodLayout->addWidget(angleRadio);
    methodLayout->addWidget(heightRadio);
    layout->addWidget(methodGroup);
    
    // Parameters
    QGroupBox* paramGroup = new QGroupBox("Parameters");
    QGridLayout* paramLayout = new QGridLayout(paramGroup);
    
    paramLayout->addWidget(new QLabel("Mounting Angle (degrees):"), 0, 0);
    angleSpinBox = new QSpinBox();
    angleSpinBox->setRange(0, 90);
    angleSpinBox->setValue(0);
    paramLayout->addWidget(angleSpinBox, 0, 1);
    
    paramLayout->addWidget(new QLabel("Mounting Height (m):"), 1, 0);
    heightSpinBox = new QDoubleSpinBox();
    heightSpinBox->setRange(0.1, 50.0);
    heightSpinBox->setValue(3.0);
    heightSpinBox->setSingleStep(0.1);
    paramLayout->addWidget(heightSpinBox, 1, 1);
    
    layout->addWidget(paramGroup);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Cancel");
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // Connect radio buttons to update controls
    connect(angleRadio, &QRadioButton::toggled, this, &AngleCorrectionDialog::updateControls);
    connect(heightRadio, &QRadioButton::toggled, this, &AngleCorrectionDialog::updateControls);
}

void AngleCorrectionDialog::updateControls()
{
    angleSpinBox->setEnabled(angleRadio->isChecked());
    heightSpinBox->setEnabled(heightRadio->isChecked());
}

AngleCorrectionDialog::AngleCorrectionSettings AngleCorrectionDialog::getSettings() const
{
    AngleCorrectionSettings settings;
    
    settings.method = angleRadio->isChecked() ? 
        AngleCorrectionSettings::MOUNTING_ANGLE : 
        AngleCorrectionSettings::MOUNTING_HEIGHT;
    settings.mountingAngle = angleSpinBox->value();
    settings.mountingHeight = heightSpinBox->value();
    
    return settings;
}

void AngleCorrectionDialog::setSettings(const AngleCorrectionSettings& settings)
{
    angleRadio->setChecked(settings.method == AngleCorrectionSettings::MOUNTING_ANGLE);
    heightRadio->setChecked(settings.method == AngleCorrectionSettings::MOUNTING_HEIGHT);
    angleSpinBox->setValue(settings.mountingAngle);
    heightSpinBox->setValue(settings.mountingHeight);
    
    updateControls();
}

void AngleCorrectionDialog::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        applyThemeStyleSheet();
    }
}

void AngleCorrectionDialog::applyThemeStyleSheet()
{
    QString styleSheet = m_isDarkTheme ? R"(
        QDialog { background-color: #171717; color: #fafafa; }
        QGroupBox { color: #fafafa; border: 1px solid #262626; border-radius: 6px; margin-top: 12px; background-color: #0a0a0a; }
        QLabel { color: #a3a3a3; }
        QRadioButton { color: #a3a3a3; }
        QSpinBox, QDoubleSpinBox { background-color: #0a0a0a; border: 1px solid #262626; color: #fafafa; padding: 4px; }
        QPushButton { background-color: #262626; color: #fafafa; border: 1px solid #404040; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #404040; }
    )" : R"(
        QDialog { background-color: #fafafa; color: #1a1a1a; }
        QGroupBox { color: #1a1a1a; border: 1px solid #d4d4d4; border-radius: 6px; margin-top: 12px; background-color: #ffffff; }
        QLabel { color: #525252; }
        QRadioButton { color: #525252; }
        QSpinBox, QDoubleSpinBox { background-color: #ffffff; border: 1px solid #d4d4d4; color: #1a1a1a; padding: 4px; }
        QPushButton { background-color: #e5e5e5; color: #1a1a1a; border: 1px solid #a3a3a3; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #d4d4d4; }
    )";
    setStyleSheet(styleSheet);
}

// Amplification Dialog Implementation
AmplificationDialog::AmplificationDialog(QWidget* parent)
    : QDialog(parent)
    , m_isDarkTheme(false)
{
    setWindowTitle("Amplification Settings");
    setModal(true);
    
    setupUI();
    applyThemeStyleSheet();
    updateControls();
}

void AmplificationDialog::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Manual amplification
    QGroupBox* manualGroup = new QGroupBox("Manual Amplification");
    QVBoxLayout* manualLayout = new QVBoxLayout(manualGroup);
    
    QHBoxLayout* sliderLayout = new QHBoxLayout();
    amplificationSlider = new QSlider(Qt::Horizontal);
    amplificationSlider->setRange(0, 60);
    amplificationSlider->setValue(20);
    amplificationLabel = new QLabel("20 dB");
    
    sliderLayout->addWidget(new QLabel("Amplification:"));
    sliderLayout->addWidget(amplificationSlider);
    sliderLayout->addWidget(amplificationLabel);
    
    connect(amplificationSlider, &QSlider::valueChanged, this, &AmplificationDialog::onAmplificationChanged);
    
    manualLayout->addLayout(sliderLayout);
    layout->addWidget(manualGroup);
    
    // Automatic amplification
    QGroupBox* autoGroup = new QGroupBox("Automatic Amplification");
    QVBoxLayout* autoLayout = new QVBoxLayout(autoGroup);
    
    autoEnabledCheckBox = new QCheckBox("Enable automatic amplification");
    autoLayout->addWidget(autoEnabledCheckBox);
    
    // Threshold settings
    QGridLayout* thresholdLayout = new QGridLayout();
    
    thresholdLayout->addWidget(new QLabel("Inner Threshold:"), 0, 0);
    innerThresholdSpinBox = new QSpinBox();
    innerThresholdSpinBox->setRange(0, 100);
    innerThresholdSpinBox->setValue(30);
    innerThresholdSpinBox->setSuffix(" dB");
    thresholdLayout->addWidget(innerThresholdSpinBox, 0, 1);
    
    thresholdLayout->addWidget(new QLabel("Outer Threshold:"), 1, 0);
    outerThresholdSpinBox = new QSpinBox();
    outerThresholdSpinBox->setRange(0, 100);
    outerThresholdSpinBox->setValue(70);
    outerThresholdSpinBox->setSuffix(" dB");
    thresholdLayout->addWidget(outerThresholdSpinBox, 1, 1);
    
    autoLayout->addLayout(thresholdLayout);
    layout->addWidget(autoGroup);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    storeButton = new QPushButton("Store");
    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Cancel");
    
    buttonLayout->addWidget(storeButton);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    connect(storeButton, &QPushButton::clicked, this, &AmplificationDialog::storeSettings);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // Enable/disable controls based on checkbox
    connect(autoEnabledCheckBox, &QCheckBox::toggled, this, &AmplificationDialog::onAutoEnabledChanged);
}

void AmplificationDialog::updateControls()
{
    bool autoEnabled = autoEnabledCheckBox->isChecked();
    innerThresholdSpinBox->setEnabled(autoEnabled);
    outerThresholdSpinBox->setEnabled(autoEnabled);
    amplificationSlider->setEnabled(!autoEnabled);
}

void AmplificationDialog::onAmplificationChanged(int value)
{
    amplificationLabel->setText(QString("%1 dB").arg(value));
    emit amplificationChanged(value);
}

void AmplificationDialog::onAutoEnabledChanged(bool enabled)
{
    updateControls();
}

void AmplificationDialog::storeSettings()
{
    QSettings settings;
    AmplificationSettings ampSettings = getSettings();
    
    settings.setValue("Amplification/manual", ampSettings.manualAmplification);
    settings.setValue("Amplification/autoEnabled", ampSettings.automaticEnabled);
    settings.setValue("Amplification/innerThreshold", ampSettings.innerThreshold);
    settings.setValue("Amplification/outerThreshold", ampSettings.outerThreshold);
    
    QMessageBox::information(this, "Store", "Amplification settings stored to EEPROM!");
    emit settingsStored();
}

AmplificationDialog::AmplificationSettings AmplificationDialog::getSettings() const
{
    AmplificationSettings settings;
    
    settings.manualAmplification = amplificationSlider->value();
    settings.automaticEnabled = autoEnabledCheckBox->isChecked();
    settings.innerThreshold = innerThresholdSpinBox->value();
    settings.outerThreshold = outerThresholdSpinBox->value();
    
    return settings;
}

void AmplificationDialog::setSettings(const AmplificationSettings& settings)
{
    amplificationSlider->setValue(settings.manualAmplification);
    autoEnabledCheckBox->setChecked(settings.automaticEnabled);
    innerThresholdSpinBox->setValue(settings.innerThreshold);
    outerThresholdSpinBox->setValue(settings.outerThreshold);
    
    updateControls();
}

void AmplificationDialog::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        applyThemeStyleSheet();
    }
}

void AmplificationDialog::applyThemeStyleSheet()
{
    QString styleSheet = m_isDarkTheme ? R"(
        QDialog { background-color: #171717; color: #fafafa; }
        QGroupBox { color: #fafafa; border: 1px solid #262626; border-radius: 6px; margin-top: 12px; background-color: #0a0a0a; }
        QLabel { color: #a3a3a3; }
        QCheckBox { color: #a3a3a3; }
        QSpinBox { background-color: #0a0a0a; border: 1px solid #262626; color: #fafafa; padding: 4px; }
        QSlider::groove:horizontal { background: #0a0a0a; border: 1px solid #262626; height: 8px; }
        QSlider::handle:horizontal { background: #fafafa; width: 18px; margin: -5px 0; border-radius: 9px; }
        QSlider::sub-page:horizontal { background: #fafafa; }
        QPushButton { background-color: #262626; color: #fafafa; border: 1px solid #404040; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #404040; }
    )" : R"(
        QDialog { background-color: #fafafa; color: #1a1a1a; }
        QGroupBox { color: #1a1a1a; border: 1px solid #d4d4d4; border-radius: 6px; margin-top: 12px; background-color: #ffffff; }
        QLabel { color: #525252; }
        QCheckBox { color: #525252; }
        QSpinBox { background-color: #ffffff; border: 1px solid #d4d4d4; color: #1a1a1a; padding: 4px; }
        QSlider::groove:horizontal { background: #ffffff; border: 1px solid #d4d4d4; height: 8px; }
        QSlider::handle:horizontal { background: #1a1a1a; width: 18px; margin: -5px 0; border-radius: 9px; }
        QSlider::sub-page:horizontal { background: #1a1a1a; }
        QPushButton { background-color: #e5e5e5; color: #1a1a1a; border: 1px solid #a3a3a3; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #d4d4d4; }
    )";
    setStyleSheet(styleSheet);
}

// Filter Configuration Dialog Implementation
FilterConfigDialog::FilterConfigDialog(QWidget* parent)
    : QDialog(parent)
    , m_isDarkTheme(false)
{
    setWindowTitle("Filter Configuration");
    setModal(true);
    
    setupUI();
    applyThemeStyleSheet();
}

void FilterConfigDialog::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Filter type
    QGroupBox* filterGroup = new QGroupBox("Filter Configuration");
    QGridLayout* filterLayout = new QGridLayout(filterGroup);
    
    filterLayout->addWidget(new QLabel("Filter Type:"), 0, 0);
    filterTypeCombo = new QComboBox();
    filterTypeCombo->addItems({
        "Inactive (no targets)",
        "None (show all targets)", 
        "Highest Amplitude",
        "Mean Value (range)",
        "Median Value (range)",
        "Mean Value (velocity)",
        "Median Value (velocity)"
    });
    filterTypeCombo->setCurrentIndex(2); // Highest Amplitude
    filterLayout->addWidget(filterTypeCombo, 0, 1);
    
    // Target filter options
    QGroupBox* targetFilterGroup = new QGroupBox("Target Filter Options");
    QGridLayout* targetFilterLayout = new QGridLayout(targetFilterGroup);
    
    // Speed range
    targetFilterLayout->addWidget(new QLabel("Speed Range (km/h):"), 0, 0);
    speedMinSpinBox = new QSpinBox();
    speedMinSpinBox->setRange(0, 250);
    speedMaxSpinBox = new QSpinBox();
    speedMaxSpinBox->setRange(0, 250);
    speedMaxSpinBox->setValue(250);
    
    QHBoxLayout* speedLayout = new QHBoxLayout();
    speedLayout->addWidget(speedMinSpinBox);
    speedLayout->addWidget(new QLabel(" - "));
    speedLayout->addWidget(speedMaxSpinBox);
    targetFilterLayout->addLayout(speedLayout, 0, 1);
    
    // Distance range
    targetFilterLayout->addWidget(new QLabel("Distance Range (m):"), 1, 0);
    distanceMinSpinBox = new QSpinBox();
    distanceMinSpinBox->setRange(0, 150);
    distanceMaxSpinBox = new QSpinBox();
    distanceMaxSpinBox->setRange(0, 150);
    distanceMaxSpinBox->setValue(50);
    
    QHBoxLayout* distLayout = new QHBoxLayout();
    distLayout->addWidget(distanceMinSpinBox);
    distLayout->addWidget(new QLabel(" - "));
    distLayout->addWidget(distanceMaxSpinBox);
    targetFilterLayout->addLayout(distLayout, 1, 1);
    
    // Signal level
    targetFilterLayout->addWidget(new QLabel("Signal Level (dB):"), 2, 0);
    signalMinSpinBox = new QSpinBox();
    signalMinSpinBox->setRange(0, 250);
    signalMaxSpinBox = new QSpinBox();
    signalMaxSpinBox->setRange(0, 250);
    signalMaxSpinBox->setValue(250);
    
    QHBoxLayout* signalLayout = new QHBoxLayout();
    signalLayout->addWidget(signalMinSpinBox);
    signalLayout->addWidget(new QLabel(" - "));
    signalLayout->addWidget(signalMaxSpinBox);
    targetFilterLayout->addLayout(signalLayout, 2, 1);
    
    // Direction filter
    targetFilterLayout->addWidget(new QLabel("Direction:"), 3, 0);
    directionCombo = new QComboBox();
    directionCombo->addItems({"Both", "Approaching Only", "Receding Only"});
    targetFilterLayout->addWidget(directionCombo, 3, 1);
    
    // Single target filter
    targetFilterLayout->addWidget(new QLabel("Single Target Filter (%):"), 4, 0);
    singleTargetSpinBox = new QSpinBox();
    singleTargetSpinBox->setRange(0, 100);
    singleTargetSpinBox->setValue(20);
    targetFilterLayout->addWidget(singleTargetSpinBox, 4, 1);
    
    filterLayout->addWidget(targetFilterGroup, 1, 0, 1, 2);
    
    // Show histogram option
    showHistogramCheckBox = new QCheckBox("Show Histogram");
    showHistogramCheckBox->setChecked(true);
    filterLayout->addWidget(showHistogramCheckBox, 2, 0, 1, 2);
    
    layout->addWidget(filterGroup);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("OK");
    QPushButton* cancelButton = new QPushButton("Cancel");
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

FilterConfigDialog::FilterSettings FilterConfigDialog::getSettings() const
{
    FilterSettings settings;
    
    settings.type = static_cast<FilterSettings::FilterType>(filterTypeCombo->currentIndex());
    settings.speedMin = speedMinSpinBox->value();
    settings.speedMax = speedMaxSpinBox->value();
    settings.distanceMin = distanceMinSpinBox->value();
    settings.distanceMax = distanceMaxSpinBox->value();
    settings.signalMin = signalMinSpinBox->value();
    settings.signalMax = signalMaxSpinBox->value();
    settings.direction = static_cast<FilterSettings::Direction>(directionCombo->currentIndex());
    settings.singleTargetFilter = singleTargetSpinBox->value();
    settings.showHistogram = showHistogramCheckBox->isChecked();
    
    return settings;
}

void FilterConfigDialog::setSettings(const FilterSettings& settings)
{
    filterTypeCombo->setCurrentIndex(settings.type);
    speedMinSpinBox->setValue(settings.speedMin);
    speedMaxSpinBox->setValue(settings.speedMax);
    distanceMinSpinBox->setValue(settings.distanceMin);
    distanceMaxSpinBox->setValue(settings.distanceMax);
    signalMinSpinBox->setValue(settings.signalMin);
    signalMaxSpinBox->setValue(settings.signalMax);
    directionCombo->setCurrentIndex(settings.direction);
    singleTargetSpinBox->setValue(settings.singleTargetFilter);
    showHistogramCheckBox->setChecked(settings.showHistogram);
}

void FilterConfigDialog::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        applyThemeStyleSheet();
    }
}

void FilterConfigDialog::applyThemeStyleSheet()
{
    QString styleSheet = m_isDarkTheme ? R"(
        QDialog { background-color: #171717; color: #fafafa; }
        QGroupBox { color: #fafafa; border: 1px solid #262626; border-radius: 6px; margin-top: 12px; background-color: #0a0a0a; }
        QLabel { color: #a3a3a3; }
        QCheckBox { color: #a3a3a3; }
        QComboBox, QSpinBox { background-color: #0a0a0a; border: 1px solid #262626; color: #fafafa; padding: 4px; }
        QPushButton { background-color: #262626; color: #fafafa; border: 1px solid #404040; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #404040; }
    )" : R"(
        QDialog { background-color: #fafafa; color: #1a1a1a; }
        QGroupBox { color: #1a1a1a; border: 1px solid #d4d4d4; border-radius: 6px; margin-top: 12px; background-color: #ffffff; }
        QLabel { color: #525252; }
        QCheckBox { color: #525252; }
        QComboBox, QSpinBox { background-color: #ffffff; border: 1px solid #d4d4d4; color: #1a1a1a; padding: 4px; }
        QPushButton { background-color: #e5e5e5; color: #1a1a1a; border: 1px solid #a3a3a3; border-radius: 6px; padding: 8px 16px; }
        QPushButton:hover { background-color: #d4d4d4; }
    )";
    setStyleSheet(styleSheet);
}

// DSP Settings Dialog Implementation
DSPSettingsDialog::DSPSettingsDialog(QWidget* parent)
    : QDialog(parent)
    , m_isDarkTheme(false)  // Default to light theme
{
    setWindowTitle("DSP Settings");
    setModal(true);
    // Responsive minimum size based on screen
    if (QScreen* screen = QGuiApplication::primaryScreen()) {
        qreal dpiScale = screen->logicalDotsPerInch() / 96.0;
        int minWidth = qMax(550, static_cast<int>(550 * dpiScale));
        int minHeight = qMax(450, static_cast<int>(450 * dpiScale));
        setMinimumSize(minWidth, minHeight);
    } else {
        setMinimumSize(600, 500);
    }
    
    setupUI();
    connectSignals();
    applyThemeStyleSheet();  // Apply initial theme
    
    // Load default settings
    DSP_Settings_Extended_t defaults;
    setSettings(defaults);
}

DSPSettingsDialog::~DSPSettingsDialog() = default;

void DSPSettingsDialog::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        applyThemeStyleSheet();
    }
}

void DSPSettingsDialog::applyThemeStyleSheet()
{
    // ============================================
    // CLASSIC MONOCHROME PALETTE
    // Light Theme: Dark charcoal (#1a1a1a) accents on light background (#fafafa)
    // Dark Theme: White/light grey (#fafafa) accents on dark background (#0a0a0a)
    // No blue, green, red - all meanings derived from grey tones
    // ============================================
    
    if (m_isDarkTheme) {
        // Dark theme stylesheet - Classic monochrome design
        setStyleSheet(R"(
            /* ========== DIALOG - DARK THEME (MONOCHROME) ========== */
            QDialog {
                background-color: #171717;
                color: #fafafa;
            }
            
            /* ========== TAB WIDGET - DARK MONOCHROME ========== */
            QTabWidget::pane {
                border: 1px solid #262626;
                background-color: #171717;
                border-radius: 4px;
            }
            QTabBar::tab {
                background-color: #262626;
                color: #a3a3a3;
                border: 1px solid #404040;
                border-bottom: none;
                padding: 8px 16px;
                margin-right: 2px;
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
            }
            QTabBar::tab:selected {
                background-color: #171717;
                color: #fafafa;
                border-bottom: 2px solid #fafafa;
            }
            QTabBar::tab:hover:!selected {
                background-color: #404040;
                color: #e5e5e5;
            }
            
            /* ========== GROUP BOX - DARK MONOCHROME ========== */
            QGroupBox {
                font-weight: bold;
                color: #fafafa;
                border: 1px solid #262626;
                border-radius: 6px;
                margin-top: 12px;
                padding-top: 10px;
                background-color: #0a0a0a;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                subcontrol-position: top left;
                padding: 4px 8px;
                color: #fafafa;
                background-color: #171717;
                border-radius: 4px;
            }
            
            /* ========== LABELS - DARK MONOCHROME ========== */
            QLabel {
                color: #a3a3a3;
                background-color: transparent;
            }
            
            /* ========== SPIN BOXES - DARK MONOCHROME ========== */
            QSpinBox, QDoubleSpinBox {
                background-color: #0a0a0a;
                border: 1px solid #262626;
                border-radius: 4px;
                padding: 4px 8px;
                color: #fafafa;
                selection-background-color: #fafafa;
                selection-color: #0a0a0a;
            }
            QSpinBox:focus, QDoubleSpinBox:focus {
                border-color: #fafafa;
            }
            QSpinBox::up-button, QDoubleSpinBox::up-button,
            QSpinBox::down-button, QDoubleSpinBox::down-button {
                background-color: #262626;
                border: none;
                width: 16px;
            }
            QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
            QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
                background-color: #404040;
            }
            QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
                image: none;
                border-left: 4px solid transparent;
                border-right: 4px solid transparent;
                border-bottom: 6px solid #a3a3a3;
                width: 0;
                height: 0;
            }
            QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
                image: none;
                border-left: 4px solid transparent;
                border-right: 4px solid transparent;
                border-top: 6px solid #a3a3a3;
                width: 0;
                height: 0;
            }
            
            /* ========== COMBO BOX - DARK MONOCHROME ========== */
            QComboBox {
                background-color: #0a0a0a;
                border: 1px solid #262626;
                border-radius: 4px;
                padding: 4px 8px;
                color: #fafafa;
                min-width: 100px;
            }
            QComboBox:focus {
                border-color: #fafafa;
            }
            QComboBox::drop-down {
                subcontrol-origin: padding;
                subcontrol-position: top right;
                width: 20px;
                border-left: 1px solid #262626;
                background-color: #262626;
                border-top-right-radius: 4px;
                border-bottom-right-radius: 4px;
            }
            QComboBox::down-arrow {
                image: none;
                border-left: 4px solid transparent;
                border-right: 4px solid transparent;
                border-top: 6px solid #a3a3a3;
                width: 0;
                height: 0;
            }
            QComboBox QAbstractItemView {
                background-color: #171717;
                border: 1px solid #262626;
                color: #fafafa;
                selection-background-color: #fafafa;
                selection-color: #0a0a0a;
            }
            
            /* ========== CHECK BOX - DARK MONOCHROME ========== */
            QCheckBox {
                color: #a3a3a3;
                spacing: 8px;
            }
            QCheckBox::indicator {
                width: 18px;
                height: 18px;
                border: 2px solid #404040;
                border-radius: 4px;
                background-color: #0a0a0a;
            }
            QCheckBox::indicator:checked {
                background-color: #fafafa;
                border-color: #fafafa;
            }
            QCheckBox::indicator:hover {
                border-color: #fafafa;
            }
            
            /* ========== SLIDER - DARK MONOCHROME ========== */
            QSlider::groove:horizontal {
                border: 1px solid #262626;
                height: 8px;
                background: #0a0a0a;
                border-radius: 4px;
            }
            QSlider::handle:horizontal {
                background: #fafafa;
                border: none;
                width: 18px;
                margin: -5px 0;
                border-radius: 9px;
            }
            QSlider::handle:horizontal:hover {
                background: #e5e5e5;
            }
            QSlider::sub-page:horizontal {
                background: #fafafa;
                border-radius: 4px;
            }
            
            /* ========== BUTTONS - DARK MONOCHROME ========== */
            QPushButton {
                background-color: #262626;
                color: #fafafa;
                border: 1px solid #404040;
                border-radius: 6px;
                padding: 8px 16px;
                font-weight: 500;
                min-width: 80px;
            }
            QPushButton:hover {
                background-color: #404040;
                border-color: #525252;
            }
            QPushButton:pressed {
                background-color: #171717;
            }
            QPushButton:disabled {
                background-color: #171717;
                color: #525252;
                border-color: #262626;
            }
        )");
        
        // Send button uses primary white for monochrome design
        if (sendButton) {
            sendButton->setStyleSheet("QPushButton { background-color: #fafafa; color: #0a0a0a; font-weight: bold; } QPushButton:hover { background-color: #e5e5e5; }");
        }
    } else {
        // Light theme stylesheet - Classic monochrome design
        setStyleSheet(R"(
            /* ========== DIALOG - LIGHT THEME (MONOCHROME) ========== */
            QDialog {
                background-color: #fafafa;
                color: #0a0a0a;
            }
            
            /* ========== TAB WIDGET - LIGHT MONOCHROME ========== */
            QTabWidget::pane {
                border: 1px solid #e5e5e5;
                background-color: #ffffff;
                border-radius: 4px;
            }
            QTabBar::tab {
                background-color: #f5f5f5;
                color: #525252;
                border: 1px solid #e5e5e5;
                border-bottom: none;
                padding: 8px 16px;
                margin-right: 2px;
                border-top-left-radius: 4px;
                border-top-right-radius: 4px;
            }
            QTabBar::tab:selected {
                background-color: #ffffff;
                color: #0a0a0a;
                border-bottom: 2px solid #1a1a1a;
            }
            QTabBar::tab:hover:!selected {
                background-color: #e5e5e5;
                color: #333333;
            }
            
            /* ========== GROUP BOX - LIGHT MONOCHROME ========== */
            QGroupBox {
                font-weight: bold;
                color: #0a0a0a;
                border: 1px solid #e5e5e5;
                border-radius: 6px;
                margin-top: 12px;
                padding-top: 10px;
                background-color: #ffffff;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                subcontrol-position: top left;
                padding: 4px 8px;
                color: #1a1a1a;
                background-color: #fafafa;
                border-radius: 4px;
            }
            
            /* ========== LABELS - LIGHT MONOCHROME ========== */
            QLabel {
                color: #525252;
                background-color: transparent;
            }
            
            /* ========== SPIN BOXES - LIGHT MONOCHROME ========== */
            QSpinBox, QDoubleSpinBox {
                background-color: #ffffff;
                border: 1px solid #d4d4d4;
                border-radius: 4px;
                padding: 4px 8px;
                color: #0a0a0a;
                selection-background-color: #1a1a1a;
                selection-color: #ffffff;
            }
            QSpinBox:focus, QDoubleSpinBox:focus {
                border-color: #1a1a1a;
            }
            QSpinBox::up-button, QDoubleSpinBox::up-button,
            QSpinBox::down-button, QDoubleSpinBox::down-button {
                background-color: #f5f5f5;
                border: none;
                width: 16px;
            }
            QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
            QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
                background-color: #e5e5e5;
            }
            QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
                image: none;
                border-left: 4px solid transparent;
                border-right: 4px solid transparent;
                border-bottom: 6px solid #525252;
                width: 0;
                height: 0;
            }
            QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
                image: none;
                border-left: 4px solid transparent;
                border-right: 4px solid transparent;
                border-top: 6px solid #525252;
                width: 0;
                height: 0;
            }
            
            /* ========== COMBO BOX - LIGHT MONOCHROME ========== */
            QComboBox {
                background-color: #ffffff;
                border: 1px solid #d4d4d4;
                border-radius: 4px;
                padding: 4px 8px;
                color: #0a0a0a;
                min-width: 100px;
            }
            QComboBox:focus {
                border-color: #1a1a1a;
            }
            QComboBox::drop-down {
                subcontrol-origin: padding;
                subcontrol-position: top right;
                width: 20px;
                border-left: 1px solid #e5e5e5;
                background-color: #f5f5f5;
                border-top-right-radius: 4px;
                border-bottom-right-radius: 4px;
            }
            QComboBox::down-arrow {
                image: none;
                border-left: 4px solid transparent;
                border-right: 4px solid transparent;
                border-top: 6px solid #525252;
                width: 0;
                height: 0;
            }
            QComboBox QAbstractItemView {
                background-color: #ffffff;
                border: 1px solid #e5e5e5;
                color: #0a0a0a;
                selection-background-color: #1a1a1a;
                selection-color: #ffffff;
            }
            
            /* ========== CHECK BOX - LIGHT MONOCHROME ========== */
            QCheckBox {
                color: #525252;
                spacing: 8px;
            }
            QCheckBox::indicator {
                width: 18px;
                height: 18px;
                border: 2px solid #d4d4d4;
                border-radius: 4px;
                background-color: #ffffff;
            }
            QCheckBox::indicator:checked {
                background-color: #1a1a1a;
                border-color: #1a1a1a;
            }
            QCheckBox::indicator:hover {
                border-color: #1a1a1a;
            }
            
            /* ========== SLIDER - LIGHT MONOCHROME ========== */
            QSlider::groove:horizontal {
                border: 1px solid #e5e5e5;
                height: 8px;
                background: #f5f5f5;
                border-radius: 4px;
            }
            QSlider::handle:horizontal {
                background: #1a1a1a;
                border: none;
                width: 18px;
                margin: -5px 0;
                border-radius: 9px;
            }
            QSlider::handle:horizontal:hover {
                background: #333333;
            }
            QSlider::sub-page:horizontal {
                background: #1a1a1a;
                border-radius: 4px;
            }
            
            /* ========== BUTTONS - LIGHT MONOCHROME ========== */
            QPushButton {
                background-color: #f5f5f5;
                color: #0a0a0a;
                border: 1px solid #e5e5e5;
                border-radius: 6px;
                padding: 8px 16px;
                font-weight: 500;
                min-width: 80px;
            }
            QPushButton:hover {
                background-color: #e5e5e5;
                border-color: #d4d4d4;
            }
            QPushButton:pressed {
                background-color: #d4d4d4;
            }
            QPushButton:disabled {
                background-color: #fafafa;
                color: #a3a3a3;
                border-color: #e5e5e5;
            }
        )");
        
        // Send button uses primary dark for monochrome design
        if (sendButton) {
            sendButton->setStyleSheet("QPushButton { background-color: #1a1a1a; color: #ffffff; font-weight: bold; } QPushButton:hover { background-color: #333333; }");
        }
    }
}

void DSPSettingsDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Create tab widget
    tabWidget = new QTabWidget();
    tabWidget->addTab(createDetectionTab(), "Detection");
    tabWidget->addTab(createFFTTab(), "FFT");
    tabWidget->addTab(createFilterTab(), "Filters");
    tabWidget->addTab(createAmplificationTab(), "Amplification");
    tabWidget->addTab(createTargetTab(), "Target");
    tabWidget->addTab(createAzimuthTab(), "Azimuth");
    
    mainLayout->addWidget(tabWidget);
    
    // Button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    loadFromFileButton = new QPushButton("Load From File...");
    saveToFileButton = new QPushButton("Save to File...");
    applyButton = new QPushButton("Apply");
    sendButton = new QPushButton("Send to Radar");
    closeButton = new QPushButton("Close");
    
    sendButton->setStyleSheet("QPushButton { background-color: #1a1a1a; color: white; font-weight: bold; }");
    
    buttonLayout->addWidget(loadFromFileButton);
    buttonLayout->addWidget(saveToFileButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(applyButton);
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
}

QWidget* DSPSettingsDialog::createDetectionTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Threshold settings
    QGroupBox* thresholdGroup = new QGroupBox("Threshold Settings");
    QGridLayout* thresholdLayout = new QGridLayout(thresholdGroup);
    
    thresholdLayout->addWidget(new QLabel("Detection Threshold (dB):"), 0, 0);
    detectionThresholdSpinBox = new QSpinBox();
    detectionThresholdSpinBox->setRange(-50, 50);
    detectionThresholdSpinBox->setValue(0);
    thresholdLayout->addWidget(detectionThresholdSpinBox, 0, 1);
    
    thresholdLayout->addWidget(new QLabel("CFAR Threshold (dB):"), 1, 0);
    cfarThresholdSpinBox = new QSpinBox();
    cfarThresholdSpinBox->setRange(0, 30);
    cfarThresholdSpinBox->setValue(10);
    thresholdLayout->addWidget(cfarThresholdSpinBox, 1, 1);
    
    layout->addWidget(thresholdGroup);
    
    // Range settings
    QGroupBox* rangeGroup = new QGroupBox("Range Settings");
    QGridLayout* rangeLayout = new QGridLayout(rangeGroup);
    
    rangeLayout->addWidget(new QLabel("Min Range (m):"), 0, 0);
    rangeMinSpinBox = new QDoubleSpinBox();
    rangeMinSpinBox->setRange(0.0, 100.0);
    rangeMinSpinBox->setValue(0.5);
    rangeMinSpinBox->setSingleStep(0.1);
    rangeMinSpinBox->setDecimals(1);
    rangeLayout->addWidget(rangeMinSpinBox, 0, 1);
    
    rangeLayout->addWidget(new QLabel("Max Range (m):"), 1, 0);
    rangeMaxSpinBox = new QDoubleSpinBox();
    rangeMaxSpinBox->setRange(1.0, 150.0);
    rangeMaxSpinBox->setValue(50.0);
    rangeMaxSpinBox->setSingleStep(1.0);
    rangeMaxSpinBox->setDecimals(1);
    rangeLayout->addWidget(rangeMaxSpinBox, 1, 1);
    
    layout->addWidget(rangeGroup);
    
    // Speed settings
    QGroupBox* speedGroup = new QGroupBox("Speed Settings");
    QGridLayout* speedLayout = new QGridLayout(speedGroup);
    
    speedLayout->addWidget(new QLabel("Min Speed (m/s):"), 0, 0);
    speedMinSpinBox = new QDoubleSpinBox();
    speedMinSpinBox->setRange(0.0, 50.0);
    speedMinSpinBox->setValue(0.0);
    speedMinSpinBox->setSingleStep(0.5);
    speedMinSpinBox->setDecimals(1);
    speedLayout->addWidget(speedMinSpinBox, 0, 1);
    
    speedLayout->addWidget(new QLabel("Max Speed (m/s):"), 1, 0);
    speedMaxSpinBox = new QDoubleSpinBox();
    speedMaxSpinBox->setRange(1.0, 100.0);
    speedMaxSpinBox->setValue(50.0);
    speedMaxSpinBox->setSingleStep(1.0);
    speedMaxSpinBox->setDecimals(1);
    speedLayout->addWidget(speedMaxSpinBox, 1, 1);
    
    layout->addWidget(speedGroup);
    layout->addStretch();
    
    return tab;
}

QWidget* DSPSettingsDialog::createFFTTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QGroupBox* fftGroup = new QGroupBox("FFT Configuration");
    QGridLayout* fftLayout = new QGridLayout(fftGroup);
    
    fftLayout->addWidget(new QLabel("FFT Size:"), 0, 0);
    fftSizeCombo = new QComboBox();
    fftSizeCombo->addItems({"64", "128", "256", "512", "1024"});
    fftSizeCombo->setCurrentIndex(2); // Default to 256
    fftLayout->addWidget(fftSizeCombo, 0, 1);
    
    fftLayout->addWidget(new QLabel("Window Type:"), 1, 0);
    fftWindowTypeCombo = new QComboBox();
    fftWindowTypeCombo->addItems({"None", "Hann", "Hamming", "Blackman"});
    fftWindowTypeCombo->setCurrentIndex(1); // Default to Hann
    fftLayout->addWidget(fftWindowTypeCombo, 1, 1);
    
    fftLayout->addWidget(new QLabel("FFT Averaging:"), 2, 0);
    fftAveragingSpinBox = new QSpinBox();
    fftAveragingSpinBox->setRange(1, 16);
    fftAveragingSpinBox->setValue(4);
    fftLayout->addWidget(fftAveragingSpinBox, 2, 1);
    
    layout->addWidget(fftGroup);
    layout->addStretch();
    
    return tab;
}

QWidget* DSPSettingsDialog::createFilterTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // General filter settings
    QGroupBox* generalFilterGroup = new QGroupBox("General Filter Settings");
    QVBoxLayout* generalLayout = new QVBoxLayout(generalFilterGroup);
    
    filterEnabledCheckBox = new QCheckBox("Enable Filters");
    filterEnabledCheckBox->setChecked(true);
    generalLayout->addWidget(filterEnabledCheckBox);
    
    QHBoxLayout* movingAvgLayout = new QHBoxLayout();
    movingAvgEnabledCheckBox = new QCheckBox("Moving Average Filter");
    movingAvgLayout->addWidget(movingAvgEnabledCheckBox);
    movingAvgLayout->addWidget(new QLabel("Window Size:"));
    movingAvgWindowSpinBox = new QSpinBox();
    movingAvgWindowSpinBox->setRange(1, 32);
    movingAvgWindowSpinBox->setValue(4);
    movingAvgLayout->addWidget(movingAvgWindowSpinBox);
    movingAvgLayout->addStretch();
    generalLayout->addLayout(movingAvgLayout);
    
    layout->addWidget(generalFilterGroup);
    
    // Line filter settings
    QGroupBox* lineFilterGroup = new QGroupBox("Line Filter Settings");
    QVBoxLayout* lineLayout = new QVBoxLayout(lineFilterGroup);
    
    lineFilter50HzCheckBox = new QCheckBox("50 Hz Line Filter");
    lineFilter100HzCheckBox = new QCheckBox("100 Hz Line Filter");
    lineFilter150HzCheckBox = new QCheckBox("150 Hz Line Filter");
    
    lineLayout->addWidget(lineFilter50HzCheckBox);
    lineLayout->addWidget(lineFilter100HzCheckBox);
    lineLayout->addWidget(lineFilter150HzCheckBox);
    
    layout->addWidget(lineFilterGroup);
    
    // Signal processing settings
    QGroupBox* signalProcessingGroup = new QGroupBox("Signal Processing");
    QVBoxLayout* signalLayout = new QVBoxLayout(signalProcessingGroup);
    
    noiseFloorTrackingCheckBox = new QCheckBox("Noise Floor Tracking");
    noiseFloorTrackingCheckBox->setChecked(true);
    clutterRemovalCheckBox = new QCheckBox("Clutter Removal");
    dopplerCompensationCheckBox = new QCheckBox("Doppler Compensation");
    
    signalLayout->addWidget(noiseFloorTrackingCheckBox);
    signalLayout->addWidget(clutterRemovalCheckBox);
    signalLayout->addWidget(dopplerCompensationCheckBox);
    
    layout->addWidget(signalProcessingGroup);
    layout->addStretch();
    
    return tab;
}

QWidget* DSPSettingsDialog::createAmplificationTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Manual amplification
    QGroupBox* manualGroup = new QGroupBox("Manual Amplification");
    QVBoxLayout* manualLayout = new QVBoxLayout(manualGroup);
    
    QHBoxLayout* sliderLayout = new QHBoxLayout();
    sliderLayout->addWidget(new QLabel("Amplification:"));
    amplificationSlider = new QSlider(Qt::Horizontal);
    amplificationSlider->setRange(0, 60);
    amplificationSlider->setValue(20);
    amplificationLabel = new QLabel("20 dB");
    amplificationLabel->setMinimumWidth(50);
    sliderLayout->addWidget(amplificationSlider);
    sliderLayout->addWidget(amplificationLabel);
    manualLayout->addLayout(sliderLayout);
    
    layout->addWidget(manualGroup);
    
    // Auto amplification
    QGroupBox* autoGroup = new QGroupBox("Automatic Amplification");
    QVBoxLayout* autoLayout = new QVBoxLayout(autoGroup);
    
    autoAmplificationCheckBox = new QCheckBox("Enable Automatic Amplification");
    autoLayout->addWidget(autoAmplificationCheckBox);
    
    QGridLayout* thresholdLayout = new QGridLayout();
    thresholdLayout->addWidget(new QLabel("Inner Threshold (dB):"), 0, 0);
    autoAmpInnerThresholdSpinBox = new QSpinBox();
    autoAmpInnerThresholdSpinBox->setRange(0, 100);
    autoAmpInnerThresholdSpinBox->setValue(30);
    thresholdLayout->addWidget(autoAmpInnerThresholdSpinBox, 0, 1);
    
    thresholdLayout->addWidget(new QLabel("Outer Threshold (dB):"), 1, 0);
    autoAmpOuterThresholdSpinBox = new QSpinBox();
    autoAmpOuterThresholdSpinBox->setRange(0, 100);
    autoAmpOuterThresholdSpinBox->setValue(70);
    thresholdLayout->addWidget(autoAmpOuterThresholdSpinBox, 1, 1);
    
    autoLayout->addLayout(thresholdLayout);
    layout->addWidget(autoGroup);
    layout->addStretch();
    
    return tab;
}

QWidget* DSPSettingsDialog::createTargetTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QGroupBox* targetGroup = new QGroupBox("Target Selection Settings");
    QGridLayout* targetLayout = new QGridLayout(targetGroup);
    
    targetLayout->addWidget(new QLabel("Selection Mode:"), 0, 0);
    targetSelectionModeCombo = new QComboBox();
    targetSelectionModeCombo->addItems({"All Targets", "Nearest Target", "Fastest Target", "Strongest Target"});
    targetLayout->addWidget(targetSelectionModeCombo, 0, 1);
    
    targetLayout->addWidget(new QLabel("Max Targets:"), 1, 0);
    maxTargetsSpinBox = new QSpinBox();
    maxTargetsSpinBox->setRange(1, 10);
    maxTargetsSpinBox->setValue(5);
    targetLayout->addWidget(maxTargetsSpinBox, 1, 1);
    
    targetLayout->addWidget(new QLabel("Direction Filter:"), 2, 0);
    directionFilterCombo = new QComboBox();
    directionFilterCombo->addItems({"Both Directions", "Approaching Only", "Receding Only"});
    targetLayout->addWidget(directionFilterCombo, 2, 1);
    
    layout->addWidget(targetGroup);
    layout->addStretch();
    
    return tab;
}

QWidget* DSPSettingsDialog::createAzimuthTab()
{
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    QGroupBox* azimuthGroup = new QGroupBox("Azimuth Settings");
    QGridLayout* azimuthLayout = new QGridLayout(azimuthGroup);
    
    azimuthLayout->addWidget(new QLabel("Azimuth Offset (deg):"), 0, 0);
    azimuthOffsetSpinBox = new QDoubleSpinBox();
    azimuthOffsetSpinBox->setRange(-45.0, 45.0);
    azimuthOffsetSpinBox->setValue(0.0);
    azimuthOffsetSpinBox->setSingleStep(0.5);
    azimuthOffsetSpinBox->setDecimals(1);
    azimuthLayout->addWidget(azimuthOffsetSpinBox, 0, 1);
    
    azimuthLayout->addWidget(new QLabel("Min Azimuth (deg):"), 1, 0);
    azimuthMinSpinBox = new QDoubleSpinBox();
    azimuthMinSpinBox->setRange(-60.0, 60.0);
    azimuthMinSpinBox->setValue(-45.0);
    azimuthMinSpinBox->setSingleStep(1.0);
    azimuthMinSpinBox->setDecimals(1);
    azimuthLayout->addWidget(azimuthMinSpinBox, 1, 1);
    
    azimuthLayout->addWidget(new QLabel("Max Azimuth (deg):"), 2, 0);
    azimuthMaxSpinBox = new QDoubleSpinBox();
    azimuthMaxSpinBox->setRange(-60.0, 60.0);
    azimuthMaxSpinBox->setValue(45.0);
    azimuthMaxSpinBox->setSingleStep(1.0);
    azimuthMaxSpinBox->setDecimals(1);
    azimuthLayout->addWidget(azimuthMaxSpinBox, 2, 1);
    
    layout->addWidget(azimuthGroup);
    layout->addStretch();
    
    return tab;
}

void DSPSettingsDialog::connectSignals()
{
    connect(amplificationSlider, &QSlider::valueChanged, [this](int value) {
        amplificationLabel->setText(QString("%1 dB").arg(value));
    });
    
    connect(autoAmplificationCheckBox, &QCheckBox::toggled, this, &DSPSettingsDialog::updateAmplificationControls);
    
    connect(applyButton, &QPushButton::clicked, this, &DSPSettingsDialog::onApplyClicked);
    connect(sendButton, &QPushButton::clicked, this, &DSPSettingsDialog::onSendClicked);
    connect(loadFromFileButton, &QPushButton::clicked, this, &DSPSettingsDialog::onLoadFromFileClicked);
    connect(saveToFileButton, &QPushButton::clicked, this, &DSPSettingsDialog::onSaveToFileClicked);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

void DSPSettingsDialog::updateAmplificationControls()
{
    bool autoEnabled = autoAmplificationCheckBox->isChecked();
    amplificationSlider->setEnabled(!autoEnabled);
    autoAmpInnerThresholdSpinBox->setEnabled(autoEnabled);
    autoAmpOuterThresholdSpinBox->setEnabled(autoEnabled);
}

void DSPSettingsDialog::onApplyClicked()
{
    DSP_Settings_Extended_t settings = getSettings();
    emit settingsChanged(settings);
    QMessageBox::information(this, "Settings Applied", "DSP settings have been applied locally.");
}

void DSPSettingsDialog::onSendClicked()
{
    DSP_Settings_Extended_t settings = getSettings();
    settings.updateChecksum();
    emit sendSettingsRequested(settings);
}

void DSPSettingsDialog::onLoadFromFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Load DSP Settings", 
        QString(),
        "DSP Config Files (*.json *.dsp);;All Files (*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Load Error", 
            QString("Could not open file for reading:\n%1").arg(file.errorString()));
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(this, "Parse Error", 
            QString("Failed to parse JSON:\n%1").arg(parseError.errorString()));
        return;
    }
    
    if (!doc.isObject()) {
        QMessageBox::warning(this, "Format Error", "Invalid config file format.");
        return;
    }
    
    QJsonObject obj = doc.object();
    DSP_Settings_Extended_t settings;
    
    // Detection threshold settings
    if (obj.contains("detection_threshold"))
        settings.detection_threshold = static_cast<int16_t>(obj["detection_threshold"].toInt());
    if (obj.contains("cfar_threshold"))
        settings.cfar_threshold = static_cast<int16_t>(obj["cfar_threshold"].toInt());
    
    // Range settings
    if (obj.contains("range_min"))
        settings.range_min = static_cast<float>(obj["range_min"].toDouble());
    if (obj.contains("range_max"))
        settings.range_max = static_cast<float>(obj["range_max"].toDouble());
    
    // Speed settings
    if (obj.contains("speed_min"))
        settings.speed_min = static_cast<float>(obj["speed_min"].toDouble());
    if (obj.contains("speed_max"))
        settings.speed_max = static_cast<float>(obj["speed_max"].toDouble());
    
    // FFT settings
    if (obj.contains("fft_size"))
        settings.fft_size = static_cast<uint16_t>(obj["fft_size"].toInt());
    if (obj.contains("fft_window_type"))
        settings.fft_window_type = static_cast<uint8_t>(obj["fft_window_type"].toInt());
    if (obj.contains("fft_averaging"))
        settings.fft_averaging = static_cast<uint8_t>(obj["fft_averaging"].toInt());
    
    // Filter settings
    if (obj.contains("filter_enabled"))
        settings.filter_enabled = obj["filter_enabled"].toBool() ? 1 : 0;
    if (obj.contains("moving_avg_enabled"))
        settings.moving_avg_enabled = obj["moving_avg_enabled"].toBool() ? 1 : 0;
    if (obj.contains("moving_avg_window"))
        settings.moving_avg_window = static_cast<uint8_t>(obj["moving_avg_window"].toInt());
    
    // Line filter settings
    if (obj.contains("line_filter_50hz"))
        settings.line_filter_50hz = obj["line_filter_50hz"].toBool() ? 1 : 0;
    if (obj.contains("line_filter_100hz"))
        settings.line_filter_100hz = obj["line_filter_100hz"].toBool() ? 1 : 0;
    if (obj.contains("line_filter_150hz"))
        settings.line_filter_150hz = obj["line_filter_150hz"].toBool() ? 1 : 0;
    
    // Amplification settings
    if (obj.contains("amplification"))
        settings.amplification = static_cast<int16_t>(obj["amplification"].toInt());
    if (obj.contains("auto_amplification"))
        settings.auto_amplification = obj["auto_amplification"].toBool() ? 1 : 0;
    if (obj.contains("auto_amp_inner_threshold"))
        settings.auto_amp_inner_threshold = static_cast<int16_t>(obj["auto_amp_inner_threshold"].toInt());
    if (obj.contains("auto_amp_outer_threshold"))
        settings.auto_amp_outer_threshold = static_cast<int16_t>(obj["auto_amp_outer_threshold"].toInt());
    
    // Target selection settings
    if (obj.contains("target_selection_mode"))
        settings.target_selection_mode = static_cast<uint8_t>(obj["target_selection_mode"].toInt());
    if (obj.contains("max_targets"))
        settings.max_targets = static_cast<uint8_t>(obj["max_targets"].toInt());
    if (obj.contains("direction_filter"))
        settings.direction_filter = static_cast<uint8_t>(obj["direction_filter"].toInt());
    
    // Signal processing settings
    if (obj.contains("noise_floor_tracking"))
        settings.noise_floor_tracking = obj["noise_floor_tracking"].toBool() ? 1 : 0;
    if (obj.contains("clutter_removal"))
        settings.clutter_removal = obj["clutter_removal"].toBool() ? 1 : 0;
    if (obj.contains("doppler_compensation"))
        settings.doppler_compensation = obj["doppler_compensation"].toBool() ? 1 : 0;
    
    // Azimuth settings
    if (obj.contains("azimuth_offset"))
        settings.azimuth_offset = static_cast<float>(obj["azimuth_offset"].toDouble());
    if (obj.contains("azimuth_min"))
        settings.azimuth_min = static_cast<float>(obj["azimuth_min"].toDouble());
    if (obj.contains("azimuth_max"))
        settings.azimuth_max = static_cast<float>(obj["azimuth_max"].toDouble());
    
    setSettings(settings);
    QMessageBox::information(this, "Settings Loaded", 
        QString("DSP settings have been loaded from:\n%1").arg(fileName));
}

void DSPSettingsDialog::onSaveToFileClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Save DSP Settings", 
        "dsp_settings.json",
        "DSP Config Files (*.json *.dsp);;All Files (*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    // Ensure file has .json extension if no extension specified
    if (!fileName.contains('.')) {
        fileName += ".json";
    }
    
    DSP_Settings_Extended_t settings = getSettings();
    
    QJsonObject obj;
    
    // Detection threshold settings
    obj["detection_threshold"] = settings.detection_threshold;
    obj["cfar_threshold"] = settings.cfar_threshold;
    
    // Range settings
    obj["range_min"] = static_cast<double>(settings.range_min);
    obj["range_max"] = static_cast<double>(settings.range_max);
    
    // Speed settings
    obj["speed_min"] = static_cast<double>(settings.speed_min);
    obj["speed_max"] = static_cast<double>(settings.speed_max);
    
    // FFT settings
    obj["fft_size"] = settings.fft_size;
    obj["fft_window_type"] = settings.fft_window_type;
    obj["fft_averaging"] = settings.fft_averaging;
    
    // Filter settings
    obj["filter_enabled"] = settings.filter_enabled != 0;
    obj["moving_avg_enabled"] = settings.moving_avg_enabled != 0;
    obj["moving_avg_window"] = settings.moving_avg_window;
    
    // Line filter settings
    obj["line_filter_50hz"] = settings.line_filter_50hz != 0;
    obj["line_filter_100hz"] = settings.line_filter_100hz != 0;
    obj["line_filter_150hz"] = settings.line_filter_150hz != 0;
    
    // Amplification settings
    obj["amplification"] = settings.amplification;
    obj["auto_amplification"] = settings.auto_amplification != 0;
    obj["auto_amp_inner_threshold"] = settings.auto_amp_inner_threshold;
    obj["auto_amp_outer_threshold"] = settings.auto_amp_outer_threshold;
    
    // Target selection settings
    obj["target_selection_mode"] = settings.target_selection_mode;
    obj["max_targets"] = settings.max_targets;
    obj["direction_filter"] = settings.direction_filter;
    
    // Signal processing settings
    obj["noise_floor_tracking"] = settings.noise_floor_tracking != 0;
    obj["clutter_removal"] = settings.clutter_removal != 0;
    obj["doppler_compensation"] = settings.doppler_compensation != 0;
    
    // Azimuth settings
    obj["azimuth_offset"] = static_cast<double>(settings.azimuth_offset);
    obj["azimuth_min"] = static_cast<double>(settings.azimuth_min);
    obj["azimuth_max"] = static_cast<double>(settings.azimuth_max);
    
    QJsonDocument doc(obj);
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Save Error", 
            QString("Could not open file for writing:\n%1").arg(file.errorString()));
        return;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    QMessageBox::information(this, "Settings Saved", 
        QString("DSP settings have been saved to:\n%1").arg(fileName));
}

DSP_Settings_Extended_t DSPSettingsDialog::getSettings() const
{
    DSP_Settings_Extended_t settings;
    
    // Detection threshold settings
    settings.detection_threshold = static_cast<int16_t>(detectionThresholdSpinBox->value());
    settings.cfar_threshold = static_cast<int16_t>(cfarThresholdSpinBox->value());
    
    // Range settings
    settings.range_min = static_cast<float>(rangeMinSpinBox->value());
    settings.range_max = static_cast<float>(rangeMaxSpinBox->value());
    
    // Speed settings
    settings.speed_min = static_cast<float>(speedMinSpinBox->value());
    settings.speed_max = static_cast<float>(speedMaxSpinBox->value());
    
    // FFT settings
    int fftIndex = fftSizeCombo->currentIndex();
    uint16_t fftSizes[] = {64, 128, 256, 512, 1024};
    settings.fft_size = fftSizes[fftIndex];
    settings.fft_window_type = static_cast<uint8_t>(fftWindowTypeCombo->currentIndex());
    settings.fft_averaging = static_cast<uint8_t>(fftAveragingSpinBox->value());
    
    // Filter settings
    settings.filter_enabled = filterEnabledCheckBox->isChecked() ? 1 : 0;
    settings.moving_avg_enabled = movingAvgEnabledCheckBox->isChecked() ? 1 : 0;
    settings.moving_avg_window = static_cast<uint8_t>(movingAvgWindowSpinBox->value());
    
    // Line filter settings
    settings.line_filter_50hz = lineFilter50HzCheckBox->isChecked() ? 1 : 0;
    settings.line_filter_100hz = lineFilter100HzCheckBox->isChecked() ? 1 : 0;
    settings.line_filter_150hz = lineFilter150HzCheckBox->isChecked() ? 1 : 0;
    
    // Amplification settings
    settings.amplification = static_cast<int16_t>(amplificationSlider->value());
    settings.auto_amplification = autoAmplificationCheckBox->isChecked() ? 1 : 0;
    settings.auto_amp_inner_threshold = static_cast<int16_t>(autoAmpInnerThresholdSpinBox->value());
    settings.auto_amp_outer_threshold = static_cast<int16_t>(autoAmpOuterThresholdSpinBox->value());
    
    // Target selection settings
    settings.target_selection_mode = static_cast<uint8_t>(targetSelectionModeCombo->currentIndex());
    settings.max_targets = static_cast<uint8_t>(maxTargetsSpinBox->value());
    settings.direction_filter = static_cast<uint8_t>(directionFilterCombo->currentIndex());
    
    // Signal processing settings
    settings.noise_floor_tracking = noiseFloorTrackingCheckBox->isChecked() ? 1 : 0;
    settings.clutter_removal = clutterRemovalCheckBox->isChecked() ? 1 : 0;
    settings.doppler_compensation = dopplerCompensationCheckBox->isChecked() ? 1 : 0;
    
    // Azimuth settings
    settings.azimuth_offset = static_cast<float>(azimuthOffsetSpinBox->value());
    settings.azimuth_min = static_cast<float>(azimuthMinSpinBox->value());
    settings.azimuth_max = static_cast<float>(azimuthMaxSpinBox->value());
    
    return settings;
}

void DSPSettingsDialog::setSettings(const DSP_Settings_Extended_t& settings)
{
    // Detection threshold settings
    detectionThresholdSpinBox->setValue(settings.detection_threshold);
    cfarThresholdSpinBox->setValue(settings.cfar_threshold);
    
    // Range settings
    rangeMinSpinBox->setValue(settings.range_min);
    rangeMaxSpinBox->setValue(settings.range_max);
    
    // Speed settings
    speedMinSpinBox->setValue(settings.speed_min);
    speedMaxSpinBox->setValue(settings.speed_max);
    
    // FFT settings
    int fftIndex = 2; // Default to 256
    switch (settings.fft_size) {
        case 64: fftIndex = 0; break;
        case 128: fftIndex = 1; break;
        case 256: fftIndex = 2; break;
        case 512: fftIndex = 3; break;
        case 1024: fftIndex = 4; break;
    }
    fftSizeCombo->setCurrentIndex(fftIndex);
    fftWindowTypeCombo->setCurrentIndex(settings.fft_window_type);
    fftAveragingSpinBox->setValue(settings.fft_averaging);
    
    // Filter settings
    filterEnabledCheckBox->setChecked(settings.filter_enabled != 0);
    movingAvgEnabledCheckBox->setChecked(settings.moving_avg_enabled != 0);
    movingAvgWindowSpinBox->setValue(settings.moving_avg_window);
    
    // Line filter settings
    lineFilter50HzCheckBox->setChecked(settings.line_filter_50hz != 0);
    lineFilter100HzCheckBox->setChecked(settings.line_filter_100hz != 0);
    lineFilter150HzCheckBox->setChecked(settings.line_filter_150hz != 0);
    
    // Amplification settings
    amplificationSlider->setValue(settings.amplification);
    amplificationLabel->setText(QString("%1 dB").arg(settings.amplification));
    autoAmplificationCheckBox->setChecked(settings.auto_amplification != 0);
    autoAmpInnerThresholdSpinBox->setValue(settings.auto_amp_inner_threshold);
    autoAmpOuterThresholdSpinBox->setValue(settings.auto_amp_outer_threshold);
    
    // Target selection settings
    targetSelectionModeCombo->setCurrentIndex(settings.target_selection_mode);
    maxTargetsSpinBox->setValue(settings.max_targets);
    directionFilterCombo->setCurrentIndex(settings.direction_filter);
    
    // Signal processing settings
    noiseFloorTrackingCheckBox->setChecked(settings.noise_floor_tracking != 0);
    clutterRemovalCheckBox->setChecked(settings.clutter_removal != 0);
    dopplerCompensationCheckBox->setChecked(settings.doppler_compensation != 0);
    
    // Azimuth settings
    azimuthOffsetSpinBox->setValue(settings.azimuth_offset);
    azimuthMinSpinBox->setValue(settings.azimuth_min);
    azimuthMaxSpinBox->setValue(settings.azimuth_max);
    
    updateAmplificationControls();
}

void DSPSettingsDialog::updateUIFromSettings()
{
    updateAmplificationControls();
}
