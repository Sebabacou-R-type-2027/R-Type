cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(R-Type LANGUAGES CXX)

include(CTest)
enable_testing()
find_package(nlohmann_json CONFIG REQUIRED)


# `import std;` support
if(${CMAKE_CXX_STANDARD} IN_LIST CMAKE_CXX_COMPILER_IMPORT_STD)
    set(CMAKE_CXX_MODULE_STD ON)
else()
    message(FATAL_ERROR "Compiler does not support `import std;`")
endif()

add_subdirectory(src)
add_subdirectory(lib)

install(EXPORT main DESTINATION lib/cmake/r-type
    NAMESPACE rtype::
    EXPORT_PACKAGE_DEPENDENCIES
)

include(CPack)
