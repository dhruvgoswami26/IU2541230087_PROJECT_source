// Pass.cpp
#include "Pass.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>

Pass::Pass() : passId(""), personId(""), vehicleNo("NONE"), issueDate(""), expiryDate(""), status("ACTIVE") {}

Pass::Pass(const std::string& id, const std::string& pId, const std::string& vNo,
           const std::string& issueDt, const std::string& expiryDt, const std::string& st)
    : passId(id), personId(pId), vehicleNo(vNo), issueDate(issueDt), expiryDate(expiryDt), status(st) {}

Pass::~Pass() {}

void Pass::issue(const std::string& id, const std::string& pId, const std::string& vNo,
                 const std::string& issueDt, const std::string& expiryDt) {
    if (!Validation::isValidDate(issueDt))
        throw ValidationException("Issue date must be a valid date in YYYY-MM-DD format");
    if (!Validation::isValidDate(expiryDt))
        throw ValidationException("Expiry date must be a valid date in YYYY-MM-DD format");
    if (expiryDt < issueDt)                       // YYYY-MM-DD strings compare correctly
        throw ValidationException("Expiry date cannot be before the issue date");
    passId = id;
    personId = pId;
    vehicleNo = vNo;
    issueDate = issueDt;
    expiryDate = expiryDt;
    status = "ACTIVE";
}

void Pass::revoke() {
    if (status == "REVOKED")
        throw PassException("Pass " + passId + " is already revoked");
    status = "REVOKED";
}

bool Pass::isActive() const { return status == "ACTIVE"; }
bool Pass::isExpired(const std::string& today) const { return expiryDate < today; }

std::string Pass::getPassId() const { return passId; }
std::string Pass::getPersonId() const { return personId; }
std::string Pass::getVehicleNo() const { return vehicleNo; }
std::string Pass::getIssueDate() const { return issueDate; }
std::string Pass::getExpiryDate() const { return expiryDate; }
std::string Pass::getStatus() const { return status; }

void Pass::setExpiryDate(const std::string& newExpiry) {
    if (!Validation::isValidDate(newExpiry))
        throw ValidationException("Expiry date must be a valid date in YYYY-MM-DD format");
    if (newExpiry < issueDate)
        throw ValidationException("Expiry date cannot be before the issue date");
    expiryDate = newExpiry;
}

void Pass::display() const {
    std::cout << "[PASS]     " << passId << " | Person: " << personId
              << " | Vehicle: " << vehicleNo
              << " | Issued: " << issueDate << " | Expires: " << expiryDate
              << " | Status: " << status;
    if (isActive() && isExpired(Validation::todayDate())) std::cout << " (EXPIRED)";
    std::cout << "\n";
}

std::string Pass::toRecord() const {
    return passId + "|" + personId + "|" + vehicleNo + "|" + issueDate + "|" + expiryDate + "|" + status;
}
