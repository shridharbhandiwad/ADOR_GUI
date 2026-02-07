# UDP Data Flow Issue Fix

## Problem Description

When UDP data packets stopped being received, the time series plots continued to display old/residual data instead of clearing. This occurred because:

1. When no new UDP packets arrived, `m_currentTargets` in MainWindow retained the last received target data
2. The `updateDisplay()` timer continued calling `updateFromTargets()` with this stale data
3. The time series plots had no mechanism to detect that the data was old and should be cleared
4. Data cleanup only happened when new data points were added, not continuously

## Root Cause

The data flow was:
```
UDP Packets → m_currentTargets → updateDisplay() (timer) → updateFromTargets() → Plot Data
```

When UDP packets stopped:
- `m_currentTargets` still contained old targets with old `lastUpdateTime` values
- The update timer kept passing this stale data to the plots
- Plots would continue displaying data points until they aged out of the time window
- No mechanism existed to detect "no new UDP data" and clear immediately

## Solution Implemented

### 1. Added Automatic Data Cleanup Timer (`TimeSeriesPlotWidget`)

**File:** `TimeSeriesPlotsWidget.cpp`, `TimeSeriesPlotsWidget.h`

- Added `m_cleanupTimer` member variable to `TimeSeriesPlotWidget` class
- Timer runs every 1 second to automatically remove data points outside the time window
- Separated cleanup logic into `cleanupOldData()` method
- Cleanup now happens continuously, not just when new data arrives

```cpp
// In constructor
m_cleanupTimer = new QTimer(this);
connect(m_cleanupTimer, &QTimer::timeout, this, &TimeSeriesPlotWidget::cleanupOldData);
m_cleanupTimer->start(1000); // Check every 1 second

// New cleanup method
void TimeSeriesPlotWidget::cleanupOldData()
{
    qint64 cutoffTime = QDateTime::currentMSecsSinceEpoch() - (m_timeWindowSeconds * 1000);
    
    // Remove all data points older than the time window
    while (!m_dataPoints.isEmpty() && m_dataPoints.first().first < cutoffTime) {
        m_dataPoints.removeFirst();
    }
    
    // Trigger repaint
    update();
}
```

### 2. Added Stale Data Detection

**File:** `TimeSeriesPlotsWidget.cpp` - `updateFromTargets()` method

- Check if target data is stale by examining `lastUpdateTime` of all targets
- If all targets are older than 2 seconds, consider UDP data stream dead
- Immediately clear all time series plots when stale data detected

```cpp
// Check if the target data is stale (no new UDP packets received)
bool dataIsStale = true;
for (size_t i = 0; i < targets.numTracks && i < targets.targets.size(); ++i) {
    qint64 targetAge = currentTime - targets.targets[i].lastUpdateTime;
    if (targetAge < 2000) {  // Data is fresh if updated within last 2 seconds
        dataIsStale = false;
        break;
    }
}

// If all target data is stale (no new UDP packets), clear the time series plots
if (dataIsStale) {
    if (m_velocityTimePlot) m_velocityTimePlot->clearData();
    if (m_rangeTimePlot) m_rangeTimePlot->clearData();
    if (m_rangeRatePlot) m_rangeRatePlot->clearData();
    m_lastDataReceivedTime = 0;
    return;
}
```

### 3. Added Cleanup Timer to Main Widget

**File:** `TimeSeriesPlotsWidget.cpp` - `TimeSeriesPlotsWidget` constructor

- Added cleanup timer to periodically update time axes even when no data arrives
- Ensures UI stays responsive and time labels remain accurate

## Behavior After Fix

### When UDP Data Stops:
1. ✅ Plots detect stale data (> 2 seconds old)
2. ✅ All time series plots clear immediately
3. ✅ Only the axes and grid remain visible
4. ✅ No residual/old data points displayed

### When UDP Data Resumes:
1. ✅ Fresh data detected (< 2 seconds old)
2. ✅ Plots start displaying new data points immediately
3. ✅ Smooth transition back to normal operation

### Continuous Operation:
1. ✅ Old data points automatically removed every second
2. ✅ Time window maintained correctly
3. ✅ Memory usage kept under control
4. ✅ Time axes always show current time

## Files Modified

1. **TimeSeriesPlotsWidget.h**
   - Added destructor declaration for cleanup
   - Added `m_cleanupTimer` member to `TimeSeriesPlotWidget`
   - Added `cleanupOldData()` method declaration
   - Added `m_cleanupTimer` member to `TimeSeriesPlotsWidget`

2. **TimeSeriesPlotsWidget.cpp**
   - Implemented `TimeSeriesPlotWidget` destructor
   - Added cleanup timer initialization in constructor
   - Implemented `cleanupOldData()` method
   - Enhanced `updateFromTargets()` with stale data detection
   - Added cleanup timer to main widget constructor

## Testing Recommendations

1. **No UDP Data Test:**
   - Start application without UDP data source
   - Verify plots show only axes/grid (no data points)

2. **UDP Stop Test:**
   - Start application with UDP data
   - Wait for plots to populate
   - Stop UDP data source
   - Verify plots clear within 2-3 seconds

3. **UDP Resume Test:**
   - Continue from stop test
   - Resume UDP data
   - Verify plots immediately show new data

4. **Time Window Test:**
   - Send continuous UDP data
   - Verify old points disappear after time window expires
   - Test with different time windows (30s, 60s, 120s)

5. **Long Running Test:**
   - Run application for extended period (30+ minutes)
   - Verify memory usage remains stable
   - Verify no data accumulation issues

## Technical Details

- **Stale Data Threshold:** 2000ms (2 seconds)
- **Cleanup Timer Interval:** 1000ms (1 second)
- **Time Window:** Configurable per plot (default 60 seconds)
- **Data Point Limit:** 100,000 points per plot (MAX_DATA_POINTS)

## Benefits

1. ✅ **Accurate Display:** Shows only current, valid data
2. ✅ **Responsive UI:** Immediate feedback when data stops
3. ✅ **Memory Efficient:** Continuous cleanup prevents accumulation
4. ✅ **User Friendly:** Clear visual indication of data status
5. ✅ **Robust:** Handles intermittent UDP connections gracefully
