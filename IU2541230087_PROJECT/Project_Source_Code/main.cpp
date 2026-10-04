// main.cpp - creates the system objects, loads saved data and starts the menu.
// All real logic lives in the classes (GateSystem, FileManager, ...).
#include <iostream>
#include "GateSystem.h"
#include "FileManager.h"
#include "Menu.h"
#include "Exceptions.h"

int main() {
    GateSystem system;                  // owns all records
    FileManager files("Data/");         // reads / writes the data files

    // START -> load existing records
    try {
        int n = files.load(system);
        std::cout << "Loaded " << n << " record(s) from the Data folder.\n";
    } catch (const GateException& e) {
        std::cout << "[ERROR] " << e.what() << "\nStarting with empty data.\n";
    }

    runMenu(system, files);             // operations, then save on exit
    return 0;
}
