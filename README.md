# FastPlume

FastPlume is a high-performance simulation library designed to model plume dispersion with customizable input data and output options. This project provides flexibility for users to set up tasks either through programmatic configuration or input files, with outputs directed to vectors or CSV files.

## Table of Contents
1. [Installation](#installation)
2. [Usage](#usage)
3. [Building and Running Examples](#building-and-running-examples)
4. [Configuration](#configuration)
5. [Dependencies](#dependencies)

---

### Installation

You can install FastPlume either locally in your project directory or globally to your system.

#### Prerequisites
Ensure you have `cmake` (version 3.12 or later) and a C++17-compatible compiler installed. You can verify your `cmake` version with:
cmake --version

Build and Install:
mkdir build && cd build

Generate Build Files:
cmake ..

Compile the Library:
make

Install (Optionally with sudo for system-wide installation):
sudo make install

By default, this installs FastPlume under /usr/local/FastPlume. To install in a different location, specify CMAKE_INSTALL_PREFIX during the configuration step:
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install ..
make
sudo make install

Installed Files Structure
After installation, the directory structure will look like this:
/usr/local/FastPlume/
├── bin/
│   └── FastPlume_Test          # Example executable
├── lib/
│   ├── libFastPlume.so         # Shared library
│   └── libFastPlume.a          # Static library
├── include/
│   ├── FastPlume.h
│   └── taskDataRow.h           # Public headers
├── examples/
│   ├── example.cpp             # Example code
│   └── Makefile                # Makefile to build example
└── fixture/
    └── hpac_dispersion_coefs.csv # Example data file for dispersion coefficients

Building and Running Examples
An example program, example.cpp, is provided under the examples/ directory. You can compile and run this example as follows:
cd /usr/local/FastPlume/examples
make
./example_run


