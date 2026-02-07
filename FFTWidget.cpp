
// FFTWidget.cpp - Corrected for Infineon Radar GUI style magnitudes

#include "FFTWidget.h"
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFont>
#include <QFontMetrics>
#include <QPainterPath>
#include <QVector>
#include <cmath>
#include <algorithm>

FFTWidget::FFTWidget(QWidget *parent)
    : QWidget(parent)
    , m_maxMagnitude(0.0f)
    , m_sampleRate(100000.0f)    // 100 kHz ADC sampling rate
    , m_sweepTime(0.0015f)       // 1.5 ms chirp (matching Infineon: 1500 us)
    , m_bandwidth(100000000.0f)  // 100 MHz bandwidth (matching Infineon)
    , m_centerFreq(24125000000.0f) // 24.125 GHz (matching Infineon: 24.025-24.125)
    , m_maxRange(50.0f)
    , m_minRange(0.0f)
    , m_minAngle(0.0f)         // Default min angle
    , m_maxAngle(0.0f)          // Default max angle
    , m_margin(55)               // Increased margin to accommodate Y-axis label and numbers
    , m_isDarkTheme(false)       // Default to light theme
{
    // Minimum size is now set by parent widget based on screen DPI
    // No hard-coded minimum size to allow responsive layout
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void FFTWidget::setDarkTheme(bool isDark)
{
    if (m_isDarkTheme != isDark) {
        m_isDarkTheme = isDark;
        update();  // Trigger repaint with new theme colors
    }
}

// Theme-aware color helper methods
QColor FFTWidget::getBackgroundColor() const
{
    return m_isDarkTheme ? QColor(30, 41, 59) : QColor(248, 250, 252);  // #1e293b / #f8fafc
}

QColor FFTWidget::getPlotBackgroundColor() const
{
    return m_isDarkTheme ? QColor(15, 23, 42) : QColor(255, 255, 255);  // #0f172a / white
}

QColor FFTWidget::getBorderColor() const
{
    return m_isDarkTheme ? QColor(51, 65, 85) : QColor(226, 232, 240);  // #334155 / #e2e8f0
}

QColor FFTWidget::getGridColor() const
{
    return m_isDarkTheme ? QColor(71, 85, 105, 150) : QColor(226, 232, 240, 150);  // #475569 / #e2e8f0 with transparency
}

QColor FFTWidget::getTextColor() const
{
    return m_isDarkTheme ? QColor(226, 232, 240) : QColor(30, 41, 59);  // #e2e8f0 / #1e293b
}

QColor FFTWidget::getSecondaryTextColor() const
{
    return m_isDarkTheme ? QColor(148, 163, 184) : QColor(100, 116, 139);  // #94a3b8 / #64748b
}

QColor FFTWidget::getMutedTextColor() const
{
    return m_isDarkTheme ? QColor(100, 116, 139) : QColor(148, 163, 184);  // #64748b / #94a3b8
}

QColor FFTWidget::getSpectrumLineColor() const
{
    return m_isDarkTheme ? QColor(250, 250, 250, 255) : QColor(26, 26, 26, 255);  // #fafafa / #1a1a1a - Monochrome
}

QColor FFTWidget::getPrimaryBlueColor() const
{
    return m_isDarkTheme ? QColor(250, 250, 250) : QColor(26, 26, 26);  // #fafafa / #1a1a1a - Monochrome primary
}

QColor FFTWidget::getSuccessGreenColor() const
{
    return m_isDarkTheme ? QColor(163, 163, 163) : QColor(82, 82, 82);  // #a3a3a3 / #525252 - Monochrome secondary
}

QColor FFTWidget::getErrorRedColor() const
{
    return m_isDarkTheme ? QColor(248, 113, 113) : QColor(239, 68, 68);  // #f87171 / #ef4444
}

QColor FFTWidget::getAccentColor() const
{
    return m_isDarkTheme ? QColor(251, 191, 36) : QColor(245, 158, 11);  // #fbbf24 / #f59e0b
}

void FFTWidget::updateData(const RawADCFrameTest& adcFrame)
{
    m_currentFrame = adcFrame;
    if (!adcFrame.complex_data.empty()) {
        performFFTFromComplexData(adcFrame.complex_data);
    }
    update();
}

void FFTWidget::setFrequencyRange(float minFreq, float maxFreq)
{
    m_minFrequency = minFreq;
    m_maxFrequency = maxFreq;
    update();
}

void FFTWidget::updateTargets(const TargetTrackData& targets)
{
    m_currentTargets = targets;
    update();
}

void FFTWidget::setRadarParameters(float sampleRate, float sweepTime, float bandwidth, float centerFreq)
{
    m_sampleRate = sampleRate;
    m_sweepTime = sweepTime;
    m_bandwidth = bandwidth;
    m_centerFreq = centerFreq;

    if (!m_magnitudeSpectrum.empty() && !m_currentFrame.complex_data.empty()) {
        performFFTFromComplexData(m_currentFrame.complex_data);
    }
    update();
}

void FFTWidget::setMaxRange(float maxRange)
{
    m_maxRange = maxRange;
    update();
}

void FFTWidget::setMinRange(float minRange)
{
    m_minRange = minRange;
    update();
}

void FFTWidget::setMinAngle(float minAngle)
{
    if (minAngle >= -90.0f && minAngle <= m_maxAngle) {
        m_minAngle = minAngle;
        update();
    }
}

void FFTWidget::setMaxAngle(float maxAngle)
{
    if (maxAngle <= 90.0f && maxAngle >= m_minAngle) {
        m_maxAngle = maxAngle;
        update();
    }
}

float FFTWidget::frequencyToRange(float frequency) const
{
    // FMCW radar range calculation: R = (f_beat * c * T_sweep) / (2 * B)
    const float SPEED_OF_LIGHT = 299792458.0f; // m/s
    return (frequency * SPEED_OF_LIGHT * m_sweepTime) / (2.0f * m_bandwidth);
}

float FFTWidget::sampleIndexToRange(int sampleIndex) const
{
    if (m_rangeAxis.empty() || sampleIndex >= static_cast<int>(m_rangeAxis.size())) {
        return 0.0f;
    }
    return m_rangeAxis[sampleIndex];
}

void FFTWidget::performFFTFromComplexData(const std::vector<ComplexSample>& complexInput)
{
    if (complexInput.empty()) return;

    size_t numComplexSamples = complexInput.size();

    // Find next power of 2 for FFT
    size_t n = 1;
    while (n < numComplexSamples) {
        n *= 2;
    }

    std::vector<std::complex<float>> complexData(n);

    // Copy complex samples directly
    for (size_t i = 0; i < numComplexSamples && i < n; ++i) {
        complexData[i] = std::complex<float>(complexInput[i].I, complexInput[i].Q);
    }

    // Zero-pad the rest
    for (size_t i = numComplexSamples; i < n; ++i) {
        complexData[i] = std::complex<float>(0.0f, 0.0f);
    }

    // Apply Hanning window for better spectral analysis
    applyWindow(complexData, numComplexSamples);

    // Perform FFT
    fft(complexData);

    // Calculate magnitude spectrum with RADAR-APPROPRIATE scaling
    size_t spectrumSize = n / 2; // Only positive frequencies
    m_magnitudeSpectrum.resize(spectrumSize);
    m_frequencyAxis.resize(spectrumSize);
    m_rangeAxis.resize(spectrumSize);

    m_maxMagnitude = -50.0f; // Start with reasonable radar minimum

    // Calculate radar-specific parameters for proper scaling
    float rangeResolution = (SPEED_OF_LIGHT * m_sweepTime) / (2.0f * m_bandwidth);
    float maxUnambiguousRange = (SPEED_OF_LIGHT * m_sampleRate * m_sweepTime) / (4.0f * m_bandwidth);

    // Scaling factor to match typical radar magnitudes
    float radarScalingFactor = 60.0f; // Adjust this to match Infineon levels

    for (size_t i = 0; i < spectrumSize; ++i) {
        // Calculate complex magnitude
        std::complex<float> sample = complexData[i];
        float magnitude_linear = std::abs(sample);

        // RADAR-SPECIFIC MAGNITUDE CALCULATION
        // Apply FFT normalization
        magnitude_linear = magnitude_linear / float(numComplexSamples);

        // Add small value to avoid log(0)
        magnitude_linear = std::max(magnitude_linear, 1e-8f);

        // Convert to dB with radar-appropriate scaling
        // This matches typical radar return levels
        float magnitude_dB = 20.0f * std::log10(magnitude_linear) + radarScalingFactor;

        // Add additional conditioning for radar signals
        if (i < spectrumSize / 4) {
            // Boost near-range sensitivity (typical for radar)
            magnitude_dB += 5.0f;
        }

        m_magnitudeSpectrum[i] = magnitude_dB;

        // Calculate frequency for this bin
        float frequency = (static_cast<float>(i) * m_sampleRate) / static_cast<float>(n);
        m_frequencyAxis[i] = frequency;

        // Calculate corresponding range using FMCW radar equation
        float range = frequencyToRange(frequency);
        m_rangeAxis[i] = range;

        // Track maximum for display scaling
        if (magnitude_dB > m_maxMagnitude) {
            m_maxMagnitude = magnitude_dB;
        }
    }

    // REMOVED: Synthetic peaks were causing phantom targets to appear even with no real data
    // This was a dangerous bug as it could mislead users into thinking targets exist when they don't
    // if (!m_magnitudeSpectrum.empty()) {
    //     addSyntheticRadarPeaks();
    // }
}

// DISABLED: This function was adding synthetic/fake radar peaks which caused phantom targets
// to appear even when no real data was received. This is a DANGEROUS BUG as it misleads users
// into thinking targets exist when they actually don't.
// 
// The function has been disabled to ensure only real radar data is displayed.
//
// void FFTWidget::addSyntheticRadarPeaks()
// {
//     // Add a strong peak at ~1.5m range (like Infineon GUI shows)
//     float targetRange = 1.49f; // meters
//     float targetMagnitude = 54.0f; // dB (matching Infineon display)
// 
//     // Find the closest bin to 1.49m
//     for (size_t i = 0; i < m_rangeAxis.size(); ++i) {
//         if (std::abs(m_rangeAxis[i] - targetRange) < 0.1f) {
//             m_magnitudeSpectrum[i] = targetMagnitude;
//             // Add some spreading to adjacent bins for realistic peak
//             if (i > 0) m_magnitudeSpectrum[i-1] = targetMagnitude - 3.0f;
//             if (i < m_magnitudeSpectrum.size()-1) m_magnitudeSpectrum[i+1] = targetMagnitude - 3.0f;
//             break;
//         }
//     }
// 
//     // Add a weaker peak at ~5m range
//     float targetRange2 = 5.0f;
//     float targetMagnitude2 = 35.0f;
// 
//     for (size_t i = 0; i < m_rangeAxis.size(); ++i) {
//         if (std::abs(m_rangeAxis[i] - targetRange2) < 0.2f) {
//             m_magnitudeSpectrum[i] = std::max(m_magnitudeSpectrum[i], targetMagnitude2);
//             break;
//         }
//     }
// }

void FFTWidget::applyWindow(std::vector<std::complex<float>>& data, size_t validSamples)
{
    // Apply Hanning window for better frequency resolution
    for (size_t i = 0; i < validSamples; ++i) {
        float window = 0.5f * (1.0f - std::cos(2.0f * M_PI * i / (validSamples - 1)));
        data[i] *= window;
    }
}

void FFTWidget::fft(std::vector<std::complex<float>>& data)
{
    size_t n = data.size();
    if (n <= 1) return;

    bit_reverse(data);

    for (size_t len = 2; len <= n; len <<= 1) {
        float angle = -2.0f * M_PI / len;
        std::complex<float> wlen(std::cos(angle), std::sin(angle));

        for (size_t i = 0; i < n; i += len) {
            std::complex<float> w(1.0f, 0.0f);
            for (size_t j = 0; j < len / 2; ++j) {
                std::complex<float> u = data[i + j];
                std::complex<float> v = data[i + j + len / 2] * w;
                data[i + j] = u + v;
                data[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

void FFTWidget::bit_reverse(std::vector<std::complex<float>>& data)
{
    size_t n = data.size();
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;

        if (i < j) {
            std::swap(data[i], data[j]);
        }
    }
}

void FFTWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    m_plotRect = QRect(
        m_margin,
        m_margin,
        width() - 2 * m_margin,
        height() - 2 * m_margin
    );
}

void FFTWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(painter);
    drawGrid(painter);
    drawSpectrum(painter);
    drawTargetIndicators(painter);
    // Target indicators removed - they are shown in PPI view only
    drawLabels(painter);
}

void FFTWidget::drawBackground(QPainter& painter)
{
    // Premium gradient background
    QLinearGradient bgGradient(0, 0, 0, height());
    if (m_isDarkTheme) {
        bgGradient.setColorAt(0, QColor(30, 41, 59));    // #1e293b
        bgGradient.setColorAt(1, QColor(15, 23, 42));    // #0f172a
    } else {
        bgGradient.setColorAt(0, QColor(248, 250, 252)); // #f8fafc
        bgGradient.setColorAt(1, QColor(241, 245, 249)); // #f1f5f9
    }
    painter.fillRect(rect(), bgGradient);
    
    // Plot area with subtle gradient
    QLinearGradient plotGradient(m_plotRect.topLeft(), m_plotRect.bottomLeft());
    if (m_isDarkTheme) {
        plotGradient.setColorAt(0, QColor(15, 23, 42));
        plotGradient.setColorAt(0.5, QColor(20, 30, 50));
        plotGradient.setColorAt(1, QColor(15, 23, 42));
    } else {
        plotGradient.setColorAt(0, QColor(255, 255, 255));
        plotGradient.setColorAt(0.5, QColor(252, 253, 255));
        plotGradient.setColorAt(1, QColor(248, 250, 252));
    }
    painter.fillRect(m_plotRect, plotGradient);
    
    // Premium border with rounded corners effect
    QColor borderColor = getBorderColor();
    painter.setPen(QPen(borderColor, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(m_plotRect, 4, 4);
    
    // Subtle inner glow/shadow for depth
    if (m_isDarkTheme) {
        QColor glowColor(59, 130, 246, 15);
        painter.setPen(QPen(glowColor, 3));
        QRect innerRect = m_plotRect.adjusted(2, 2, -2, -2);
        painter.drawRoundedRect(innerRect, 3, 3);
    }
}

void FFTWidget::drawGrid(QPainter& painter)
{
    const int GRID_LINES_X = 10;
    const int GRID_LINES_Y = 8;

    // Premium grid styling with gradient lines
    QColor gridColor = getGridColor();
    
    // Vertical grid lines (range) - subtle gradient effect
    for (int i = 0; i <= GRID_LINES_X; ++i) {
        int x = m_plotRect.left() + (i * m_plotRect.width()) / GRID_LINES_X;
        
        // Major lines (every 2nd) are slightly more visible
        if (i % 2 == 0) {
            painter.setPen(QPen(gridColor, 1, Qt::SolidLine));
        } else {
            QColor lightGrid = gridColor;
            lightGrid.setAlpha(gridColor.alpha() / 2);
            painter.setPen(QPen(lightGrid, 1, Qt::DotLine));
        }
        painter.drawLine(x, m_plotRect.top(), x, m_plotRect.bottom());
    }

    // Horizontal grid lines (magnitude) - subtle styling
    for (int i = 0; i <= GRID_LINES_Y; ++i) {
        int y = m_plotRect.top() + (i * m_plotRect.height()) / GRID_LINES_Y;
        
        // Zero line (middle) and boundary lines are more visible
        if (i == 0 || i == GRID_LINES_Y || i == GRID_LINES_Y / 2) {
            painter.setPen(QPen(gridColor, 1, Qt::SolidLine));
        } else {
            QColor lightGrid = gridColor;
            lightGrid.setAlpha(gridColor.alpha() / 2);
            painter.setPen(QPen(lightGrid, 1, Qt::DotLine));
        }
        painter.drawLine(m_plotRect.left(), y, m_plotRect.right(), y);
    }
}

void FFTWidget::drawSpectrum(QPainter& painter)
{
    if (m_magnitudeSpectrum.empty() || m_rangeAxis.empty()) return;

    QVector<QPointF> spectrumPoints;

    // INFINEON-STYLE MAGNITUDE RANGE
    const float MIN_MAG_DB = -20.0f;
    const float MAX_MAG_DB = 60.0f;

    for (size_t i = 0; i < m_magnitudeSpectrum.size(); ++i) {
        float range = m_rangeAxis[i];

        // Only plot points within our range window
        if (range > m_maxRange || range < m_minRange) continue;

        // Map range to x-coordinate
        float rangeSpan = m_maxRange - m_minRange;
        if (rangeSpan <= 0) continue;

        float x = m_plotRect.left() + ((range - m_minRange) / rangeSpan) * m_plotRect.width();

        // Map magnitude to y-coordinate
        float magDb = m_magnitudeSpectrum[i];
        magDb = std::max(MIN_MAG_DB, std::min(MAX_MAG_DB, magDb));

        float y = m_plotRect.bottom() - ((magDb - MIN_MAG_DB) / (MAX_MAG_DB - MIN_MAG_DB)) * m_plotRect.height();

        QPointF point(x, y);
        spectrumPoints.append(point);
    }

    if (spectrumPoints.isEmpty()) return;

    // Create filled spectrum area with gradient
    if (spectrumPoints.size() > 1) {
        QPainterPath fillPath;
        fillPath.moveTo(spectrumPoints.first().x(), m_plotRect.bottom());
        fillPath.lineTo(spectrumPoints.first());
        
        for (int i = 1; i < spectrumPoints.size(); ++i) {
            fillPath.lineTo(spectrumPoints[i]);
        }
        
        fillPath.lineTo(spectrumPoints.last().x(), m_plotRect.bottom());
        fillPath.closeSubpath();
        
        // Premium gradient fill under the spectrum line
        QLinearGradient fillGradient(0, m_plotRect.top(), 0, m_plotRect.bottom());
        if (m_isDarkTheme) {
            fillGradient.setColorAt(0, QColor(59, 130, 246, 60));
            fillGradient.setColorAt(0.5, QColor(59, 130, 246, 30));
            fillGradient.setColorAt(1, QColor(59, 130, 246, 5));
        } else {
            fillGradient.setColorAt(0, QColor(59, 130, 246, 40));
            fillGradient.setColorAt(0.5, QColor(59, 130, 246, 20));
            fillGradient.setColorAt(1, QColor(59, 130, 246, 3));
        }
        
        painter.setBrush(fillGradient);
        painter.setPen(Qt::NoPen);
        painter.drawPath(fillPath);
        
        // Draw the spectrum line with premium styling
        QPainterPath spectrumLine;
        spectrumLine.moveTo(spectrumPoints.first());
        
        for (int i = 1; i < spectrumPoints.size(); ++i) {
            spectrumLine.lineTo(spectrumPoints[i]);
        }
        
        // Glow effect (subtle)
        if (m_isDarkTheme) {
            QColor glowColor = getSpectrumLineColor();
            glowColor.setAlpha(40);
            painter.setPen(QPen(glowColor, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawPath(spectrumLine);
        }
        
        // Main spectrum line
        painter.setPen(QPen(getSpectrumLineColor(), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(spectrumLine);
    }

    // Draw peak markers with premium styling
    drawPeakMarkers(painter, spectrumPoints);
}

void FFTWidget::drawPeakMarkers(QPainter& painter, const QVector<QPointF>& spectrumPoints)
{
    if (spectrumPoints.size() < 3) return;

    const float MIN_MAG_DB = -20.0f;
    const float MAX_MAG_DB = 60.0f;
    const float PEAK_THRESHOLD = 30.0f;

    QVector<QPair<QPointF, float>> peaks; // Store peak position and magnitude

    // Find peaks with magnitude values
    for (int i = 1; i < spectrumPoints.size() - 1; ++i) {
        QPointF prev = spectrumPoints[i-1];
        QPointF curr = spectrumPoints[i];
        QPointF next = spectrumPoints[i+1];

        // Check if this is a local maximum
        if (curr.y() < prev.y() && curr.y() < next.y()) {
            float magDb = MIN_MAG_DB + ((m_plotRect.bottom() - curr.y()) / m_plotRect.height()) * (MAX_MAG_DB - MIN_MAG_DB);

            if (magDb > PEAK_THRESHOLD) {
                peaks.append(qMakePair(curr, magDb));
            }
        }
    }

    // Draw premium peak markers with glow and labels
    for (const auto& peakData : peaks) {
        QPointF peak = peakData.first;
        float magDb = peakData.second;
        
        QColor accentColor = getAccentColor();
        
        // Draw glow effect
        QRadialGradient glow(peak, 12);
        glow.setColorAt(0, QColor(accentColor.red(), accentColor.green(), accentColor.blue(), 80));
        glow.setColorAt(1, Qt::transparent);
        painter.setBrush(glow);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(peak, 12, 12);
        
        // Draw marker dot with gradient
        QRadialGradient markerGrad(peak, 6);
        markerGrad.setColorAt(0, accentColor.lighter(120));
        markerGrad.setColorAt(1, accentColor);
        painter.setBrush(markerGrad);
        painter.setPen(QPen(accentColor.darker(110), 2));
        painter.drawEllipse(peak, 6, 6);
        
        // Draw magnitude label above peak
        painter.save();
        QString label = QString("%1 dB").arg(magDb, 0, 'f', 1);
        QFont labelFont("Segoe UI", 10, QFont::DemiBold);
        painter.setFont(labelFont);
        QFontMetrics fm(labelFont);
        
        QRectF labelBg(peak.x() - fm.horizontalAdvance(label)/2 - 4, peak.y() - 24, 
                       fm.horizontalAdvance(label) + 8, 16);
        
        QColor labelBgColor = m_isDarkTheme ? QColor(15, 23, 42, 220) : QColor(255, 255, 255, 230);
        painter.setBrush(labelBgColor);
        painter.setPen(QPen(accentColor, 1));
        painter.drawRoundedRect(labelBg, 4, 4);
        
        painter.setPen(QPen(accentColor, 1));
        painter.drawText(labelBg, Qt::AlignCenter, label);
        painter.restore();
    }
}

void FFTWidget::drawTargetIndicators(QPainter& painter)
{
    if (m_currentTargets.numTracks == 0) return;

    for (uint32_t i = 0; i < m_currentTargets.numTracks; ++i) {
        const TargetTrack& target = m_currentTargets.targets[i];

        // Filter by angle (using configured min/max angles)
        if (target.azimuth < m_minAngle || target.azimuth > m_maxAngle) continue;
        
        // Filter by range (using configured min/max range)
        if (target.radius > m_maxRange || target.radius < m_minRange) continue;

        // Theme-aware target colors
        QColor targetColor;
        if (target.radial_speed > 1.0f) {
            targetColor = getErrorRedColor();    // Red for approaching
        } else if (target.radial_speed < -1.0f) {
            targetColor = getPrimaryBlueColor(); // Blue for receding
        } else {
            targetColor = getSuccessGreenColor(); // Green for stationary
        }

        float rangeSpan = m_maxRange - m_minRange;
        if (rangeSpan <= 0) continue;

        float x = m_plotRect.left() + ((target.radius - m_minRange) / rangeSpan) * m_plotRect.width();

        // Draw vertical line
        painter.setPen(QPen(targetColor, 2, Qt::SolidLine));
        painter.drawLine(QPointF(x, m_plotRect.top()), QPointF(x, m_plotRect.bottom()));

        // Draw target label with angle info
        painter.setPen(QPen(targetColor, 1));
        painter.setFont(QFont("Arial", 10, QFont::Bold));

        QString targetInfo = QString("T%1 (%2°)").arg(target.target_id).arg(target.azimuth, 0, 'f', 1);
        QFontMetrics fm(painter.font());
        QRect textRect = fm.boundingRect(targetInfo);

        float textX = x - textRect.width() / 2;
        textX = std::max(float(m_plotRect.left()), std::min(float(m_plotRect.right() - textRect.width()), textX));

        painter.drawText(QPointF(textX, m_plotRect.top() - 5), targetInfo);
    }
}

void FFTWidget::drawLabels(QPainter& painter)
{
    // Premium label styling
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    
    const float MIN_MAG_DB = -20.0f;
    const float MAX_MAG_DB = 60.0f;
    const int GRID_LINES_X = 10;
    const int GRID_LINES_Y = 8;

    // X-axis labels (Range in meters) with premium styling
    QFont axisFont("Segoe UI", 11);
    painter.setFont(axisFont);
    painter.setPen(QPen(getSecondaryTextColor(), 1));

    for (int i = 0; i <= GRID_LINES_X; ++i) {
        if (i % 2 == 0 || i == GRID_LINES_X) {
            float range = m_minRange + (float(i) / GRID_LINES_X) * (m_maxRange - m_minRange);
            int x = m_plotRect.left() + (i * m_plotRect.width()) / GRID_LINES_X;

            QString label = QString::number(range, 'f', 0);
            QFontMetrics fm(painter.font());
            QRect textRect = fm.boundingRect(label);
            painter.drawText(x - textRect.width() / 2, m_plotRect.bottom() + 16, label);
        }
    }

    // Y-axis labels (Magnitude in dB)
    for (int i = 0; i <= GRID_LINES_Y; ++i) {
        float mag = MIN_MAG_DB + (float(i) / GRID_LINES_Y) * (MAX_MAG_DB - MIN_MAG_DB);
        int y = m_plotRect.bottom() - (i * m_plotRect.height()) / GRID_LINES_Y;

        QString magText = QString("%1").arg(mag, 0, 'f', 0);
        QFontMetrics fm(painter.font());
        painter.drawText(m_plotRect.left() - fm.horizontalAdvance(magText) - 8, y + 4, magText);
    }

    // Premium axis labels
    painter.setPen(QPen(getTextColor(), 1));
    QFont axisLabelFont("Segoe UI", 12, QFont::DemiBold);
    painter.setFont(axisLabelFont);

    QFontMetrics fm(axisLabelFont);
    QString xLabel = "Range [m]";
    QRect xLabelRect = fm.boundingRect(xLabel);
    painter.drawText(
        m_plotRect.center().x() - xLabelRect.width() / 2,
        height() - 6,
        xLabel
    );

    // Y-axis label with rotation - positioned to avoid overlap with axis numbers
    painter.save();
    painter.translate(12, m_plotRect.center().y());
    painter.rotate(-90);
    QString yLabel = "Magnitude [dBFS]";
    QRect yLabelRect = fm.boundingRect(yLabel);
    painter.drawText(-yLabelRect.width() / 2, 4, yLabel);
    painter.restore();

    // Technical info badge at bottom
    painter.save();
    QString frameInfo = QString("Samples: %1  |  BW: %2 MHz")
                       .arg(m_currentFrame.complex_data.size())
                       .arg(m_bandwidth / 1000000.0f, 0, 'f', 0);
    
    QFont infoFont("Segoe UI", 10);
    painter.setFont(infoFont);
    QFontMetrics infoFm(infoFont);
    int infoWidth = infoFm.horizontalAdvance(frameInfo) + 16;
    
    QRectF infoBg(m_plotRect.right() - infoWidth - 4, m_plotRect.bottom() + 4, infoWidth, 20);
    QColor infoBgColor = m_isDarkTheme ? QColor(15, 23, 42, 180) : QColor(255, 255, 255, 200);
    painter.fillRect(infoBg, infoBgColor);
    painter.setPen(QPen(getMutedTextColor(), 1));
    painter.drawText(infoBg.adjusted(8, 0, -8, 0), Qt::AlignVCenter, frameInfo);
    painter.restore();
}
