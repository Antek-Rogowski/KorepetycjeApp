#include "time.h"
#include <algorithm>

TimeBlock::TimeBlock(const QDateTime& start, const QDateTime& end)
    : startTime(start), endTime(end) {}

QDateTime TimeBlock::getStartTime() const {
    return startTime;
}

QDateTime TimeBlock::getEndTime() const {
    return endTime;
}

bool TimeBlock::overlapsWith(const TimeBlock& other) const {
    return std::max(startTime, other.getStartTime()) < std::min(endTime, other.getEndTime());
}

TimeBlock TimeBlock::getIntersection(const TimeBlock& other) const {
    QDateTime newStart = std::max(startTime, other.getStartTime());
    QDateTime newEnd = std::min(endTime, other.getEndTime());
    return TimeBlock(newStart, newEnd);
}

QList<TimeBlock> CalendarAnalyzer::findCommonSlots(
    const QList<TimeBlock>& availabilityA,
    const QList<TimeBlock>& availabilityB) {

    QList<TimeBlock> commonSlots;

    for (const auto& blockA : availabilityA) {
        for (const auto& blockB : availabilityB) {
            if (blockA.overlapsWith(blockB)) {
                commonSlots.append(blockA.getIntersection(blockB));
            }
        }
    }
    return commonSlots;
}