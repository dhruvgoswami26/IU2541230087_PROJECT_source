// Menu.cpp
#include "Menu.h"
#include "Validation.h"
#include "Exceptions.h"
#include <iostream>
#include <string>

using namespace std;

namespace {

// Reads one line; throws InputEnded if the input stream is closed.
string readLine(const string& prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) throw InputEnded();
    return Validation::trim(s);
}

// Reads a menu choice; returns -1 if the text is not a whole number.
int readChoice(const string& prompt) {
    string s = readLine(prompt);
    try {
        size_t used = 0;
        int v = stoi(s, &used);
        return (used == s.size()) ? v : -1;
    } catch (...) {
        return -1;
    }
}

void invalidChoice() { cout << "Invalid choice. Please try again.\n"; }

// ---------------- 1. Add Record ----------------
void addMenu(GateSystem& sys) {
    cout << "\n--- ADD RECORD ---\n 1. Employee\n 2. Visitor\n 3. Vehicle\n 4. Security Staff\n 0. Back\n";
    switch (readChoice("Choice: ")) {
        case 1: {
            string id = readLine("Employee ID   : ");
            string name = readLine("Name          : ");
            string dept = readLine("Department    : ");
            sys.addEmployee(id, name, dept);
            cout << "Employee added successfully.\n";
            break;
        }
        case 2: {
            string id = readLine("Visitor ID    : ");
            string name = readLine("Name          : ");
            string purpose = readLine("Purpose       : ");
            string host = readLine("Host name     : ");
            sys.addVisitor(id, name, purpose, host);
            cout << "Visitor added successfully.\n";
            break;
        }
        case 3: {
            string no = readLine("Vehicle number: ");
            string type = readLine("Type (Car/Bike/Scooter/Truck/Van/Bus): ");
            string owner = readLine("Owner person ID: ");
            sys.addVehicle(no, type, owner);
            cout << "Vehicle added successfully.\n";
            break;
        }
        case 4: {
            string id = readLine("Staff ID      : ");
            string name = readLine("Name          : ");
            string shift = readLine("Shift (Morning/Evening/Night): ");
            sys.addStaff(id, name, shift);
            cout << "Security staff added successfully.\n";
            break;
        }
        case 0: break;
        default: invalidChoice();
    }
}

// ---------------- 2. Display Records ----------------
void displayMenu(GateSystem& sys) {
    cout << "\n--- DISPLAY RECORDS ---\n 1. Persons\n 2. Vehicles\n 3. Security Staff\n 4. Passes\n"
            " 5. Gate Entry Log\n 6. Everything\n 0. Back\n";
    switch (readChoice("Choice: ")) {
        case 1: sys.displayPersons(); break;
        case 2: sys.displayVehicles(); break;
        case 3: sys.displayStaff(); break;
        case 4: sys.displayPasses(); break;
        case 5: sys.displayLogs(); break;
        case 6: sys.displayPersons(); sys.displayVehicles(); sys.displayStaff();
                sys.displayPasses(); sys.displayLogs(); break;
        case 0: break;
        default: invalidChoice();
    }
}

// ---------------- 3. Search Record ----------------
void searchMenu(GateSystem& sys) {
    cout << "\n--- SEARCH RECORD ---\n 1. Person by ID\n 2. Person by name\n 3. Vehicle by number\n"
            " 4. Pass by ID\n 5. All passes of a person\n 0. Back\n";
    switch (readChoice("Choice: ")) {
        case 1: {
            sys.searchPerson(readLine("Person ID: "))->display();
            break;
        }
        case 2: {
            int n = sys.searchPersonsByName(readLine("Name (or part of it): "));
            if (n == 0) cout << "No person matched.\n";
            break;
        }
        case 3: {
            sys.searchVehicle(readLine("Vehicle number: ")).display();
            break;
        }
        case 4: {
            sys.searchPass(readLine("Pass ID: ")).display();
            break;
        }
        case 5: {
            int n = sys.searchPassesByPerson(readLine("Person ID: "));
            if (n == 0) cout << "This person has no passes.\n";
            break;
        }
        case 0: break;
        default: invalidChoice();
    }
}

// ---------------- 4. Update Record ----------------
void updateMenu(GateSystem& sys) {
    cout << "\n--- UPDATE RECORD ---\n 1. Person name\n 2. Person department / purpose\n"
            " 3. Vehicle owner\n 4. Vehicle type\n 5. Staff shift\n 6. Pass expiry date\n 0. Back\n";
    switch (readChoice("Choice: ")) {
        case 1: {
            string id = readLine("Person ID: ");
            sys.updatePersonName(id, readLine("New name: "));
            cout << "Name updated.\n";
            break;
        }
        case 2: {
            string id = readLine("Person ID: ");
            sys.updatePersonDetail(id, readLine("New department / purpose: "));
            cout << "Detail updated.\n";
            break;
        }
        case 3: {
            string no = readLine("Vehicle number: ");
            sys.updateVehicleOwner(no, readLine("New owner person ID: "));
            cout << "Vehicle owner updated.\n";
            break;
        }
        case 4: {
            string no = readLine("Vehicle number: ");
            sys.updateVehicleType(no, readLine("New type (Car/Bike/Scooter/Truck/Van/Bus): "));
            cout << "Vehicle type updated.\n";
            break;
        }
        case 5: {
            string id = readLine("Staff ID: ");
            sys.updateStaffShift(id, readLine("New shift (Morning/Evening/Night): "));
            cout << "Shift updated.\n";
            break;
        }
        case 6: {
            string id = readLine("Pass ID: ");
            sys.updatePassExpiry(id, readLine("New expiry date (YYYY-MM-DD): "));
            cout << "Expiry date updated.\n";
            break;
        }
        case 0: break;
        default: invalidChoice();
    }
}

// ---------------- 5. Delete Record ----------------
void deleteMenu(GateSystem& sys) {
    cout << "\n--- DELETE RECORD ---\n 1. Person\n 2. Vehicle\n 3. Security Staff\n 4. Pass\n 0. Back\n";
    int c = readChoice("Choice: ");
    if (c < 0 || c > 4) { invalidChoice(); return; }
    if (c == 0) return;
    string key = readLine(c == 2 ? "Vehicle number: " : (c == 4 ? "Pass ID: " : "ID: "));
    string sure = Validation::toUpper(readLine("Delete " + key + "? (Y/N): "));
    if (sure != "Y") { cout << "Delete cancelled.\n"; return; }
    switch (c) {
        case 1: sys.deletePerson(key); break;
        case 2: sys.deleteVehicle(key); break;
        case 3: sys.deleteStaff(key); break;
        case 4: sys.deletePass(key); break;
    }
    cout << "Record deleted.\n";
}

// ---------------- 6. Main Transaction ----------------
void transactionMenu(GateSystem& sys) {
    cout << "\n--- GATE PASS TRANSACTIONS ---\n 1. Issue a new pass\n 2. Verify a pass\n"
            " 3. Record gate entry / exit\n 4. Revoke a pass\n 0. Back\n";
    switch (readChoice("Choice: ")) {
        case 1: {
            string pid = readLine("Person ID: ");
            string vno = readLine("Vehicle number (blank = no vehicle): ");
            string issueDt = readLine("Issue date YYYY-MM-DD (blank = today): ");
            string expiry = readLine("Expiry date YYYY-MM-DD: ");
            Pass p = sys.issuePass(pid, vno, issueDt, expiry);
            cout << "Pass issued successfully:\n";
            p.display();
            break;
        }
        case 2: {
            string passId = readLine("Pass ID: ");
            string staffId = readLine("Guard (staff) ID: ");
            if (sys.verifyPass(passId, staffId)) cout << "Pass is VALID.\n";
            break;
        }
        case 3: {
            string passId = readLine("Pass ID: ");
            string staffId = readLine("Guard (staff) ID: ");
            string dir = readLine("Direction (IN/OUT): ");
            EntryRecord r = sys.recordEntry(passId, staffId, dir);
            cout << "Gate movement recorded:\n";
            r.display();
            break;
        }
        case 4: {
            sys.revokePass(readLine("Pass ID: "));
            cout << "Pass revoked.\n";
            break;
        }
        case 0: break;
        default: invalidChoice();
    }
}

// ---------------- 8. Save / Load ----------------
void saveLoadMenu(GateSystem& sys, FileManager& files) {
    cout << "\n--- SAVE / LOAD ---\n 1. Save all records to files\n"
            " 2. Reload records from files (unsaved changes are lost)\n 0. Back\n";
    switch (readChoice("Choice: ")) {
        case 1:
            files.save(sys);
            cout << "All records saved.\n";
            break;
        case 2: {
            string sure = Validation::toUpper(readLine("Discard unsaved changes and reload? (Y/N): "));
            if (sure != "Y") { cout << "Reload cancelled.\n"; break; }
            int n = files.load(sys);
            cout << n << " record(s) loaded.\n";
            break;
        }
        case 0: break;
        default: invalidChoice();
    }
}

void showMainMenu() {
    cout << "\n=========================================\n"
            "      SECURITY GATE PASS SYSTEM\n"
            "=========================================\n"
            " 1. Add Record\n"
            " 2. Display Records\n"
            " 3. Search Record\n"
            " 4. Update Record\n"
            " 5. Delete Record\n"
            " 6. Main Transaction (Issue / Verify / Entry / Revoke)\n"
            " 7. Report\n"
            " 8. Save / Load\n"
            " 9. Exit\n"
            "=========================================\n";
}

void saveOnExit(GateSystem& sys, FileManager& files) {
    try {
        files.save(sys);
        cout << "Records saved to the Data folder.\n";
    } catch (const GateException& e) {
        cout << "[ERROR] " << e.what() << "\n";
    }
}

}

void runMenu(GateSystem& sys, FileManager& files) {
    try {
        bool running = true;
        while (running) {
            showMainMenu();
            int choice = readChoice("Enter choice: ");
            try {
                switch (choice) {
                    case 1: addMenu(sys); break;
                    case 2: displayMenu(sys); break;
                    case 3: searchMenu(sys); break;
                    case 4: updateMenu(sys); break;
                    case 5: deleteMenu(sys); break;
                    case 6: transactionMenu(sys); break;
                    case 7: sys.generateReport(); break;
                    case 8: saveLoadMenu(sys, files); break;
                    case 9: {
                        string ans = Validation::toUpper(readLine("Save changes before exit? (Y/N): "));
                        if (ans != "N") saveOnExit(sys, files);
                        running = false;
                        break;
                    }
                    default: invalidChoice();
                }
            } catch (const GateException& e) {
                // every validation / not-found / pass / file error ends up here
                cout << "\n[ERROR] " << e.what() << "\n";
            }
        }
        cout << "Goodbye!\n";
    } catch (const InputEnded&) {
        cout << "\nInput closed. Saving and exiting.\n";
        saveOnExit(sys, files);
    }
}
