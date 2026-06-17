#include "useravailability.h"

UserAvailability::UserAvailability(const QString& id) : userId(id) {}

QString UserAvailability::getUserId() const {
    return userId;
}

void UserAvailability::addTimeBlock(const TimeBlock& block) {
    availableBlocks.append(block);
}

QList<TimeBlock> UserAvailability::getAvailableBlocks() const {
    return availableBlocks;
}