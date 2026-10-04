// Visitor.h - a Person visiting the premises (derived class)
#ifndef VISITOR_H
#define VISITOR_H

#include "Person.h"

class Visitor : public Person {       // Inheritance
private:
    std::string purpose;
    std::string hostName;

public:
    Visitor();
    Visitor(const std::string& id, const std::string& name,
            const std::string& purpose, const std::string& hostName);
    ~Visitor();

    std::string getPurpose() const;
    std::string getHostName() const;

    std::string getDetail() const override;
    void setDetail(const std::string& detail) override;
    void display() const override;
    std::string toRecord() const override;
};

#endif
