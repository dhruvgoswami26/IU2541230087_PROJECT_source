// GateSystem.cpp
#include "GateSystem.h"
#include "Employee.h"
#include "Visitor.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

namespace {

string normId(const string& s) { return Validation::toUpper(Validation::trim(s)); }

string makeId(char prefix, int number) {
    ostringstream os;
    os << prefix << setw(3) << setfill('0') << number;
    return os.str();
}

// "P012" -> 12 (0 if the id has no numeric part)
int numericPart(const string& id) {
    if (id.size() < 2) return 0;
    try { return stoi(id.substr(1)); } catch (...) { return 0; }
}

}

// Doubles the capacity of a dynamic array: allocate bigger block, copy, free the old one.
template <typename T>
static void growArray(T*& arr, int count, int& cap) {
    int newCap = cap * 2;
    T* bigger = new T[newCap];
    for (int i = 0; i < count; i++) bigger[i] = arr[i];
    delete[] arr;
    arr = bigger;
    cap = newCap;
}

// ===================== Construction / destruction =====================

GateSystem::GateSystem() : GateSystem(4) {}

GateSystem::GateSystem(int initialCapacity)
    : persons(nullptr), vehicles(nullptr), staffList(nullptr), passes(nullptr), logs(nullptr),
      personCount(0), vehicleCount(0), staffCount(0), passCount(0), logCount(0),
      nextPassNo(1), nextLogNo(1) {
    if (initialCapacity < 1) initialCapacity = 1;
    personCap = vehicleCap = staffCap = passCap = logCap = initialCapacity;
    persons   = new Person*[initialCapacity];
    vehicles  = new Vehicle[initialCapacity];
    staffList = new SecurityStaff[initialCapacity];
    passes    = new Pass[initialCapacity];
    logs      = new EntryRecord[initialCapacity];
}

GateSystem::~GateSystem() {
    clearAll();                 // deletes the Employee/Visitor objects the pointers refer to
    delete[] persons;
    delete[] vehicles;
    delete[] staffList;
    delete[] passes;
    delete[] logs;
}

void GateSystem::clearAll() {
    for (int i = 0; i < personCount; i++) delete persons[i];   // virtual destructor runs here
    personCount = vehicleCount = staffCount = passCount = logCount = 0;
    nextPassNo = nextLogNo = 1;
}

// ===================== Private helpers =====================

int GateSystem::findPerson(const string& id) const {
    for (int i = 0; i < personCount; i++)
        if (persons[i]->getId() == id) return i;
    return -1;
}

int GateSystem::findVehicle(const string& no) const {
    for (int i = 0; i < vehicleCount; i++)
        if (vehicles[i].getVehicleNo() == no) return i;
    return -1;
}

int GateSystem::findStaff(const string& id) const {
    for (int i = 0; i < staffCount; i++)
        if (staffList[i].getStaffId() == id) return i;
    return -1;
}

int GateSystem::findPass(const string& id) const {
    for (int i = 0; i < passCount; i++)
        if (passes[i].getPassId() == id) return i;
    return -1;
}

void GateSystem::checkNewPerson(const string& id, const string& name) const {
    if (!Validation::isValidId(id))
        throw ValidationException("ID must be 2-10 letters/digits (no spaces or symbols)");
    if (!Validation::isValidName(name))
        throw ValidationException("Name must be 2-40 characters (letters, spaces, dots only)");
    if (findPerson(id) >= 0)
        throw DuplicateException("Person ID " + id + " already exists");
}

void GateSystem::ensurePersonSpace()  { if (personCount  == personCap)  growArray(persons,   personCount,  personCap); }
void GateSystem::ensureVehicleSpace() { if (vehicleCount == vehicleCap) growArray(vehicles,  vehicleCount, vehicleCap); }
void GateSystem::ensureStaffSpace()   { if (staffCount   == staffCap)   growArray(staffList, staffCount,   staffCap); }
void GateSystem::ensurePassSpace()    { if (passCount    == passCap)    growArray(passes,    passCount,    passCap); }
void GateSystem::ensureLogSpace()     { if (logCount     == logCap)     growArray(logs,      logCount,     logCap); }

// ===================== Add record =====================

void GateSystem::addEmployee(const string& id, const string& name, const string& department) {
    string pid = normId(id), nm = Validation::trim(name), dept = Validation::trim(department);
    checkNewPerson(pid, nm);
    if (!Validation::isSafeText(dept))
        throw ValidationException("Department must be 1-40 characters and must not contain '|'");
    ensurePersonSpace();
    persons[personCount++] = new Employee(pid, nm, dept);   // Person* pointing to an Employee
}

void GateSystem::addVisitor(const string& id, const string& name,
                            const string& purpose, const string& hostName) {
    string pid = normId(id), nm = Validation::trim(name);
    string pur = Validation::trim(purpose), host = Validation::trim(hostName);
    checkNewPerson(pid, nm);
    if (!Validation::isSafeText(pur))
        throw ValidationException("Purpose must be 1-40 characters and must not contain '|'");
    if (!Validation::isValidName(host))
        throw ValidationException("Host name must be 2-40 characters (letters, spaces, dots only)");
    ensurePersonSpace();
    persons[personCount++] = new Visitor(pid, nm, pur, host);   // Person* pointing to a Visitor
}

void GateSystem::addVehicle(const string& vehicleNo, const string& type, const string& ownerId) {
    string no = Validation::toUpper(Validation::trim(vehicleNo));
    string tp = Validation::capitalize(Validation::trim(type));
    string owner = normId(ownerId);
    if (!Validation::isValidVehicleNo(no))
        throw ValidationException("Vehicle number must be 4-12 letters/digits (no spaces or symbols)");
    if (!Validation::isValidVehicleType(tp))
        throw ValidationException("Vehicle type must be Car, Bike, Scooter, Truck, Van or Bus");
    if (findPerson(owner) < 0)
        throw NotFoundException("Owner ID " + owner + " is not a registered person");
    if (findVehicle(no) >= 0)
        throw DuplicateException("Vehicle " + no + " already exists");
    ensureVehicleSpace();
    vehicles[vehicleCount++] = Vehicle(no, tp, owner);
}

void GateSystem::addStaff(const string& id, const string& name, const string& shift) {
    string sid = normId(id), nm = Validation::trim(name);
    string sh = Validation::capitalize(Validation::trim(shift));
    if (!Validation::isValidId(sid))
        throw ValidationException("Staff ID must be 2-10 letters/digits (no spaces or symbols)");
    if (!Validation::isValidName(nm))
        throw ValidationException("Name must be 2-40 characters (letters, spaces, dots only)");
    if (!Validation::isValidShift(sh))
        throw ValidationException("Shift must be Morning, Evening or Night");
    if (findStaff(sid) >= 0)
        throw DuplicateException("Staff ID " + sid + " already exists");
    ensureStaffSpace();
    staffList[staffCount++] = SecurityStaff(sid, nm, sh);
}

void GateSystem::addPassRecord(const Pass& pass) {
    if (pass.getPassId().empty() || findPass(pass.getPassId()) >= 0)
        throw DuplicateException("Pass ID " + pass.getPassId() + " is empty or already exists");
    if (pass.getStatus() != "ACTIVE" && pass.getStatus() != "REVOKED")
        throw ValidationException("Pass status must be ACTIVE or REVOKED");
    if (!Validation::isValidDate(pass.getIssueDate()) || !Validation::isValidDate(pass.getExpiryDate()))
        throw ValidationException("Pass dates must be valid YYYY-MM-DD dates");
    ensurePassSpace();
    passes[passCount++] = pass;
    int n = numericPart(pass.getPassId());
    if (n >= nextPassNo) nextPassNo = n + 1;
}

void GateSystem::addLog(const EntryRecord& record) {
    if (record.getDirection() != "IN" && record.getDirection() != "OUT")
        throw ValidationException("Log direction must be IN or OUT");
    ensureLogSpace();
    logs[logCount++] = record;
    int n = numericPart(record.getLogId());
    if (n >= nextLogNo) nextLogNo = n + 1;
}

// ===================== Display =====================

void GateSystem::displayPersons() const {
    cout << "\n=== PERSONS (" << personCount << ") ===\n";
    if (personCount == 0) { cout << "No records found.\n"; return; }
    for (int i = 0; i < personCount; i++)
        persons[i]->display();          // runtime polymorphism: Employee::display or Visitor::display
}

void GateSystem::displayVehicles() const {
    cout << "\n=== VEHICLES (" << vehicleCount << ") ===\n";
    if (vehicleCount == 0) { cout << "No records found.\n"; return; }
    for (int i = 0; i < vehicleCount; i++) vehicles[i].display();
}

void GateSystem::displayStaff() const {
    cout << "\n=== SECURITY STAFF (" << staffCount << ") ===\n";
    if (staffCount == 0) { cout << "No records found.\n"; return; }
    for (int i = 0; i < staffCount; i++) staffList[i].display();
}

void GateSystem::displayPasses() const {
    cout << "\n=== PASSES (" << passCount << ") ===\n";
    if (passCount == 0) { cout << "No records found.\n"; return; }
    for (int i = 0; i < passCount; i++) passes[i].display();
}

void GateSystem::displayLogs() const {
    cout << "\n=== GATE ENTRY LOG (" << logCount << ") ===\n";
    if (logCount == 0) { cout << "No records found.\n"; return; }
    for (int i = 0; i < logCount; i++) logs[i].display();
}

// ===================== Search =====================

const Person* GateSystem::searchPerson(const string& id) const {
    int i = findPerson(normId(id));
    if (i < 0) throw NotFoundException("Person ID " + normId(id));
    return persons[i];
}

int GateSystem::searchPersonsByName(const string& part) const {
    string key = Validation::toUpper(Validation::trim(part));
    if (key.empty()) throw ValidationException("Search text cannot be empty");
    int found = 0;
    for (int i = 0; i < personCount; i++) {
        if (Validation::toUpper(persons[i]->getName()).find(key) != string::npos) {
            persons[i]->display();
            found++;
        }
    }
    return found;
}

const Vehicle& GateSystem::searchVehicle(const string& vehicleNo) const {
    string no = Validation::toUpper(Validation::trim(vehicleNo));
    int i = findVehicle(no);
    if (i < 0) throw NotFoundException("Vehicle " + no);
    return vehicles[i];
}

const Pass& GateSystem::searchPass(const string& passId) const {
    string id = normId(passId);
    int i = findPass(id);
    if (i < 0) throw NotFoundException("Pass " + id);
    return passes[i];
}

int GateSystem::searchPassesByPerson(const string& personId) const {
    string pid = normId(personId);
    if (findPerson(pid) < 0) throw NotFoundException("Person ID " + pid);
    int found = 0;
    for (int i = 0; i < passCount; i++) {
        if (passes[i].getPersonId() == pid) { passes[i].display(); found++; }
    }
    return found;
}

// ===================== Update =====================

void GateSystem::updatePersonName(const string& id, const string& newName) {
    int i = findPerson(normId(id));
    if (i < 0) throw NotFoundException("Person ID " + normId(id));
    persons[i]->setName(Validation::trim(newName));
}

void GateSystem::updatePersonDetail(const string& id, const string& newDetail) {
    int i = findPerson(normId(id));
    if (i < 0) throw NotFoundException("Person ID " + normId(id));
    persons[i]->setDetail(Validation::trim(newDetail));   // virtual: department or purpose
}

void GateSystem::updateVehicleOwner(const string& vehicleNo, const string& newOwnerId) {
    string no = Validation::toUpper(Validation::trim(vehicleNo));
    string owner = normId(newOwnerId);
    int i = findVehicle(no);
    if (i < 0) throw NotFoundException("Vehicle " + no);
    if (findPerson(owner) < 0) throw NotFoundException("Owner ID " + owner);
    for (int p = 0; p < passCount; p++)
        if (passes[p].isActive() && passes[p].getVehicleNo() == no)
            throw IntegrityException("Vehicle " + no + " is used by active pass " + passes[p].getPassId()
                                     + ". Revoke the pass first.");
    vehicles[i].setOwnerId(owner);
}

void GateSystem::updateVehicleType(const string& vehicleNo, const string& newType) {
    string no = Validation::toUpper(Validation::trim(vehicleNo));
    int i = findVehicle(no);
    if (i < 0) throw NotFoundException("Vehicle " + no);
    vehicles[i].setType(Validation::trim(newType));
}

void GateSystem::updateStaffShift(const string& id, const string& newShift) {
    int i = findStaff(normId(id));
    if (i < 0) throw NotFoundException("Staff ID " + normId(id));
    staffList[i].setShift(Validation::trim(newShift));
}

void GateSystem::updatePassExpiry(const string& passId, const string& newExpiry) {
    int i = findPass(normId(passId));
    if (i < 0) throw NotFoundException("Pass " + normId(passId));
    passes[i].setExpiryDate(Validation::trim(newExpiry));
}

// ===================== Delete =====================

void GateSystem::deletePerson(const string& id) {
    string pid = normId(id);
    int i = findPerson(pid);
    if (i < 0) throw NotFoundException("Person ID " + pid);
    for (int p = 0; p < passCount; p++)
        if (passes[p].isActive() && passes[p].getPersonId() == pid)
            throw IntegrityException("Person " + pid + " has active pass " + passes[p].getPassId()
                                     + ". Revoke it first.");
    for (int v = 0; v < vehicleCount; v++)
        if (vehicles[v].getOwnerId() == pid)
            throw IntegrityException("Person " + pid + " owns vehicle " + vehicles[v].getVehicleNo()
                                     + ". Delete the vehicle first.");
    delete persons[i];                                       // free the object
    for (int j = i; j < personCount - 1; j++) persons[j] = persons[j + 1];
    personCount--;
}

void GateSystem::deleteVehicle(const string& vehicleNo) {
    string no = Validation::toUpper(Validation::trim(vehicleNo));
    int i = findVehicle(no);
    if (i < 0) throw NotFoundException("Vehicle " + no);
    for (int p = 0; p < passCount; p++)
        if (passes[p].isActive() && passes[p].getVehicleNo() == no)
            throw IntegrityException("Vehicle " + no + " is used by active pass " + passes[p].getPassId()
                                     + ". Revoke the pass first.");
    for (int j = i; j < vehicleCount - 1; j++) vehicles[j] = vehicles[j + 1];
    vehicleCount--;
}

void GateSystem::deleteStaff(const string& id) {
    int i = findStaff(normId(id));
    if (i < 0) throw NotFoundException("Staff ID " + normId(id));
    for (int j = i; j < staffCount - 1; j++) staffList[j] = staffList[j + 1];
    staffCount--;
}

void GateSystem::deletePass(const string& passId) {
    string id = normId(passId);
    int i = findPass(id);
    if (i < 0) throw NotFoundException("Pass " + id);
    if (isInside(id))
        throw IntegrityException("The holder of pass " + id + " is currently inside. Record the OUT entry first.");
    for (int j = i; j < passCount - 1; j++) passes[j] = passes[j + 1];
    passCount--;
}

// ===================== Main transaction =====================

Pass GateSystem::issuePass(const string& personId, const string& vehicleNo,
                           const string& issueDate, const string& expiryDate) {
    string pid = normId(personId);
    string vno = Validation::toUpper(Validation::trim(vehicleNo));
    string issueDt = Validation::trim(issueDate);
    if (issueDt.empty()) issueDt = Validation::todayDate();     // blank = today
    if (vno.empty()) vno = "NONE";

    if (findPerson(pid) < 0) throw NotFoundException("Person ID " + pid);
    if (vno != "NONE") {
        int v = findVehicle(vno);
        if (v < 0) throw NotFoundException("Vehicle " + vno);
        if (vehicles[v].getOwnerId() != pid)
            throw IntegrityException("Vehicle " + vno + " is not registered to person " + pid);
    }
    Pass p;
    p.issue(makeId('P', nextPassNo), pid, vno, issueDt, Validation::trim(expiryDate));  // validates dates first

    for (int i = 0; i < passCount; i++)
        if (passes[i].isActive() && passes[i].getPersonId() == pid && passes[i].getVehicleNo() == vno)
            throw DuplicateException("Active pass " + passes[i].getPassId()
                                     + " already exists for this person and vehicle");

    ensurePassSpace();
    passes[passCount++] = p;
    nextPassNo++;
    return p;
}

bool GateSystem::verifyPass(const string& passId, const string& staffId) const {
    int p = findPass(normId(passId));
    if (p < 0) throw NotFoundException("Pass " + normId(passId));
    int s = findStaff(normId(staffId));
    if (s < 0) throw NotFoundException("Staff ID " + normId(staffId));
    return staffList[s].verifyPass(passes[p], Validation::todayDate());   // throws PassException if invalid
}

EntryRecord GateSystem::recordEntry(const string& passId, const string& staffId, const string& direction) {
    string id = normId(passId), sid = normId(staffId), dir = Validation::toUpper(Validation::trim(direction));
    if (dir != "IN" && dir != "OUT")
        throw ValidationException("Direction must be IN or OUT");
    verifyPass(id, sid);                                       // throws if the pass is not acceptable
    bool inside = isInside(id);
    if (dir == "IN" && inside)
        throw PassException("pass " + id + " is already inside (record OUT first)");
    if (dir == "OUT" && !inside)
        throw PassException("pass " + id + " has no IN entry (cannot record OUT)");
    EntryRecord r(makeId('L', nextLogNo), id, sid, dir, Validation::todayDate(), Validation::currentTime());
    ensureLogSpace();
    logs[logCount++] = r;
    nextLogNo++;
    return r;
}

void GateSystem::revokePass(const string& passId) {
    int i = findPass(normId(passId));
    if (i < 0) throw NotFoundException("Pass " + normId(passId));
    passes[i].revoke();
}

// A pass is "inside" if its most recent log entry is IN.
bool GateSystem::isInside(const string& passId) const {
    for (int i = logCount - 1; i >= 0; i--)
        if (logs[i].getPassId() == passId) return logs[i].getDirection() == "IN";
    return false;
}

// ===================== Report =====================

void GateSystem::generateReport() const {
    string today = Validation::todayDate();
    int employees = 0, visitors = 0;
    for (int i = 0; i < personCount; i++) {
        if (persons[i]->getType() == "Employee") employees++;
        else if (persons[i]->getType() == "Visitor") visitors++;
    }
    int valid = 0, expired = 0, revoked = 0;
    for (int i = 0; i < passCount; i++) {
        if (!passes[i].isActive()) revoked++;
        else if (passes[i].isExpired(today)) expired++;
        else valid++;
    }
    int inToday = 0, outToday = 0;
    for (int i = 0; i < logCount; i++) {
        if (logs[i].getDate() != today) continue;
        if (logs[i].getDirection() == "IN") inToday++; else outToday++;
    }

    cout << "\n========== GATE PASS REPORT (" << today << ") ==========\n";
    cout << "Persons          : " << personCount << " (Employees: " << employees << ", Visitors: " << visitors << ")\n";
    cout << "Vehicles         : " << vehicleCount << "\n";
    cout << "Security staff   : " << staffCount << "\n";
    cout << "Passes           : " << passCount << " (Valid: " << valid << ", Expired: " << expired
         << ", Revoked: " << revoked << ")\n";
    cout << "Gate log (total) : " << logCount << "\n";
    cout << "Gate log (today) : IN " << inToday << ", OUT " << outToday << "\n";

    cout << "\nCurrently inside the premises:\n";
    int inside = 0;
    for (int i = 0; i < passCount; i++) {
        if (!isInside(passes[i].getPassId())) continue;
        int p = findPerson(passes[i].getPersonId());
        cout << "  " << passes[i].getPassId() << " - " << passes[i].getPersonId() << " ("
             << (p >= 0 ? persons[p]->getName() : string("unknown")) << ") - Vehicle: "
             << passes[i].getVehicleNo() << "\n";
        inside++;
    }
    if (inside == 0) cout << "  (nobody)\n";
    cout << "=================================================\n";
}

// ===================== Read-only access =====================

int GateSystem::getPersonCount() const  { return personCount; }
int GateSystem::getVehicleCount() const { return vehicleCount; }
int GateSystem::getStaffCount() const   { return staffCount; }
int GateSystem::getPassCount() const    { return passCount; }
int GateSystem::getLogCount() const     { return logCount; }

const Person&        GateSystem::getPersonAt(int i) const  { return *persons[i]; }
const Vehicle&       GateSystem::getVehicleAt(int i) const { return vehicles[i]; }
const SecurityStaff& GateSystem::getStaffAt(int i) const   { return staffList[i]; }
const Pass&          GateSystem::getPassAt(int i) const    { return passes[i]; }
const EntryRecord&   GateSystem::getLogAt(int i) const     { return logs[i]; }
