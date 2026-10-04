# Security Gate Pass System (CPP58)

Console-based C++ OOP project: manages persons (employees / visitors), vehicles, security staff,
gate passes and gate entry/exit records, with data saved in text files.

**Enrollment No.:** IU2541230087  
**GitHub:** <paste your repository link here>

## Build

Windows (g++ from MSYS2 / MinGW on PATH):

    build.bat
    -- or --
    g++ -std=c++17 -Wall -Wextra -O2 main.cpp Menu.cpp GateSystem.cpp FileManager.cpp Person.cpp Employee.cpp Visitor.cpp Vehicle.cpp Pass.cpp SecurityStaff.cpp EntryRecord.cpp Validation.cpp -o gatepass.exe

Linux / macOS: `make`

## Run

Open a terminal **in this folder** (so the `Data/` folder is found) and run:

    gatepass.exe

A ready-built `gatepass.exe` is included. Sample records are already in `Data/`.
To start with empty data, delete the `.txt` files inside `Data/` (keep the folder).

## Sample data (included)

| Type | IDs |
|---|---|
| Employees | E101 (Rahul Shah), E102 (Priya Patel) |
| Visitor | V201 (Amit Verma) |
| Vehicles | GJ01AB1234 (owner E101), GJ01CD5678 (owner V201) |
| Security staff | S01 (Mahesh Kumar), S02 (Anil Singh) |
| Passes | P001 (E101 + GJ01AB1234), P002 (E102, no vehicle) |

## Menu

1 Add Record - 2 Display Records - 3 Search Record - 4 Update Record - 5 Delete Record -
6 Main Transaction (issue / verify / record entry-exit / revoke pass) - 7 Report - 8 Save / Load - 9 Exit

## Source structure

| File(s) | Class / purpose |
|---|---|
| `Person`, `Employee`, `Visitor` | Base class and two derived classes (inheritance, virtual `display()`) |
| `Vehicle` | Vehicle registered to a person |
| `Pass` | Gate pass: `issue()`, `revoke()` |
| `SecurityStaff` | Guard: `verifyPass()` |
| `EntryRecord` | One IN / OUT gate movement |
| `GateSystem` | Core class; owns dynamic arrays (`new[]` / `delete[]`) |
| `FileManager` | `save()` / `load()` of the text files in `Data/` |
| `Menu` | Console menus (input and output only) |
| `Validation`, `Exceptions.h` | Input checks and custom exception classes |
| `main.cpp` | Creates the objects, loads data, runs the menu |

## Data files (`Data/`, one record per line, fields separated by `|`)

`persons.txt`, `vehicles.txt`, `staff.txt`, `passes.txt`, `entries.txt`

## Rules enforced

- IDs: 2-10 letters/digits; names: letters, spaces, dots; dates: valid `YYYY-MM-DD`
- A vehicle can only be used on a pass of its registered owner
- One active pass per person + vehicle combination
- A pass must be active and within its dates to be accepted at the gate
- A person cannot be recorded IN twice, or OUT without an IN
- A person with an active pass or a registered vehicle cannot be deleted; a vehicle on an active pass cannot be deleted
