cmake_minimum_required(VERSION 3.10)

project(R-Type)

# Check if we are on Windows or Unix
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32")
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIX")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Include directories for ecs headers
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/ecs)
include_directories(${CMAKE_SOURCE_DIR}/src/ecs/systems)
include_directories(${CMAKE_SOURCE_DIR}/src/ecs/factories)
include_directories(${CMAKE_SOURCE_DIR}/src/ecs/components)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Correct the paths for SERVER_SOURCES and CLIENT_SOURCES
file(GLOB SERVER_SOURCES src/server/*.cpp src/share/*.cpp)
file(GLOB CLIENT_SOURCES src/client/*.cpp src/ecs/*.cpp src/ecs/systems/*.cpp src/ecs/factories/*.cpp)

message(STATUS "SERVER_SOURCES: ${SERVER_SOURCES}")
message(STATUS "CLIENT_SOURCES: ${CLIENT_SOURCES}")

if (NOT SERVER_SOURCES)
    message(FATAL_ERROR "No source files found for r-type_server")
endif()

if (NOT CLIENT_SOURCES)
    message(FATAL_ERROR "No source files found for r-type_client")
endif()

# Add executable targets
add_executable(r-type_server ${SERVER_SOURCES})
add_executable(r-type_client ${CLIENT_SOURCES})

# Link SFML to r-type_client and r-type_server if needed
target_link_libraries(r-type_client sfml-graphics sfml-window sfml-system)
# Uncomment the next line if your server also needs SFML
# target_link_libraries(r-type_server sfml-graphics sfml-window sfml-system)

# Custom clean-all target to remove build artifacts
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/r-type_client
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_SOURCE_DIR}/r-type_server
    COMMENT "Cleaning log and build files"
)
