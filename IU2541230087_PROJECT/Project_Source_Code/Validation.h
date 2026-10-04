// Validation.h - helper functions for input validation and text/date utilities
#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <vector>

namespace Validation {

std::string trim(const std::string& s);
std::string toUpper(const std::string& s);
std::string capitalize(const std::string& s);   // "car" -> "Car"

bool isValidId(const std::string& s);           // 2-10 letters/digits
bool isValidName(const std::string& s);         // 2-40 letters, spaces or dots
bool isSafeText(const std::string& s);          // 1-40 chars, no '|' (file delimiter)
bool isValidVehicleNo(const std::string& s);    // 4-12 letters/digits
bool isValidVehicleType(const std::string& s);  // Car, Bike, Scooter, Truck, Van, Bus
bool isValidShift(const std::string& s);        // Morning, Evening, Night
bool isValidDate(const std::string& s);         // YYYY-MM-DD (real calendar date)

std::string todayDate();                        // system date as YYYY-MM-DD
std::string currentTime();                      // system time as HH:MM:SS

std::vector<std::string> split(const std::string& s, char delimiter);

}

#endif
