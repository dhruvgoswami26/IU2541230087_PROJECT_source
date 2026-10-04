// SecurityStaff.h - a guard who verifies passes at the gate
#ifndef SECURITYSTAFF_H
#define SECURITYSTAFF_H

#include <string>
#include "Pass.h"

class SecurityStaff {
private:
    std::string staffId;
    std::string name;
    std::string shift;         // Morning, Evening or Night

public:
    SecurityStaff();
    SecurityStaff(const std::string& staffId, const std::string& name, const std::string& shift);
    ~SecurityStaff();

    std::string getStaffId() const;
    std::string getName() const;
    std::string getShift() const;
    void setShift(const std::string& newShift);        // validates the shift

    void display() const;
    bool verifyPass(const Pass& pass, const std::string& today) const;  // throws PassException if not valid
    std::string toRecord() const;
};

#endif
