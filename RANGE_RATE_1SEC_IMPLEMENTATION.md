# Range Rate Computation - 1-Second Interval Implementation

## Overview

This implementation enhances the logging control to compute range rate every one second by collecting high-frequency data within each second interval and calculating range rate based on the aggregated data.

## Key Features

### 1. Second-Interval Data Aggregation
- Data points received at high frequency are grouped into 1-second intervals
- Each interval is defined by truncating the timestamp to the second boundary: `(timestamp_ms / 1000) * 1000`
- All data points within each second are collected and stored together

### 2. Per-Second Range Rate Computation
- **Formula**: `Range Rate = (avgRange_current_second - avgRange_previous_second) / 1.0 second`
- Average range is computed from all data points collected within each second
- Range rate is calculated by comparing consecutive second intervals
- This approach leverages the high-frequency data to get accurate average values per second

### 3. Data Structures

#### SecondIntervalData
```cpp
struct SecondIntervalData {
    qint64 intervalStart;         // Start timestamp of the 1-second interval
    QVector<LoggedTrackDataPoint> points;  // All points in this second
    float computedRangeRate;      // Computed range rate (m/s)
    float avgRange;               // Average range in this interval (m)
    float avgSpeed;               // Average speed in this interval (m/s)
};
```

#### TrackLogData Enhancement
```cpp
struct TrackLogData {
    // ... existing fields ...
    QMap<qint64, SecondIntervalData> secondIntervals;  // Maps second timestamp to data
    qint64 currentSecond;  // Current second being accumulated
};
```

## Implementation Details

### Data Collection Process

1. **Real-time Logging** (`logTrackData`):
   - When target data arrives, determine current second boundary
   - Check if we've transitioned to a new second
   - If new second detected, process the completed interval
   - Add data point to both raw data list and current second interval

2. **Interval Processing** (`processSecondIntervalData`):
   - Calculate average range and speed from all points in the interval
   - Find previous second interval for comparison
   - Compute range rate using finite difference method
   - Update all data points in the interval with computed range rate
   - Log detailed information about the computation

3. **Range Rate Calculation** (`computeRangeRateFromInterval`):
   - Primary method: `(currentAvgRange - previousAvgRange) / deltaTime`
   - Fallback: Average of speeds from both intervals if time difference is invalid
   - Handles edge cases when no previous interval exists

### Status Display Enhancements

- Status label shows: `"Status: Logging (X tracks, Y data points, Z second intervals)"`
- Algorithm output displays:
  - Second timestamp
  - Number of points collected in that second
  - Average range computed
  - Computed range rate value

### Example Output

```
Track 1: Second 14:32:15 - Collected 47 points, Avg Range: 125.34 m, Computed Range Rate: -2.45 m/s
Track 1: Second 14:32:16 - Collected 51 points, Avg Range: 122.89 m, Computed Range Rate: -2.45 m/s
Track 2: Second 14:32:15 - Collected 52 points, Avg Range: 238.67 m, Computed Range Rate: 5.23 m/s
```

## Benefits

1. **High-Frequency Data Utilization**: Makes full use of data received at high frequency by averaging all points within each second
2. **Stable Measurements**: Reduces noise by computing average values before calculating range rate
3. **Accurate Time Intervals**: Always uses exactly 1-second intervals for consistent measurements
4. **Detailed Logging**: Provides visibility into how many data points contribute to each computation
5. **Backward Compatible**: Existing range rate computation methods remain available

## Usage

1. **Start Logging**: Click "Start Logging" button to begin data collection
2. **Automatic Processing**: As data arrives, second intervals are automatically processed
3. **View Results**: 
   - Check "Algorithm Output" panel for detailed per-second computations
   - Status bar shows total number of second intervals processed
   - Plots and data table display computed range rates
4. **Manual Recomputation**: Click "Compute Range Rate" button to reprocess all intervals

## Technical Notes

- Minimum data requirement: At least 2 consecutive second intervals needed for range rate computation
- First second of each track uses radial speed as fallback since no previous interval exists
- Incomplete seconds are processed when logging stops
- Second intervals are stored efficiently using QMap with timestamp keys
- Memory management: Old second intervals can be cleared if needed (future enhancement)

## Future Enhancements

1. Configurable interval duration (e.g., 0.5s, 2s)
2. Export per-second interval statistics to CSV
3. Visualization of points-per-second distribution
4. Adaptive smoothing based on data point density
5. Memory optimization for long-running sessions
