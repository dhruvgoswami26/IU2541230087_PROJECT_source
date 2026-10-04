// Validation.cpp
#include "Validation.h"
#include <algorithm>
#include <cctype>
#include <ctime>

namespace Validation {

std::string trim(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) b++;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) e--;
    return s.substr(b, e - b);
}

std::string toUpper(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return r;
}

std::string capitalize(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (!r.empty()) r[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(r[0])));
    return r;
}

static bool allAlnum(const std::string& s) {
    for (char c : s)
        if (!std::isalnum(static_cast<unsigned char>(c))) return false;
    return true;
}

bool isValidId(const std::string& s) {
    return s.size() >= 2 && s.size() <= 10 && allAlnum(s);
}

bool isValidName(const std::string& s) {
    if (s.size() < 2 || s.size() > 40) return false;
    bool hasLetter = false;
    for (char c : s) {
        unsigned char u = static_cast<unsigned char>(c);
        if (std::isalpha(u)) hasLetter = true;
        else if (c != ' ' && c != '.') return false;
    }
    return hasLetter;
}

bool isSafeText(const std::string& s) {
    if (s.empty() || s.size() > 40) return false;
    for (char c : s)
        if (c == '|' || std::iscntrl(static_cast<unsigned char>(c))) return false;
    return true;
}

bool isValidVehicleNo(const std::string& s) {
    return s.size() >= 4 && s.size() <= 12 && allAlnum(s);
}

bool isValidVehicleType(const std::string& s) {
    std::string t = capitalize(s);
    return t == "Car" || t == "Bike" || t == "Scooter" || t == "Truck" || t == "Van" || t == "Bus";
}

bool isValidShift(const std::string& s) {
    std::string t = capitalize(s);
    return t == "Morning" || t == "Evening" || t == "Night";
}

bool isValidDate(const std::string& s) {
    if (s.size() != 10 || s[4] != '-' || s[7] != '-') return false;
    for (size_t i = 0; i < s.size(); i++) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    int y = std::stoi(s.substr(0, 4));
    int m = std::stoi(s.substr(5, 2));
    int d = std::stoi(s.substr(8, 2));
    if (y < 2000 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    int maxDay = days[m - 1] + ((m == 2 && leap) ? 1 : 0);
    return d <= maxDay;
}

std::string todayDate() {
    std::time_t t = std::time(nullptr);
    std::tm* lt = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
    return std::string(buf);
}

std::string currentTime() {
    std::time_t t = std::time(nullptr);
    std::tm* lt = std::localtime(&t);
    char buf[16];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", lt);
    return std::string(buf);
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> parts;
    std::string current;
    for (char c : s) {
        if (c == delimiter) {
            parts.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    parts.push_back(current);
    return parts;
}

}
