file(GLOB client *.cpp *.hpp)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})

add_subdirectory(components)
add_subdirectory(systems)
