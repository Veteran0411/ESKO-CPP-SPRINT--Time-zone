#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>

class UserManager {
private:
    const std::string adminUsername = "admin";
    const std::string adminPassword = "admin123";
    const std::string favoritesFile = "favorites.csv";
    bool isAdminLoggedIn = false;

    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t");
        size_t end = str.find_last_not_of(" \t");

        // no position
        return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1); // substr(start index, to how many characters/index)
    }

public:
    bool login(const std::string& username, const std::string& password) {
        std::string trimmedUser = trim(username);
        std::string trimmedPass = trim(password);

        if (trimmedUser == adminUsername && trimmedPass == adminPassword) {
            isAdminLoggedIn = true;
            return true;
        }
        return false;
    }


    // to check if user is admin
    // if i dont write const it gives compile error (getter should be marked as const becouse it should not modify members)
    bool isAdmin() const {
        return isAdminLoggedIn;
    }


    // add favorite only if it is admin
    void addFavorite(const std::string& city) {
        try {
            std::ofstream file(favoritesFile, std::ios::app);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open favorites file");
            }
            file << trim(city) << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Error adding favorite: " << e.what() << std::endl;
        }
    }


    // to store all the values in vector and then print it on console
    std::vector<std::string> getFavorites() {
        std::vector<std::string> favorites;
        try {
            std::ifstream file(favoritesFile);
            if (!file.is_open()) {
                return favorites; // Return empty if file doesn't exist
            }

            std::string city;
            while (std::getline(file, city)) {
                if (!city.empty()) {
                    favorites.push_back(city);
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error reading favorites: " << e.what() << std::endl;
        }
        return favorites;
    }
};

#endif 