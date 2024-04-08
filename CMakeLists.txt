cmake_minimum_required(VERSION 3.10)
project(cpp_rest)

set(CMAKE_CXX_STANDARD 20)

# Add debugging symbols
set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(Crow REQUIRED)

set(SOURCES
    src/Rest_Server/Rest_Server.cpp
    src/Device_Manager/Device_Manager.cpp
    src/Location_Manager/Location_Manager.cpp
    src/SQL_Client/SQL_Client.cpp
)

include_directories(
    src/Rest_Server
    src/Device_Manager
    src/Location_Manager
    src/SQL_Client
)

add_executable(cpp_rest ${SOURCES})
set_target_properties(cpp_rest PROPERTIES COMPILE_FLAGS -DCROW_JSON_USE_MAP)
target_link_libraries(cpp_rest PRIVATE Crow::Crow crypto -lmysqlclient -lmysqlcppconn)
