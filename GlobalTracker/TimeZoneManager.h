#ifndef TIMEZONEMANAGER_H
#define TIMEZONEMANAGER_H

#include <string>

class TimeZoneManager {
private:
    bool useDST;

public:
    // this is member initializer better than constructor initialization
    // it is way to initialze class before constructor is executed (it helps in direct initializayion without extra initialization)
    TimeZoneManager() : useDST(true) {}

    void printCurrentTimeInZone(const std::string& tzID);
    void convertTimeBetweenZones();
    void toggleDST(bool enable);

    // getter should be marked const because it should not modify other members
    bool isDSTEnabled() const;
};

#endif 