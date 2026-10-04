// Employee.h - a Person who works at the premises (derived class)
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Person.h"

class Employee : public Person {      // Inheritance
private:
    std::string department;

public:
    Employee();
    Employee(const std::string& id, const std::string& name, const std::string& department);
    ~Employee();

    std::string getDepartment() const;

    std::string getDetail() const override;
    void setDetail(const std::string& detail) override;
    void display() const override;
    std::string toRecord() const override;
};

#endif
