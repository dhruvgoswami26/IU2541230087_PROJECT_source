// Vehicle.h - a vehicle registered to a person
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle {
private:
    std::string vehicleNo;
    std::string type;
    std::string ownerId;

public:
    Vehicle();
    Vehicle(const std::string& vehicleNo, const std::string& type, const std::string& ownerId);
    ~Vehicle();

    std::string getVehicleNo() const;
    std::string getType() const;
    std::string getOwnerId() const;
    void setType(const std::string& newType);       // validates the type
    void setOwnerId(const std::string& newOwnerId);

    void display() const;
    std::string toRecord() const;
};

#endif
