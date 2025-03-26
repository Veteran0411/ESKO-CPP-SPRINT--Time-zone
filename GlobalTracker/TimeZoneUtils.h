#ifndef TIMEZONEUTILS_H
#define TIMEZONEUTILS_H

#include <string>

class TimeZoneUtils {
public:
    static void printCurrentTimeInZone(const std::string& tzID, bool useDST);
    static void convertTimeBetweenZones(bool useDST);
};

#endif 