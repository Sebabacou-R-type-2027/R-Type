cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(R-Type)

include(CTest)
enable_testing()

find_package(asio CONFIG REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system audio CONFIG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(FLAC REQUIRED)
find_package(Vorbis REQUIRED)

add_executable(r-type_server)
add_executable(r-type_client)

add_subdirectory(src)
add_subdirectory(lib)

target_link_libraries(r-type_server PRIVATE packet asio::asio)
target_link_libraries(r-type_client PRIVATE packet asio::asio sfml-graphics sfml-window sfml-system sfml-audio)

target_include_directories(r-type_server PRIVATE
    src/server
    src/server/client
)

target_include_directories(r-type_client PRIVATE
    src/client
    src/client/networkClient
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
