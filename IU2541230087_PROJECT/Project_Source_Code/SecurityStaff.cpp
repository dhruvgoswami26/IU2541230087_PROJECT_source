// SecurityStaff.cpp
#include "SecurityStaff.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>

SecurityStaff::SecurityStaff() : staffId(""), name(""), shift("") {}

SecurityStaff::SecurityStaff(const std::string& id, const std::string& n, const std::string& s)
    : staffId(id), name(n), shift(s) {}

SecurityStaff::~SecurityStaff() {}

std::string SecurityStaff::getStaffId() const { return staffId; }
std::string SecurityStaff::getName() const { return name; }
std::string SecurityStaff::getShift() const { return shift; }

void SecurityStaff::setShift(const std::string& newShift) {
    if (!Validation::isValidShift(newShift))
        throw ValidationException("Shift must be Morning, Evening or Night");
    shift = Validation::capitalize(newShift);
}

void SecurityStaff::display() const {
    std::cout << "[STAFF]    " << staffId << " | " << name << " | Shift: " << shift << "\n";
}

// A guard checks: not revoked, already started, not expired.
bool SecurityStaff::verifyPass(const Pass& pass, const std::string& today) const {
    if (!pass.isActive())
        throw PassException("pass " + pass.getPassId() + " has been revoked");
    if (pass.getIssueDate() > today)
        throw PassException("pass " + pass.getPassId() + " is not valid until " + pass.getIssueDate());
    if (pass.isExpired(today))
        throw PassException("pass " + pass.getPassId() + " expired on " + pass.getExpiryDate());
    return true;
}

std::string SecurityStaff::toRecord() const {
    return staffId + "|" + name + "|" + shift;
}
