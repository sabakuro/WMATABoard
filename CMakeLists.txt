cmake_minimum_required(VERSION 3.20)
project(WMATABoard CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JSON parsing library (downloaded automatically at configure time)
include(FetchContent)
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# Core code: data models + API client. Reusable by the LED board code later.
add_library(wmata_core STATIC
       src/api/WmataClient.cpp
       src/api/WmataHttp_win.cpp
   )
target_include_directories(wmata_core PUBLIC src)
target_link_libraries(wmata_core PUBLIC nlohmann_json::nlohmann_json)
if(WIN32)
    target_link_libraries(wmata_core PUBLIC winhttp)
endif()

# The runnable test program
add_executable(wmata_board src/main-local.cpp)
target_link_libraries(wmata_board PRIVATE wmata_core)