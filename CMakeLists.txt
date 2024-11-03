cmake_minimum_required(VERSION 3.12)
project(FastPlume VERSION 1.0.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# Automatically detect all .cpp files in the src directory
file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add the executable
add_executable(FastPlume ${SOURCE_FILES})