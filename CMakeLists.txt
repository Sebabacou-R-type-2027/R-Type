cmake_minimum_required(VERSION 3.10)

project(R-Type)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32")
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIX")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

file(GLOB SERVER_SOURCES src/server/*.cpp src/server/*.hpp src/share/*.cpp src/share/*.hpp)
file(GLOB CLIENT_SOURCES src/client/*.cpp src/client/*.hpp src/share/*.cpp src/share/*.hpp)

message(STATUS "SERVER_SOURCES: ${SERVER_SOURCES}")
message(STATUS "CLIENT_SOURCES: ${CLIENT_SOURCES}")

if (NOT SERVER_SOURCES)
    message(FATAL_ERROR "No source files found for r-type_server")
endif()

if (NOT CLIENT_SOURCES)
    message(FATAL_ERROR "No source files found for r-type_client")
endif()

add_executable(r-type_server ${SERVER_SOURCES})
add_executable(r-type_client ${CLIENT_SOURCES})