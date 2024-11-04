cmake_minimum_required(VERSION 3.12)
project(FastPlume VERSION 1.0.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Automatically detect all .cpp files in the src directory
file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Create both shared and static library targets
add_library(FastPlumeShared SHARED ${SOURCE_FILES})
add_library(FastPlumeStatic STATIC ${SOURCE_FILES})

# Specify output names for the libraries
set_target_properties(FastPlumeShared PROPERTIES OUTPUT_NAME "FastPlume")
set_target_properties(FastPlumeStatic PROPERTIES OUTPUT_NAME "FastPlume")

# Specify the include directories for the library
target_include_directories(FastPlumeShared PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(FastPlumeStatic PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Set library version
set_target_properties(FastPlumeShared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(FastPlumeStatic PROPERTIES VERSION ${PROJECT_VERSION})

# Set the default installation prefix to /usr/local/FastPlume
set(CMAKE_INSTALL_PREFIX "/usr/local/FastPlume" CACHE PATH "Installation directory" FORCE)

# Define installation rules for libraries
install(TARGETS FastPlumeShared FastPlumeStatic
    LIBRARY DESTINATION lib    # For shared libraries
    ARCHIVE DESTINATION lib    # For static libraries
)

# Install headers directly under include
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

# Install the hpac_dispersion_coefs.csv to the fixture directory in the install path
install(FILES ${PROJECT_SOURCE_DIR}/fixture/hpac_dispersion_coefs.csv
        DESTINATION ${CMAKE_INSTALL_PREFIX}/fixture)

# Add an example executable for testing, using example.cpp
add_executable(FastPlume_Test ${PROJECT_SOURCE_DIR}/example/example.cpp)
set_target_properties(FastPlume_Test PROPERTIES INSTALL_RPATH "/usr/local/FastPlume/lib")

# Link the example executable against the shared library
target_link_libraries(FastPlume_Test FastPlumeShared)

# Install the example executable
install(TARGETS FastPlume_Test
    RUNTIME DESTINATION bin  # Install under bin directory
)

# Install all files in the data directory under examples
install(DIRECTORY ${PROJECT_SOURCE_DIR}/example/data/ DESTINATION examples/data)

# Install the example code file under examples
install(FILES ${PROJECT_SOURCE_DIR}/example/example.cpp DESTINATION examples)
