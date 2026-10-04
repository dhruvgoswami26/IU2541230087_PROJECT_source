// Person.h - base class for everyone who can hold a gate pass
#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
private:                         // Encapsulation: data is private, accessed via functions
    std::string personId;
    std::string name;
    std::string type;

public:
    Person();                                                        // default constructor
    Person(const std::string& id, const std::string& name, const std::string& type); // parameterized
    virtual ~Person();                                               // virtual destructor (needed for delete via Person*)

    std::string getId() const;
    std::string getName() const;
    std::string getType() const;
    void setName(const std::string& newName);                        // validates the name

    virtual std::string getDetail() const;                           // department / purpose
    virtual void setDetail(const std::string& detail);
    virtual void display() const;                                    // overridden in derived classes (runtime polymorphism)
    virtual std::string toRecord() const;                            // one line for the data file
};

#endif
