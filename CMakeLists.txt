cmake_minimum_required(VERSION 3.12)
project(mtplume VERSION 1.0.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Add mtplume_internal include directory
# Include directories
include_directories(
    include
)

# Add the nlohmann/json include directory


#find_package(CUDA REQUIRED)
#include_directories(${CUDA_INCLUDE_DIRS})


# Automatically add all source files in the src/common directory
# Explicitly list source files instead of globbing
set(SOURCE_FILES
    main.cpp
    JsonInputHandler.cpp
    SigmaInterp.cpp
    # Add other .cpp files here if necessary
)

# If you have source files in the cpu or gpu directories, add them to the list
# file(GLOB CPU_SOURCES "src/cpu/*.cpp")
# file(GLOB GPU_SOURCES "src/gpu/*.cu")
# list(APPEND SOURCE_FILES ${CPU_SOURCES} ${GPU_SOURCES})



# Add the executable
add_executable(mtplume ${SOURCE_FILES})
#target_link_libraries(mtplume ${CUDA_LIBRARIES})

# Link any necessary libraries
# If you have additional libraries, use target_link_libraries
# target_link_libraries(mtplume PUBLIC some_library)

# For Doxygen

# Add subdirectories for examples and tests if they contain CMakeLists.txt
# add_subdirectory(examples)
# add_subdirectory(tests)

# Set up Python bindings if necessary
# This assumes you're using pybind11 or similar
# find_package(pybind11 REQUIRED)
# pybind11_add_module(mtplume_py MODULE python/mtplume_py/mtplume_py.cpp)
# target_link_libraries(mtplume_py PRIVATE mtplume)

# Install targets (if necessary)
# install(TARGETS mtplume DESTINATION bin)
# install(FILES include/mtplume/mtplume.hpp DESTINATION include/mtplume)
