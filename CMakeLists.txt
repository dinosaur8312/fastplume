cmake_minimum_required(VERSION 3.12)
project(FastPlume VERSION 1.0.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define a variable for the default installation path
set(DEFAULT_INSTALL_PATH "/usr/local/FastPlume")
set(CMAKE_INSTALL_PREFIX "${DEFAULT_INSTALL_PATH}" CACHE PATH "Installation directory" FORCE)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Automatically detect all .cpp files in the src directory
file(GLOB SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Create both shared and static library targets
add_library(FastPlumeShared SHARED ${SOURCE_FILES})
add_library(FastPlumeStatic STATIC ${SOURCE_FILES})

# Set output names and library version for the libraries
set_target_properties(FastPlumeShared PROPERTIES OUTPUT_NAME "FastPlume" VERSION ${PROJECT_VERSION})
set_target_properties(FastPlumeStatic PROPERTIES OUTPUT_NAME "FastPlume" VERSION ${PROJECT_VERSION})

# Specify the include directories for the library
target_include_directories(FastPlumeShared PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include/FastPlume>)
target_include_directories(FastPlumeStatic PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include/FastPlume>)


# Define installation rules for libraries with EXPORT set to FastPlumeTargets
install(TARGETS FastPlumeShared FastPlumeStatic
    EXPORT FastPlumeTargets
    LIBRARY DESTINATION lib    # For shared libraries
    ARCHIVE DESTINATION lib    # For static libraries
)

# Install headers
install(FILES ${PROJECT_SOURCE_DIR}/include/FastPlume.h
              ${PROJECT_SOURCE_DIR}/include/taskDataRow.h
    DESTINATION include/FastPlume
)

# Install the hpac_dispersion_coefs.csv to the fixture directory in the install path
install(FILES ${PROJECT_SOURCE_DIR}/fixture/hpac_dispersion_coefs.csv
        DESTINATION ${CMAKE_INSTALL_PREFIX}/fixture)

# Add an example executable for testing, using example.cpp
add_executable(FastPlume_Test ${PROJECT_SOURCE_DIR}/example/example.cpp)
set_target_properties(FastPlume_Test PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Link the example executable against the shared library
target_link_libraries(FastPlume_Test FastPlumeShared)

# Install the example executable
install(TARGETS FastPlume_Test
    RUNTIME DESTINATION bin  # Install under bin directory
)

# Install the entire example directory, excluding .cpp files in `example/data`
install(DIRECTORY ${PROJECT_SOURCE_DIR}/example/
        DESTINATION examples/FastPlume
        FILES_MATCHING PATTERN "*.*"
        PATTERN "*.cpp" EXCLUDE)

# Configure package for use with find_package
include(CMakePackageConfigHelpers)

# Create a version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FastPlumeConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Export the targets to a configuration file
install(EXPORT FastPlumeTargets
    FILE FastPlumeTargets.cmake
    NAMESPACE FastPlume::
    DESTINATION lib/cmake/FastPlume
)

# Install the package config and version files
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/FastPlumeConfigVersion.cmake"
        DESTINATION lib/cmake/FastPlume)

# Define testing support
enable_testing()
add_test(NAME FastPlume_Test COMMAND FastPlume_Test)

# Post-install message to guide the user
message(STATUS "FastPlume installed in ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Example and test files are located in ${CMAKE_INSTALL_PREFIX}/examples/FastPlume")
