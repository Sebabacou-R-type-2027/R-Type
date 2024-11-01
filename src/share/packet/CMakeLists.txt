file(GLOB client *.cpp *.hpp)
file(GLOB server *.cpp *.hpp)

add_subdirectory(packet_type)

set_property(GLOBAL APPEND PROPERTY CLIENT_SOURCES ${client})
set_property(GLOBAL APPEND PROPERTY SERVER_SOURCES ${server})
