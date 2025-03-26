#include <iostream>
#include <vector>
#include "TimeZoneManager.h"
#include "UserManager.h"

using namespace std;

void displayMenu(const TimeZoneManager& manager, const UserManager& userManager) {
    cout << "\n=========== Global Time Zone Application ===========\n";
    if (userManager.isAdmin()) {
        cout << "ADMIN MODE\n";
    }
    cout << "1. View current time in different time zones\n";
    cout << "2. Convert time between different time zones\n";
    cout << "3. Toggle DST handling (Currently: " << (manager.isDSTEnabled() ? "ON" : "OFF") << ")\n";
    cout << "4. Add hotspot city (Admin only)\n";
    cout << "5. View hotspot cities\n";
    cout << "6. Admin login\n";
    cout << "7. Exit\n";
    cout << "====================================================\n";
    cout << "Enter your choice: ";
}

void displayTimeZoneMenu() {
    cout << "\nSelect a time zone:\n";
    cout << "1. America/New_York\n";
    cout << "2. Europe/London\n";
    cout << "3. Asia/Kolkata\n";
    cout << "4. Asia/Tokyo\n";
    cout << "5. Australia/Sydney\n";
    cout << "Enter your choice: ";
}

void adminLogin(UserManager& userManager) {
    string username, password;
    cout << "Admin username: ";
    getline(cin, username);
    cout << "Admin password: ";
    getline(cin, password);

    if (userManager.login(username, password)) {
        cout << "Admin login successful!\n";
    }
    else {
        cout << "Invalid admin credentials\n";
    }
}

int main() {
    TimeZoneManager manager;
    UserManager userManager;
    int choice;

    while (true) {
        displayMenu(manager, userManager);
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (cin.fail()) {
            cerr << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1: {
            displayTimeZoneMenu();
            int tzChoice;
            cin >> tzChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string tzID;
            switch (tzChoice) {
            case 1: tzID = "America/New_York"; break;
            case 2: tzID = "Europe/London"; break;
            case 3: tzID = "Asia/Kolkata"; break;
            case 4: tzID = "Asia/Tokyo"; break;
            case 5: tzID = "Australia/Sydney"; break;
            default: cout << "Invalid choice!\n"; continue;
            }
            manager.printCurrentTimeInZone(tzID);
            break;
        }
        case 2:
            manager.convertTimeBetweenZones();
            break;
        case 3: {
            bool enableDST;
            cout << "Enable DST handling? (1 for YES, 0 for NO): ";
            cin >> enableDST;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            manager.toggleDST(enableDST);
            break;
        }
        case 4:
            if (!userManager.isAdmin()) {
                cout << "Admin privileges required!\n";
                break;
            }
            {
                string city;
                cout << "Enter city/timezone to add to hotspot: ";
                getline(cin, city);
                userManager.addFavorite(city);
                cout << "Added to hotspot.\n";
            }
            break;
        case 5: {
            vector<string> favorites = userManager.getFavorites();
            if (favorites.empty()) {
                cout << "No hotspot cities available.\n";
            }
            else {
                cout << "hotspot cities:\n";
                for (const auto& city : favorites) {
                    manager.printCurrentTimeInZone(city);
                }
            }
            break;
        }
        case 6:
            adminLogin(userManager);
            break;
        case 7:
            cout << "Exiting program...\n";
            return 0;
        default:
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}