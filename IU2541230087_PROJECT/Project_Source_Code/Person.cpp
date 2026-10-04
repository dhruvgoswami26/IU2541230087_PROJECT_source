// Person.cpp
#include "Person.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>

Person::Person() : personId(""), name(""), type("Person") {}

Person::Person(const std::string& id, const std::string& n, const std::string& t)
    : personId(id), name(n), type(t) {}

Person::~Person() {}   // nothing to free here; virtual so derived destructors run too

std::string Person::getId() const { return personId; }
std::string Person::getName() const { return name; }
std::string Person::getType() const { return type; }

void Person::setName(const std::string& newName) {
    if (!Validation::isValidName(newName))
        throw ValidationException("Name must be 2-40 characters (letters, spaces, dots only)");
    name = newName;
}

std::string Person::getDetail() const { return "-"; }

void Person::setDetail(const std::string&) {
    throw ValidationException("This type of person has no detail field");
}

void Person::display() const {
    std::cout << "[PERSON]   " << personId << " | " << name << "\n";
}

std::string Person::toRecord() const {
    return type + "|" + personId + "|" + name;
}
