@echo off
REM Build script for Windows (needs g++ from MSYS2/MinGW on PATH)
g++ -std=c++17 -Wall -Wextra -O2 main.cpp Menu.cpp GateSystem.cpp FileManager.cpp Person.cpp Employee.cpp Visitor.cpp Vehicle.cpp Pass.cpp SecurityStaff.cpp EntryRecord.cpp Validation.cpp -o gatepass.exe
if %errorlevel% neq 0 (
    echo Build FAILED
) else (
    echo Build OK - run gatepass.exe
)
pause
