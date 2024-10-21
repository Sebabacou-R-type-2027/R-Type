cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(R-Type)

include(CTest)
enable_testing()

add_executable(r-type_server)
add_executable(r-type_client)

add_subdirectory(src)

find_package(asio CONFIG REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system audio CONFIG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(FLAC REQUIRED)
find_package(Vorbis REQUIRED)

target_link_libraries(r-type_server PRIVATE asio::asio)
target_link_libraries(r-type_client PRIVATE asio::asio sfml-graphics sfml-window sfml-system sfml-audio)

target_include_directories(r-type_server PRIVATE
    src/server
    src/server/client
    src/share
    src/share/packet
    src/share/packet/packet_type
)

target_include_directories(r-type_client PRIVATE
    src/client
    src/client/networkClient
    src/share
    src/share/packet
    src/share/packet/packet_type
    src/share
    src/ecs
    src/ecs/systems
    src/ecs/factories
    src/ecs/components
)

# Symlink the executables to the root directory (Unix only)

if (UNIX)
    add_custom_target(symlink-executables ALL BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/r-type_server ${CMAKE_CURRENT_SOURCE_DIR}/r-type_client
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/r-type_server ${CMAKE_CURRENT_SOURCE_DIR}/r-type_server
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/r-type_client ${CMAKE_CURRENT_SOURCE_DIR}/r-type_client
    )
    add_dependencies(symlink-executables r-type_server r-type_client)
endif()

include(CPack)
