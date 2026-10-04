// Vehicle.cpp
#include "Vehicle.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>

Vehicle::Vehicle() : vehicleNo(""), type(""), ownerId("") {}

Vehicle::Vehicle(const std::string& no, const std::string& t, const std::string& owner)
    : vehicleNo(no), type(t), ownerId(owner) {}

Vehicle::~Vehicle() {}

std::string Vehicle::getVehicleNo() const { return vehicleNo; }
std::string Vehicle::getType() const { return type; }
std::string Vehicle::getOwnerId() const { return ownerId; }

void Vehicle::setType(const std::string& newType) {
    if (!Validation::isValidVehicleType(newType))
        throw ValidationException("Vehicle type must be Car, Bike, Scooter, Truck, Van or Bus");
    type = Validation::capitalize(newType);
}

void Vehicle::setOwnerId(const std::string& newOwnerId) { ownerId = newOwnerId; }

void Vehicle::display() const {
    std::cout << "[VEHICLE]  " << vehicleNo << " | Type: " << type
              << " | Owner: " << ownerId << "\n";
}

std::string Vehicle::toRecord() const {
    return vehicleNo + "|" + type + "|" + ownerId;
}
