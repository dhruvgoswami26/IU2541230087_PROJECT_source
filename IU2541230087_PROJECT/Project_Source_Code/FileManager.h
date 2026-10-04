// FileManager.h - saves and loads all records to/from text files in the data folder
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "GateSystem.h"

class FileManager {
private:
    std::string dataDir;       // e.g. "Data/"

    std::string pathOf(const std::string& fileName) const;
    bool readLines(const std::string& fileName, std::vector<std::string>& lines) const;
    void writeLines(const std::string& fileName, const std::vector<std::string>& lines) const;

    int loadPersons(GateSystem& sys) const;
    int loadVehicles(GateSystem& sys) const;
    int loadStaff(GateSystem& sys) const;
    int loadPasses(GateSystem& sys) const;
    int loadLogs(GateSystem& sys) const;

public:
    FileManager();                                  // default folder "Data/"
    explicit FileManager(const std::string& dataDir);
    ~FileManager();

    void save(const GateSystem& sys) const;         // throws FileException if a file cannot be written
    int  load(GateSystem& sys) const;               // returns number of records loaded
};

#endif
