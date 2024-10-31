#!/bin/bash

# Function to check if pybind11 is installed
check_pybind11() {
    if python -m pybind11 --includes &> /dev/null; then
        echo "pybind11 is already installed."
        return 0
    else
        echo "pybind11 is not installed."
        return 1
    fi
}

# Function to install pybind11
install_pybind11() {
    echo "Installing pybind11 via pip..."
    pip install pybind11
    if [ $? -ne 0 ]; then
        echo "Error: Failed to install pybind11."
        exit 1
    fi
    echo "pybind11 installed successfully."
}

# Function to check if pandas is installed
check_pandas() {
    if python -c "import pandas" &> /dev/null; then
        echo "pandas is already installed."
        return 0
    else
        echo "pandas is not installed."
        return 1
    fi
}

# Function to install pandas
install_pandas() {
    echo "Installing pandas via pip..."
    pip install pandas
    if [ $? -ne 0 ]; then
        echo "Error: Failed to install pandas."
        exit 1
    fi
    echo "pandas installed successfully."
}

# Step 1: Check if pybind11 is installed
if ! check_pybind11; then
    install_pybind11
fi

# Step 2: Check if pandas is installed
if ! check_pandas; then
    install_pandas
fi

# Step 3: Get the CMake directory for pybind11
PYBIND11_CMAKE_DIR=$(python -m pybind11 --cmakedir)

# Step 4: Create a build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Step 5: Run CMake to configure the project
echo "Running CMake..."
cmake .. -DPYBIND11_DIR=$PYBIND11_CMAKE_DIR
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed."
    exit 1
fi

# Step 6: Build the project using make
echo "Building the project..."
make
if [ $? -ne 0 ]; then
    echo "Error: Build failed."
    exit 1
fi

echo "Build completed successfully."
