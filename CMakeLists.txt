cmake_minimum_required(VERSION 3.20)
project(CardinalWeaponsFramework VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch CommonLibSSE-NG for official SKSE & Papyrus VM headers/vtables
FetchContent_Declare(
    CommonLibSSE-NG
    GIT_REPOSITORY https://github.com/CharmedBaryon/CommonLibSSE-NG.git
    GIT_TAG        main
)

# Fetch nlohmann_json for robust JSON content pack parsing
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

FetchContent_MakeAvailable(CommonLibSSE-NG json)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/CWFramework/*.h")

# Shared Library (DLL) Output
add_library(CWFramework SHARED ${SOURCES} ${HEADERS})

if (TARGET CommonLibSSE-NG::CommonLibSSE-NG)
    target_link_libraries(CWFramework PRIVATE CommonLibSSE-NG::CommonLibSSE-NG)
endif()

if (TARGET nlohmann_json::nlohmann_json)
    target_link_libraries(CWFramework PRIVATE nlohmann_json::nlohmann_json)
endif()

set_target_properties(CWFramework PROPERTIES
    OUTPUT_NAME "CWFramework"
    PREFIX ""
    SUFFIX ".dll"
)

# Installation/Deployment Target
install(TARGETS CWFramework DESTINATION "SKSE/Plugins")
