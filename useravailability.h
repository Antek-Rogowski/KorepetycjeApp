#ifndef USERAVAILABILITY_H
#define USERAVAILABILITY_H

#include <QString>
#include <QList>
#include "time.h"

class UserAvailability {
private:
    QString userId;
    QList<TimeBlock> availableBlocks;

public:
    UserAvailability(const QString& id);

    QString getUserId() const;
    void addTimeBlock(const TimeBlock& block);
    QList<TimeBlock> getAvailableBlocks() const;
};

#endif // USERAVAILABILITY_H