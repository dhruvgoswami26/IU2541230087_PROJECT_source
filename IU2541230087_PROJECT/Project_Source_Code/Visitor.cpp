// Visitor.cpp
#include "Visitor.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>

Visitor::Visitor() : Person(), purpose(""), hostName("") {}

Visitor::Visitor(const std::string& id, const std::string& name,
                 const std::string& p, const std::string& host)
    : Person(id, name, "Visitor"), purpose(p), hostName(host) {}

Visitor::~Visitor() {}

std::string Visitor::getPurpose() const { return purpose; }
std::string Visitor::getHostName() const { return hostName; }
std::string Visitor::getDetail() const { return purpose; }

void Visitor::setDetail(const std::string& detail) {
    if (!Validation::isSafeText(detail))
        throw ValidationException("Purpose must be 1-40 characters and must not contain '|'");
    purpose = detail;
}

void Visitor::display() const {
    std::cout << "[VISITOR]  " << getId() << " | " << getName()
              << " | Purpose: " << purpose << " | Host: " << hostName << "\n";
}

std::string Visitor::toRecord() const {
    return Person::toRecord() + "|" + purpose + "|" + hostName;
}
