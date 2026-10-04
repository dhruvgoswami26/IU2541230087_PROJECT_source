# Makefile - build with:  make        (or mingw32-make on Windows)
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC      = main.cpp Menu.cpp GateSystem.cpp FileManager.cpp Person.cpp Employee.cpp Visitor.cpp \
           Vehicle.cpp Pass.cpp SecurityStaff.cpp EntryRecord.cpp Validation.cpp
TARGET   = gatepass

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe
