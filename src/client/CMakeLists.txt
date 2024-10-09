file(GLOB client *.cpp *.hpp)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})

add_subdirectory(components)
add_subdirectory(factories)
add_subdirectory(systems)
add_subdirectory(utils)
add_subdirectory(game)
add_subdirectory(menu)