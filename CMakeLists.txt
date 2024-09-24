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

add_executable(r-type_server ${SERVER_SOURCES}
        src/server/UdpServer.cpp
        src/server/UdpServer.hpp
        src/server/Lobby.cpp
        src/server/Lobby.hpp
        src/server/client/client.cpp
)
add_executable(r-type_client ${CLIENT_SOURCES}
        src/client/package.cpp
        src/client/package.hpp
        src/client/Client.cpp
        src/client/Client.hpp)

add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/CMakeCache.txt
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/cmake_install.cmake
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/build.ninja
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/.ninja*
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/CMakeFiles
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/cmake-build-debug
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/Testing
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/r-type_client
        COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/r-type_server
        COMMENT "Cleaning log and build files"
)