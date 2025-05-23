#include "TimeZoneUtils.h"
#include <unicode/timezone.h>
#include <unicode/calendar.h>
#include <unicode/unistr.h>
#include <unicode/smpdtfmt.h>
#include <unicode/locid.h>
#include <iostream>
#include <ctime>
#include <stdexcept>
#include <memory>
#include <limits>

using namespace std;

void TimeZoneUtils::printCurrentTimeInZone(const string& tzID, bool useDST) {
    UErrorCode status = U_ZERO_ERROR;

    try {
        time_t now = time(nullptr); // ctime library , returns seconds elapsed from 1970 (datatype)
        if (now == -1) {
            throw runtime_error("Failed to get current time");
        }

        UDate nowMillis = static_cast<UDate>(now) * 1000.0;  // type double convert seconds to milliseconds

        // Create timezone (let ICU handle DST by default)

        // unique pointer also called as smart pointer , amutomatically deletes the pointer when its out of scope

        //the timezone returns the dynamically allocated object so pointer is required to manage memory manually so it returns address of memory
        unique_ptr<icu::TimeZone> tz(icu::TimeZone::createTimeZone(icu::UnicodeString::fromUTF8(tzID)));
        icu::UnicodeString tzIDCheck;
        tz->getID(tzIDCheck); // to check the valid region in timezone database

        if (tzIDCheck == icu::UnicodeString("GMT") && tzID != "GMT") {
            throw runtime_error("Invalid timezone: " + tzID);
        }

        if (!useDST) {
            // creating manual dst using raw offset
            int32_t rawOffset = tz->getRawOffset();
            icu::UnicodeString noDstID = icu::UnicodeString("GMT") +
                (rawOffset >= 0 ? "+" : "") +
                to_string(rawOffset / 3600000).c_str();
            unique_ptr<icu::TimeZone> noDstTz(icu::TimeZone::createTimeZone(noDstID));
            tz = move(noDstTz);
        }

        // Create calendar and set time
        unique_ptr<icu::Calendar> calendar(icu::Calendar::createInstance(*tz, status));
        if (U_FAILURE(status)) {
            throw runtime_error("Calendar creation failed: " + string(u_errorName(status)));
        }
        calendar->setTime(nowMillis, status);

        // Format the time
        icu::SimpleDateFormat formatter(
            icu::UnicodeString("yyyy-MM-dd HH:mm:ss z"),
            icu::Locale::getDefault(),
            status
        );
        formatter.setTimeZone(*tz);

        icu::UnicodeString formattedTime;
        formatter.format(nowMillis, formattedTime);

        string timeStr;
        formattedTime.toUTF8String(timeStr);


        cout << "\n" << endl;
        cout << "Time in " << tzID << ": " << timeStr<<endl;
        int convertedYear = calendar->get(UCAL_YEAR, status);
        int convertedMonth = calendar->get(UCAL_MONTH, status) + 1;
        int convertedDay = calendar->get(UCAL_DATE, status);
        int convertedHour = calendar->get(UCAL_HOUR_OF_DAY, status);
        int convertedMinute = calendar->get(UCAL_MINUTE, status);

        
        cout << "Year: " << convertedYear << endl;
        cout << "Month: " << convertedMonth << endl;
        cout << "Day: " << convertedDay << endl;
        cout << "Hour: " << convertedHour << endl;
        cout << "Minute: " << convertedMinute << endl;
     
        if (useDST && tz->inDaylightTime(nowMillis, status)) {
            cout << " (DST)";
        }
        cout << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void TimeZoneUtils::convertTimeBetweenZones(bool useDST) {
    UErrorCode status = U_ZERO_ERROR;

    try {
        string fromTZ, toTZ;
        int year, month, day, hour, minute;

        cout << "Enter source timezone (e.g., America/New_York): ";
        getline(cin, fromTZ);

        cout << "Enter target timezone (e.g., Europe/London): ";
        getline(cin, toTZ);

        cout << "Enter date (YYYY MM DD HH MM): ";
        if (!(cin >> year >> month >> day >> hour >> minute)) {
            throw runtime_error("Invalid date input");
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Create timezones (let ICU handle DST by default)
        unique_ptr<icu::TimeZone> fromZone(icu::TimeZone::createTimeZone(icu::UnicodeString::fromUTF8(fromTZ)));
        unique_ptr<icu::TimeZone> toZone(icu::TimeZone::createTimeZone(icu::UnicodeString::fromUTF8(toTZ)));

        // Validate timezones
        icu::UnicodeString fromZoneID;
        fromZone->getID(fromZoneID);
        if (fromZoneID == icu::UnicodeString("GMT") && fromTZ != "GMT") {
            throw runtime_error("Invalid source timezone: " + fromTZ);
        }

        icu::UnicodeString toZoneID;
        toZone->getID(toZoneID);
        if (toZoneID == icu::UnicodeString("GMT") && toTZ != "GMT") {
            throw runtime_error("Invalid target timezone: " + toTZ);
        }

        if (!useDST) {
            // Override with raw offsets (ignore DST)
            int32_t fromRawOffset = fromZone->getRawOffset();
            icu::UnicodeString noDstFromID = icu::UnicodeString("GMT") +
                (fromRawOffset >= 0 ? "+" : "") +
                to_string(fromRawOffset / 3600000).c_str();
            fromZone.reset(icu::TimeZone::createTimeZone(noDstFromID));

            int32_t toRawOffset = toZone->getRawOffset();
            icu::UnicodeString noDstToID = icu::UnicodeString("GMT") +
                (toRawOffset >= 0 ? "+" : "") +
                to_string(toRawOffset / 3600000).c_str();
            toZone.reset(icu::TimeZone::createTimeZone(noDstToID));
        }

        // Create and configure source calendar
        unique_ptr<icu::Calendar> calendar(icu::Calendar::createInstance(*fromZone, status));
        if (U_FAILURE(status)) {
            throw runtime_error("Calendar creation failed: " + string(u_errorName(status)));
        }

        calendar->clear();
        calendar->set(UCAL_YEAR, year);
        calendar->set(UCAL_MONTH, month - 1);  // 0-based month
        calendar->set(UCAL_DATE, day);
        calendar->set(UCAL_HOUR_OF_DAY, hour);
        calendar->set(UCAL_MINUTE, minute);
        calendar->set(UCAL_SECOND, 0);
        calendar->set(UCAL_MILLISECOND, 0);

        UDate sourceTime = calendar->getTime(status); // stores the ms from 1970 to specified time in sourceTime
        if (U_FAILURE(status)) {
            throw runtime_error("Failed to get source time");
        }

        // Convert to target timezone
        unique_ptr<icu::Calendar> targetCalendar(icu::Calendar::createInstance(*toZone, status));
        targetCalendar->setTime(sourceTime, status);

        // Format the result ( parses the date and time to unicode string )
        icu::SimpleDateFormat formatter(
            icu::UnicodeString("yyyy-MM-dd HH:mm:ss z"),
            icu::Locale::getDefault(),
            status
        );
        formatter.setTimeZone(*toZone);


        // unicode string to human readable format using utf 8
        icu::UnicodeString convertedTime;
        formatter.format(targetCalendar->getTime(status), convertedTime);

        string timeStr;
        convertedTime.toUTF8String(timeStr);
        cout << "\nConverted Time: " << timeStr<<endl;

        int convertedYear = targetCalendar->get(UCAL_YEAR, status);
        int convertedMonth = targetCalendar->get(UCAL_MONTH, status) + 1;
        int convertedDay = targetCalendar->get(UCAL_DATE, status);
        int convertedHour = targetCalendar->get(UCAL_HOUR_OF_DAY, status);
        int convertedMinute = targetCalendar->get(UCAL_MINUTE, status);

        cout << "Year: " << convertedYear << endl;
        cout << "Month: " << convertedMonth << endl;
        cout << "Day: " << convertedDay << endl;
        cout << "Hour: " << convertedHour << endl;
        cout << "Minute: " << convertedMinute << endl;

        if (useDST && toZone->inDaylightTime(targetCalendar->getTime(status), status)) {
            cout << " (DST)";
        }
        cout << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}