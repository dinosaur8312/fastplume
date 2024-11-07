# Set the compiler
CXX = g++

# Set the paths for FastPlume library and include files
LIB_PATH = ../lib      # Adjust if FastPlume library is in a different path
INCLUDE_PATH = ../include    # Adjust if FastPlume headers are in a different path

# Set the output binary name
TARGET = example_run

# Specify the source file
SRC = example.cpp

# Compiler flags
CXXFLAGS = -std=c++17 -I$(INCLUDE_PATH)
LDFLAGS = -L$(LIB_PATH) -lFastPlume -Wl,-rpath=$(LIB_PATH)

# Target rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(TARGET)
