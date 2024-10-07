file(GLOB client *.cpp *.hpp)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})

add_subdirectory(animation)
add_subdirectory(control)
add_subdirectory(draw)
add_subdirectory(position)
