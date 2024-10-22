cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(R-Type)

include(CTest)
enable_testing()

find_package(asio CONFIG REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system audio CONFIG REQUIRED)
find_package(OpenAL REQUIRED)
find_package(FLAC REQUIRED)
find_package(Vorbis REQUIRED)

# `import std;` support
if(${CMAKE_CXX_STANDARD} IN_LIST CMAKE_CXX_COMPILER_IMPORT_STD)
    set(CMAKE_CXX_MODULE_STD ON)
    add_compile_definitions(__cpp_lib_modules=202207L)
else()
    message(WARNING "Compiler does not support `import std;`")
endif()

add_executable(r-type_server)

add_subdirectory(src)
add_subdirectory(lib)

target_link_libraries(r-type_server PRIVATE packet asio::asio)

target_include_directories(r-type_server PRIVATE
    src/server
    src/server/client
)

# Symlink the executables to the root directory (Unix only)

if (UNIX)
    add_custom_target(symlink-executables ALL BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/r-type_server ${CMAKE_CURRENT_SOURCE_DIR}/sample
        COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:r-type_server> ${CMAKE_CURRENT_SOURCE_DIR}/r-type_server
        COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:sample> ${CMAKE_CURRENT_SOURCE_DIR}/sample
    )
    add_dependencies(symlink-executables r-type_server sample)
endif()

include(CPack)
