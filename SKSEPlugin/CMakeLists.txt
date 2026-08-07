cmake_minimum_required(VERSION 3.20)
project(CWFramework CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    CommonLibSSE-NG
    GIT_REPOSITORY https://github.com/CharmedBaryon/CommonLibSSE-NG.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(CommonLibSSE-NG)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

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
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibSSE::CommonLibSSE nlohmann_json::nlohmann_json)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SKSE_SUPPORT_XBYAK
)
