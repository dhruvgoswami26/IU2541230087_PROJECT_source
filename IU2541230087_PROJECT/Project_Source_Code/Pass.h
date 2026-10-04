// Pass.h - a gate pass issued to a person (optionally linked to a vehicle)
#ifndef PASS_H
#define PASS_H

#include <string>

class Pass {
private:
    std::string passId;
    std::string personId;
    std::string vehicleNo;     // "NONE" when the pass is for a person on foot
    std::string issueDate;     // YYYY-MM-DD
    std::string expiryDate;    // YYYY-MM-DD
    std::string status;        // ACTIVE or REVOKED

public:
    Pass();
    Pass(const std::string& passId, const std::string& personId, const std::string& vehicleNo,
         const std::string& issueDate, const std::string& expiryDate, const std::string& status);
    ~Pass();

    void issue(const std::string& passId, const std::string& personId, const std::string& vehicleNo,
               const std::string& issueDt, const std::string& expiryDt);  // validates dates, sets ACTIVE
    void revoke();                                                         // sets REVOKED

    bool isActive() const;
    bool isExpired(const std::string& today) const;

    std::string getPassId() const;
    std::string getPersonId() const;
    std::string getVehicleNo() const;
    std::string getIssueDate() const;
    std::string getExpiryDate() const;
    std::string getStatus() const;
    void setExpiryDate(const std::string& newExpiry);

    void display() const;
    std::string toRecord() const;
};

#endif
