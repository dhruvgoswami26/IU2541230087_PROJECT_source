// GateSystem.h - the core class: owns all records in dynamically allocated arrays
#ifndef GATESYSTEM_H
#define GATESYSTEM_H

#include <string>
#include "Person.h"
#include "Vehicle.h"
#include "Pass.h"
#include "SecurityStaff.h"
#include "EntryRecord.h"

class GateSystem {
private:
    // Dynamic memory: arrays are created with new[] and grow (double) when full
    Person**       persons;       // array of Person pointers -> Employee / Visitor objects (polymorphism)
    Vehicle*       vehicles;
    SecurityStaff* staffList;
    Pass*          passes;
    EntryRecord*   logs;

    int personCount,  personCap;
    int vehicleCount, vehicleCap;
    int staffCount,   staffCap;
    int passCount,    passCap;
    int logCount,     logCap;
    int nextPassNo, nextLogNo;    // used to auto-generate P001, P002 ... and L001, L002 ...

    // lookups: return index or -1
    int findPerson(const std::string& id) const;
    int findVehicle(const std::string& vehicleNo) const;
    int findStaff(const std::string& id) const;
    int findPass(const std::string& passId) const;

    void checkNewPerson(const std::string& id, const std::string& name) const;
    void ensurePersonSpace();
    void ensureVehicleSpace();
    void ensureStaffSpace();
    void ensurePassSpace();
    void ensureLogSpace();

public:
    GateSystem();                       // default capacity
    explicit GateSystem(int initialCapacity);   // parameterized
    ~GateSystem();                      // releases every array with delete[]
    GateSystem(const GateSystem&) = delete;             // owns raw memory -> no copying
    GateSystem& operator=(const GateSystem&) = delete;

    void clearAll();                    // remove all records (arrays are kept)

    // ----- Add record -----
    void addEmployee(const std::string& id, const std::string& name, const std::string& department);
    void addVisitor(const std::string& id, const std::string& name,
                    const std::string& purpose, const std::string& hostName);
    void addVehicle(const std::string& vehicleNo, const std::string& type, const std::string& ownerId);
    void addStaff(const std::string& id, const std::string& name, const std::string& shift);
    void addPassRecord(const Pass& pass);        // used when loading from file
    void addLog(const EntryRecord& record);      // used when loading from file

    // ----- Display -----
    void displayPersons() const;
    void displayVehicles() const;
    void displayStaff() const;
    void displayPasses() const;
    void displayLogs() const;

    // ----- Search -----
    const Person*        searchPerson(const std::string& id) const;
    int                  searchPersonsByName(const std::string& part) const;   // prints matches, returns count
    const Vehicle&       searchVehicle(const std::string& vehicleNo) const;
    const Pass&          searchPass(const std::string& passId) const;
    int                  searchPassesByPerson(const std::string& personId) const;

    // ----- Update -----
    void updatePersonName(const std::string& id, const std::string& newName);
    void updatePersonDetail(const std::string& id, const std::string& newDetail);
    void updateVehicleOwner(const std::string& vehicleNo, const std::string& newOwnerId);
    void updateVehicleType(const std::string& vehicleNo, const std::string& newType);
    void updateStaffShift(const std::string& id, const std::string& newShift);
    void updatePassExpiry(const std::string& passId, const std::string& newExpiry);

    // ----- Delete -----
    void deletePerson(const std::string& id);
    void deleteVehicle(const std::string& vehicleNo);
    void deleteStaff(const std::string& id);
    void deletePass(const std::string& passId);

    // ----- Main transaction -----
    Pass        issuePass(const std::string& personId, const std::string& vehicleNo,
                          const std::string& issueDate, const std::string& expiryDate);
    bool        verifyPass(const std::string& passId, const std::string& staffId) const;
    EntryRecord recordEntry(const std::string& passId, const std::string& staffId,
                            const std::string& direction);
    void        revokePass(const std::string& passId);
    bool        isInside(const std::string& passId) const;

    // ----- Report -----
    void generateReport() const;

    // ----- Read-only access used by FileManager -----
    int getPersonCount() const;
    int getVehicleCount() const;
    int getStaffCount() const;
    int getPassCount() const;
    int getLogCount() const;
    const Person&        getPersonAt(int index) const;
    const Vehicle&       getVehicleAt(int index) const;
    const SecurityStaff& getStaffAt(int index) const;
    const Pass&          getPassAt(int index) const;
    const EntryRecord&   getLogAt(int index) const;
};

#endif
