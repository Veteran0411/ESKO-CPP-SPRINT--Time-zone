#include "ManualDST.h"
#include <iostream>

using namespace std;

void ManualDST::adjustTime() {
    int hours;
    cout << "Enter the number of hours to adjust for DST (e.g., 1 for forward, -1 for backward): ";
    cin >> hours;

    if (cin.fail()) {
        cerr << "Invalid input! Please enter a number.\n";
        cin.clear();
        return;
    }

    cout << "Time adjusted by " << hours << " hour(s) for DST.\n";
}