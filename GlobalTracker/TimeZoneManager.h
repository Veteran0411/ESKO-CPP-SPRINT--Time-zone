#ifndef TIMEZONEMANAGER_H
#define TIMEZONEMANAGER_H

#include <string>

class TimeZoneManager {
private:
    bool useDST;

public:
    // this is member initializer better than constructor initialization
    TimeZoneManager() : useDST(true) {}

    void printCurrentTimeInZone(const std::string& tzID);
    void convertTimeBetweenZones();
    void toggleDST(bool enable);
    bool isDSTEnabled() const;
};

#endif 