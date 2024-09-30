cmake_minimum_required(VERSION 3.10)

project(R-Type)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32")
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIX")
endif()

set(CMAKE_CXX_STANDARD 17)

define_property(GLOBAL PROPERTY SERVER_SOURCES)
define_property(GLOBAL PROPERTY CLIENT_SOURCES)

add_subdirectory(src)

get_property(server GLOBAL PROPERTY SERVER_SOURCES)
get_property(client GLOBAL PROPERTY CLIENT_SOURCES)

find_package(asio CONFIG REQUIRED)

add_executable(r-type_server ${server})
add_executable(r-type_client ${client})

target_link_libraries(r-type_server PRIVATE asio::asio)
target_link_libraries(r-type_client PRIVATE asio::asio)

target_include_directories(r-type_server PRIVATE
    src/server
    src/share
)

target_include_directories(r-type_client PRIVATE
    src/client
    src/share
)

# Symlink the executables to the root directory (Unix only)
if (UNIX)
    add_custom_target(symlink-executables ALL BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/r-type_server ${CMAKE_CURRENT_SOURCE_DIR}/r-type_client
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/r-type_server ${CMAKE_CURRENT_SOURCE_DIR}/r-type_server
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/r-type_client ${CMAKE_CURRENT_SOURCE_DIR}/r-type_client
    )
    add_dependencies(symlink-executables r-type_server r-type_client)
endif()
