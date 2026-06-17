#ifndef TIME_H
#define TIME_H

#include <QDateTime>
#include <QList>

class TimeBlock {
private:
    QDateTime startTime;
    QDateTime endTime;

public:
    TimeBlock(const QDateTime& start, const QDateTime& end);

    QDateTime getStartTime() const;
    QDateTime getEndTime() const;

    bool overlapsWith(const TimeBlock& other) const;
    TimeBlock getIntersection(const TimeBlock& other) const;
};

class CalendarAnalyzer {
public:
    static QList<TimeBlock> findCommonSlots(
        const QList<TimeBlock>& availabilityA,
        const QList<TimeBlock>& availabilityB);
};

#endif // TIME_H