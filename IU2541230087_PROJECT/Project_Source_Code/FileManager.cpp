// FileManager.cpp
#include "FileManager.h"
#include "Validation.h"
#include "Exceptions.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <system_error>

using namespace std;

static const char* PERSONS_FILE  = "persons.txt";
static const char* VEHICLES_FILE = "vehicles.txt";
static const char* STAFF_FILE    = "staff.txt";
static const char* PASSES_FILE   = "passes.txt";
static const char* LOGS_FILE     = "entries.txt";

FileManager::FileManager() : dataDir("Data/") {}

FileManager::FileManager(const string& dir) : dataDir(dir) {
    if (!dataDir.empty() && dataDir.back() != '/' && dataDir.back() != '\\') dataDir += "/";
}

FileManager::~FileManager() {}   // streams are local to each function and close automatically

string FileManager::pathOf(const string& fileName) const { return dataDir + fileName; }

// Returns false if the file does not exist (normal on the very first run).
bool FileManager::readLines(const string& fileName, vector<string>& lines) const {
    ifstream in(pathOf(fileName));
    if (!in.is_open()) return false;
    string line;
    while (getline(in, line)) {
        line = Validation::trim(line);
        if (!line.empty()) lines.push_back(line);
    }
    return true;
}

void FileManager::writeLines(const string& fileName, const vector<string>& lines) const {
    ofstream out(pathOf(fileName));
    if (!out.is_open())
        throw FileException("cannot open " + pathOf(fileName) + " for writing");
    for (const string& l : lines) out << l << "\n";
    out.flush();
    if (!out.good())
        throw FileException("failed while writing " + pathOf(fileName));
}

// ===================== Save =====================

void FileManager::save(const GateSystem& sys) const {
    error_code ec;
    filesystem::create_directories(dataDir, ec);           // make Data/ if it does not exist
    if (ec) throw FileException("cannot create folder " + dataDir);

    vector<string> lines;
    for (int i = 0; i < sys.getPersonCount(); i++) lines.push_back(sys.getPersonAt(i).toRecord());  // virtual toRecord
    writeLines(PERSONS_FILE, lines);

    lines.clear();
    for (int i = 0; i < sys.getVehicleCount(); i++) lines.push_back(sys.getVehicleAt(i).toRecord());
    writeLines(VEHICLES_FILE, lines);

    lines.clear();
    for (int i = 0; i < sys.getStaffCount(); i++) lines.push_back(sys.getStaffAt(i).toRecord());
    writeLines(STAFF_FILE, lines);

    lines.clear();
    for (int i = 0; i < sys.getPassCount(); i++) lines.push_back(sys.getPassAt(i).toRecord());
    writeLines(PASSES_FILE, lines);

    lines.clear();
    for (int i = 0; i < sys.getLogCount(); i++) lines.push_back(sys.getLogAt(i).toRecord());
    writeLines(LOGS_FILE, lines);
}

// ===================== Load =====================
// Each load function reads one file. A bad line is reported and skipped; the rest still load.

static void warnBadLine(const string& file, int lineNo, const string& why) {
    cout << "  [WARNING] " << file << " line " << lineNo << " skipped: " << why << "\n";
}

int FileManager::loadPersons(GateSystem& sys) const {
    vector<string> lines;
    if (!readLines(PERSONS_FILE, lines)) return 0;
    int loaded = 0, lineNo = 0;
    for (const string& line : lines) {
        lineNo++;
        try {
            vector<string> f = Validation::split(line, '|');
            if (f[0] == "Employee" && f.size() == 4) sys.addEmployee(f[1], f[2], f[3]);
            else if (f[0] == "Visitor" && f.size() == 5) sys.addVisitor(f[1], f[2], f[3], f[4]);
            else throw FileException("unknown person record format");
            loaded++;
        } catch (const GateException& e) {
            warnBadLine(PERSONS_FILE, lineNo, e.what());
        }
    }
    return loaded;
}

int FileManager::loadVehicles(GateSystem& sys) const {
    vector<string> lines;
    if (!readLines(VEHICLES_FILE, lines)) return 0;
    int loaded = 0, lineNo = 0;
    for (const string& line : lines) {
        lineNo++;
        try {
            vector<string> f = Validation::split(line, '|');
            if (f.size() != 3) throw FileException("expected 3 fields");
            sys.addVehicle(f[0], f[1], f[2]);
            loaded++;
        } catch (const GateException& e) {
            warnBadLine(VEHICLES_FILE, lineNo, e.what());
        }
    }
    return loaded;
}

int FileManager::loadStaff(GateSystem& sys) const {
    vector<string> lines;
    if (!readLines(STAFF_FILE, lines)) return 0;
    int loaded = 0, lineNo = 0;
    for (const string& line : lines) {
        lineNo++;
        try {
            vector<string> f = Validation::split(line, '|');
            if (f.size() != 3) throw FileException("expected 3 fields");
            sys.addStaff(f[0], f[1], f[2]);
            loaded++;
        } catch (const GateException& e) {
            warnBadLine(STAFF_FILE, lineNo, e.what());
        }
    }
    return loaded;
}

int FileManager::loadPasses(GateSystem& sys) const {
    vector<string> lines;
    if (!readLines(PASSES_FILE, lines)) return 0;
    int loaded = 0, lineNo = 0;
    for (const string& line : lines) {
        lineNo++;
        try {
            vector<string> f = Validation::split(line, '|');
            if (f.size() != 6) throw FileException("expected 6 fields");
            sys.addPassRecord(Pass(f[0], f[1], f[2], f[3], f[4], f[5]));
            loaded++;
        } catch (const GateException& e) {
            warnBadLine(PASSES_FILE, lineNo, e.what());
        }
    }
    return loaded;
}

int FileManager::loadLogs(GateSystem& sys) const {
    vector<string> lines;
    if (!readLines(LOGS_FILE, lines)) return 0;
    int loaded = 0, lineNo = 0;
    for (const string& line : lines) {
        lineNo++;
        try {
            vector<string> f = Validation::split(line, '|');
            if (f.size() != 6) throw FileException("expected 6 fields");
            sys.addLog(EntryRecord(f[0], f[1], f[2], f[3], f[4], f[5]));
            loaded++;
        } catch (const GateException& e) {
            warnBadLine(LOGS_FILE, lineNo, e.what());
        }
    }
    return loaded;
}

int FileManager::load(GateSystem& sys) const {
    sys.clearAll();
    // Order matters: vehicles refer to persons, so persons are loaded first.
    int total = 0;
    total += loadPersons(sys);
    total += loadVehicles(sys);
    total += loadStaff(sys);
    total += loadPasses(sys);
    total += loadLogs(sys);
    return total;
}
