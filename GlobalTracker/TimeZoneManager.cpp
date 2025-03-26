#include "TimeZoneManager.h"
#include "TimeZoneUtils.h"
#include <iostream>

using namespace std;

void TimeZoneManager::printCurrentTimeInZone(const string& tzID) {
    TimeZoneUtils::printCurrentTimeInZone(tzID, useDST);
}

void TimeZoneManager::convertTimeBetweenZones() {
    TimeZoneUtils::convertTimeBetweenZones(useDST);
}

void TimeZoneManager::toggleDST(bool enable) {
    useDST = enable;
    cout << "DST handling is now " << (useDST ? "ENABLED" : "DISABLED") << ".\n";
}

bool TimeZoneManager::isDSTEnabled() const {
    return useDST;
}