cmake_minimum_required(VERSION 3.10)
project(cpp_rest)

set(CMAKE_CXX_STANDARD 14)

# Add debugging symbols
set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(Crow REQUIRED)

set(SOURCES
    src/Rest_Server/Rest_Server.cpp
    src/Device_Manager/Device_Manager.cpp
)

include_directories(
    src/Rest_Server
    src/Device_Manager
)

add_executable(cpp_rest ${SOURCES})
target_link_libraries(cpp_rest PRIVATE Crow::Crow crypto)
