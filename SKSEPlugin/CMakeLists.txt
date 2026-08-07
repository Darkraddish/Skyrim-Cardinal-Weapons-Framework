cmake_minimum_required(VERSION 3.24)
project(CWFramework CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SKIP_INSTALL_RULES ON)

include(FetchContent)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.2.1
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.13.0
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG        v8.84
)
FetchContent_MakeAvailable(rapidcsv)
FetchContent_GetProperties(rapidcsv)
if(NOT rapidcsv_POPULATED)
    FetchContent_Populate(rapidcsv)
endif()
set(RAPIDCSV_INCLUDE_DIRS "${rapidcsv_SOURCE_DIR}/src" CACHE PATH "" FORCE)

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ENABLE_SKYRIM_SE ON CACHE BOOL "" FORCE)
set(ENABLE_SKYRIM_AE ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    CommonLibSSE-NG
    GIT_REPOSITORY https://github.com/CharmedBaryon/CommonLibSSE-NG.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(CommonLibSSE-NG)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/DataLoader.h
    src/DataLoader.cpp
    src/InputHandler.h
    src/InputHandler.cpp
    src/SaveLoad.h
    src/SaveLoad.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibSSE::CommonLibSSE nlohmann_json::nlohmann_json spdlog::spdlog fmt::fmt)
