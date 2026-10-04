// EntryRecord.cpp
#include "EntryRecord.h"
#include <iostream>

EntryRecord::EntryRecord() : logId(""), passId(""), staffId(""), direction(""), date(""), time("") {}

EntryRecord::EntryRecord(const std::string& l, const std::string& p, const std::string& s,
                         const std::string& d, const std::string& dt, const std::string& t)
    : logId(l), passId(p), staffId(s), direction(d), date(dt), time(t) {}

EntryRecord::~EntryRecord() {}

std::string EntryRecord::getLogId() const { return logId; }
std::string EntryRecord::getPassId() const { return passId; }
std::string EntryRecord::getStaffId() const { return staffId; }
std::string EntryRecord::getDirection() const { return direction; }
std::string EntryRecord::getDate() const { return date; }
std::string EntryRecord::getTime() const { return time; }

void EntryRecord::display() const {
    std::cout << "[LOG]      " << logId << " | Pass: " << passId << " | " << direction
              << " | " << date << " " << time << " | Guard: " << staffId << "\n";
}

std::string EntryRecord::toRecord() const {
    return logId + "|" + passId + "|" + staffId + "|" + direction + "|" + date + "|" + time;
}
