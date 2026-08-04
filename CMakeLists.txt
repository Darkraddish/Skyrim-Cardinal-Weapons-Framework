cmake_minimum_required(VERSION 3.20)
project(CardinalWeaponsFramework VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/CWFramework/*.h")

# Shared Library (DLL) Output
add_library(CWFramework SHARED ${SOURCES} ${HEADERS})

set_target_properties(CWFramework PROPERTIES
    OUTPUT_NAME "CWFramework"
    PREFIX ""
    SUFFIX ".dll"
)

# Installation/Deployment Target
install(TARGETS CWFramework DESTINATION "SKSE/Plugins")
