// Employee.cpp
#include "Employee.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>

Employee::Employee() : Person(), department("") {}

Employee::Employee(const std::string& id, const std::string& name, const std::string& dept)
    : Person(id, name, "Employee"), department(dept) {}

Employee::~Employee() {}

std::string Employee::getDepartment() const { return department; }
std::string Employee::getDetail() const { return department; }

void Employee::setDetail(const std::string& detail) {
    if (!Validation::isSafeText(detail))
        throw ValidationException("Department must be 1-40 characters and must not contain '|'");
    department = detail;
}

void Employee::display() const {
    std::cout << "[EMPLOYEE] " << getId() << " | " << getName()
              << " | Dept: " << department << "\n";
}

std::string Employee::toRecord() const {
    return Person::toRecord() + "|" + department;
}
